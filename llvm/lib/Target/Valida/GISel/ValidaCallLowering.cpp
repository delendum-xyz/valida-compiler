//===-- ValidaCallLowering.cpp - Call lowering ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file implements the lowering of LLVM calls to machine code calls for
/// GlobalISel.
//
//===----------------------------------------------------------------------===//

#include "ValidaCallingConv.h"
#include "ValidaCallLowering.h"
#include "ValidaISelLowering.h"
#include "ValidaInstrInfo.h"
#include "ValidaRegisterInfo.h"
#include "ValidaSubtarget.h"
#include "ValidaTargetMachine.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/GlobalISel/CallLowering.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/FunctionLoweringInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/TargetCallingConv.h"

using namespace llvm;

#include "ValidaGenCallingConv.inc"

ValidaCallLowering::ValidaCallLowering(const ValidaTargetLowering &TLI)
    : CallLowering(&TLI) {}

struct ValidaReturnValueHandler : public CallLowering::OutgoingValueHandler {
  ValidaReturnValueHandler(MachineIRBuilder &MIRBuilder,
                               MachineRegisterInfo &MRI,
                               MachineInstrBuilder MIB)
      : OutgoingValueHandler(MIRBuilder, MRI), MIB(MIB),
        DL(MIRBuilder.getMF().getDataLayout()),
        STI(MIRBuilder.getMF().getSubtarget<ValidaSubtarget>()) {}

  uint64_t StackUsed;

  // NO-OP: all values are in the stack
  void assignValueToReg(Register ValVReg, Register PhysReg,
                        CCValAssign VA) override {}

  void assignValueToAddress(Register ValVReg, Register Addr, LLT MemTy,
                            MachinePointerInfo &MPO, CCValAssign &VA) override {
    MachineFunction &MF = MIRBuilder.getMF();
    Register ExtReg = extendRegister(ValVReg, VA);
    auto *MMO = MF.getMachineMemOperand(MPO, MachineMemOperand::MOStore, MemTy,
                                        inferAlignFromPtrInfo(MF, MPO));
    MIRBuilder.buildStore(ExtReg, Addr, *MMO);
  }

  Register getStackAddress(uint64_t Size, int64_t Offset,
                           MachinePointerInfo &MPO,
                           ISD::ArgFlagsTy Flags) override {
    auto &MFI = MIRBuilder.getMF().getFrameInfo();
    const bool IsImmutable = !Flags.isByVal();
    int FI = MFI.CreateFixedObject(Size, Offset, IsImmutable);
    MPO = MachinePointerInfo::getFixedStack(MIRBuilder.getMF(), FI);

    // Build Frame Index
    llvm::LLT FramePtr = LLT::pointer(
        0, MIRBuilder.getMF().getDataLayout().getPointerSizeInBits());
    MachineInstrBuilder AddrReg = MIRBuilder.buildFrameIndex(FramePtr, FI);
    StackUsed = std::max(StackUsed, Size + Offset);
    return AddrReg.getReg(0);
  }

  MachineInstrBuilder MIB;
  const DataLayout &DL;
  const ValidaSubtarget &STI;
};

struct ValidaCallValueHandler : public CallLowering::OutgoingValueHandler {
  ValidaCallValueHandler(MachineIRBuilder &MIRBuilder,
                               MachineRegisterInfo &MRI,
                               MachineInstrBuilder MIB)
      : OutgoingValueHandler(MIRBuilder, MRI), MIB(MIB),
        DL(MIRBuilder.getMF().getDataLayout()),
        STI(MIRBuilder.getMF().getSubtarget<ValidaSubtarget>()) {}

  uint64_t StackUsed;

  // NO-OP: all values are in the stack
  void assignValueToReg(Register ValVReg, Register PhysReg,
                        CCValAssign VA) override {}

  void assignValueToAddress(Register ValVReg, Register Addr, LLT MemTy,
                            MachinePointerInfo &MPO, CCValAssign &VA) override {
    MachineFunction &MF = MIRBuilder.getMF();
    Register ExtReg = extendRegister(ValVReg, VA);
    auto *MMO = MF.getMachineMemOperand(MPO, MachineMemOperand::MOStore, MemTy,
                                        inferAlignFromPtrInfo(MF, MPO));
    MIRBuilder.buildStore(ExtReg, Addr, *MMO);
  }

  Register getStackAddress(uint64_t Size, int64_t Offset,
                           MachinePointerInfo &MPO,
                           ISD::ArgFlagsTy Flags) override {
    auto &MFI = MIRBuilder.getMF().getFrameInfo();
    int FI = MFI.CreateStackObject(Size, Align(4), false);
    MPO = MachinePointerInfo::getStack(MIRBuilder.getMF(), FI);

    // Build Frame Index
    llvm::LLT FramePtr = LLT::pointer(
        0, MIRBuilder.getMF().getDataLayout().getPointerSizeInBits());
    MachineInstrBuilder AddrReg = MIRBuilder.buildFrameIndex(FramePtr, FI);
    StackUsed = std::max(StackUsed, Size + Offset);
    return AddrReg.getReg(0);
  }

  MachineInstrBuilder MIB;
  const DataLayout &DL;
  const ValidaSubtarget &STI;
};

// NO-OP: all values are in the stack
void ValidaIncomingValueHandler::assignValueToReg(Register ValVReg,
                                                    Register PhysReg,
                                                    CCValAssign VA) {}

void ValidaIncomingValueHandler::assignValueToAddress(Register ValVReg,
                                                        Register Addr,
                                                        LLT MemTy,
                                                        MachinePointerInfo &MPO,
                                                        CCValAssign &VA) {
  MachineFunction &MF = MIRBuilder.getMF();
  Register ExtReg = extendRegister(ValVReg, VA);

  auto *MMO = MF.getMachineMemOperand(MPO, MachineMemOperand::MOStore, MemTy,
                                      inferAlignFromPtrInfo(MF, MPO));
  MIRBuilder.buildStore(ExtReg, Addr, *MMO);
}

Register ValidaIncomingValueHandler::getStackAddress(uint64_t Size,
                                                       int64_t Offset,
                                                       MachinePointerInfo &MPO,
                                                       ISD::ArgFlagsTy Flags) {
  auto &MFI = MIRBuilder.getMF().getFrameInfo();
  const bool IsImmutable = !Flags.isByVal();
  int FI = MFI.CreateFixedObject(Size, Offset, IsImmutable);
  MPO = MachinePointerInfo::getFixedStack(MIRBuilder.getMF(), FI);

  // Build Frame Index
  llvm::LLT FramePtr = LLT::pointer(
      0, MIRBuilder.getMF().getDataLayout().getPointerSizeInBits());
  MachineInstrBuilder AddrReg = MIRBuilder.buildFrameIndex(FramePtr, FI);
  StackUsed = std::max(StackUsed, Size + Offset);
  return AddrReg.getReg(0);
}

bool ValidaCallLowering::lowerFormalArguments(MachineIRBuilder &MIRBuilder,
                                            const Function &F,
                                            ArrayRef<ArrayRef<Register>> VRegs,
                                            FunctionLoweringInfo &FLI) const {
  MachineFunction &MF = MIRBuilder.getMF();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  const auto &DL = F.getParent()->getDataLayout();

  if (!VRegs.empty()) {
    SmallVector<ArgInfo, 8> InArgs;
    unsigned I = 0;
    for (auto &Arg : F.args()) {
      ArgInfo OrigArg{VRegs[I], Arg.getType(), 0};
      setArgFlags(OrigArg, I + AttributeList::FirstArgIndex, DL, F);
      InArgs.push_back(OrigArg);
      ++I;
    }

    FormalArgHandler Handler(MIRBuilder, MRI);

    CCAssignFn *AssignFn = Valida_CCallingConv;
    OutgoingValueAssigner Assigner(AssignFn);

    determineAndHandleAssignments(Handler, Assigner, InArgs, MIRBuilder, 
                                  F.getCallingConv(), F.isVarArg());
  }

  return true;
}

bool ValidaCallLowering::lowerCall(MachineIRBuilder &MIRBuilder,
                                     CallLoweringInfo &Info) const {
  MachineFunction &MF = MIRBuilder.getMF();
  Function &F = MF.getFunction();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  auto &DL = F.getParent()->getDataLayout();

  SmallVector<ArgInfo, 8> OutArgs;
  for (auto &OrigArg : Info.OrigArgs)
    splitToValueTypes(OrigArg, OutArgs, DL, Info.CallConv);

  SmallVector<ArgInfo, 8> InArgs;
  if (!Info.OrigRet.Ty->isVoidTy())
    splitToValueTypes(Info.OrigRet, InArgs, DL, Info.CallConv);

  auto MIB = MIRBuilder.buildInstrNoInsert(DL::CALL);

  // Set up the call arguments
  CCAssignFn *AssignFn = Valida_CCallingConv;
  OutgoingValueAssigner Assigner(AssignFn);
  ValidaCallValueHandler Handler(MIRBuilder, MRI, MIB);

  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(Info.CallConv, Info.IsVarArg, MF, ArgLocs, F.getContext());

  // Allocate space for local variables on the stack
  MachineFrameInfo &MFI = MF.getFrameInfo();
  for (unsigned I = 0; I < MFI.getNumObjects(); I++) {
    CCInfo.AllocateStack(4, Align(4)); // Local variable
  }
  CCInfo.AllocateStack(4, Align(4)); // Current FP
  CCInfo.AllocateStack(4, Align(4)); // Return value
  CCInfo.AllocateStack(4, Align(4)); // Return address

  if (!determineAssignments(Assigner, OutArgs, CCInfo))
    return false;

  if (!handleAssignments(Handler, OutArgs, CCInfo, ArgLocs, MIRBuilder))
    return false;

  // Get call frame size (bytes required for arguments) from callee
  int NextStackOffset = CCInfo.getNextStackOffset();

  // Add the return value vreg to the call instruction
  MIB.addReg(InArgs[0].Regs[0], RegState::Implicit | RegState::Define);
  // Add the offset of the return value
  MIB.addImm(-NextStackOffset + 4); 
  // Add the offset of the return address
  MIB.add(Info.Callee);
  // Add the offset of the current FP
  MIB.addImm(-NextStackOffset);
  // Insert call pseudoinstruction
  MIRBuilder.insertInstr(MIB);

  return true;
}

bool ValidaCallLowering::lowerReturn(MachineIRBuilder &MIRBuilder,
                                       const Value *Val, ArrayRef<Register> VRegs,
                                       FunctionLoweringInfo &FLI) const {
  bool Success = true;
  if (!VRegs.empty()) {
    MachineFunction &MF = MIRBuilder.getMF();
    const Function &F = MF.getFunction();
    MachineRegisterInfo &MRI = MF.getRegInfo();
    auto &DL = F.getParent()->getDataLayout();

    auto MIB = MIRBuilder.buildInstrNoInsert(DL::JALV)
        .addImm(-4) // Assign pc+1 to a dummy stack slot
        .addImm(0)
        .addImm(8); // This is where the previous FP is stored

    SmallVector<ArgInfo, 8> SplitArgs;
    ArgInfo OrigArg{VRegs, Val->getType(), 0};
    setArgFlags(OrigArg, AttributeList::ReturnIndex, DL, F);
    splitToValueTypes(OrigArg, SplitArgs, DL, F.getCallingConv());

    CCAssignFn *AssignFn = Valida_CRetConv;
    OutgoingValueAssigner Assigner(AssignFn);
    ValidaReturnValueHandler Handler(MIRBuilder, MRI, MIB);

    Success = determineAndHandleAssignments(Handler, Assigner, SplitArgs,
                                            MIRBuilder, F.getCallingConv(),
                                            F.isVarArg());

    MIRBuilder.insertInstr(MIB);
  }
  return Success;
}

bool ValidaCallLowering::enableBigEndian() const { return true; }
