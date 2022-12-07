//===-- Frequent Path Loop Invariant Code Motion Pass ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// EECS583 F22 - This pass can be used as a template for your Frequent Path LICM
//               homework assignment. The pass gets registered as "fplicm".
//
// This pass performs loop invariant code motion, attempting to remove as much
// code from the body of a loop as possible.  It does this by either hoisting
// code into the preheader block, or by sinking code to the exit blocks if it is
// safe.
//
////===----------------------------------------------------------------------===//
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <stack>


using namespace llvm;

#define DEBUG_TYPE "fplicm"


namespace Performance{
class HoistData {
    public:
        explicit HoistData(Value *Operand, LoadInst* loadInstruction, StoreInst* storeInstruction) {
            operand = Operand;
            loads.push_back(loadInstruction);
            stores.push_back(storeInstruction);
        }

    public:
        Value *operand; 
        std::vector<LoadInst*> loads;
        std::vector<StoreInst*> stores;
};
struct FPLICMPass : public LoopPass {
    static char ID;
    FPLICMPass() : LoopPass(ID) {}

    bool runOnLoop(Loop *L, LPPassManager &LPM) override {
        BlockFrequencyInfo &bfi = getAnalysis<BlockFrequencyInfoWrapperPass>().getBFI();
        BranchProbabilityInfo &bpi = getAnalysis<BranchProbabilityInfoWrapperPass>().getBPI();
        LoopInfo &LoopInfo = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

        /* *******Implementation Starts Here******* */
        ArrayRef<BasicBlock*>  basicBlocks = L->getBlocks();
        BasicBlock *current = basicBlocks[1];
        std::set<BasicBlock*> fb;
        std::set<BasicBlock*> ifb;
        std::vector<Instruction*> infrequent_stores;
        std::vector<Instruction*> frequent_loads;
        std::vector<Instruction*> frequent_stores;

        getFrequentLoadsandStores(basicBlocks, bpi, current, fb, ifb, frequent_loads, frequent_stores);
        getInfrequentStores(infrequent_stores, ifb, fb, L, LoopInfo);
        
        std::map<Value*, HoistData> hoistData;
        for (auto I : infrequent_stores) {
            auto operand = I->getOperand(1);
            for (auto li : frequent_loads) {
                if (li->getOperand(0) == operand) {
                    bool hoistable = true;
                    for (auto fs : frequent_stores) {
                        if (fs->getOperand(1) == operand) {
                            hoistable = false;
                            break;
                        }
                    }
                    if (hoistable) {
                        //errs() << "hoistable " << *li << "\n";
                        auto ite = hoistData.find(operand);
                        if (ite != hoistData.end()) {
                            if (std::find(ite->second.loads.begin(), ite->second.loads.end(), dyn_cast<LoadInst>(li)) == ite->second.loads.end())
                                ite->second.loads.push_back(dyn_cast<LoadInst>(li));
                            if (std::find(ite->second.stores.begin(), ite->second.stores.end(), dyn_cast<StoreInst>(I)) == ite->second.stores.end())
                                ite->second.stores.push_back(dyn_cast<StoreInst>(I));
                        }else{
                            hoistData.insert(ite, std::pair<Value*, HoistData>(operand, HoistData(operand, dyn_cast<LoadInst>(li), dyn_cast<StoreInst>(I))));
                        }
                    }
                }
            }
        }

        bool secondIter = true; 
        if (hoistData.empty()){
            secondIter = false; 
            //We are not hoisting anything 
            return false;
        } 

        for (auto ite : hoistData) {
            FPLICM(L->getLoopPreheader(), ite.second);
        }
        replaceConstant(basicBlocks[1], L->getLoopPreheader());





        
        /* *******Implementation Ends Here******* */



        return true;
    }


void getFrequentLoadsandStores(ArrayRef<BasicBlock*> basicBlocks, BranchProbabilityInfo &bpi, BasicBlock *current,std::set<BasicBlock*> &fb, std::set<BasicBlock*> &ifb, std::vector<Instruction*> &frequent_loads, std::vector<Instruction*> &frequent_stores){
  
    while (current != basicBlocks[0]) {
            fb.insert(current);
            for (auto &I : *current) {
                if (I.getOpcode() == Instruction::Load){
                    frequent_loads.push_back(&I);
                }else if (I.getOpcode() == Instruction::Store) {
                    frequent_stores.push_back(&I);
                }
            }


            auto exit_ins = current->getTerminator();
            if (exit_ins->getNumSuccessors() > 1){
                auto ite = successors(current).begin();
                BasicBlock *left  = *ite;
                BasicBlock *right = *(ite+1);
                if(bpi.getEdgeProbability(&*current, left) >= BranchProbability(4,5)){
                  current = left;
                  ifb.insert(right);
                }
                else{
                  current = right;
                  ifb.insert(left);

                }
            }else{
                current = current->getUniqueSuccessor();
            }
        }
}

void getInfrequentStores(std::vector<Instruction*> &infrequent_stores,  std::set<BasicBlock*> ifb, std::set<BasicBlock*> fb,Loop *L,LoopInfo &LoopInfo){
  std::deque<BasicBlock*> bfs;
        for (auto *BB : ifb) {
            bfs.push_back(BB);
            while (!bfs.empty()) {
                for (auto &I : *bfs.front()) {
                    if (I.getOpcode() == Instruction::Store) {
                        infrequent_stores.push_back(&I);
                    }
                }
                for (auto *succ : successors(bfs.front())) {
                    if (std::find(bfs.begin(), bfs.end(), succ) == bfs.end()
                        && fb.find(succ) == fb.end()
                        && !inSubLoop(succ, L, &LoopInfo)){
                        bfs.push_back(succ);
                    }
                }
                bfs.pop_front();
            }
        }  
}

    void FPLICM(BasicBlock *PreHeader, HoistData& hoistData) {
        Instruction *terminator = PreHeader->getTerminator();
        std::vector<Instruction *> ins_list;
        unsigned instListSize = 0;
        for (auto load : hoistData.loads) {
            auto *current = dyn_cast<Instruction>(*load->user_begin());
            while (true) {
                if (current->getNumOperands() == 2) {
                    if (dyn_cast<Instruction>(current->getOperand(1)) == nullptr) {
                        ins_list.push_back(current);
                        current = dyn_cast<Instruction>(*current->user_begin());
                        continue;
                    }

                    break;

                } else {
                    ins_list.push_back(current);
                    current = dyn_cast<Instruction>(*current->user_begin());
                }
            }

            auto prev = *ins_list.rbegin();
            load->moveBefore(terminator);
            for(auto ite = ins_list.begin()+instListSize; ite != ins_list.end(); ite++){
                (*ite)->moveBefore(terminator);
            }

            auto *var = new AllocaInst(prev->getType(), 0, nullptr, Align(16), "newvar", terminator);
            ins_list.push_back(new StoreInst(prev, var, terminator));
            auto *new_load = new LoadInst(prev->getType(), var, "newLoad", current);
            if (current->getOpcode() == Instruction::Store) {
                std::vector<Value*> temp_save;
                for (auto *usr : current->getOperand(1)->users()) {
                    if (dyn_cast<Instruction>(usr)->getOpcode() == Instruction::Load 
                        && dyn_cast<Instruction>(usr)->getParent() == dyn_cast<Instruction>(current)->getParent()){
                            usr->replaceAllUsesWith(new_load);
                            temp_save.push_back(usr);
                    }
                }
                for (auto *i : temp_save) dyn_cast<Instruction>(i)->eraseFromParent();
                current->eraseFromParent();
                dyn_cast<Instruction>(current->getOperand(1))->eraseFromParent();
            }else{
                current->setOperand(current->getOperand(0) == prev ? 0 : 1, new_load);
            }

            instListSize = ins_list.size();
        }

        Value *curr, *prev, *origin;
        for (auto store : hoistData.stores) {
            origin = store->getOperand(0);
            prev = origin;
            for (auto I : ins_list) {
                curr = I->clone();
                auto Icurr = dyn_cast<Instruction>(curr);
                if (I->getOpcode() == Instruction::Store){
                    Icurr->setOperand(0, prev);
                    prev = origin;
                }else{
                    unsigned idx = 0;
                    for (auto p = I->op_begin(); p != I->op_end() ; p++ ) {
                        if (*p == I->getPrevNode()){
                            Icurr->setOperand(idx, prev);
                        }
                        idx++;
                    }
                    prev = curr;
                }
                Icurr->insertBefore(store);
            }
            store->eraseFromParent();
        }


    }

    static void replaceConstant(BasicBlock* current_bb, BasicBlock* PreHeader) {
        std::vector<Instruction*> loads;
        std::vector<Instruction*> stores;
        for (auto &I: *current_bb) {
            if (I.getOpcode() == Instruction::Store) stores.push_back(&I);
        }
        for (auto store : stores) {
            for (auto usr : store->getOperand(1)->users()) {
                auto load = dyn_cast<Instruction>(usr);
                if (store == load || load->getParent() != current_bb) continue; 
                loads.push_back(load);
            }
            if (!loads.empty()) {

                for (auto li : loads){
                    li->replaceAllUsesWith(store->getOperand(0));
                    li->eraseFromParent();
                }
                store->eraseFromParent();
                dyn_cast<Instruction>(store->getOperand(1))->eraseFromParent();
                loads.clear();
            }
        }
    }


void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<BranchProbabilityInfoWrapperPass>();
    AU.addRequired<BlockFrequencyInfoWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
}


bool inSubLoop(BasicBlock *BB, Loop *currentLoop, LoopInfo *loadInstruction) {
        return loadInstruction->getLoopFor(BB) != currentLoop && BB != loadInstruction->getLoopFor(BB)->getHeader();
}

};


} // end of namespace Performance

char Performance::FPLICMPass::ID = 0;
static RegisterPass<Performance::FPLICMPass> Y("fplicm-performance", "Frequent Loop Invariant Code Motion for performance test",
                                               false, false);