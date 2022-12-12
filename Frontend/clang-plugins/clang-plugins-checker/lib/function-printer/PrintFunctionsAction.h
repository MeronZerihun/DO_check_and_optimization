
#ifndef PRINT_FUNCTIONS_ACTION_H
#define PRINT_FUNCTIONS_ACTION_H

#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include <memory>

#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <string.h>
#include <map>

using namespace llvm;
using namespace clang;
using namespace std;

namespace printfunctions
{

  class FunctionNameVisitor : public clang::RecursiveASTVisitor<FunctionNameVisitor>
  {
  private:
    ASTContext *Context;

  public:
    explicit FunctionNameVisitor(ASTContext *Context)
        : Context(Context) {}

    // map<string, int> taint_list;
    // vector<FunctionDecl *> return_list;
    // vector<string> for_vars;

    // string curr_decl = "";
    // string my_file = "";

    // int count = 0;
    // bool is_IfStmt = false;
    // bool is_ForStmt = false;
    // bool is_ArrStmt = false;
    // bool is_ReturnStmt = false;
    // FunctionDecl *curr_function;
    // string curr_function_name = "";
    // string curr_decl_type = "";
    // bool is_main = false;
    // bool contains_enc = false;
    // bool arr_enc = false;
    // bool arr_impl = false;

    // ---------- Helper functions ----------
    void PrintError(SourceLocation Loc, string errorStmt);
    void PrintWarning(SourceLocation Loc, string warningStmt);

    void analyzeChildren_binaryOperator(BinaryOperator *S);

    void analyzeChildren_implicitCast(ImplicitCastExpr *S);

    void analyzeChildren_arrayExpr(ArraySubscriptExpr *S);

  public:
    ASTContext *getContext() { return Context; }
    bool VisitFunctionDecl(clang::FunctionDecl *f);

    bool VisitStmt(Stmt *S);

    bool VisitDecl(Decl *D);

    bool VisitCompoundStmt(CompoundStmt *S);

    bool VisitIfStmt(IfStmt *S);

    bool VisitForStmt(ForStmt *S);

    bool VisitImplicitCastExpr(ImplicitCastExpr *S);

    bool VisitDeclRefExpr(DeclRefExpr *S);

    bool VisitVarDecl(VarDecl *S);

    bool VisitBinaryOperator(BinaryOperator *S);
    bool VisitArraySubscriptExpr(ArraySubscriptExpr *S);

    bool VisitReturnStmt(ReturnStmt *S);

    bool VisitInitListExpr(InitListExpr *S);

    bool VisitIntegerLiteral(IntegerLiteral *S);
  };

  class PrintFunctionsConsumer : public clang::ASTConsumer
  {
  private:
    FunctionNameVisitor _Visitor;

  public:
    explicit PrintFunctionsConsumer(ASTContext *Context)
        : _Visitor(Context) {}

    virtual void HandleTranslationUnit(clang::ASTContext &Context)
    {

      _Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
  };

  class PrintFunctionsAction : public clang::ASTFrontendAction
  {

    virtual std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef InFile) override
    {
      return std::make_unique<PrintFunctionsConsumer>(&ci.getASTContext());
    }
  };
}

#endif
