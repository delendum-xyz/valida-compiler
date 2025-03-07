//===-- ValidaLegalizerInfo ---------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// This file declares the targeting of the MachineLegalizer class for
/// Valida.
/// \todo This should be generated by TableGen.
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_GLSEL_VALIDALEGALIZERINFO_H
#define LLVM_LIB_TARGET_VALIDA_GLSEL_VALIDALEGALIZERINFO_H

#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"

namespace llvm {

class ValidaSubtarget;

/// This struct provides the information for the target register banks.
struct ValidaLegalizerInfo : public LegalizerInfo {
public:
  ValidaLegalizerInfo(const ValidaSubtarget &ST);

  bool legalizeCustom(LegalizerHelper &Helper, MachineInstr &MI) const override;
};
} // end namespace llvm
#endif // LLVM_LIB_TARGET_VALIDA_GLSEL_VALIDALEGALIZERINFO_H
