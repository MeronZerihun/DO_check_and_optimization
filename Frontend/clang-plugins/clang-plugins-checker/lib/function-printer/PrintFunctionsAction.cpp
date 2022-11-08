

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <clang/Frontend/FrontendPluginRegistry.h>
#include <string.h>

#include "PrintFunctionsAction.h"

using namespace llvm;
using namespace clang;
using namespace printfunctions;
using namespace std;

static clang::FrontendPluginRegistry::Add<PrintFunctionsAction>
    X("function-printer-demo", "Print the names of functions inside the file.");

class FunctionNameVisitor : public clang::RecursiveASTVisitor<FunctionNameVisitor>
{

  int count = 0;
  bool is_IfStmt = false;
  bool is_ArrStmt = false;
  bool contains_enc = false;
  bool arr_enc = false;

  // ---------- Helper functions ----------
  void printChildren(Stmt *st)
  {

    errs() << "\n---- Parent ------\n";
    errs() << st->getStmtClassName() << " " << st << "\n";
    for (auto c : st->children())
    {
      count++;
      errs() << "\n---- Child " + std::to_string(count) + " ------\n";
      errs() << c->getStmtClassName() << " " << c << "\n";
      printChildren(c);
    }
  }

  void printChildren(BinaryOperator *S)
  {
    for (auto st : S->children())
    {
      auto op = dyn_cast_or_null<BinaryOperator>(st);
      if (op)
        errs() << "Binary operator: " << op << " \t " << op->getLHS()->getType().getAsString() << " " << S->getRHS()->getType().getAsString() << "\n";
    }
  }

  // void analyzeRHS(Expr* E){
  //   for(auto r : E->getExprStmt()->children()){
  //     auto rhs = r->getType().getAsString();

  //   }
  // }

  void analyzeChildren_binaryOperator(BinaryOperator *S)
  {
    auto lhs = S->getLHS();
    auto rhs = S->getRHS();

    string lhs_arg_type = lhs->getType().getAsString();
    string rhs_arg_type = rhs->getType().getAsString();

    errs() << "L: " << lhs_arg_type << " " << lhs->getStmtClassName() << "\n";
    errs() << "R: " << rhs_arg_type << " " << rhs->getStmtClassName() << "\n";
    if (is_ArrStmt && (lhs_arg_type).find("enc") != string::npos)
    {
      arr_enc = true;
    }
    else if (is_IfStmt && (lhs_arg_type).find("enc") != string::npos)
    {
      contains_enc = true;
    }
    if (is_ArrStmt && (rhs_arg_type).find("enc") != string::npos)
    {
      arr_enc = true;
    }
    else if (is_IfStmt && (rhs_arg_type).find("enc") != string::npos)
    {
      contains_enc = true;
    }
    for (auto op1 : lhs->getExprStmt()->children())
    {
      errs() << "--- Left Children ---\n";
      if (auto b1 = dyn_cast_or_null<BinaryOperator>(op1))
      {
        analyzeChildren_binaryOperator(b1);
      }
      else
      {
        errs() << "N: " << op1->getStmtClassName() << "\n";
        if (auto I = dyn_cast_or_null<ImplicitCastExpr>(op1))
        {
          errs() << I->getType().getAsString() << "\n";
        }

        for (auto op : op1->children())
        {
          if (auto b = dyn_cast_or_null<BinaryOperator>(op))
          {
            analyzeChildren_binaryOperator(b);
          }
          else
          {
            for (auto op : op1->children())
            {
              if (auto b = dyn_cast_or_null<BinaryOperator>(op))
              {
                analyzeChildren_binaryOperator(b);
              }
            }
          }
        }
      }
    }
    for (auto op2 : rhs->getExprStmt()->children())
    {
      errs() << "--- Right Children ---\n";
      if (auto b2 = dyn_cast_or_null<BinaryOperator>(op2))
      {
        analyzeChildren_binaryOperator(b2);
      }
      else
      {
        errs() << "N: " << op2->getStmtClassName() << "\n";
        if (auto I = dyn_cast_or_null<ImplicitCastExpr>(op2))
        {
          errs() << I->getType().getAsString() << "\n";
        }

        for (auto op : op2->children())
        {
          if (auto b = dyn_cast_or_null<BinaryOperator>(op))
          {
            analyzeChildren_binaryOperator(b);
          }
        }
      }
    }
  }

  void analyzeChildren_implicitCast(ImplicitCastExpr *S)
  {
    string arg_type = S->getType().getAsString();
    errs()
        << "Implicit Cast: " << arg_type << "\n";
    if (is_ArrStmt && (arg_type).find("enc") != string::npos)
    {
      arr_enc = true;
    }
    else if (is_IfStmt && (arg_type).find("enc") != string::npos)
    {
      contains_enc = true;
    }
    for (auto op : S->children())
    {
      if (auto I = dyn_cast_or_null<ImplicitCastExpr>(op))
      {
        analyzeChildren_implicitCast(I);
      }
    }
  }

  void analyzeChildren_arrayExpr(ArraySubscriptExpr *S)
  {
    errs() << "Array Expr: " << S->getType().getAsString() << "\n";
    for (auto op : S->children())
    {
      errs() << "E: " << op->getStmtClassName() << "\n";
      if (auto d = dyn_cast_or_null<ImplicitCastExpr>(op))
      {
        // errs() << "Impl : " << d->getType().getAsString() << "\n";
        analyzeChildren_implicitCast(d);
      }
      else if (auto d = dyn_cast_or_null<BinaryOperator>(op))
      {
        analyzeChildren_binaryOperator(d);
      }
    }
  }

public:
  bool
  VisitFunctionDecl(clang::FunctionDecl *f)
  {
    // llvm::outs() << "Function "
    //              << (f->hasBody() ? "Def" : "Decl")
    //              << " "
    //              << f->getNameInfo().getName()
    //              << "\n";
    // f->getBody()->dumpColor();

    return true;
  }
  bool VisitStmt(Stmt *S)
  {
    // printf("------------- VistStmt ---------------");
    // printf("\n\t%s \n", s->getStmtClassName());
    if (S->getStmtClass() == Stmt::StmtClass::IfStmtClass)
    {

      errs() << "We have an if stmt\n";
    }

    return true;
  }

  bool VisitCallExpr(clang::CallExpr *E)
  {
    for (int i = 0, j = E->getNumArgs(); i < j; i++)
    {
      errs() << "Call Exp: " << E->getArg(i)->getType().getAsString() << "\n";
    }
    return true;
  }
  bool VisitDecl(Decl *D)
  {

    if (D->getBody() != NULL)
    {
      errs() << "\n\n";
      D->getBody()->dumpColor();
      if (contains_enc)
      {
        errs() << "--- Contains Enc ---\n";
      }
      if (arr_enc)
      {
        errs() << "--- Contains Arr Enc ---\n";
      }
      if (is_ArrStmt)
      {
        errs() << "--- Is Arr Stmt ---\n";
      }
      if (is_IfStmt)
      {
        errs() << "--- Is If Stmt ---\n";
      }
      contains_enc = false;
    }

    return true;
  }

  bool VisitCompoundStmt(CompoundStmt *S)
  {
    if (is_IfStmt)
    {
      is_IfStmt = false;
    }
    return true;
  }

  bool VisitIfStmt(IfStmt *S)
  {

    auto exp = S->getCond()->getExprStmt();

    errs() << "If statement: " << S->getStmtClass()
           << " : " << exp
           << "\n";
    is_IfStmt = true;

    return true;
  }
  bool VisitImplicitCastExpr(ImplicitCastExpr *S)
  {

    if (is_IfStmt)
    {
      analyzeChildren_implicitCast(S);
    }
    return true;
  }
  bool VisitBinaryOperator(BinaryOperator *S)
  {

    if (is_IfStmt)
    {
      auto lhs = S->getLHS()->getType().getAsString();
      auto rhs = S->getRHS()->getType().getAsString();
      errs() << "Binary operator: " << S->getExprStmt() << " \t "
             << lhs << " "
             << rhs << "\n";

      if (lhs.find("enc") != string::npos || rhs.find("enc") != string::npos)
      {
        contains_enc = true;
      }
      else
      {
        analyzeChildren_binaryOperator(S);
      }
    }
    is_IfStmt = false;

    return true;
  }

  bool VisitArraySubscriptExpr(ArraySubscriptExpr *S)
  {
    is_ArrStmt = true;
    analyzeChildren_arrayExpr(S);
    is_ArrStmt = false;
    return true;
  }
};

void PrintFunctionsAction::EndSourceFileAction()
{
  auto &ci = getCompilerInstance();
  auto &context = ci.getASTContext();

  auto &input = getCurrentInput();
  llvm::StringRef fileName = input.getFile();
  llvm::outs() << "Filename in Action: " << fileName << "\n";

  auto *unit = context.getTranslationUnitDecl();
  FunctionNameVisitor visitor;
  visitor.TraverseDecl(unit);

  clang::ASTFrontendAction::EndSourceFileAction();
}
