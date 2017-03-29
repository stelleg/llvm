//===- Legion.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Legion World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include <cxxabi.h>
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "legion"

namespace {
  // Legion - The second implementation with getAnalysisUsage implemented.
  struct LoopToIndex : public LoopPass {
    static char ID;
    LoopToIndex() : LoopPass(ID) {}
   
    bool runOnLoop(Loop* l, LPPassManager &LPM){
      if(LPM.ID){}
      std::vector<BasicBlock*> bs = l->getBlocks();
      for(auto const& b: bs){
        for(auto const& i: *b){
          // Prints all calls within loop bodies
          const Instruction & ir = i;
          switch (i.getOpcode()){
            case Instruction::Call: {
              const CallInst& cir = static_cast<const CallInst&>(ir);
              auto name = cir.getCalledValue()->getName();
              if(name.contains("Legion") && name.contains("execute_task")){
                errs() << "Called execute task in loop with args: \n";
                for(unsigned i = 0; i < cir.getNumArgOperands(); i++){
                  errs() << "  " << *cir.getArgOperand(i) << "\n";
                }
              }
              break;
            }
            case Instruction::Invoke: {
              const InvokeInst& iir = static_cast<const InvokeInst&>(ir);
              auto name = iir.getCalledValue()->getName();
              if(name.contains("Legion") && name.contains("execute_task")){
                errs() << "Invoked execute task in loop with args: \n";
                for(unsigned i = 0; i < iir.getNumArgOperands(); i++){
                  errs() << "  " << *iir.getArgOperand(i) << "\n";
                }
              }
              break;
            }
          }
        }
      }
      return true;
    }
  };

}

char LoopToIndex::ID = 1;
static RegisterPass<LoopToIndex> loop_to_index("legion", "Legion Pass");
