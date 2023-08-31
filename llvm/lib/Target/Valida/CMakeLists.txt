add_llvm_component_group(Valida)

set(LLVM_TARGET_DEFINITIONS Valida.td)

tablegen(LLVM ValidaGenGlobalISel.inc        -gen-global-isel)
tablegen(LLVM ValidaGenRegisterBank.inc      -gen-register-bank)
tablegen(LLVM ValidaGenAsmWriter.inc         -gen-asm-writer)
tablegen(LLVM ValidaGenAsmMatcher.inc        -gen-asm-matcher)
tablegen(LLVM ValidaGenDAGISel.inc           -gen-dag-isel)
tablegen(LLVM ValidaGenCallingConv.inc       -gen-callingconv)
tablegen(LLVM ValidaGenInstrInfo.inc         -gen-instr-info)
tablegen(LLVM ValidaGenRegisterInfo.inc      -gen-register-info)
tablegen(LLVM ValidaGenSubtargetInfo.inc     -gen-subtarget)
tablegen(LLVM ValidaGenMCCodeEmitter.inc     -gen-emitter)
tablegen(LLVM ValidaGenDisassemblerTable.inc -gen-disassembler)

add_public_tablegen_target(ValidaCommonTableGen)

add_llvm_target(ValidaCodeGen
  GISel/ValidaCallLowering.cpp
  GISel/ValidaLegalizerInfo.cpp
  GISel/ValidaRegisterBankInfo.cpp
  GISel/ValidaInstructionSelector.cpp
  ValidaAsmPrinter.cpp
  ValidaDeadStackSlotEliminator.cpp
  ValidaInstrInfo.cpp
  ValidaISelDAGToDAG.cpp
  ValidaISelLowering.cpp
  ValidaFrameLowering.cpp
  ValidaMachineFunction.cpp
  ValidaMCInstLower.cpp
  ValidaPreLegalizerCombiner.cpp
  ValidaSubtarget.cpp
  ValidaRegisterInfo.cpp
  ValidaTargetMachine.cpp
  ValidaTargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  GlobalISel
  MC
  SelectionDAG
  Support
  Target
  ValidaDesc
  ValidaInfo

  ADD_TO_COMPONENT
  Valida
)

add_subdirectory(Disassembler)
add_subdirectory(AsmParser)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
