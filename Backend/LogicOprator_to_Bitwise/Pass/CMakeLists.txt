add_llvm_library( ProjectBackend1 MODULE
  LogicOperators_to_BitwisePass.cpp

  PLUGIN_TOOL
  opt
  )