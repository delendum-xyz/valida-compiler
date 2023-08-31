//===-- ValidaRegisterBanks.td ---------------------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Define the Valida register banks used for GlobalISel.
///
//===----------------------------------------------------------------------===//

/// General Purpose Registers
def I32RegBank : RegisterBank<"AnyGPR", [I32]>;
