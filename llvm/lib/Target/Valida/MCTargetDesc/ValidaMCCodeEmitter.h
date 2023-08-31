//===- ValidaMCCodeEmitter.h - Convert Valida Code to Machine Code --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the ValidaMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_MCTARGETDESC_VALIDAMCCODEEMITTER_H
#define LLVM_LIB_TARGET_VALIDA_MCTARGETDESC_VALIDAMCCODEEMITTER_H

#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {

class MCContext;
class MCFixup;
class MCInst;
class MCInstrInfo;
class MCSubtargetInfo;

class ValidaMCCodeEmitter : public MCCodeEmitter {
  const MCInstrInfo &MCII;
  MCContext &Ctx;

public:
  ValidaMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx)
      : MCII(MCII), Ctx(Ctx) {}
  ~ValidaMCCodeEmitter() override = default;

  // TableGen'erated function for getting the binary encoding for an instruction.
  void getBinaryCodeForInstr(const MCInst &MI, SmallVectorImpl<MCFixup> &Fixups,
                             APInt &Inst, APInt &Scratch,
                             const MCSubtargetInfo &STI) const;

  // Return binary encoding of operand.
  void getMachineOpValue(const MCInst &MI, const MCOperand &Op,
                         unsigned InsertPos, APInt &Value,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const;

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_VALIDA_MCTARGETDESC_VALIDAMCCODEEMITTER_H
