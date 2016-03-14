add_llvm_loadable_module( LLVMMAST
	LLVMDomTreePass.cpp
)

add_dependencies(LLVMMAST intrinsics_gen)
