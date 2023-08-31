//===-- ValidaLegalizerInfo.cpp -----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// This file implements the targeting of the Machinelegalizer class for Valida.
//===----------------------------------------------------------------------===//

#include "ValidaLegalizerInfo.h"
#include "llvm/CodeGen/GlobalISel/LegalizerHelper.h"
#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"

using namespace llvm;

ValidaLegalizerInfo::ValidaLegalizerInfo(const ValidaSubtarget &ST) {
  using namespace TargetOpcode;
  const LLT s8 = LLT::scalar(8);
  const LLT s16 = LLT::scalar(16);
  const LLT s32 = LLT::scalar(32);
  const LLT p0 = LLT::pointer(0, 32);

  getActionDefinitionsBuilder({G_ADD, G_SUB, G_MUL, G_AND, G_OR, G_XOR, G_SHL})
      .legalFor({p0, s32})
      .clampScalar(0, s8, s32);

  getActionDefinitionsBuilder({G_FRAME_INDEX, G_GLOBAL_VALUE}).legalFor({p0});

  getActionDefinitionsBuilder(G_BRCOND)
      .legalFor({s32})
      .minScalar(0, s32);

  getActionDefinitionsBuilder(G_ICMP)
      .legalForCartesianProduct({s32}, {s32, p0})
      .clampScalar(1, s32, s32)
      .minScalar(0, s32);

  getActionDefinitionsBuilder(G_CONSTANT)
      .legalFor({s32, p0})
      .clampScalar(0, s32, s32);

  getActionDefinitionsBuilder({G_STORE, G_LOAD})
      .legalFor({p0, s32})
      .clampScalar(0, s8, s32);

  getLegacyLegalizerInfo().computeTables();
}

bool ValidaLegalizerInfo::legalizeCustom(LegalizerHelper &Helper,
                                           MachineInstr &MI) const {
  // TODO
  //using namespace TargetOpcode;

  //switch (MI.getOpcode()) {
  //case G_LOAD:
  //case G_STORE: {
  //  MI.eraseFromParent();
  //  break;
  //}
  //default:
  //  return false;
  //}
  //return true;
}
