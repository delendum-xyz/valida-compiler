//===-- ValidaTargetMachine.h - Define TargetMachine for Valida -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Valida specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef VALIDATARGETMACHINE_H
#define VALIDATARGETMACHINE_H

#include "ValidaSubtarget.h"
#include "MCTargetDesc/ValidaMCTargetDesc.h"

#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class formatted_raw_ostream;

class ValidaTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  ValidaSubtarget Subtarget;

  mutable StringMap<std::unique_ptr<ValidaSubtarget>> SubtargetMap;

public:
  ValidaTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                        StringRef FS, const TargetOptions &Options,
                        std::optional<Reloc::Model> RM,
                        std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
                        bool JIT);

  const ValidaSubtarget *getSubtargetImpl() const { return &Subtarget; }
  const ValidaSubtarget *getSubtargetImpl(const Function &F) const override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
} // End llvm namespace

#endif
