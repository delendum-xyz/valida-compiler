//===-- ValidaBaseInfo.h - Top level definitions for VALIDA MC ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the Valida target useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//
#ifndef VALIDABASEINFO_H
#define VALIDABASEINFO_H

#include "ValidaMCTargetDesc.h"

#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_INSTRINFO_MI_OPS_INFO
#define GET_INSTRINFO_OPERAND_TYPES_ENUM
#define GET_INSTRINFO_LOGICAL_OPERAND_SIZE_MAP
#include "ValidaGenInstrInfo.inc"

namespace llvm {

/// ValidaII - This namespace holds all of the target specific flags that
/// instruction info tracks.
namespace ValidaII {
  // TODO
}

}

#endif
