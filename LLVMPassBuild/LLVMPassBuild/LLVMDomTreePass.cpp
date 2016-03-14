#include <unistd.h>
#define DEBUG_TYPE "mast"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/Statistic.h"
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Linker/Linker.h>
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/UnifyFunctionExitNodes.h"
#include "llvm/IR/Dominators.h"
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <list>
#include <map>
#include <tuple>
#include <vector>


using namespace llvm;
std::vector<std::vector<std::tuple<std::string, int>>> z3_con;

namespace {

	struct DoTrePass: public ModulePass {
		DoTrePass() : ModulePass(ID) { }
		
		static char ID;
		bool first_hit;
		void getAnalysisUsage(AnalysisUsage &AU) const override {
			ModulePass::getAnalysisUsage(AU);
			AU.addRequired<DominatorTreeWrapperPass>();
		}
		
		virtual const char *getPassName() const override
		{
			return "Remove All Obfuscator Symbols";
		}
		
		virtual bool runOnFunction(Function &F) 
		{

			//DominatorTree *DT = &getAnalysis<DominatorTreeWrapperPass>().getDomTree();
			outs()<<"<-- Run on Full Module to Successfully Generate the Dominator Tree --> \n\n";
			return true;
		}
	

		// function recursively checks for operations applied to the variable, climbing backwards up the Use-Def Chain until a return or end of operation indicator is hit
		bool recursiveEndCheck(Instruction *inst, std::unordered_set<Instruction*> &used_insts, std::vector<std::tuple<std::string, int>> &new_arg)
		{
			if (std::find(used_insts.begin(), used_insts.end(), inst) == used_insts.end()) 
			{
				used_insts.insert(inst);
				outs()<<"\t\t\t[-] RECURSING THROUGH OPERATION\n";
				if(inst->getOpcode() == Instruction::Xor)
				{
					ConstantInt *_xor = cast<ConstantInt>(inst->getOperand(1));
					outs()<<"\t\t\t\t==> Found Operator: Xor :: "<<_xor->getZExtValue()<<"\n";
					std::tuple<std::string, int> base_cmp2( "^", (int)_xor->getZExtValue());
					new_arg.push_back(base_cmp2);
				}else if(inst->getOpcode() == Instruction::Add ) 
				{
					ConstantInt *_add = cast<ConstantInt>(inst->getOperand(1));
					outs()<<"\t\t\t\t==> Found Operator: Add :: "<<_add->getZExtValue()<<"\n";
					std::tuple<std::string, int> base_cmp2( "+", (int)_add->getZExtValue());
					new_arg.push_back(base_cmp2);
				}else if(inst->getOpcode() == Instruction::Sub ) 
				{
					ConstantInt *_sub = cast<ConstantInt>(inst->getOperand(1));
					outs()<<"\t\t\t\t==> Found Operator: Sub :: "<<_sub->getZExtValue()<<"\n";
					std::tuple<std::string, int> base_cmp2( "-", (int)_sub->getZExtValue());
					new_arg.push_back(base_cmp2);
				}
				return false;
			}else{
				return true;
			}
		}

		// TODO: add support for more conditions such as finite for loops 
		//
		// This recursive function tells us where our variable got its value. Aka an allocation, a store instruction, or is it a command line argument.
		// This tool automcatically focuses on command line input but can be modified to target variables with controllable input from a variety of methods`
		void recursiveValueTracer(Instruction* delme, std::unordered_set<Instruction*> &used_insts, std::vector<std::tuple<std::string, int>> &new_arg)
		{

			for(Instruction::op_iterator it = delme->op_begin(); it != delme->op_end(); it++) 
			{
				
					if (AllocaInst *All = dyn_cast<AllocaInst>(it))
					{
						if(std::find(used_insts.begin(), used_insts.end(), All) == used_insts.end()){
							outs()<<"\t[!] FOUND TERMINATING ALLOCATION\n\n";	
							recursiveEndCheck(All, used_insts, new_arg);
							outs()<<"\t\t[ TRACING BACK HISTORY OF OPERAND VALUE ]\n";
							for(Instruction::user_iterator all_user = All->user_begin(); all_user != All->user_end(); all_user++){
								if(StoreInst *all_str = dyn_cast<StoreInst>(*all_user)){
									outs()<<"\t\t[+] Found StoreInst to trace\n";
									recursiveValueTracer(all_str, used_insts, new_arg);	
								}
							}
						}
					}
					else if(Instruction *Inst = dyn_cast<Instruction>(it)) 
					{
					
						if(std::find(used_insts.begin(), used_insts.end(), Inst) == used_insts.end()){
							recursiveEndCheck(Inst, used_insts, new_arg);
							recursiveValueTracer(Inst, used_insts, new_arg);
						}
					}
					else
					{
						if(first_hit){
							first_hit = false;
							outs()<<"\t\t.... Potential find. Store took value from :: argv\n\n";
						}
					}
			}

			recursiveEndCheck(delme, used_insts, new_arg);

		}



		virtual bool runOnModule(Module &M) override 
		{

		    	for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
			{
				if(F->getName().str() == "main"){
					outs()<<"\n<-- Starting analysis from main() --> \n";
					if (!F->empty()){
						DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>(*F).getDomTree();
					
						// we know we only need to trace the most dominated path for the example binary
						// TODO: for future test binarys, dynamically choose a dominated path  or analayze all dominated paths 
						outs()<<"\t[+] Tracing first path of DominatorTree\n\n\n";

						for (auto node = GraphTraits<DominatorTree *>::nodes_begin(&DT); node != GraphTraits<DominatorTree *>::nodes_end(&DT); ++node) 
						{
							// iterate through all blocks in the most dominated path
							// check if an ending block, if not analyze, if it is an ending block, save constraints for this one sub-path
							
							// TODO: add support for paths with multiple returns 
							// (i.e. function calls and returns from functions which still operate on the variables of interest)
							
							BasicBlock *BB = node->getBlock();
							if(( (BB->getName()).str().find("end") == std::string::npos) && ( (BB->getName()).str().find("return") == std::string::npos)){
								// pulls out the values and operations for controlled variables through each basic block 
								for(BasicBlock::iterator Ins = BB->begin(), E = BB->end(); Ins != E; ++Ins){
									if(isa<CmpInst>(Ins)){
										sleep(1);
										outs()<<"[ COMPARE FOUND ]\n";
										std::vector<std::tuple<std::string, int>> new_arg;
										std::string cmp_op = "";

										// switch case for compare instuction types 
										// easy 'conditions' to pull out that our controllabe variables must satisfy
										if(cast<CmpInst>(Ins)->getPredicate() == CmpInst::ICMP_EQ)
										{
											outs()<<"\tComparison Operator: IS EQUAL (==)\n\n";
											cmp_op = "==";
										}
										else if(cast<CmpInst>(Ins)->getPredicate() == CmpInst::ICMP_NE)
										{
											outs()<<"\tComparison Operator: IS EQUAL (!=)\n\n";
											cmp_op = "!=";
										}
										else
										{
											outs()<<"\t[-] WARNING INSERT UNSUPPORTED PREDICATE\n\n";
											return false;
										}
										first_hit = true;
										
										outs()<<"\n\n\t[+] VALUE TWO: \n";
										if(Instruction *op_inst = dyn_cast<Instruction>(Ins->getOperand(1))){
											std::unordered_set<Instruction*> used_2;
										       	outs()<<"\tReovering variable operations and starting value....\n";
										  	// uses recursion to recover the 'history' of our variables
											// i.e. the dominating operations
											recursiveValueTracer(op_inst, used_2, new_arg);
										}else{
											if(ConstantInt *con = dyn_cast<ConstantInt>(Ins->getOperand(1))){
												outs()<<"\t\t\t\t==> Found Constant Operand: Integer :: "<< con->getZExtValue() <<"\n\n\n";
												std::tuple<std::string, int> base_cmp( cmp_op, (int)con->getZExtValue());
												new_arg.push_back(base_cmp);
											}
										}
										outs()<<"\n\n\t[+] VALUE ONE: \n\n";
										if(Instruction *op_inst = dyn_cast<Instruction>(Ins->getOperand(0))){
											std::unordered_set<Instruction*> used_1;
										       	outs()<<"\tReovering variable operations and starting value....\n";
											recursiveValueTracer(op_inst, used_1, new_arg);
										}else{
											if(ConstantInt *con = dyn_cast<ConstantInt>(Ins->getOperand(0))){
												outs()<<"\t\t\t\t==> Found Constant Operand: Integer :: "<< con->getZExtValue() <<"\n";
												std::tuple<std::string, int> base_cmp2( cmp_op, (int)con->getZExtValue());
												new_arg.push_back(base_cmp2);
											}
										}
										z3_con.push_back(new_arg);
									}
								}
							}
							else
							{
								outs()<<"Breaking at end of first path with # of children\n";
								outs()<<"[ CHECK ]: 0 == "<< node->getNumChildren()<<"\n\n";
								break;							
							}
						}
					
					
					
					}
				}
			}
			outs()<<"<-- Successfully Traversed Dominator Tree --> \n";
			outs()<<"[+] The below arguments are operated upon, then this redefinition must finally  be equal to the comparisons shown below.\n\n";	

			int i = 0;	
			std::vector<std::vector<std::tuple<std::string,int>> >::iterator vvt_iterator;	
			std::vector<std::tuple<std::string,int>>::iterator vt_iterator;	
			// dump contents of path conditions in order of argument to be read into z3
			std::ofstream z3File;
			z3File.open("z3Conditions.txt");

			for(vvt_iterator = z3_con.begin(); vvt_iterator != z3_con.end(); ++(vvt_iterator)){
				outs()<<"argv["<<i<<"]";
				z3File<<"argv["<<i<<"]";
				i++;
				for(vt_iterator = (*vvt_iterator).begin(); vt_iterator != (*vvt_iterator).end(); ++(vt_iterator)){
					outs()<<";"<<std::get<0>((*vt_iterator))<<" "<<std::get<1>((*vt_iterator));	
					z3File<<";"<<std::get<0>((*vt_iterator))<<" "<<std::get<1>((*vt_iterator));	
				}
				outs()<<"\n";
				z3File<<"\n";
			}
			z3File.close();
			return true;
		}

	};

}
char DoTrePass::ID = 0;
static RegisterPass<DoTrePass> X("dotrepass", "Dominator Tree Path Solver : Export to satisfiability", false, false);

