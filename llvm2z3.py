from z3 import *
import time
def main():
	print 
	print "[ LLVM Ported to Z3 ] "
	key = []
	lines = [line.rstrip('\n') for line in open('z3Conditions.txt')]
	for line in lines:
		line_arr = line.split(';')
		
		# assuming input must be in ASCII range
		s = Solver()
		if not s:
			print '[!] failure in z3 installation'
			exit(-1)

		x = Int('x')
		s.add( x > 33 )
		s.add( x < 126 )

		i = 0
		print line_arr
		final_comp = 0
		# building the equation for user controlled variable i
		print '[ BUILDING EQUATION ]'
		for arg in line_arr:
			if i == 0:
				pass
			elif '==' in arg:
				final_comp = int(arg[3:])
			elif '-' in arg:
				final_comp = final_comp + int(arg[2:])
			elif '+' in arg:
				final_comp = final_comp - int(arg[2:])
			elif '^' in arg:
				final_comp = final_comp ^ int(arg[2:])
			i += 1

		foo = "x == " + str(final_comp)
		print foo
		s.add(eval(foo))
		print '[+] Added all conditions to z3'
		time.sleep(0.5)
		print "\t[+] Checking Satisfiability"
		print "\t"+str(s.check())
		m = s.model()
		print str(line_arr[0]) + ": " + str(m[x]) + "\n"
		key.append(chr(eval(str(m[x]))))
	print
	print "[ KEY FOUND ]"
	print 
	print ''.join(key)
	print


if __name__ == "__main__":
	main()
