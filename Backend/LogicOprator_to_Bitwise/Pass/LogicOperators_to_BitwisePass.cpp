#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Support/Format.h"

using namespace std;
using namespace llvm;
namespace Correctness{
	struct to_bitwisePass: public FunctionPass {
        static char ID;
		to_bitwisePass() : FunctionPass(ID) {
        }

        bool isBooleanType(Type *t){
            if (t->isPointerTy()) {
                llvm::Type* inner = t->getPointerElementType();
                if (inner->isIntegerTy()) {
                    llvm::IntegerType* it = (llvm::IntegerType*) inner;
                    if (it->getBitWidth() == 1) {
                        errs()<<"Is Bool:"<<it->getBitWidth()<<"\n";
                        return true;
                    }
                }
            }
            return false;
        }
        bool changed = false;
		virtual bool runOnFunction(Function &F) override{
            /* *******Implementation of Your code ******* */
//          The logical and operator ‘&&’ expects its operands to be boolean expressions (either 1 or 0) 
//          and returns a boolean value. 
//          The bitwise and operator ‘&’ work on Integral (short, int, unsigned, char, bool, unsigned char, long) 
//          values and return Integral value. 
            for(Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb){
                
                for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i){
                    
                    char op;
                    if(i->getOpcode() == Instruction::And)
                    {
                        auto lhs = i->getOperand(0);
                        auto rhs = i->getOperand(1);
                        if(isBooleanType(lhs->getType())&& isBooleanType(rhs->getType()))
                        {
                            //found logical And operation. Change it to bitwise by padding the operands' 1 bit int to 32 bit int(?)
                            Value *v;
                            IRBuilder<> Builder(&*i);
                            v = Builder.CreateIntCast(v, Type::getInt32Ty(F.getContext()), true);

                            changed = true;
                        }
                    }
                    else if(i->getOpcode() == Instruction::Or)
                    {
                        auto lhs = i->getOperand(0);
                        auto rhs = i->getOperand(1);
                        if(isBooleanType(lhs->getType())&& isBooleanType(rhs->getType()))
                        {
                            //found logical Or operation. Change it to bitwise by padding the operands' 1 bit int to 32 bit int(?)
                            Value *v;
                            IRBuilder<> Builder(&*i);
                            v = Builder.CreateIntCast(v, Type::getInt32Ty(F.getContext()), true);

                            changed = true;
                        }
                    }
                    
                }  
               
                              
            }

          
            
            
            
            
			return changed; // template code is just return false
		}

        
	};
}
char Correctness::to_bitwisePass::ID = 0;
static RegisterPass<Correctness::to_bitwisePass> X("to_bitwisePass", "to_bitwisePass pass",
    false /* Only looks at CFG */,
    false /* Analysis Pass */);

