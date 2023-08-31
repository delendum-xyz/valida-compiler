add_llvm_component_library(LLVMValidaInfo
  ValidaTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  Valida
  )
