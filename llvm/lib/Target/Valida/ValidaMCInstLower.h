//===-- ValidaMCInstLower.h - Lower MachineInstr to MCInst --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains code to lower Valida MachineInstrs to their
/// corresponding MCInst records.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_VALIDAMCINSTLOWER_H
#define LLVM_LIB_TARGET_VALIDA_VALIDAMCINSTLOWER_H

#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class MCContext;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineFunction;
class ValidaAsmPrinter;

class ValidaMCInstLower {
  typedef MachineOperand::MachineOperandType MachineOperandType;
  MCContext &Ctx;
  MachineFunction &MF;
  const TargetMachine &TM;
  const MCAsmInfo &MAI;
  ValidaAsmPrinter &AsmPrinter;

public:
  ValidaMCInstLower(MachineFunction &MF, ValidaAsmPrinter &AP);

  MCOperand LowerSymbolOperand(const MachineOperand &MO) const;

  std::optional<MCOperand> LowerOperand(const MachineInstr *MI,
                                        const MachineOperand &MO) const;
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_VALIDA_VALIDAMCINSTLOWER_H

