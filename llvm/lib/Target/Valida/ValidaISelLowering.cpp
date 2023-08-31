//===-- ValidaISelLowering.cpp - Valida DAG Lowering Implementation ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the interfaces that Valida uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#include "ValidaCallingConv.h"
#include "ValidaISelLowering.h"
#include "ValidaRegisterInfo.h"
#include "ValidaTargetMachine.h"
#include "ValidaTargetObjectFile.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/KnownBits.h"
using namespace llvm;

#include "ValidaGenCallingConv.inc"

ValidaTargetLowering::ValidaTargetLowering(const TargetMachine &TM,
                                               const ValidaSubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) { }


SDValue ValidaTargetLowering::LowerOperation(SDValue Op,
                                               SelectionDAG &DAG) const {
}

bool ValidaTargetLowering::useSoftFloat() const {
}

/// computeKnownBitsForTargetNode - Determine which of the bits specified
/// in Mask are known to be either zero or one and return them in the
/// KnownZero/KnownOne bitsets.
void ValidaTargetLowering::computeKnownBitsForTargetNode(const SDValue Op,
                                                           KnownBits &Known,
                                                           const APInt &DemandedElts,
                                                           const SelectionDAG &DAG,
                                                           unsigned Depth) const {
}

MachineBasicBlock *
ValidaTargetLowering::EmitInstrWithCustomInserter(MachineInstr &MI,
                                                    MachineBasicBlock *MBB) const {
}

const char *ValidaTargetLowering::getTargetNodeName(unsigned Opcode) const {}

ValidaTargetLowering::ConstraintType
ValidaTargetLowering::getConstraintType(StringRef Constraint) const {}

ValidaTargetLowering::ConstraintWeight
ValidaTargetLowering::getSingleConstraintMatchWeight(AsmOperandInfo &info,
                               const char *constraint) const {}
void ValidaTargetLowering::LowerAsmOperandForConstraint(SDValue Op,
                                  std::string &Constraint,
                                  std::vector<SDValue> &Ops,
                                  SelectionDAG &DAG) const {}

std::pair<unsigned, const TargetRegisterClass *>
ValidaTargetLowering::getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                                                     StringRef Constraint, MVT VT) const {
};

bool ValidaTargetLowering::isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const {
};

Register
ValidaTargetLowering::getRegisterByName(const char* RegName, LLT VT,
                                         const MachineFunction &MF) const {};

unsigned ValidaTargetLowering::getNumRegistersForCallingConv(
    LLVMContext &Context, CallingConv::ID CC, EVT VT) const {
  return 1;
  // FIXME: Why is getNumRegisters not returning the correct number of virtual regs?
  //return getNumRegisters(Context, VT);
}

MVT ValidaTargetLowering::getRegisterTypeForCallingConv(LLVMContext &Context,
                                                          CallingConv::ID CC,
                                                          EVT VT) const {
  return MVT::i32;
  // FIXME: Why is getRegisterType not returning the correct type?
  //return getRegisterType(Context, VT);
}

/// Override to support customized stack guard loading.
bool ValidaTargetLowering::useLoadStackGuardNode() const {};
void ValidaTargetLowering::insertSSPDeclarations(Module &M) const {};

/// getSetCCResultType - Return the ISD::SETCC ValueType
EVT 
ValidaTargetLowering::getSetCCResultType(const DataLayout &DL,
                                           LLVMContext &Context,
                                           EVT VT) const {};

SDValue
ValidaTargetLowering::LowerFormalArguments(SDValue Chain,
                                             CallingConv::ID CallConv,
                                             bool isVarArg,
                                             const SmallVectorImpl<ISD::InputArg> &Ins,
                                             const SDLoc &dl, SelectionDAG &DAG,
                                             SmallVectorImpl<SDValue> &InVals) const {
  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, Valida_CCallingConv);

  SDValue ArgValue;
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    InVals.push_back(ArgValue);
  }

  return Chain;
};

SDValue
ValidaTargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                  SmallVectorImpl<SDValue> &InVals) const {};

bool ValidaTargetLowering::CanLowerReturn(CallingConv::ID CallConv,
                                            MachineFunction &MF,
                                            bool isVarArg,
                                            const SmallVectorImpl<ISD::OutputArg> &Outs,
                                            LLVMContext &Context) const {
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, MF, RVLocs, Context);
  return CCInfo.CheckReturn(Outs, Valida_CRetConv);
};

SDValue
ValidaTargetLowering::LowerReturn(SDValue Chain, 
                                    CallingConv::ID CallConv, bool isVarArg,
                                    const SmallVectorImpl<ISD::OutputArg> &Outs,
                                    const SmallVectorImpl<SDValue> &OutVals,
                                    const SDLoc &dl, SelectionDAG &DAG) const {
  return Chain;
};

SDValue
ValidaTargetLowering::PerformDAGCombine(SDNode *N,
                                          DAGCombinerInfo &DCI) const {
};

ValidaTargetLowering::AtomicExpansionKind
ValidaTargetLowering::shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const {};

void ValidaTargetLowering::ReplaceNodeResults(SDNode *N,
                                                SmallVectorImpl<SDValue>& Results,
                                                SelectionDAG &DAG) const {};

