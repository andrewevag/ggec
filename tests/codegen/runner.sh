

from os import listdir
from os.path import isfile, join
import subprocess
import difflib

testerdir = './tests/codegen'

def getDirContents(mypath):
	return [f"{mypath}/{f}" for f in listdir(mypath) if isfile(join(mypath, f))]

programs  = getDirContents(f'{testerdir}/programs')
# results   = getDirContents(f'{testerdir}/results')

for prog in programs:
	
	# result = f"{prog.replace('programs', 'results').split('.eds')[0]}.res"
	paradigm = f"{prog.replace('programs', 'inC').split('.eds')[0]}"
	inputp   = f"{prog.replace('programs', 'inputs').split('.eds')[0]}.inp"
	# print(prog, paradigm, inputp)
	# f = open(result)
	# g = open(prog)
	# output = f.read()
	g = open(inputp)
	inputp = g.read()
	
	cmd = f"{paradigm}"
	paradigmProcess = subprocess.run(cmd.split(" "),capture_output=True, text=True, input=inputp)
	output    = paradigmProcess.stdout

	cmd = f"{testerdir}/compile.sh {prog}"
	process = subprocess.run(cmd.split(" "),capture_output=True, text=True)
	exitcode = process.returncode
	producedOutput = process.stdout
	producedError  = process.stderr
	if exitcode != 0:
		print(f'❌   Test {prog} failed!')
		print(f'expected :{output}')
		print(f'Got {producedOutput}')
		print(f'Got stderr: {producedError}')
		print(exitcode)
	else:
		process = subprocess.run('./out', capture_output=True, text=True, input=inputp)
		producedOutput = process.stdout
		if producedOutput != output or exitcode != 0:
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
		else:
			print(f'✔️   Test {prog} passed!')
			# print(f'produced : {producedOutput}')
		# if output == "success":
		# 	print(f'✔️   Test {prog} passed!')
		# else: 
		# 	print(f'❌   Test {prog} failed!')
		# 	print(f'produced :{producedOutput}')
		# 	print(f'expected :{output}')
		# 	lineno = 0
		# 	for i, c in enumerate(producedOutput):
		# 		if c == '\n':
		# 			lineno +=1
		# 		if producedOutput[i] != output[i]:
		# 			print('first mistake @', i, 'line: ', lineno)
		# 			break
				


