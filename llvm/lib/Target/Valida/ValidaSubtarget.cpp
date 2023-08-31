//===-- ValidaSubtarget.cpp - Valida Subtarget Information ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements the Valida specific subclass of TargetSubtargetInfo.
///
//===----------------------------------------------------------------------===//


#include "Valida.h"
#include "ValidaMachineFunction.h"
#include "ValidaRegisterInfo.h"
#include "ValidaTargetMachine.h"

#include "ValidaSubtarget.h"
#include "GISel/ValidaCallLowering.h"
#include "GISel/ValidaRegisterBankInfo.h"
#include "GISel/ValidaLegalizerInfo.h"

#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "valida-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "ValidaGenSubtargetInfo.inc"

/// Select the Valida CPU for the given triple and cpu name.
static StringRef selectValidaCPU(Triple TT, StringRef CPU) {
  if (CPU.empty() || CPU == "generic") {
    CPU = "Valida";
  }
  return CPU;
}

void ValidaSubtarget::anchor() {}

ValidaSubtarget &ValidaSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                                      StringRef FS) {
  // Determine default and user specified characteristics
  std::string CPUName = std::string(CPU);
  CPUName = "";

  // Parse features string.
  ParseSubtargetFeatures(CPUName, /*TuneCPU*/ CPUName, FS);

  return *this;
}

ValidaSubtarget::ValidaSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                                     const ValidaTargetMachine &TM)
    : ValidaGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS), TM(TM),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)),
      TLInfo(TM, *this), FrameLowering(*this) {
  CallLoweringInfo.reset(new ValidaCallLowering(*getTargetLowering()));
  Legalizer.reset(new ValidaLegalizerInfo(*this));
  auto *RBI = new ValidaRegisterBankInfo(*getRegisterInfo());
  RegBankInfo.reset(RBI);
  InstSelector.reset(createValidaInstructionSelector(TM, *this, *RBI));
}

const CallLowering *ValidaSubtarget::getCallLowering() const {
  return CallLoweringInfo.get();
}

const LegalizerInfo *ValidaSubtarget::getLegalizerInfo() const {
  return Legalizer.get();
}

const RegisterBankInfo *ValidaSubtarget::getRegBankInfo() const {
  return RegBankInfo.get();
}

InstructionSelector *ValidaSubtarget::getInstructionSelector() const {
  return InstSelector.get();
}
