//===-- ValidaMCInstLower.cpp - Valida MachineInstr to MCInst -------*- C++ -*-===//
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

#include "ValidaMCInstLower.h"

#include "ValidaAsmPrinter.h"
#include "ValidaInstrInfo.h"

#include "MCTargetDesc/ValidaBaseInfo.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

#define DEBUG_TYPE "valida-mc-inst-lower"

ValidaMCInstLower::ValidaMCInstLower(MachineFunction &MF,
                                         ValidaAsmPrinter &AP)
    : Ctx(MF.getContext()), MF(MF), TM(MF.getTarget()),
      MAI(*TM.getMCAsmInfo()), AsmPrinter(AP) {}

MCOperand
ValidaMCInstLower::LowerSymbolOperand(const MachineOperand &MO) const {
  const MCSymbol *Sym;
  const MCExpr *Expr = nullptr;

  SmallString<128> Name;
  switch (MO.getType()) {
  case MachineOperand::MO_MachineBasicBlock:
    Sym = MO.getMBB()->getSymbol();
    break;
  case MachineOperand::MO_GlobalAddress:
    Sym = AsmPrinter.getSymbol(MO.getGlobal());
    break;
  default:
    break;
  }

  if (!Sym)
    Sym = Ctx.getOrCreateSymbol(Name);

  MCSymbolRefExpr::VariantKind RefKind = MCSymbolRefExpr::VK_None;
  Expr = MCSymbolRefExpr::create(Sym, RefKind, Ctx);

  return MCOperand::createExpr(Expr);
}

std::optional<MCOperand>
ValidaMCInstLower::LowerOperand(const MachineInstr *MI,
                                  const MachineOperand &MO) const {
  switch (MO.getType()) {
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  case MachineOperand::MO_MachineBasicBlock:
  case MachineOperand::MO_GlobalAddress:
    return LowerSymbolOperand(MO);
  default:
    llvm_unreachable("unknown operand type");
  }
}

void ValidaMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  unsigned Opcode = MI->getOpcode();
  OutMI.setOpcode(Opcode);

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    std::optional<MCOperand> MCOp = LowerOperand(MI, MO);

    if (MCOp.has_value() && MCOp.value().isValid())
      OutMI.addOperand(MCOp.value());
  }
}

