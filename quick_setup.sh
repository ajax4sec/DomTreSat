python -c "import z3; a = z3.Solver()"
if [ $? -ne 0 ]; then
	echo "[!] Please install Microsoft's Z3 solver!"
	exit 1
fi

echo "Building the Pass Libraries"
cd LLVMPassBuild/
./build.sh
cd ../
./complete_test.sh
