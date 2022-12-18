
import os
import sys
import subprocess
from tqdm import tqdm

syntaxGenScriptPath=f'{os.getcwd()}/examples/syntax_gen/gen.sh'
progsdir = f'{os.getcwd()}/examples/syntax_gen/progs'

syntaxGenScriptPath=f'/home/andreas/Projects/EdsgerProgramGenerator/gen.sh'
progsdir = f'/home/andreas/Projects/EdsgerProgramGenerator'


if len(sys.argv) < 2:
	print('Usage: ./runner_gen number_of_programs')
	exit(1)

num_of_tests = int(sys.argv[1])
noFailed = 0
failed = []
for i in tqdm(range(1, num_of_tests+1)):
	
	p = subprocess.Popen([f'./tests/semantics/semanticstest', f'{progsdir}/p{i}.eds'], stdout=subprocess.DEVNULL)
	# f = open(f'{progsdir}/p{i}.eds')
	# inp = f.read()
	# f.close()
	# p.communicate(input=inp.encode())
	p.wait()
	if p.returncode != 0:
		failed.append(i)
		noFailed += 1

if noFailed > 0:
	for i in failed:
		print(f'❌   Test {progsdir}/p{i}.eds failed!')
	print(f'❌   Failed in {noFailed}/{num_of_tests}')
else:
	print(f'✔️   Passed All Random Tests!')
