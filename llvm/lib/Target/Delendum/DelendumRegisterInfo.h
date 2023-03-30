//===-- DelendumRegisterInfo.h - Delendum Register Information Impl -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the Delendum implementation of the TargetRegisterInfo
/// class.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DELENDUM_DELENDUMREGISTERINFO_H
#define LLVM_LIB_TARGET_DELENDUM_DELENDUMREGISTERINFO_H

#include "Delendum.h"

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "DelendumGenRegisterInfo.inc"

namespace llvm {
class DelendumSubtarget;
class TargetInstrInfo;
class Type;

class DelendumRegisterInfo : public DelendumGenRegisterInfo {
  virtual void anchor();

  /// Physical register used as stack ptr.
  unsigned StackPtr;

  /// Physical register used as frame ptr.
  unsigned FramePtr;

  /// Physical register used as a base ptr in complex stack frames.  I.e., when
  /// we need a 3rd base, not just SP and FP, due to variable size stack
  /// objects.
  unsigned BasePtr;

  /// Physical register used to store GOT address if needed.
  unsigned GlobalBasePtr;

protected:
  const DelendumSubtarget &Subtarget;

public:
  DelendumRegisterInfo(const DelendumSubtarget &Subtarget);

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool requiresRegisterScavenging(const MachineFunction &MF) const override;

  bool trackLivenessAfterRegAlloc(const MachineFunction &MF) const override;

  /// FrameIndex represent objects inside a abstract stack. We must replace
  /// FrameIndex with an stack/frame pointer direct reference.
  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  /// True if the stack can be realigned for the target.
  bool canRealignStack(const MachineFunction &MF) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;

  const TargetRegisterClass *
  getCrossCopyRegClass(const TargetRegisterClass *RC) const override {
    if (RC == &DL::GPRRegClass)
      return &DL::GPRRegClass;
    return RC;
  }

  unsigned getStackRegister() const { return StackPtr; }
  unsigned getBaseRegister() const { return BasePtr; }
  unsigned getGlobalBaseRegister() const { return GlobalBasePtr; }

  const TargetRegisterClass *intRegClass(unsigned Size) const;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DELENDUM_DELENDUMREGISTERINFO_H
