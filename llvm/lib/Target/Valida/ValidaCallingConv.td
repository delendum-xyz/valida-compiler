//===------ ValidaCallingConv.td - Calling Conventions -------*- tablegen -*-//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for Valida architecture.
//===----------------------------------------------------------------------===//

def Valida_CCallingConv : CallingConv<[
  // Handles byval parameters. Tell LLVM what the size and alignment
  // requirements of each stack slot are
  CCIfByVal<CCPassByVal<4, 4>>,

  // Promote i8/i16 to i32
  CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,

  // Integer values get stored in stack slots that are 4 bytes in size and
  // 4-byte aligned
  CCIfType<[i32], CCCustom<"CC_Valida_CAssignToStack">>,
]>;

// C Return Convention
def Valida_CRetConv : CallingConv<[
  CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,

  // Integer values get stored in stack slots that are 4 bytes in size and
  // 4-byte aligned
  CCIfType<[i32], CCCustom<"CC_Valida_RAssignToStack">>,
]>;
