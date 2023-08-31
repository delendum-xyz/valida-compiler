//===-- ValidaISelLowering.h - Valida DAG Lowering Interface ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Valida uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_VALIDA_VALIDAISELLOWERING_H
#define LLVM_LIB_TARGET_VALIDA_VALIDAISELLOWERING_H

#include "Valida.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace DLISD {

/// DL Specific DAG nodes
enum NodeType {
  RET,
};

} // namespace DLISD

namespace llvm {
  class ValidaSubtarget;

  class ValidaTargetLowering : public TargetLowering {
    const ValidaSubtarget *Subtarget;
  public:
    ValidaTargetLowering(const TargetMachine &TM, const ValidaSubtarget &STI);
    SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

    bool useSoftFloat() const override;

    /// computeKnownBitsForTargetNode - Determine which of the bits specified
    /// in Mask are known to be either zero or one and return them in the
    /// KnownZero/KnownOne bitsets.
    void computeKnownBitsForTargetNode(const SDValue Op,
                                       KnownBits &Known,
                                       const APInt &DemandedElts,
                                       const SelectionDAG &DAG,
                                       unsigned Depth = 0) const override;

    MachineBasicBlock *
    EmitInstrWithCustomInserter(MachineInstr &MI,
                                MachineBasicBlock *MBB) const override;

    const char *getTargetNodeName(unsigned Opcode) const override;

    ConstraintType getConstraintType(StringRef Constraint) const override;
    ConstraintWeight
    getSingleConstraintMatchWeight(AsmOperandInfo &info,
                                   const char *constraint) const override;
    void LowerAsmOperandForConstraint(SDValue Op,
                                      std::string &Constraint,
                                      std::vector<SDValue> &Ops,
                                      SelectionDAG &DAG) const override;

    std::pair<unsigned, const TargetRegisterClass *>
    getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                                 StringRef Constraint, MVT VT) const override;

    bool isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const override;
    MVT getScalarShiftAmountTy(const DataLayout &, EVT) const override {
      // TODO
      return MVT::i32;
    }

    Register getRegisterByName(const char* RegName, LLT VT,
                               const MachineFunction &MF) const override;

    unsigned getNumRegistersForCallingConv(LLVMContext &Context,
                                           CallingConv::ID CC,
                                           EVT VT) const override;

    MVT getRegisterTypeForCallingConv(LLVMContext &Context, CallingConv::ID CC,
                                      EVT VT) const override;

    /// If a physical register, this returns the register that receives the
    /// exception address on entry to an EH pad.
    Register
    getExceptionPointerRegister(const Constant *PersonalityFn) const override {
      // TODO
    }

    /// If a physical register, this returns the register that receives the
    /// exception typeid on entry to a landing pad.
    Register
    getExceptionSelectorRegister(const Constant *PersonalityFn) const override {
      // TODO
    }

    /// Override to support customized stack guard loading.
    bool useLoadStackGuardNode() const override;
    void insertSSPDeclarations(Module &M) const override;

    /// getSetCCResultType - Return the ISD::SETCC ValueType
    EVT getSetCCResultType(const DataLayout &DL, LLVMContext &Context,
                           EVT VT) const override;

    SDValue
    LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                         const SmallVectorImpl<ISD::InputArg> &Ins,
                         const SDLoc &dl, SelectionDAG &DAG,
                         SmallVectorImpl<SDValue> &InVals) const override;

    SDValue
    LowerCall(TargetLowering::CallLoweringInfo &CLI,
              SmallVectorImpl<SDValue> &InVals) const override;

    bool CanLowerReturn(CallingConv::ID CallConv, MachineFunction &MF,
                        bool isVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        LLVMContext &Context) const override;

    SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        const SmallVectorImpl<SDValue> &OutVals,
                        const SDLoc &dl, SelectionDAG &DAG) const override;

    SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;

    bool ShouldShrinkFPConstant(EVT VT) const override {
      // Do not shrink FP constpool if VT == MVT::f128.
      // (ldd, call _Q_fdtoq) is more expensive than two ldds.
      // TODO
    }

    bool shouldInsertFencesForAtomic(const Instruction *I) const override {
      // FIXME: We insert fences for each atomics and generate
      // sub-optimal code for PSO/TSO. (Approximately nobody uses any
      // mode but TSO, which makes this even more silly)
      return true;
    }

    AtomicExpansionKind shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const override;

    void ReplaceNodeResults(SDNode *N,
                            SmallVectorImpl<SDValue>& Results,
                            SelectionDAG &DAG) const override;

  };
} // end namespace llvm

#endif    // VALIDA_ISELLOWERING_H
