//===-- Valida.h - Top-level interface for Valida representation ----*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the entry points for global functions defined in the
/// Valida target library, as used by the LLVM JIT.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_VALIDA_H
#define LLVM_LIB_TARGET_VALIDA_VALIDA_H

namespace llvm {

class FunctionPass;
class InstructionSelector;
class ValidaRegisterBankInfo;
class ValidaSubtarget;
class ValidaTargetMachine;
class PassRegistry;

FunctionPass *createValidaISelDag(ValidaTargetMachine &TM);
FunctionPass *createValidaPreLegalizeCombiner();
FunctionPass *createValidaDeadStackSlotEliminator();

InstructionSelector *
createValidaInstructionSelector(const ValidaTargetMachine &,
                                  const ValidaSubtarget &,
                                  const ValidaRegisterBankInfo &);

void initializeValidaDAGToDAGISelPass(PassRegistry &);
void initializeValidaPreLegalizerCombinerPass(PassRegistry &);
void initializeValidaDeadStackSlotEliminatorPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_VALIDA_VALIDA_H
