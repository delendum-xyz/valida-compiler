//===-- ValidaMCTargetDesc.cpp - Valida Target Descriptions -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Valida specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "ValidaMCTargetDesc.h"
#include "ValidaInstPrinter.h"
#include "ValidaMCAsmInfo.h"
#include "TargetInfo/ValidaTargetInfo.h"

#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "ValidaGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "ValidaGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "ValidaGenRegisterInfo.inc"

static MCAsmInfo *createValidaMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT,
                                       const MCTargetOptions &Options) {
  // TODO
  MCAsmInfo *MAI = new ValidaELFMCAsmInfo(TT);
  return MAI;
}

static MCRegisterInfo *createValidaMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitValidaMCRegisterInfo(X, 0, 0, 0, 0);
  return X;
}

static MCSubtargetInfo *
createValidaMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  if (CPU.empty())
    CPU = "";
  return createValidaMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCInstrInfo *createValidaMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitValidaMCInstrInfo(X); // defined in ValidaGenInstrInfo.inc
  return X;
}

static MCInstPrinter *createValidaMCInstPrinter(const Triple &T,
                                                  unsigned SyntaxVariant,
                                                  const MCAsmInfo &MAI,
                                                  const MCInstrInfo &MII,
                                                  const MCRegisterInfo &MRI) {
  return new ValidaInstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializeValidaTargetMC() {
  Target &T = getTheValidaTarget();

  RegisterMCAsmInfoFn X(T, createValidaMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(T, createValidaMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(T, createValidaMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(T, createValidaMCSubtargetInfo);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(T, createValidaMCInstPrinter);

  // Register the code emitter.
  TargetRegistry::RegisterMCCodeEmitter(T, createValidaMCCodeEmitter);
}
