DIR=".//LLVMPassBuild/DTS_BUILD/llvm-3.7.1/build/Release+Asserts/"
echo
echo "[ CLANG COMPILING TEST APP SOURCES ]"
echo
$DIR/bin/clang -cc1 -emit-llvm test_subtraction.c -o test_subtraction.bc
echo
echo "[ RUNNING DOMINATOR TREE PASS ]"
echo
$DIR/bin/opt -load $DIR/lib/LLVMPassBuild.* -dotrepass test_subtraction.bc -o test_subtraction.bc
echo
echo "[ FINISHED ]"
echo
