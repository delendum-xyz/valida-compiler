//===-- ValidaAsmPrinter.cpp - Assembly Printer ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains a printer that converts from our internal representation
/// of machine-dependent LLVM code to GAS-format Valida assembly language.
///
//===----------------------------------------------------------------------===//

#include "Valida.h"
#include "ValidaAsmPrinter.h"
#include "ValidaMachineFunction.h"
#include "TargetInfo/ValidaTargetInfo.h"
#include "ValidaTargetMachine.h"
#include "MCTargetDesc/ValidaInstPrinter.h"
#include "ValidaMCInstLower.h"

#include "llvm/MC/TargetRegistry.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/Compiler.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

bool ValidaAsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  MCInstLowering = std::make_unique<ValidaMCInstLower>(MF, *this);
  AsmPrinter::runOnMachineFunction(MF);
  return true;
}

void ValidaAsmPrinter::emitInstruction(const MachineInstr *MI) {
  Valida_MC::verifyInstructionPredicates(MI->getOpcode(),
                                           getSubtargetInfo().getFeatureBits());
  MCInst OutMI;
  MCInstLowering->Lower(MI, OutMI);
  OutStreamer->emitInstruction(OutMI, getSubtargetInfo());
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeValidaAsmPrinter() {
  RegisterAsmPrinter<ValidaAsmPrinter> X(getTheValidaTarget());
}
