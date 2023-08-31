//===-- ValidaCallingConv.h - Valida Custom CC Routines -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the custom routines for the Valida Calling Convention
/// that aren't done by tablegen.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_VALIDACALLINGCONV_H
#define LLVM_LIB_TARGET_VALIDA_VALIDACALLINGCONV_H

#include "MCTargetDesc/ValidaMCTargetDesc.h"

#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Function.h"

namespace llvm {

struct ValidaCCState : public CCState {
  ArrayRef<Type *> ArgTypeList;

  ValidaCCState(ArrayRef<Type *> ArgTypes, CallingConv::ID CC, bool IsVarArg,
              MachineFunction &MF, SmallVectorImpl<CCValAssign> &Locs,
              LLVMContext &C)
      : CCState(CC, IsVarArg, MF, Locs, C), ArgTypeList(ArgTypes) {}
};

inline bool CC_Valida_CAssignToStack(unsigned &ValNo, MVT &ValVT, MVT &LocVT,
                                       CCValAssign::LocInfo &LocInfo,
                                       ISD::ArgFlagsTy &ArgFlags, CCState &State) {
  if (LocVT == MVT::i32) {
    unsigned Offset1 = State.AllocateStack(4, Align(4));
    // We need to reserve space for the return address (at fp), the 
    // return value (at fp+4), and the current frame pointer (at fp+8)
    // Function arguments are stored at fp+12, fp+16, etc.
    Offset1 += 12;
    State.addLoc(CCValAssign::getMem(ValNo, ValVT, Offset1, LocVT, LocInfo));
    return true;
  }

  return false; // CC didn't match.
}

inline bool CC_Valida_RAssignToStack(unsigned &ValNo, MVT &ValVT, MVT &LocVT,
                                       CCValAssign::LocInfo &LocInfo,
                                       ISD::ArgFlagsTy &ArgFlags, CCState &State) {
  if (LocVT == MVT::i32) {
    // The return value is located at fp+4
    unsigned Offset1 = State.AllocateStack(4, Align(4));
    Offset1 += 4;
    State.addLoc(CCValAssign::getMem(ValNo, ValVT, Offset1, LocVT, LocInfo));
    return true;
  }

  return false; // CC didn't match.
}

} // namespace llvm

#endif // LLVM_LIB_TARGET_VALIDA_VALIDACALLINGCONV_H
