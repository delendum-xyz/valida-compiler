//===-- ValidaISelDAGToDAG.cpp - A dag to dag inst selector for Valida ------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the Valida target.
//
//===----------------------------------------------------------------------===//

#include "Valida.h"

#include "ValidaMachineFunction.h"
#include "ValidaTargetMachine.h"
#include "ValidaRegisterInfo.h"

#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;


#define DEBUG_TYPE "valida-isel"
#define PASS_NAME "Valida DAG->DAG Pattern Instruction Selection"

//===----------------------------------------------------------------------===//
// Instruction Selector Implementation
//===----------------------------------------------------------------------===//

//===--------------------------------------------------------------------===//
/// ValidaDAGToDAGISel - Valida specific code to select Valida machine
/// instructions for SelectionDAG operations.
///
namespace {
class ValidaDAGToDAGISel : public SelectionDAGISel {
  /// Subtarget - Keep a pointer to the Valida Subtarget around so that we can
  /// make the right decision when generating code for different targets.
  const ValidaSubtarget *Subtarget = nullptr;
public:
  static char ID;

  ValidaDAGToDAGISel() = delete;

  explicit ValidaDAGToDAGISel(ValidaTargetMachine &tm) : SelectionDAGISel(ID, tm) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
    Subtarget = &MF.getSubtarget<ValidaSubtarget>();
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

  void Select(SDNode *N) override;

  // Complex Pattern Selectors.
  bool SelectADDRrr(SDValue N, SDValue &R1, SDValue &R2);
  bool SelectADDRri(SDValue N, SDValue &Base, SDValue &Offset);

  bool SelectAddr(SDNode *Parent, SDValue N, SDValue &Base, SDValue &Offset);

  /// SelectInlineAsmMemoryOperand - Implement addressing mode selection for
  /// inline asm expressions.
  bool SelectInlineAsmMemoryOperand(const SDValue &Op,
                                    unsigned ConstraintID,
                                    std::vector<SDValue> &OutOps) override;


private:
  SDNode* getGlobalBaseReg();
  bool tryInlineAsm(SDNode *N);

  // Include the pieces autogenerated from the target description.
#include "ValidaGenDAGISel.inc"
};
}  // end anonymous namespace

char ValidaDAGToDAGISel::ID = 0;

INITIALIZE_PASS(ValidaDAGToDAGISel, DEBUG_TYPE, PASS_NAME, false, false)

SDNode* ValidaDAGToDAGISel::getGlobalBaseReg() {
  // TODO
}

bool ValidaDAGToDAGISel::SelectADDRrr(SDValue N, SDValue &R1, SDValue &R2) {
  // TODO
}

bool ValidaDAGToDAGISel::SelectADDRri(SDValue N, SDValue &Base, SDValue &Offset) {
  // TODO
}

bool ValidaDAGToDAGISel::SelectAddr(SDNode *Parent, SDValue N, 
                                      SDValue &Base, SDValue &Offset) {
  // TODO
}

bool ValidaDAGToDAGISel::tryInlineAsm(SDNode *N){
  // TODO
}

void ValidaDAGToDAGISel::Select(SDNode *N) {
  SelectCode(N);
}


/// SelectInlineAsmMemoryOperand - Implement addressing mode selection for
/// inline asm expressions.
bool
ValidaDAGToDAGISel::SelectInlineAsmMemoryOperand(const SDValue &Op,
                                                   unsigned ConstraintID,
                                                   std::vector<SDValue> &OutOps) {
  SDValue Op0, Op1;
  switch (ConstraintID) {
  default: return true;
  case InlineAsm::Constraint_o:
  case InlineAsm::Constraint_m: // memory
   if (!SelectADDRrr(Op, Op0, Op1))
     SelectADDRri(Op, Op0, Op1);
   break;
  }

  OutOps.push_back(Op0);
  OutOps.push_back(Op1);
  return false;
}

/// createValidaISelDag - This pass converts a legalized DAG into a
/// Valida-specific DAG, ready for instruction scheduling.
///
FunctionPass *llvm::createValidaISelDag(ValidaTargetMachine &TM) {
  return new ValidaDAGToDAGISel(TM);
}
