//===-- ValidaInstPrinter.cpp - Convert MCInst to assembly syntax --------==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an Valida MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "ValidaInstPrinter.h"
#include "Valida.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define GET_INSTRUCTION_NAME
#define PRINT_ALIAS_INSTR
#include "ValidaGenAsmWriter.inc"

void ValidaInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) const {
  OS << '%' << StringRef(getRegisterName(Reg)).lower();
}

void ValidaInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                    StringRef Annot, 
                                    const MCSubtargetInfo &STI,
                                    raw_ostream &O) {
  if (!printAliasInstr(MI, Address, O) &&
      !printValidaAliasInstr(MI, O))
    printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

bool ValidaInstPrinter::printValidaAliasInstr(const MCInst *MI,
                                                  raw_ostream &O) {
  return false;
}

void ValidaInstPrinter::printOperand(const MCInst *MI, int opNum,
                                       raw_ostream &O) {
  MCOperand Op = MI->getOperand(opNum);

  if (Op.isReg()) {
    O << "%" << getRegisterName(Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  assert(Op.isExpr() && "unknown operand kind in printOperand");
  Op.getExpr()->print(O, &MAI, true);
}

void ValidaInstPrinter::printImmOp32(const MCInst *MI, int opNum, 
                                      raw_ostream &OS, const char *Modifier) {
  printOperand(MI, opNum, OS);
}
void ValidaInstPrinter::printFPOp32(const MCInst *MI, int opNum,
                                      raw_ostream &OS, const char *Modifier) {
  printOperand(MI, opNum, OS);
  OS << "(fp)";
}
