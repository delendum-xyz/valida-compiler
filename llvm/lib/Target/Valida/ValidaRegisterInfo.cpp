//===-- ValidaRegisterInfo.h - Valida Register Information Impl -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the Valida implementation of the TargetRegisterInfo
/// class.
///
//===----------------------------------------------------------------------===//

#include "Valida.h"
#include "ValidaRegisterInfo.h"
#include "ValidaSubtarget.h"
#include "MCTargetDesc/ValidaMCTargetDesc.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "ValidaGenRegisterInfo.inc"

#define DEBUG_TYPE "valida-reg-info"

using namespace llvm;

// Pin the vtable to this file.
void ValidaRegisterInfo::anchor() {}

ValidaRegisterInfo::ValidaRegisterInfo(const ValidaSubtarget &ST)
    : ValidaGenRegisterInfo(0, 0, 0, 0, 0), Subtarget(ST) {
}

const MCPhysReg*
ValidaRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const MCPhysReg CalleeSavedRegs[] = {0};
  return CalleeSavedRegs;
}

const uint32_t*
ValidaRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                           CallingConv::ID) const {
  // TODO
}

BitVector
ValidaRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
    BitVector Reserved(getNumRegs());
    return Reserved;
};

bool
ValidaRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const {
  return false;
};

bool
ValidaRegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const {
  // TODO
};

/// FrameIndex represent objects inside a abstract stack. We must replace
/// FrameIndex with an stack/frame pointer direct reference.
bool
ValidaRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  // TODO
};

/// True if the stack can be realigned for the target.
bool
ValidaRegisterInfo::canRealignStack(const MachineFunction &MF) const {
  // TODO
};

Register ValidaRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  // TODO
};
