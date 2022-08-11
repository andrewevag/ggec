#!/opt/homebrew/bin/python3

import sys
import subprocess
from tqdm import tqdm
syntaxGenScriptPath='/Users/nicole/Documents/ggec/examples/syntax_gen/gen.sh'
progsdir = '/Users/nicole/Documents/ggec/examples/syntax_gen/progs'

if len(sys.argv) < 2:
	print('Usage: ./runner_gen number_of_programs')
	exit(1)

num_of_tests = int(sys.argv[1])
noFailed = 0
failed = []
for i in tqdm(range(1, num_of_tests+1)):
	
	p = subprocess.Popen([f'./tests/parser/parsertest'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
	f = open(f'{progsdir}/p{i}.eds')
	inp = f.read()
	f.close()
	p.communicate(input=inp.encode())
	if p.returncode != 0:
		failed.append(i)
		noFailed += 1

if noFailed > 0:
	for i in failed:
		print(f'❌   Test {progsdir}/p{i}.eds failed!')
	print(f'❌   Failed in {noFailed}/{num_of_tests}')
else:
	print(f'✔️   Passed All Random Tests!')
