//===-- ValidaInstrInfo.cpp - Valida Instruction Information ----------===//
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

#include "ValidaInstrInfo.h"
#include "ValidaSubtarget.h"
#include "ValidaTargetMachine.h"
#include "MCTargetDesc/ValidaMCCodeEmitter.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "valida-instr-info"

#define GET_INSTRINFO_CTOR_DTOR
#include "ValidaGenInstrInfo.inc"

// Pin the vtable to this file.
void ValidaInstrInfo::anchor() {}

ValidaInstrInfo::ValidaInstrInfo(ValidaSubtarget &ST)
  : ValidaGenInstrInfo(DL::ADJCALLSTACKDOWN, DL::ADJCALLSTACKUP),
    RI(ST), Subtarget(ST) {}
