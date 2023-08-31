//===-- ValidaDisassembler.cpp - Disassembler for Valida ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is part of the Valida Disassembler.
//
//===----------------------------------------------------------------------===//

#include "Valida.h"
#include "ValidaRegisterInfo.h"
#include "ValidaSubtarget.h"
#include "MCTargetDesc/ValidaMCCodeEmitter.h"
#include "MCTargetDesc/ValidaMCTargetDesc.h"
#include "TargetInfo/ValidaTargetInfo.h"

#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCDecoderOps.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "valida-disassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

static DecodeStatus DecodeImmOp32(MCInst &Inst, uint64_t Imm, uint64_t Address,
                                  const void *Decoder) {
  Inst.addOperand(MCOperand::createImm(Imm));
  return DecodeStatus::Success;
}

static DecodeStatus DecodeI32RegisterClass(MCInst &Inst, uint64_t RegNo,
                                           uint64_t Address,
                                           const void *Decoder) {
  if (RegNo >= 4)
    return DecodeStatus::Fail;
  static unsigned I32List[] = {DL::PC, DL::FP};
  Inst.addOperand(MCOperand::createReg(I32List[RegNo]));
  return DecodeStatus::Success;
}

#include "ValidaGenDisassemblerTable.inc"

/// A disassembler class for Valida.
struct ValidaDisassembler : public MCDisassembler {
  ValidaDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}
  virtual ~ValidaDisassembler() {}

  DecodeStatus getInstruction(MCInst &Instr, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};

DecodeStatus ValidaDisassembler::getInstruction(MCInst &Instr, uint64_t &Size,
                                                  ArrayRef<uint8_t> Bytes,
                                                  uint64_t Address,
                                                  raw_ostream &CStream) const {
  DecodeStatus Result;
  auto MakeUp = [&](APInt &Insn, unsigned InstrBits) {
    unsigned Idx = Insn.getBitWidth() >> 3;
    unsigned RoundUp = alignTo(InstrBits, Align(32));
    if (RoundUp > Insn.getBitWidth())
      Insn = Insn.zext(RoundUp);
    RoundUp = RoundUp >> 3;
    for (; Idx < RoundUp; Idx += 4) {
      Insn.insertBits(support::endian::read32be(&Bytes[Idx]), Idx * 8, 32);
    }
  };

  APInt Insn(32, support::endian::read32be(Bytes.data()));
  // 4 bytes of data are consumed, so set Size to 4
  // If we don't do this, disassembler may generate result even
  // the encoding is invalid. We need to let it fail correctly.
  Size = 4;
  Result = decodeInstruction(DecoderTableValida192, Instr, Insn, Address,
                             this, STI, MakeUp);
  if (Result == DecodeStatus::Success)
    Size = InstrLenTable[Instr.getOpcode()] >> 3;

  LLVM_DEBUG(llvm::dbgs() << "Disassemble: "; Instr.dump());
  return Result;
}

static MCDisassembler *createValidaDisassembler(const Target &T,
                                                  const MCSubtargetInfo &STI,
                                                  MCContext &Ctx) {
  return new ValidaDisassembler(STI, Ctx);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeValidaDisassembler() {
  // Register the disassembler.
  TargetRegistry::RegisterMCDisassembler(getTheValidaTarget(),
                                         createValidaDisassembler);
}
