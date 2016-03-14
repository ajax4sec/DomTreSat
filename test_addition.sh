DIR=".//LLVMPassBuild/DTS_BUILD/llvm-3.7.1/build/Release+Asserts/"
echo
echo "[ CLANG COMPILING TEST APP SOURCES ]"
echo
$DIR/bin/clang -cc1 -emit-llvm test_addition.c -o test_addition.bc
echo
echo "[ RUNNING DOMINATOR TREE PASS ]"
echo
$DIR/bin/opt -load $DIR/lib/LLVMPassBuild.* -dotrepass test_addition.bc -o test_addition.bc
echo
echo "[ FINISHED ]"
echo
