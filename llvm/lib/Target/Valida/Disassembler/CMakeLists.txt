add_llvm_component_library(LLVMValidaDisassembler
  ValidaDisassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  ValidaDesc
  ValidaInfo
  Support

  ADD_TO_COMPONENT
  Valida
  )
