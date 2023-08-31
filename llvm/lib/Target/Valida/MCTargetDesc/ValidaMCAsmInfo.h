//===-- ValidaMCAsmInfo.h - Valida Asm Info ------------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the ValidaMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_MCTARGETDESC_VALIDAMCASMINFO_H
#define LLVM_LIB_TARGET_VALIDA_MCTARGETDESC_VALIDAMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"
#include "llvm/TargetParser/Triple.h"

namespace llvm {
  class StringRef;
  class Target;

  class ValidaELFMCAsmInfo : public MCAsmInfoELF {
    virtual void anchor();
  public:
    explicit ValidaELFMCAsmInfo(const Triple &T);
  };

} // namespace llvm

#endif
