echo 
echo "...Starting LLVM to Z3 Solver..."
echo
echo "[+] Starting Base Case Test"
./test_base_case.sh
python llvm2z3.py
echo 
echo "...Solver Finished..."
