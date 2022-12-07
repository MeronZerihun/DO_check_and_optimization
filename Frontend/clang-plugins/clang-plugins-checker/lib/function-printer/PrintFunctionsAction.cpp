#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
// #include <clang/Frontend/FrontendPluginRegistry.h>
#include <string.h>
#include <cctype>
#include <map>

#include "PrintFunctionsAction.h"

using namespace llvm;
using namespace clang;
using namespace printfunctions;

using namespace std;


map<string, int> taint_list;
vector<FunctionDecl *> return_list;
vector<string> for_vars;
map<int, int> for_lines;

string curr_decl = "";
string my_file = "";

bool is_IfStmt = false;
bool is_new = false;
bool is_ForStmt = false;
bool is_ArrStmt = false;
bool is_ArrDecl = false;
bool is_ReturnStmt = false;
FunctionDecl *curr_function;
string curr_function_name = "";
string curr_decl_type = "";
bool is_main = false;
bool contains_enc = false;
bool for_enc = false;
bool arr_enc = false;
bool arr_impl = false;
bool op = false;
bool is_assignment = false;
IfStmt *if_stmt;
ArraySubscriptExpr *arr_stmt;
ForStmt *for_stmt;
VarDecl *arr_decl;

void FunctionNameVisitor::PrintError(SourceLocation Loc, string errorStmt)

{

    auto Context = getContext();
    // FullSourceLoc FullLocation = Context->getFullLoc(S->getBeginLoc());
    FullSourceLoc FullLocation = Context->getFullLoc(Loc);

    string src(FullLocation.getCharacterData());
    std::size_t new_ln = src.find("\n");

    std::string err = src.substr(0, new_ln);
    auto found = (taint_list.begin())->first;
    string err_print = err;
    for (auto it = taint_list.begin(); it != taint_list.end(); it++)
    {
        // errs() << "Print Error- " << it->first << "\n ";
        // std::cout << it->first // string (key)
        //           << ':'
        //           << it->second // string's value
        //           << std::endl;
        auto pos = it->first;
        auto temp = err.find(pos);
        if (temp != string::npos)
        {
            auto tmp = it->first;
            // found = it->first;
            if (tmp.length() == 1)
            {
                auto nxt = err.substr(temp, temp + 1);
                int r = isalpha(nxt.at(1));
                if (r == 0)
                {
                    found = tmp;
                    err_print = err.substr(0, temp) + "\e[0;33m" + err.substr(temp) + "\e[0m";
                }
            }
            else
            {
                found = tmp;
                err_print = err.substr(0, temp) + "\e[0;33m" + err.substr(temp) + "\e[0m";
            }
        }
    }
    string err_cls = errorStmt;
    string desc = "";
    if (errorStmt.find(">") != string::npos)
    {
        err_cls = errorStmt.substr(0, errorStmt.find(">"));
        desc = errorStmt.substr(errorStmt.find(">") + 1);
    }

    if (FullLocation.isValid() && for_lines.find(FullLocation.getSpellingLineNumber()) == for_lines.end())
    {
        if (for_enc)
        {
            for_lines[FullLocation.getSpellingLineNumber()] = 1;
            for_enc = false;
        }
        llvm::outs() << "\e[1m" << FullLocation.getFileEntry()->getName() << ":"
                     << FullLocation.getSpellingLineNumber()
                     << ":"
                     << FullLocation.getSpellingColumnNumber()
                     << "\e[1;31m Error: \e[0m\e[1m"
                     << err_cls << ": " << desc << " \'\e[1;33m" << found << "\e[0m\'"
                     << "\e[0m \n"
                     << FullLocation.getSpellingLineNumber() << " | \t" + err_print
                     << "\n";
    }
}

void FunctionNameVisitor::PrintWarning(SourceLocation Loc, string warningStmt)
{

    auto Context = getContext();
    FullSourceLoc FullLocation = Context->getFullLoc(Loc);

    string src(FullLocation.getCharacterData());
    std::size_t new_ln = src.find("\n");

    std::string err = src.substr(0, new_ln);
    auto found = (taint_list.begin())->first;
    string err_print = err;
    for (auto it = taint_list.begin(); it != taint_list.end(); it++)
    {
        auto pos = it->first;
        auto temp = err.find(pos);
        if (temp != string::npos)
        {
            found = it->first;
            err_print = err.substr(0, temp) + "\e[0;33m" + err.substr(temp) + "\e[0m";
        }
    }
    string err_cls = warningStmt;
    string desc = "";
    if (warningStmt.find(">") != string::npos)
    {
        err_cls = warningStmt.substr(0, warningStmt.find(">"));
        desc = warningStmt.substr(warningStmt.find(">") + 1);
    }

    if (FullLocation.isValid())
    {
        llvm::outs() << "\e[1m" << FullLocation.getFileEntry()->getName() << ":"
                     << FullLocation.getSpellingLineNumber()
                     << ":"
                     << FullLocation.getSpellingColumnNumber()
                     << "\e[1;35m Warning: \e[0m\e[1m"
                     << err_cls << ": " << desc << " \'\e[1;33m" << found << "\e[0m\'"
                     << "\e[0m \n"
                     << FullLocation.getSpellingLineNumber() << " | \t" + err_print
                     << "\n";
    }
}

// ---------- Helper functions ----------
// void printChildren(Stmt *st)
// {

//     errs() << "\n---- Parent ------\n";
//     errs() << st->getStmtClassName() << " " << st << "\n";
//     for (auto c : st->children())
//     {
//         // errs() << "\n---- Child " + std::to_string(count) + " ------\n";
//         errs() << c->getStmtClassName() << " " << c << "\n";
//         printChildren(c);
//     }
// }

// void printChildren(BinaryOperator *S)
// {
//     for (auto st : S->children())
//     {
//         auto op = dyn_cast_or_null<BinaryOperator>(st);
//         // if (op)
//         //     errs() << "Binary operator: " << op << " \t " << op->getLHS()->getType().getAsString() << " " << S->getRHS()->getType().getAsString() << "\n";
//     }
// }

void FunctionNameVisitor::analyzeChildren_binaryOperator(BinaryOperator *S)
{
    auto lhs = S->getLHS();
    auto rhs = S->getRHS();

    string lhs_arg_type = lhs->getType().getAsString();
    string rhs_arg_type = rhs->getType().getAsString();

    // errs() << "L: " << lhs_arg_type << " " << lhs->getStmtClassName() << "\n";
    // errs() << "R: " << rhs_arg_type << " " << rhs->getStmtClassName() << "\n";
    if (is_new && !(curr_decl).empty() && ((lhs_arg_type).find("enc") != string::npos || (rhs_arg_type).find("enc") != string::npos))
    {
        taint_list[curr_decl] = 1;
        // errs() << "BIN OP Analysis: Adding - " << curr_decl << "\n";
        curr_decl = "";
        is_new = false;
    }
    if (is_ArrStmt && (lhs_arg_type).find("enc") != string::npos)
    {
        arr_enc = true;
        // errs() << "##### ARR_BIN: NOT OBLIVIOUS #####";
        // PrintError(S->getBeginLoc(), "1. Non-oblivious ARRAY Access");
        // is_ArrStmt = false;
    }
    else if (is_IfStmt && (lhs_arg_type).find("enc") != string::npos)
    {
        contains_enc = true;

        // PrintError(S->getBeginLoc(), "4.Non-oblivious IF statment");
        // is_IfStmt = false;
    }
    if (is_ArrStmt && (rhs_arg_type).find("enc") != string::npos)
    {
        arr_enc = true;
    }
    else if (is_IfStmt && (rhs_arg_type).find("enc") != string::npos)
    {
        contains_enc = true;
        // PrintError(S->getBeginLoc(), "1.Non-oblivious IF statment");
        // is_IfStmt = false;
    }
    for (auto op1 : lhs->getExprStmt()->children())
    {
        // errs() << "--- Left Children ---\n";
        if (auto b1 = dyn_cast_or_null<BinaryOperator>(op1))
        {
            analyzeChildren_binaryOperator(b1);
        }
        else
        {
            // errs() << "N: " << op1->getStmtClassName() << "\n";
            // if (auto I = dyn_cast_or_null<ImplicitCastExpr>(op1))
            // {
            // errs() << I->getType().getAsString() << "\n";
            // }

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
        // errs() << "--- Right Children ---\n";
        if (auto b2 = dyn_cast_or_null<BinaryOperator>(op2))
        {
            analyzeChildren_binaryOperator(b2);
        }
        else
        {
            // errs() << "N: " << op2->getStmtClassName() << "\n";
            // if (auto I = dyn_cast_or_null<ImplicitCastExpr>(op2))
            // {
            // errs() << I->getType().getAsString() << "\n";
            // }

            for (auto op : op2->children())
            {
                if (auto b = dyn_cast_or_null<BinaryOperator>(op))
                {
                    analyzeChildren_binaryOperator(b);
                }
            }
        }
    }
    // is_new = false;
}

void FunctionNameVisitor::analyzeChildren_implicitCast(ImplicitCastExpr *S)
{
    string arg_type = S->getType().getAsString();
    // errs()
    //     << "Implicit Cast: " << arg_type << "\n";
    if (is_ArrStmt && arr_impl && (arg_type).find("enc") != string::npos)
    {
        arr_enc = true;
        // errs() << "##### ARR_IMPL: NOT OBLIVIOUS #####\n";
        // PrintError(S->getBeginLoc(), "2. Non-oblivious ARRAY Access");
        // is_ArrStmt = false;
        // arr_impl = false;
    }
    else if (is_IfStmt && (arg_type).find("enc") != string::npos)
    {
        contains_enc = true;
        // PrintError(S->getBeginLoc(), "2.Non-oblivious IF statment");
        // is_IfStmt = false;
    }
    else if (is_assignment)
    {
        // errs() << "Is assignment\n";
        is_assignment = false;
    }
    for (auto op : S->children())
    {
        if (auto I = dyn_cast_or_null<ImplicitCastExpr>(op))
        {
            analyzeChildren_implicitCast(I);
        }
    }
}

void FunctionNameVisitor::analyzeChildren_arrayExpr(ArraySubscriptExpr *S)
{
    // errs() << "Array Expr: " << S->getType().getAsString() << "\n";
    for (auto op : S->children())
    {
        // errs() << "E: " << op->getStmtClassName() << "\n";
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

bool FunctionNameVisitor::VisitStmt(Stmt *S)
{
    // auto Context = getContext();
    // FullSourceLoc FullLocation = Context->getFullLoc(S->getBeginLoc());

    // if (S->getStmtClass() == Stmt::StmtClass::IfStmtClass)
    // {
    //     PrintError(S->getBeginLoc(), "Print Error: We have an IF stmt");
    // if (FullLocation.isValid())
    // {
    //     llvm::outs() << FullLocation.getSpellingLineNumber() << ":"
    //                  << FullLocation.getSpellingColumnNumber()
    //                  << " We have an if stmt"
    //                  << "\n";
    // }
    // }
    if (is_IfStmt && contains_enc)
    {
        // errs() << "##### IF: NOT OBLIVIOUS #####\n";
        // if (FullLocation.isValid())
        // {
        //     llvm::outs() << FullLocation.getSpellingLineNumber() << ":"
        //                  //  << FullLocation.getSpellingColumnNumber()
        //                  << " "
        //                  //  << FullLocation.getCharacterData()
        //                  << " Non-oblivious IF statment"
        //                  << "\n";
        // }

        PrintError(if_stmt->getBeginLoc(), "Non-oblivious IF Statement>Illegal use of secret value in condition");

        is_IfStmt = false;
        contains_enc = false;
    }
    else if (is_ArrStmt && arr_enc)
    {
        PrintError(arr_stmt->getBeginLoc(), "Non-oblivious Array Access>Illegal use of secret value during array access");

        is_ArrStmt = false;
        arr_enc = false;
    }
    else if (arr_enc)
    {
        PrintError(arr_decl->getBeginLoc(), "Non-oblivious Array Declaration>Illegal use of secret value in array declaration");
        arr_enc = false;
    }

    else if (for_enc)
    {
        // errs() << "##### FOR: NOT OBLIVIOUS #####\n";
        // if (FullLocation.isValid())
        // {
        //     llvm::outs() << FullLocation.getSpellingLineNumber() << ":"
        //                  << FullLocation.getSpellingColumnNumber()
        //                  //  << FullLocation.getCharacterData()
        //                  << "  Non-oblivious FOR statment"
        //                  << "\n";
        // }
        PrintError(for_stmt->getBeginLoc(), "Non-oblivious FOR STATEMENT>Illegal use of secret value in for loop");
        for_enc = false;
        // is_ForStmt = false;
        // for_lines
    }

    return true;
}

bool FunctionNameVisitor::VisitFunctionDecl(clang::FunctionDecl *f)
{
    // errs() << "FUNC DECL\n";
    // auto Context = getContext();
    // FullSourceLoc FullLocation = Context->getFullLoc(f->getBeginLoc());
    curr_function_name = f->getNameInfo().getName().getAsString();
    if (curr_function_name == "main")
    {
        //     errs() << FullLocation.getCharacterData() << "\n";
        is_main = true;
    }
    // errs() << "Function: " << curr_function_name << " " << (f->getAsFunction()) << "\n";
    // errs() << FullLocation.getCharacterData() << "\n";
    // if (curr_function_name == "bar1")
    curr_function = f->getAsFunction();
    // return_list
    // llvm::outs() << "Function "
    //              << (f->hasBody() ? "Def" : "Decl")
    //              << " "
    //              << f->getNameInfo().getName()
    //              << "\n";
    // f->getBody()->dump();

    return true;
}

bool FunctionNameVisitor::VisitDecl(Decl *D)
{
    if (D->getBody() != NULL)
    {
        // errs() << "\n\n";
        // D->getBody()->dumpColor();
        // if (contains_enc)
        // {
        //     errs() << "--- Contains Enc ---\n";
        // }
        // if (arr_enc)
        // {
        //     errs() << "--- Contains Arr Enc ---\n";
        // }
        // if (is_ArrStmt)
        // {
        //     errs() << "--- Is Arr Stmt ---\n";
        // }
        // if (is_IfStmt)
        // {
        //     errs() << "--- Is If Stmt ---\n";
        // }
        contains_enc = false;
    }
    // errs() << "Taints: ";
    // for (auto i : taint_list)
    // {
    //     errs() << i.first << " " << i.second << ",";
    // }
    // errs() << "\n";

    return true;
}

bool FunctionNameVisitor::VisitCompoundStmt(CompoundStmt *S)
{
    if (is_IfStmt)
    {
        is_IfStmt = false;
    }
    if (is_ForStmt)
    {
        is_ForStmt = false;
    }
    if (is_ArrStmt)
    {
        is_ArrStmt = false;
    }
    if (contains_enc)
    {
        contains_enc = false;
    }
    curr_decl = "";
    for (string i : for_vars)
    {
        if (taint_list.find(i) != taint_list.end())
        {
            for_enc = true;
            // PrintError(for_stmt->getBeginLoc(), "2. Non-oblivious FOR statement");
            // break;
            // errs()
            //     << "2. FOR: NOT OBLIVIOUS\n";
        }
    }
    // for (auto op1 : S->children())
    // {
    //     // errs() << "--- Left Children ---\n";
    //     if (auto b1 = dyn_cast_or_null<BinaryOperator>(op1))
    //     {
    //         analyzeChildren_binaryOperator(b1);
    //     }
    // }

    return true;
}

bool FunctionNameVisitor::VisitIfStmt(IfStmt *S)
{
    curr_decl = "";
    contains_enc = false;
    // auto exp = S->getCond()->getExprStmt();

    // errs() << "If statement: " << S->getStmtClass()
    //        << " : " << exp
    //        << "\n";
    is_IfStmt = true;
    if_stmt = S;
    return true;
}

bool FunctionNameVisitor::VisitImplicitCastExpr(ImplicitCastExpr *S)
{
    string arg_type = S->getType().getAsString();
    if (is_IfStmt)
    {
        analyzeChildren_implicitCast(S);
    }
    else if (is_assignment && !curr_decl.empty() && (arg_type.find("enc") != string::npos))
    {
        is_assignment = false;
        contains_enc = true;
        taint_list[curr_decl] = 1;
        // errs() << "VisitImplicitCast - Adding: " << curr_decl << "\n";
    }

    return true;
}

bool FunctionNameVisitor::VisitDeclRefExpr(DeclRefExpr *S)
{
    auto name = S->getDecl()->getName().str();
    if (is_ForStmt)
    {
        for_vars.push_back(name);
    }
    if (is_ReturnStmt)
    {
        if (taint_list.find(name) != taint_list.end())
        {
            // errs() << "F: " << curr_function << " name: " << name << "\n";
            return_list.push_back(curr_function);
            taint_list[curr_function_name] = 1;
        }
        is_ReturnStmt = false;
    }

    if (taint_list.find(name) != taint_list.end())
    {
        // errs() << "Name: " << name << " " << S->getDecl()->getName().str() << " is enc\n";
        contains_enc = true;

        // errs() << "VisitDeclRef: 1 " << name << "\n ";
        if (is_ArrStmt && arr_impl)
        {
            arr_enc = true;
            // PrintError(S->getBeginLoc(), "3. Non-oblivious ARRAY Access");
            // errs()
            //     << "##### ARR_DECL: NOT OBLIVIOUS #####\n";
            // arr_impl = false;
        }
    }
    // else
    // {
    // if (op)
    //     curr_decl = name;
    // errs() << "C: " << curr_decl << "\n";
    if (is_assignment && !is_new)
    {

        curr_decl = name;
        // errs() << "C1: " << curr_decl << "\n";
    }
    else if ((find(return_list.begin(), return_list.end(), S->getDecl()) == return_list.end()) && !is_new)
    {

        curr_decl = name;
        // errs() << "C2: " << curr_decl << "\n";
    }

    // is_new = true;
    // }

    // errs() << S->getDecl()->getName().str() << " - " << S->getExprStmt() << "\n";
    auto temp = S->getDecl()->getType().getAsString();
    auto token = temp.substr(0, temp.find(" "));
    // errs() << "token: " << token << "\n";

    if (is_new && !curr_decl.empty() && !is_ArrStmt && !is_ArrDecl && (token).find("enc") != string::npos)
    {
        taint_list[curr_decl] = 1;
        // errs() << "1. VisitDeclRef: Adding " << curr_decl << " - " << S->getDecl()->getType().getAsString() << "\n ";
        curr_decl = "";
        is_new = false;
    }
    // else if (is_assignment && !curr_decl.empty() && !is_ArrStmt && (token).find("enc") != string::npos)
    // {
    // }
    else if (op && !curr_decl.empty() && !is_ArrStmt && (token).find("enc") != string::npos)
    {
        if (!(taint_list.count(curr_decl)))
        {
            taint_list[curr_decl] = 1;
            // errs() << "2. VisitDeclRef: Adding " << curr_decl << " - " << S->getDecl()->getType().getAsString() << "\n ";
        }
        curr_decl = "";
    }

    if (is_main)
    {
        // for (auto i : return_list)
        // {
        //     errs() << i << " ";
        // }
        // errs() << "\n";
        if (find(return_list.begin(), return_list.end(), S->getDecl()) != return_list.end())
        {
            // errs() << curr_decl << "\n";
            if ((curr_decl_type).find("enc") == string::npos)
            {
                taint_list[name] = 1;
                // errs() << "3. VisitDeclRef: Adding " << name << "\n ";
                taint_list[curr_decl] = 1;
                // errs() << "3. VisitDeclRef: Adding " << curr_decl << "\n ";
                PrintWarning(S->getBeginLoc(), "Non-oblivious FUNCTION RETURN>This function's return type should be of secret type");
            }
        }
    }

    if (is_ArrStmt)
    {
        arr_impl = true;
    }
    if (is_ArrDecl)
    {
        is_ArrDecl = false;
    }
    is_new = false;
    return true;
}

bool FunctionNameVisitor::VisitVarDecl(VarDecl *S)
{
    is_new = true;
    curr_decl = S->getName().str();
    curr_decl_type = S->getType().getAsString();
    if ((curr_decl_type).find("enc") != string::npos)
    {
        taint_list[curr_decl] = 1;
        // errs() << "VisitVarDecl : Adding - " << curr_decl << "\n";

        curr_decl = "";
    }

    auto first = curr_decl_type.find("[");
    auto last = curr_decl_type.find("]");
    if (first != string::npos && last != string::npos)
    {
        string temp = curr_decl_type.substr(first + 1, last - first - 1);
        if (!temp.empty() && taint_list.find(temp) != taint_list.end())
        {
            arr_decl = S;
            arr_enc = true;
            is_ArrDecl = true;
        }
    }

    // errs() << "Var Decl: " << S->getName() << " Type: " << S->getType().getAsString() << "\n";
    return true;
}

bool FunctionNameVisitor::VisitBinaryOperator(BinaryOperator *S)
{
    // errs() << "---Inside Binary Op---\n";
    auto lhs = S->getLHS()->getType().getAsString();
    auto rhs = S->getRHS()->getType().getAsString();

    op = S->isAssignmentOp();
    // errs() << "Binary operator: " << S->getExprStmt() << " \t "
    //        << S->getLHS()->IgnoreImplicit()->getType().getAsString() << " "
    //        << S->getRHS()->IgnoreImplicit()->getType().getAsString() << " " << curr_decl << " op: " << op << "\n";
    if (op)
    {
        // errs() << "Binary operator: " << S->getExprStmt() << " \t "
        //        << lhs << " "
        //        << rhs << " D: " << curr_decl << "\n";
        is_assignment = true;
        // contains_enc = true;
        // taint_list[curr_decl] = 1;
        // is_new = false;
        // errs() << "1. VisitBinaryOperator - Adding: " << curr_decl << "\n";
    }
    else if (is_new && !curr_decl.empty() && (lhs.find("enc") != string::npos || rhs.find("enc") != string::npos))
    {
        // if (is_assignment)
        // {
        contains_enc = true;
        taint_list[curr_decl] = 1;
        is_new = false;
        // errs() << "2. VisitBinaryOperator - Adding: " << curr_decl << "\n";
        // }
    }
    else if (is_assignment && !curr_decl.empty() && (lhs.find("enc") != string::npos || rhs.find("enc") != string::npos))
    {
        is_assignment = false;
        contains_enc = true;
        taint_list[curr_decl] = 1;
        // errs() << "3. VisitBinaryOperator - Adding: " << curr_decl << "\n";
    }

    else
    {
        analyzeChildren_binaryOperator(S);
        is_assignment = false;
    }
    curr_decl = "";
    is_new = false;

    return true;
}

bool FunctionNameVisitor::VisitInitListExpr(InitListExpr *S)
{
    is_new = false;
    return true;
}

bool FunctionNameVisitor::VisitArraySubscriptExpr(ArraySubscriptExpr *S)
{
    is_ArrStmt = true;
    arr_stmt = S;
    analyzeChildren_arrayExpr(S);

    return true;
}

bool FunctionNameVisitor::VisitReturnStmt(ReturnStmt *S)
{
    // errs() << "Return \n"
    //        << for_vars.size();
    is_ReturnStmt = true;
    for (string i : for_vars)
    {
        if (taint_list.find(i) != taint_list.end())
        {
            // errs() << "ReturnStmt: " << i << " ";
            // errs() << "1"
            //        << "\n";
            // PrintError(for_stmt->getBeginLoc(), "1. Non-oblivious FOR Statement");
            for_enc = true;
            // break;
            // errs() << "2. FOR: NOT OBLIVIOUS\n";
        }
    }
    is_ArrStmt = false;

    return true;
}

bool FunctionNameVisitor::VisitForStmt(ForStmt *S)
{
    curr_decl = "";
    contains_enc = false;
    is_ForStmt = true;
    for_stmt = S;

    return true;
}

bool FunctionNameVisitor::VisitIntegerLiteral(IntegerLiteral *S)
{
    if (is_new)
    {
        is_new = false;
    }
    return true;
}
