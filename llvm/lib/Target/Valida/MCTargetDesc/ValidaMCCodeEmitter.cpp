//===-- ValidaMCCodeEmitter.cpp - Valida code emitter -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains definitions for the Valida code emitter.
///
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/ValidaMCCodeEmitter.h"
#include "MCTargetDesc/ValidaBaseInfo.h"
//#include "MCTargetDesc/ValidaFixupKinds.h"
#include "MCTargetDesc/ValidaMCTargetDesc.h"

#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"
#include <type_traits>

using namespace llvm;

void ValidaMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                              SmallVectorImpl<MCFixup> &Fixups,
                                              const MCSubtargetInfo &STI) const {
  // TODO
}

// Return binary encoding of operand.
void ValidaMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &Op,
                       unsigned InsertPos, APInt &Value,
                       SmallVectorImpl<MCFixup> &Fixups,
                       const MCSubtargetInfo &STI) const {
}

MCCodeEmitter *llvm::createValidaMCCodeEmitter(const MCInstrInfo &MCII,
                                                      MCContext &Ctx) {
  return new ValidaMCCodeEmitter(MCII, Ctx);
}

#include "ValidaGenMCCodeEmitter.inc"
