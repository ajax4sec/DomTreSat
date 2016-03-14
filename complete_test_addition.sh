echo 
echo "...Starting LLVM to Z3 Solver..."
echo
echo "[+] Starting Addition Case Test"
./test_addition.sh
python llvm2z3.py
echo 
echo "...Solver Finished..."
