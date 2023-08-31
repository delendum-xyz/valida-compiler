//===-- ValidaMCAsmInfo.cpp - Valida Asm Properties ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the ValidaMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "ValidaMCAsmInfo.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

void ValidaELFMCAsmInfo::anchor() { }

ValidaELFMCAsmInfo::ValidaELFMCAsmInfo(const Triple &T) {
  CodePointerSize = 4;
  CalleeSaveStackSlotSize = 4;

  IsLittleEndian = false;

  // Debug Information
  SupportsDebugInformation = true;

  // Exceptions handling
  ExceptionsType = ExceptionHandling::DwarfCFI;

  CommentString = ";";
}
