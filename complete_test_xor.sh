echo 
echo "...Starting LLVM to Z3 Solver..."
echo
echo "[+] Starting Xor Case Test"
./test_xor.sh
python llvm2z3.py
echo 
echo "...Solver Finished..."
