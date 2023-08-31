//===-- ValidaFrameLowering.cpp - Valida Frame Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Valida implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "ValidaFrameLowering.h"
#include "ValidaInstrInfo.h"
#include "ValidaSubtarget.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;

ValidaFrameLowering::ValidaFrameLowering(const ValidaSubtarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown,
                          Align(4), 0,
                          Align(4)) {}

void ValidaFrameLowering::emitPrologue(MachineFunction &MF,
                                         MachineBasicBlock &MBB) const {}

void ValidaFrameLowering::emitEpilogue(MachineFunction &MF,
                                         MachineBasicBlock &MBB) const {}
