#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <string.h>
#include <cctype>
#include <map>

#include "PrintFunctionsAction.h"

using namespace llvm;
using namespace clang;
using namespace printfunctions;

using namespace std;

map<string, int> taint_list, for_vars;
vector<FunctionDecl *> return_list;
vector<string> arr_list;
map<int, int> for_lines;
int for_count = 0;

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
        // errs() << pos << "\n";
        if (temp != string::npos)
        {
            auto tmp = it->first;
            if (tmp.length() == 1)
            {
                auto nxt = err.substr(temp);
                if (nxt.length() > 1)
                {
                    int r = isalpha(nxt.at(1));
                    if (r == 0)
                    {
                        found = tmp;
                        err_print = err.substr(0, temp) + "\e[0;33m" + err.substr(temp) + "\e[0m";
                    }
                    else
                    {
                        temp = err.find_last_of(pos);
                        auto nxt = err.substr(temp, temp + 1);
                        if (nxt.length() > 1)
                        {
                            int r = isalpha(nxt.at(1));
                            if (r == 0)
                            {
                                found = tmp;
                                err_print = err.substr(0, temp) + "\e[0;33m" + err.substr(temp) + "\e[0m";
                            }
                        }
                    }
                }
                else
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

void FunctionNameVisitor::analyzeChildren_binaryOperator(BinaryOperator *S)
{
    auto lhs = S->getLHS();
    auto rhs = S->getRHS();

    string lhs_arg_type = lhs->getType().getAsString();
    string rhs_arg_type = rhs->getType().getAsString();

    if (is_new && !(curr_decl).empty() && ((lhs_arg_type).find("enc") != string::npos || (rhs_arg_type).find("enc") != string::npos))
    {
        // errs() << "t5: " << curr_decl << "\n";
        taint_list[curr_decl] = 1;
        curr_decl = "";
        is_new = false;
    }
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
        if (auto b1 = dyn_cast_or_null<BinaryOperator>(op1))
        {
            analyzeChildren_binaryOperator(b1);
        }
        else
        {
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
        if (auto b2 = dyn_cast_or_null<BinaryOperator>(op2))
        {
            analyzeChildren_binaryOperator(b2);
        }
        else
        {
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

void FunctionNameVisitor::analyzeChildren_implicitCast(ImplicitCastExpr *S)
{
    string arg_type = S->getType().getAsString();

    if (is_IfStmt && (arg_type).find("enc") != string::npos)
    {
        contains_enc = true;
    }
    else if (is_assignment)
    {
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
    for (auto op : S->children())
    {
        if (auto d = dyn_cast_or_null<ImplicitCastExpr>(op))
        {
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

    for (auto it = for_vars.begin(); it != for_vars.end(); it++)
    {
        // errs() << it->second << "\n";
        // auto i = it->first;
        // errs() << it->first // string (key)
        //        << ':'
        //        << it->second // string's value
        //        << "\n";

        if (taint_list.find(it->first) != taint_list.end() && it->second == for_count)
        {
            // errs() << "F2: " << i << "\n";
            for_enc = true;
        }
    }

    if (is_IfStmt && contains_enc)
    {
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
        PrintError(for_stmt->getBeginLoc(), "Non-oblivious FOR Statement>Illegal use of secret value in for loop");
        for_enc = false;
    }

    return true;
}

bool FunctionNameVisitor::VisitFunctionDecl(clang::FunctionDecl *f)
{
    curr_function_name = f->getNameInfo().getName().getAsString();
    if (curr_function_name == "main")
    {
        is_main = true;
    }
    curr_function = f->getAsFunction();
    // f->getBody()->dump();
    return true;
}

bool FunctionNameVisitor::VisitDecl(Decl *D)
{
    if (D->getBody() != NULL)
    {
        contains_enc = false;
    }

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

    return true;
}

bool FunctionNameVisitor::VisitIfStmt(IfStmt *S)
{
    curr_decl = "";
    contains_enc = false;
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
        // errs() << "t6: " << curr_decl << "\n";
        taint_list[curr_decl] = 1;
    }
    else if (is_new && !curr_decl.empty() && (arg_type.find("enc") != string::npos))
    {
        is_new = false;
        contains_enc = true;
        // errs() << "t7: " << curr_decl << "\n";
        taint_list[curr_decl] = 1;
    }

    return true;
}

bool FunctionNameVisitor::VisitDeclRefExpr(DeclRefExpr *S)
{
    auto name = S->getDecl()->getName().str();
    if (is_ForStmt)
    {
        // errs() << "For: " << name << " C: " << for_count << "\n";
        for_vars[name] = for_count;
    }
    if (is_ReturnStmt)
    {
        if (taint_list.find(name) != taint_list.end())
        {
            return_list.push_back(curr_function);

            taint_list[curr_function_name] = 1;
        }
        is_ReturnStmt = false;
    }

    if (taint_list.find(name) != taint_list.end())
    {
        contains_enc = true;
    }
    if (is_ArrStmt)
    {
        if (taint_list.find(name) != taint_list.end() && find(arr_list.begin(), arr_list.end(), name) == arr_list.end())
        {
            // errs() << "1: " << name
            //        << "\n";
            arr_enc = true;
        }
    }
    if (is_assignment && !is_new)
    {

        curr_decl = name;
    }
    else if ((find(return_list.begin(), return_list.end(), S->getDecl()) == return_list.end()) && !is_new)
    {

        curr_decl = name;
    }

    auto temp = S->getDecl()->getType().getAsString();
    auto token = temp.substr(0, temp.find(" "));

    if (is_new && !curr_decl.empty() && !is_ArrStmt && !is_ArrDecl && (token).find("enc") != string::npos)
    {
        // errs() << "t8: " << curr_decl << "\n";
        taint_list[curr_decl] = 1;
        curr_decl = "";
        is_new = false;
    }
    else if (op && !curr_decl.empty() && !is_ArrStmt && (token).find("enc") != string::npos)
    {
        if (!(taint_list.count(curr_decl)))
        {
            // errs() << "t9: " << curr_decl << "\n";
            taint_list[curr_decl] = 1;
        }
        curr_decl = "";
    }

    if (is_main)
    {
        if (find(return_list.begin(), return_list.end(), S->getDecl()) != return_list.end())
        {
            if ((curr_decl_type).find("enc") == string::npos)
            {
                taint_list[name] = 1;
                // errs() << "t1: " << curr_decl << "\n";
                // errs() << "t1: " << name << "\n";
                taint_list[curr_decl] = 1;
                PrintWarning(S->getBeginLoc(), "Non-oblivious Function Return>This function's return type should be of secret type");
            }
        }
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
    auto first = curr_decl_type.find("[");
    auto last = curr_decl_type.find("]");
    if (first != string::npos && last != string::npos)
    {
        arr_list.push_back(curr_decl);
    }
    if ((curr_decl_type).find("enc") != string::npos)
    {
        // errs() << "t2: " << curr_decl << "\n";
        taint_list[curr_decl] = 1;
        curr_decl = "";
    }

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

    return true;
}

bool FunctionNameVisitor::VisitBinaryOperator(BinaryOperator *S)
{
    auto lhs = S->getLHS()->getType().getAsString();
    auto rhs = S->getRHS()->getType().getAsString();

    op = S->isAssignmentOp();
    if (op)
    {
        is_assignment = true;
    }
    else if (is_new && !curr_decl.empty() && (lhs.find("enc") != string::npos || rhs.find("enc") != string::npos))
    {
        contains_enc = true;
        // errs() << "t3: " << curr_decl << "\n";
        taint_list[curr_decl] = 1;
        is_new = false;
    }
    else if (is_assignment && !curr_decl.empty() && (lhs.find("enc") != string::npos || rhs.find("enc") != string::npos))
    {
        is_assignment = false;
        contains_enc = true;
        // errs() << "t4: " << curr_decl << "\n";
        taint_list[curr_decl] = 1;
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
    is_ReturnStmt = true;
    for (auto it = for_vars.begin(); it != for_vars.end(); it++)
    {
        if (taint_list.find(it->first) != taint_list.end())
        {
            for_enc = true;
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
    is_ArrStmt = false;
    for_stmt = S;
    for_count++;

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
