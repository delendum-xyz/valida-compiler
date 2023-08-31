//===-- ValidaAsmParser.cpp - Parse assembly to MCInst instructions -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ValidaInstrInfo.h"
#include "MCTargetDesc/ValidaMCTargetDesc.h"
#include "TargetInfo/ValidaTargetInfo.h"

#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"

#include <sstream>

#define DEBUG_TYPE "valida-asm-parser"

using namespace llvm;

namespace {

class ValidaAsmParser : public MCTargetAsmParser {

#define GET_ASSEMBLER_HEADER
#include "ValidaGenAsmMatcher.inc"

public:
  ValidaAsmParser(const MCSubtargetInfo &STI, MCAsmParser &Parser,
                    const MCInstrInfo &MII, const MCTargetOptions &Options)
      : MCTargetAsmParser(Options, STI, MII) {
    // TODO
  }

  unsigned validateTargetOperandClass(MCParsedAsmOperand &Op,
                                      unsigned Kind) override;
  bool parseRegister(MCRegister &RegNo, SMLoc &StartLoc,
                     SMLoc &EndLoc) override;
  OperandMatchResultTy tryParseRegister(MCRegister &RegNo, SMLoc &StartLoc,
                                        SMLoc &EndLoc) override;
  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override;
  bool ParseDirective(AsmToken DirectiveID) override;
  bool MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                               OperandVector &Operands, MCStreamer &Out,
                               uint64_t &ErrorInfo,
                               bool MatchingInlineAsm) override;
};

unsigned ValidaAsmParser::validateTargetOperandClass(MCParsedAsmOperand &Op,
                                                       unsigned Kind) {
  // TODO
}

bool ValidaAsmParser::parseRegister(MCRegister &RegNo, SMLoc &StartLoc,
                                      SMLoc &EndLoc) {
  // TODO
}

OperandMatchResultTy ValidaAsmParser::tryParseRegister(MCRegister &RegNo, 
                                                         SMLoc &StartLoc,
                                                         SMLoc &EndLoc) {
  // TODO
}

bool ValidaAsmParser::ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                      SMLoc NameLoc, OperandVector &Operands) {
  // TODO
}

bool ValidaAsmParser::ParseDirective(AsmToken DirectiveID) {
  // TODO
}

bool ValidaAsmParser::MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                             OperandVector &Operands, MCStreamer &Out,
                             uint64_t &ErrorInfo,
                             bool MatchingInlineAsm) {
  // TODO
}

class ValidaOperand : public MCParsedAsmOperand {
  StringRef Token;
public:
  void addRegOperands(MCInst &Inst, unsigned N) const;
  void addImmOperands(MCInst &Inst, unsigned N) const;

  StringRef getToken() const;
};

} // end anonymous namespace.

void ValidaOperand::addRegOperands(MCInst &Inst, unsigned N) const {
  // TODO
}

void ValidaOperand::addImmOperands(MCInst &Inst, unsigned N) const {
  // TODO
}

StringRef ValidaOperand::getToken() const {
  assert(isToken());
  return Token;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeValidaAsmParser() {
  RegisterMCAsmParser<ValidaAsmParser> X(getTheValidaTarget());
}

#define GET_MATCHER_IMPLEMENTATION
#include "ValidaGenAsmMatcher.inc"
