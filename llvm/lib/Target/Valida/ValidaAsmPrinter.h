//===-- ValidaAsmPrinter.h - Valida LLVM Assembly Printer ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains Valida assembler printer declarations.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_VALIDAASMPRINTER_H
#define LLVM_LIB_TARGET_VALIDA_VALIDAASMPRINTER_H

#include "ValidaMCInstLower.h"
#include "ValidaTargetMachine.h"

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>
#include <utility>

namespace llvm {
class ValidaMachineFunctionInfo;

class LLVM_LIBRARY_VISIBILITY ValidaAsmPrinter : public AsmPrinter {

public:
  const ValidaSubtarget *Subtarget;
  const ValidaMachineFunctionInfo *MMFI;
  std::unique_ptr<ValidaMCInstLower> MCInstLowering;

  explicit ValidaAsmPrinter(TargetMachine &TM,
                              std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {
    Subtarget = static_cast<ValidaTargetMachine &>(TM).getSubtargetImpl();
  }

  StringRef getPassName() const override { return "Valida Assembly Printer"; }

  virtual bool runOnMachineFunction(MachineFunction &MF) override;

  void emitInstruction(const MachineInstr *MI) override;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_VALIDA_VALIDAASMPRINTER_H
