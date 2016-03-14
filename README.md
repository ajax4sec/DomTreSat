# DomTreSat
DomTreSat (DTS) is a static analysis system that takes source code as input and automatically produces path satisfiability reports for paths gathered from a created Dominator Tree structure. It obtains maximal code coverage with minimal human intervention. DomTreSat creates a dominator tree of user-controlled variables, then outputs constraints and operations placed on their values to be fed to a satisfiability solver. This generates potential points of interest in the program for vulnerability discovery. 

## Primary Motivation
The main use of this tool is determine reachability of controllable input to a target in the program, as well as what this input needs to be to get there. The target is automatically set to be the most dominated path of the tree. This helps an auditor identify a path to vulnerable code through a series of checks like so:

```
     INPUT
        \
       CHECK
       /   |
     FAIL  CHECK
          /   |
        FAIL  CHECK
             /   |
           FAIL  VULNERABILITY
```

![Alt text](./887b255301.png?raw=true "example target binary structure")

In these type of programs it is the most dominated path that we want to build and analyse the Use-Define Chain for.

## Analysis Theory
### Use of a Dominator Tree
This tool defaults to analyzing the most dominated paths first. These paths can be described as those with variables that are acted upon the most. This can be changed based on need and a quick recompilation script is in place to generate altered libraries. 

### Use of Use-Define Chain Analysis
Use-Define Chains (Use-Def) are a data structure that consists of a use of a variable, and all the definitions of that variable that can reach that use without any other intervening definitions.

This tool relies on pulling these data structures from the Most Dominated Path which is found through the Dominator Tree created. The basic idea is that given a targeted Use, i.e. the use of a variable to either get to or cause a vulnerability, we can trace this variable up through all of its (re-)definitions and determine A) if it is user controllable, and B) what operations are done on this input before it is used in the vulnerability (these become our constraints).  

## Building

DomTreSat has been tested on LLVM 3.7.1. The `$./quick_setup.sh` helper script will build the tool and run tests. It creates an `LLVMPassBuild.dylib` (or `.so`). If you want to alter or build on the analysis methods, use `$./partial_build.sh` for an incremental re-build.

## Usage
This section describes how to run tests and feed them to the SAT solver.

### Base case test

Tests for tracing definitions of variables (their Use-Def chains) through the most dominated path

`$./test_base_case.sh`

To run this test with z3 for generating input to satisfy target path:

`$./complete_test_base_case.sh`

### Addition operator test

Tests for tracing definitions of variables (their Use-Def chains) through the most dominated path, where the Use-Def Chain now contains re-definitions after addition operations are applied to the variable.

`$./test_addition.sh`

To run pass and z3 for generating input to satisfy target path:

`$./complete_test_addition.sh`

### Subtraction operator test

Tests for tracing definitions of variables (their Use-Def chains) through the most dominated path, where the Use-Def Chain now contains re-definitions after subtraction operations are applied to the variable.

`$./test_subtraction.sh`

To run pass and z3 for generating input to satisfy target path:

`$./complete_test_subtraction.sh`

### XOR operator test

Tests for tracing definitions of variables (their Use-Def chains) through the most dominated path, where the Use-Def Chain now contains re-definitions after xor operations are applied to the variable.

`$./test_xor.sh`

To run pass and z3 for generating input to satisfy target path:

`$./complete_test_xor.sh`

### Expected Output For Addition Operator Run

```
$./complete_test_base_case.sh


...Starting LLVM to Z3 Solver...

[+] Starting Base Case Test

[ CLANG COMPILING TEST APP SOURCES ]


[ RUNNING DOMINATOR TREE PASS ]


<-- Starting analysis from main() --> 
	[+] Tracing first path of DominatorTree


[ COMPARE FOUND ]
	Comparison Operator: IS EQUAL (==)



	[+] VALUE TWO: 
				==> Found Constant Operand: Integer :: 69




	[+] VALUE ONE: 

	Reovering variable operations and starting value....
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
		.... Potential find. Store took value from :: argv

			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
[ COMPARE FOUND ]
	Comparison Operator: IS EQUAL (==)



	[+] VALUE TWO: 
				==> Found Constant Operand: Integer :: 68




	[+] VALUE ONE: 

	Reovering variable operations and starting value....
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
		.... Potential find. Store took value from :: argv

			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
[ COMPARE FOUND ]
	Comparison Operator: IS EQUAL (==)



	[+] VALUE TWO: 
				==> Found Constant Operand: Integer :: 67




	[+] VALUE ONE: 

	Reovering variable operations and starting value....
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
		.... Potential find. Store took value from :: argv

			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
[ COMPARE FOUND ]
	Comparison Operator: IS EQUAL (==)



	[+] VALUE TWO: 
				==> Found Constant Operand: Integer :: 66




	[+] VALUE ONE: 

	Reovering variable operations and starting value....
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
	[!] FOUND TERMINATING ALLOCATION

			[-] RECURSING THROUGH OPERATION
		[ TRACING BACK HISTORY OF OPERAND VALUE ]
		[+] Found StoreInst to trace
		.... Potential find. Store took value from :: argv

			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
			[-] RECURSING THROUGH OPERATION
Breaking at end of first path with # of children
[ CHECK ]: 0 == 0

<-- Successfully Traversed Dominator Tree --> 
[+] The below arguments are operated upon, then this redefinition must finally  be equal to the comparisons shown below.

argv[0];== 69
argv[1];== 68
argv[2];== 67
argv[3];== 66

[ FINISHED ]


[ LLVM Ported to Z3 ] 
['argv[0]', '== 69']
[ BUILDING EQUATION ]
x == 69
[+] Added all conditions to z3
	[+] Checking Satisfiability
	sat
argv[0]: 69

['argv[1]', '== 68']
[ BUILDING EQUATION ]
x == 68
[+] Added all conditions to z3
	[+] Checking Satisfiability
	sat
argv[1]: 68

['argv[2]', '== 67']
[ BUILDING EQUATION ]
x == 67
[+] Added all conditions to z3
	[+] Checking Satisfiability
	sat
argv[2]: 67

['argv[3]', '== 66']
[ BUILDING EQUATION ]
x == 66
[+] Added all conditions to z3
	[+] Checking Satisfiability
	sat
argv[3]: 66


[ KEY FOUND ]

EDCB


...Solver Finished...
```
