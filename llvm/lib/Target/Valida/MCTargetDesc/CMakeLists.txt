add_llvm_component_library(LLVMValidaDesc
  ValidaInstPrinter.cpp
  ValidaMCAsmInfo.cpp
  ValidaMCCodeEmitter.cpp
  ValidaMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  Support
  TargetParser
  ValidaInfo

  ADD_TO_COMPONENT
  Valida
)
