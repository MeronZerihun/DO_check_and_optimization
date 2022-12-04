CXX = g++
CFLAGS = -fno-rtti

LLVM_SRC_PATH = ___PATH-TO-LLVM-SOURCE-DIR___
LLVM_BUILD_PATH = ___PATH-TO-LLVM-BUILD-DIR___

LLVM_BIN_PATH = $(LLVM_BUILD_PATH)/Debug+Asserts/bin
LLVM_LIBS=core mc
LLVM_CONFIG_COMMAND = $(LLVM_BIN_PATH)/llvm-config --cxxflags --ldflags \
                                        --libs $(LLVM_LIBS)
CLANG_BUILD_FLAGS = -I$(LLVM_SRC_PATH)/tools/clang/include \
                                      -I$(LLVM_BUILD_PATH)/tools/clang/include

CLANGLIBS = \
  -lclangFrontendTool -lclangFrontend -lclangDriver \
  -lclangSerialization -lclangCodeGen -lclangParse \
  -lclangSema -lclangStaticAnalyzerFrontend \
  -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore \
  -lclangAnalysis -lclangARCMigrate -lclangRewrite \
  -lclangEdit -lclangAST -lclangLex -lclangBasic

all: rewritersample

rewritersample: rewritersample.cpp\
      $(CXX) rewritersample.cpp $(CFLAGS) -o rewritersample \
              $(CLANG_BUILD_FLAGS) $(CLANGLIBS) `__abENT__lt;__abENT__#8260;span__abENT__gt;__abENT__lt;span style=__abENT__quot;color: #00007f; font-weight: bold__abENT__quot;__abENT__gt;$(__abENT__lt;__abENT__#8260;span__abENT__gt;LLVM_CONFIG_COMMAND__abENT__lt;span style=__abENT__quot;color: #00007f; font-weight: bold__abENT__quot;__abENT__gt;)__abENT__lt;__abENT__#8260;span__abENT__gt;__abENT__lt;span style=__abENT__quot;color: #7f007f__abENT__quot;__abENT__gt;`

clean:\
      rm -rf *.o *.ll rewritersample