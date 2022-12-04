#include <cstdio>
#include <string>
#include <sstream>

#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Format.h"

#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"

// #include "clang/TreeTransformer"

using namespace clang;
using namespace std;


class AST_Visitor : public RecursiveASTVisitor<AST_Visitor>
{
public:
    AST_Visitor(Rewriter &R) : rewriter(R){}

    bool VisitStmt(Stmt *s) {
        if (isa<clang::BinaryOperator>(s)) {
            BinaryOperator * bin_op = cast<BinaryOperator>(s);
            auto f = bin_op->getExprLoc();
            bool is_logical = bin_op->isLogicalOp();
            if(is_logical){
                auto rhs = bin_op->getRHS();
                auto lhs = bin_op->getLHS();
                std::string rhs_st= rhs->getType().getAsString();
                std::string lhs_st = lhs->getType().getAsString();

                BinaryOperator::Opcode op = bin_op->getOpcode();

                //create bitwise AND/OR inst
                //replace the old Logical AND inst with bitwise AND/OR inst
                // printf("right hand side: ",rhs_st);
                // printf("left hand side: ",lhs_st);
                auto temp= bin_op->getOpcodeStr().str();
                auto new_op = temp.at(0);// && -> & and || -> |
                std::string new_inst = lhs_st+new_op+rhs_st+";";
                auto bi_st = s->getBeginLoc();
                
                rewriter.ReplaceText(bi_st,new_inst);
            
                
                
            }
                                 
        }

        return true;
    }

   
private:
    void AddBraces(Stmt *s);

    Rewriter &rewriter;
};



class AST_Consumer : public ASTConsumer
{
public:
    AST_Consumer(Rewriter &R)
        : Visitor(R)
    {}

    
    virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
        for (DeclGroupRef::iterator b = DR.begin(), e = DR.end();
             b != e; ++b)
            // Traverse the declaration using our AST visitor.
            Visitor.TraverseDecl(*b);
        return true;
    }

private:
    AST_Visitor Visitor;
};


int main(int argc, char *argv[])
{
    if (argc <2) {
        llvm::errs() << "error \n Usage: transformer <filename>\n";
        return 1;
    }

    // CompilerInstance will hold the instance of the Clang compiler for us,
    // managing the various objects needed to run the compiler.
    CompilerInstance compiler;
    compiler.createDiagnostics();

    LangOptions &lo = compiler.getLangOpts();
    lo.CPlusPlus = 1;

    // Initialize target info with the default triple for our platform.
    auto TO = std::make_shared<TargetOptions>();
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *TI =
        TargetInfo::CreateTargetInfo(compiler.getDiagnostics(), TO);
    compiler.setTarget(TI);

    compiler.createFileManager();
    FileManager &fileMgr = compiler.getFileManager();
    compiler.createSourceManager(fileMgr);
    SourceManager &sourceMgr = compiler.getSourceManager();
    compiler.createPreprocessor(TU_Module);//TU_Complete
    compiler.createASTContext();

    // A Rewriter helps us manage the code rewriting task.
    Rewriter rewriter;
    rewriter.setSourceMgr(sourceMgr, compiler.getLangOpts());

    // Set the main file handled by the source manager to the input file.
    auto fileIn = fileMgr.getFile(argv[1]).get();
    sourceMgr.setMainFileID(
        sourceMgr.createFileID(fileIn, SourceLocation(), SrcMgr::C_User));
    compiler.getDiagnosticClient().BeginSourceFile(
        compiler.getLangOpts(), &compiler.getPreprocessor());

    // Create an AST consumer instance which is going to get called by
    // ParseAST.
    AST_Consumer _consumer(rewriter);

    // Parse the file to AST, registering our consumer as the AST consumer.
    ParseAST(compiler.getPreprocessor(), &_consumer,
            compiler.getASTContext());

    // At this point the rewriter's buffer should be full with the rewritten
    // file contents.
    const RewriteBuffer *rewriteBuf =
        rewriter.getRewriteBufferFor(sourceMgr.getMainFileID());
    llvm::outs() << std::string(rewriteBuf->begin(), rewriteBuf->end());

    return 0;
   
}
