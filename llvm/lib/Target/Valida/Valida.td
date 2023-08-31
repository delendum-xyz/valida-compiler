//===-- Valida.td - Describe the Valida Target Machine -*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Calling Conv, Instruction Descriptions, Register information, etc
//===----------------------------------------------------------------------===//

include "ValidaRegisterInfo.td"
include "GISel/ValidaRegisterBanks.td"
include "ValidaSchedule.td"
include "ValidaCallingConv.td"
include "ValidaInstrInfo.td"

def ValidaInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// Valida processors supported
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic",  NoSchedModel, []>;

//===----------------------------------------------------------------------===//
// Valida Target declaration
//===----------------------------------------------------------------------===//

def Valida : Target {
  let InstructionSet = ValidaInstrInfo;
}
