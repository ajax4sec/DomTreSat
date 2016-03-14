DIR=".//LLVMPassBuild/DTS_BUILD/llvm-3.7.1/build/Release+Asserts/"
echo
echo "[ CLANG COMPILING TEST APP SOURCES ]"
echo
$DIR/bin/clang -cc1 -emit-llvm test_xor.c -o test_xor.bc
echo
echo "[ RUNNING DOMINATOR TREE PASS ]"
echo
$DIR/bin/opt -load $DIR/lib/LLVMPassBuild.* -dotrepass test_xor.bc -o test_xor.bc
echo
echo "[ FINISHED ]"
echo
