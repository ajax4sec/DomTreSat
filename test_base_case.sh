DIR=".//LLVMPassBuild/DTS_BUILD/llvm-3.7.1/build/Release+Asserts/"
echo
echo "[ CLANG COMPILING TEST APP SOURCES ]"
echo
$DIR/bin/clang -cc1 -emit-llvm test_base_case.c -o test_base_case.bc
echo
echo "[ RUNNING DOMINATOR TREE PASS ]"
echo
$DIR/bin/opt -load $DIR/lib/LLVMPassBuild.* -dotrepass test_base_case.bc -o test_base_case.bc
echo
echo "[ FINISHED ]"
echo
