

from os import listdir
from os.path import isfile, join
import subprocess
import difflib

testerdir = './tests/semantics'


def getDirContents(mypath):
	return [f"{mypath}/{f}" for f in listdir(mypath) if isfile(join(mypath, f))]

programs = getDirContents(f'{testerdir}/programs')
results  = getDirContents(f'{testerdir}/results')


for prog in programs:
	result = f"{prog.replace('programs', 'results').split('.eds')[0]}.res"
	f = open(result)
	g = open(prog)
	output = f.read()
	inp = g.read()
	cmd = f"{testerdir}/semanticstest {prog}"
	process = subprocess.run(cmd.split(" "),capture_output=True, text=True)
	exitcode = process.returncode
	producedOutput = process.stdout
	if exitcode != 0:
		if output == "fail":
			print(f'✔️   Test {prog} passed!')
		else:
			print(f'❌   Test {prog} failed!')
			print(f'produced :{producedOutput}')
			print(f'expected :{output}')
			print(exitcode)
	else:
		if output == "success":
			print(f'✔️   Test {prog} passed!')
		else: 
			print(f'❌   Test {prog} failed!')
			print(f'produced :{producedOutput}')
			print(f'expected :{output}')
			lineno = 0
			for i, c in enumerate(producedOutput):
				if c == '\n':
					lineno +=1
				if producedOutput[i] != output[i]:
					print('first mistake @', i, 'line: ', lineno)
					break
				


