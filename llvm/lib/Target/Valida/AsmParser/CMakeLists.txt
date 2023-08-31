add_llvm_component_library(LLVMValidaAsmParser
  ValidaAsmParser.cpp

  LINK_COMPONENTS
  MC
  MCParser
  Support
  ValidaCodeGen
  ValidaInfo

  ADD_TO_COMPONENT
  Valida
)
