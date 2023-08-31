//===-- ValidaMachineFunctionInfo.cpp - Private data used for Valida ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ValidaMachineFunction.h"
#include "ValidaInstrInfo.h"
#include "ValidaSubtarget.h"
#include "MCTargetDesc/ValidaBaseInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

bool FixGlobalBaseReg = true;

bool ValidaFunctionInfo::globalBaseRegFixed() const {
  return FixGlobalBaseReg;
}

bool ValidaFunctionInfo::globalBaseRegSet() const {
  return GlobalBaseReg;
}

unsigned ValidaFunctionInfo::getGlobalBaseReg() {
  // TODO
}

void ValidaFunctionInfo::anchor() { }
