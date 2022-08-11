#!/opt/homebrew/bin/python3

from os import listdir
from os.path import isfile, join
import subprocess
import difflib
from deepdiff import DeepDiff
import json

testerdir = './tests/parser'


def getDirContents(mypath):
	return [f"{mypath}/{f}" for f in listdir(mypath) if isfile(join(mypath, f))]

programs = getDirContents(f'{testerdir}/programs')
results  = getDirContents(f'{testerdir}/results')


for prog in programs:
	result = f"{prog.replace('programs', 'results').split('.eds')[0]}.json"
	f = open(result)
	g = open(prog)
	output = f.read()
	inp = g.read()
	cmd = f"{testerdir}/parsertest"
	process = subprocess.run(cmd.split(" "),capture_output=True, text=True, input=inp)
	exitcode = process.returncode
	producedOutput = process.stdout


	producedJSON = json.loads(producedOutput)
	expectedJSON = json.loads(output)
	

	if exitcode == 1:
		if output == "fail\n":
			print(f'✔️   Test {prog} passed!')
		else:
			print(f'❌   Test {prog} failed!')
			print(f'produced :{producedOutput}')
			print(f'expected :{output}')
	else:
		if producedJSON == expectedJSON:
			print(f'✔️   Test {prog} passed!')
		else: 
			print(f'❌   Test {prog} failed!')
			print("===== JSON DIFFERENCES =====")	
			print('ProducedJSON', producedJSON)
			print('ExpectedJSON', expectedJSON)
			result = DeepDiff(producedJSON, expectedJSON)
			try:
				print('\n\n++++++++++++++Dictionary Item Added++++++++++++++\n\n', result['dictionary_item_added'], sep='\n', end='\n\n++++++++++++++++++++++++++++++++++++\n')
			except:
				pass
			try:
				print('\n\n--------------Dictionary Item Removed--------------\n\n', result['dictionary_item_removed'], sep='\n', end='\n\n-----------------------------------\n')
			except:
				pass
			try:
				print('\n\n!=!=!=!=!=!=!=Values Changed!=!=!=!=!=!=!=\n\n', result['values_changed'], sep='\n', end='\n\n!=!=!=!=!=!=!=!=!=!=!=!=!=!=\n')
			except:
				pass

