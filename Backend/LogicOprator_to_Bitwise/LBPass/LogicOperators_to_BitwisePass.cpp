#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Support/Format.h"
#include <llvm/IR/DebugLoc.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <vector>
#include <map>
#include <algorithm>
#include "llvm/Transforms/Utils/BasicBlockUtils.h"


using namespace std;
using namespace llvm;
namespace Correctness{
    struct lineinfo{
        int line;
        Instruction* instruction;
    };
    struct IR_of_a_line{
        int line;
        vector <Instruction *> inst_list;
    };
	struct to_bitwisePass: public FunctionPass {
        static char ID;
		to_bitwisePass() : FunctionPass(ID) {
        }

        bool is_short_circuit_BR(Instruction * inst){
            auto terminator = inst->getParent()->getTerminator();
            auto t = cast<BranchInst>(terminator);
            auto bb1_next1 = t->getSuccessor(0);
            auto bb2_next2 = t->getSuccessor(1);    
            auto terminator_1 = bb1_next1->getTerminator();
            auto terminator_2 = bb2_next2->getTerminator();

            
            auto num_of_succ = terminator_1->getNumSuccessors();
            vector<BasicBlock*> bb_next3;
            vector<BasicBlock*> bb_next4;
            for(int i=0; i<num_of_succ;i++){
                auto temp1 = cast<BranchInst>(terminator_1)->getSuccessor(0);
                bb_next3.push_back(temp1);
                auto temp2 = cast<BranchInst>(terminator_2)->getSuccessor(0);
                bb_next3.push_back(temp2);
            }
            for(auto ins: bb_next3){
                if(bb1_next1 == ins || bb2_next2 == ins){
                    return true;
                }
            }
            for(auto ins:bb_next4){
                if(bb1_next1==ins||bb2_next2==ins){
                    return true;
                }
            }
            return false;
        }

        bool is_And_op(){
            return true;
        }
        bool changed = false;
		virtual bool runOnFunction(Function &F) override{
            /* *******Implementation of Your code ******* */
//          The logical and operator ‘&&’ expects its operands to be boolean expressions (either 1 or 0) 
//          and returns a boolean value. 
//          The bitwise and operator ‘&’ work on Integral (short, int, unsigned, char, bool, unsigned char, long) 
//          values and return Integral value. 

            std::vector<IR_of_a_line> meta_list;
            std::vector<lineinfo> all_list;
            vector<Instruction *> inst_l;
            vector<int>line_list;
            for(Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb){
                
                for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i){
                    auto y = i->getOperand(0);
                   
                    Instruction* inst = &*i;
                    const llvm::DebugLoc &debugInfo = i->getDebugLoc();
                    bool flag=false;
                if(((i->getOpcode()==Instruction::Call)&& (i->getNextNode()->getOpcode() == Instruction::Store))||
                ((i->getOpcode()==Instruction::Store)&& (i->getNextNode()->getOpcode() == Instruction::Call))){
                    flag = true;
                }
                if(debugInfo && !flag){
                    int line = debugInfo->getLine();
                    int column = debugInfo->getColumn();
                    
                    inst_l.push_back(inst);
                    line_list.push_back(line);
                    lineinfo info;
                    info.instruction = inst;
                    info.line = line;
                    all_list.push_back(info);
                }
                }
            }
            vector <Instruction *> temp_inst;
            //group IR insts by src_code linem
            for(int i = 0; i<all_list.size()-1;i++){
                int temp = all_list[i].line;
               
                if(all_list[i].line == all_list[i+1].line )
                {   
                   temp_inst.push_back(all_list[i].instruction);
                }

                if(all_list[i].line != all_list[i+1].line )
                {
                    IR_of_a_line temp_info;
                    temp_inst.push_back(all_list[i].instruction);
                    temp_info.inst_list = temp_inst;
                    temp_info.line = temp;
                    meta_list.push_back(temp_info);
                    temp_inst.clear();
                }
               
             }
             //add the last element
            //IR_of_a_line temp_info;
            auto c = all_list[all_list.size()-1].instruction;
            auto l = all_list[all_list.size()-1].line;

            meta_list[meta_list.size()-1].inst_list.push_back(c);
            meta_list[meta_list.size()-1].line = l;

            for(auto elt : meta_list)
            {
          
                auto elt_inst_list = elt.inst_list;
                Instruction *new_inst;
                int i =0;
                bool flag = false;
                bool flag2 = false;
                for(auto in: elt_inst_list)
                {
                   
                    auto ins = elt_inst_list[i];
                    if(i==elt_inst_list.size()-2){ flag = true;}
                     i++;
                    const Value * valR = dyn_cast<Value>(ins);
                    if(isa<ReturnInst> (valR)) continue;
                    auto temp_inst = in->getOpcode();
                    if(temp_inst == Instruction::Br){
                        errs()<<"is BR!!! \n";
                        continue;
                    } 
                    auto next_inst = in->getNextNode()->getOpcode();
                   if( (temp_inst ==Instruction::ICmp) && (next_inst == Instruction::Br)&&!flag){
                 
                        //Br instruction
                        auto next_inst_Br = in->getNextNode();
                        //cast i1 of cmp output to i32
                        Value * val =  cast<Value>(in);
                        llvm::LLVMContext &llvmcontext = in->getContext();
                       
                        auto t = dyn_cast<Type>(IntegerType::getInt32Ty(llvmcontext));
                        auto new_inst1 =  new llvm::ZExtInst(val,t,"",next_inst_Br);
                        new_inst = cast<Instruction>(new_inst1);
                        // get the Br inst now that new zext inst is inserted b/n cmp and Br
                        auto Old = in->getNextNode()->getNextNode();
                        auto next_BB = Old->getSuccessor(is_short_circuit_BR(Old));
                        //modify the conditional Br to non conditional BR statment
                        BranchInst* New = BranchInst::Create(next_BB);//replace 1 with a condition
                        ReplaceInstWithInst(Old, New);

                        flag2 = true;
                    }
                        
                }
                //out
                if(elt_inst_list.size()<3) {
                    continue;
                }
                // go to the last insts: CMP and BR
                auto cmp_inst = elt_inst_list[elt_inst_list.size()-2];
                auto Br_inst = elt_inst_list[elt_inst_list.size()-1];


                // new inst and new dst Zext & insert the new Inst before BR statement
                if(flag2&&(cmp_inst->getOpcode()==Instruction::ICmp)&&(Br_inst->getOpcode()==Instruction::Br))
                {
                    Value * val =  cast<Value>(cmp_inst);
                    llvm::LLVMContext &llvmcontext = cmp_inst->getContext();
                    auto t = dyn_cast<Type>(IntegerType::getInt32Ty(llvmcontext));
                    auto new_inst1 =  new llvm::ZExtInst(val,t,"",Br_inst);
            
                
                    // new bitwise And or Or instruction
                    Instruction *ni;
                    
                    ni = BinaryOperator::CreateAnd(new_inst, new_inst1);               
                    //trunc
                    ni->insertBefore(Br_inst);
                    Value * val1 =  cast<Value>(ni);
                    llvm::LLVMContext &llvmcontext1 = ni->getContext();
                    auto t1 = dyn_cast<Type>(IntegerType::getInt1Ty(llvmcontext1));
                    auto new_inst2 =  new llvm::TruncInst(val1,t1,"",Br_inst);
                    //insert it before Br
                    
                    dyn_cast<BranchInst>(Br_inst)->setCondition(cast<Value>(new_inst2));
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

