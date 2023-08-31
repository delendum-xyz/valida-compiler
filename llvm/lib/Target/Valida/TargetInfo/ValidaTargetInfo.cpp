//===-- ValidaTargetInfo.cpp - Valida Target Implementation ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains Valida target initializer.
///
//===----------------------------------------------------------------------===//
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

namespace llvm {
Target &getTheValidaTarget() {
  static Target TheValidaTarget;
  return TheValidaTarget;
}
} // namespace llvm

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeValidaTargetInfo() {
  RegisterTarget<Triple::valida, /*HasJIT=*/false>
    X(getTheValidaTarget(), "valida", "Valida", "Valida");
}
