echo 
echo "...Starting LLVM to Z3 Solver..."
echo
echo "[+] Starting Subtraction Case Test"
./test_subtraction.sh
python llvm2z3.py
echo 
echo "...Solver Finished..."
