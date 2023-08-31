//===-- ValidaInstrInfo.h - Valida Instruction Information --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Valida implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_VALIDAINSTRINFO_H
#define LLVM_LIB_TARGET_VALIDA_VALIDAINSTRINFO_H

#include "Valida.h"
#include "ValidaRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_INSTRINFO_HEADER
#include "ValidaGenInstrInfo.inc"

namespace llvm {

class ValidaInstrInfo : public ValidaGenInstrInfo {
  const ValidaRegisterInfo RI;
  const ValidaSubtarget& Subtarget;
  virtual void anchor();

public:
  explicit ValidaInstrInfo(ValidaSubtarget &ST);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  const ValidaRegisterInfo &getRegisterInfo() const { return RI; }
};
}

#endif
