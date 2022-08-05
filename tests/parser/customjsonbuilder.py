#!/usr/bin/python3

from asyncio import subprocess
import enum
import json
from pygments import highlight
import subprocess

def prRed(skk): print("\033[91m {}\033[00m" .format(skk))
def prGreen(skk): print("\033[92m {}\033[00m" .format(skk))
def prYellow(skk): print("\033[93m {}\033[00m" .format(skk))
def prLightPurple(skk): print("\033[94m {}\033[00m" .format(skk))
def prPurple(skk): print("\033[95m {}\033[00m" .format(skk))
def prCyan(skk): print("\033[96m {}\033[00m" .format(skk))
def prLightGray(skk): print("\033[97m {}\033[00m" .format(skk))
def prBlack(skk): print("\033[98m {}\033[00m" .format(skk))
 



head = {"Program" : []}
state = head["Program"]
prevs = [head]

print('Enter Source Code Path')
sourcepath = input()
f = open(sourcepath, 'r')
allfile = f.read()
source = allfile.splitlines()
f.close()
line = 0
p = subprocess.run('pygmentize ../../tojsonstring.cpp'.split(" "), stdout=subprocess.PIPE)
res = p.stdout.decode('utf-8')
jsonlines = res.splitlines()


while True:
	print('===========Commands:===========\n\nNewNode : n')
	print('Go one up : u')
	print('Go one down : d')
	print('Simple Attribute : a')
	print('Show state : state')
	print('Save Json file: save')
	print('Delete Last Node: delete')
	print('IndexedDelete: index delete')
	print('Next Source Line: next')
	print('Previous Source Line : prev')
	print('Show Class Body: show')
	print('Show all classes: list')
	print('Show precedencies : prec')
	print('Exit: exit')
	print('===============================\n')
	print('Total JSON:')
	print(head, end='\n\n')
	print('Prev Stack')
	for i, val in enumerate(prevs):
		print('--', val)
	try:
		if prevs != [{ "Program" : []}]:
			val = list(prevs[-1][-1].keys())[-1]
			print(f'\033[95m↑ : {val}\033[00m')
			flag1 = False
			for i, val1 in enumerate(jsonlines):
				if str(val) in val1:
					flag1 = True
				if flag1:
					print(f'\033[95m>\033[00m', val1)
					if val1.startswith('}'):
						flag1 = False
	except:
		pass
		

		
	print(f'\033[93m{len(state)}', '>>', state, '\033[00m')
	print('\033[91m@ ', source[line], '\033[00m')
	print('Command> ', end='')
	try:

		inp = input()
		if inp == "exit":
			break
		if inp == "n":
			print("Enter Node Name: ")
			name = input()
			temp = {
				name : []
			}
			prevs.append(state)
			state.append(temp)
			state = state[-1][name]

		if inp == "u":
			state = prevs[-1]
			if prevs == []:
				break
			else: prevs.pop()
		if inp == "d":
			print('Name of the node to descent to')
			name = input()
			indexes = []
			flag = False
			
			for i, val in enumerate(state):
				print(i, val)
				if name in val.keys():
					flag = True
					indexes.append(i)
			if flag:
				if len(indexes) == 1:
					index = 0
					prevs.append(state)
					state = state[index][name]
					continue
				print('Pick an index: ')
				print(indexes)
				index = int(input())
				print('Index Chosen', index)
				prevs.append(state)
				print(f'state = {state}')
				state = state[index][name]
		if inp == "state":
			print(state)
			print('Enter a line to continue')
			input()
		if inp == "a":
			print('Enter Attribute Name:')
			name = input()
			print('Enter Attribute Value')
			val = str(input())
			state.append(
				{
					name : val
				}
			)
		if inp == "delete":
			if len(state) == 0:
				print('State has nothing to delete')
			else:
				state.pop()
		if inp == "save":
			print('Enter filename')
			filename = input()
			jsonout = json.dumps(head, indent=4)
			f = open(filename, 'w')
			f.write(jsonout)
			f.close()
		if inp == "index delete":
			indexes = []
			for i, val in enumerate(state):
				print(i, '->', val)
			index = int(input())
			if index in range(len(state)):
				del state[index]
		if inp == "next":
			line += 1
			if line == len(source):
				prLightPurple('No More Source')
				line -= 1
		if inp == "show":
			print('Enter which class to show')
			cl = input()
			g = open("../../ast.hpp", 'r')
			lines = g.read().splitlines()
			g.close()
			flag = False
			for i in range(len(lines)):
				if lines[i].startswith(f'class {cl}'):
					flag = True
				if flag:
					print(i, lines[i])
					if lines[i].startswith('};'):
						flag = False	
			print('Enter A Line to Continue')
			input()
		if inp == "list":
			g = open("../../ast.hpp", 'r')
			lines = g.read().splitlines()
			g.close()
			for i in range(len(lines)):
				if lines[i].startswith(f'class'):
					print(lines[i])
			print('Enter A Line to Continue')
			input()
		if inp == 'prec':
			print('[ ] ( )')
			print('++ -- postfix')
			print('& * + - ! prefix')
			print('new delete prefix')
			print('++ -- prefix')
			print('( ) typecast')
			print('* / % infix left')
			print('+ - infix left')
			print('== != >= < > <= >= infix none')
			print('&& infix left')
			print('|| infix left')
			print('? :')
			print('= += -= *= /= %= infix right')
			print(', infix left')
			print('\033[91m','@ ', source[line], '\033[00m')
			print('Enter A Line to Continue')
			input()
		if inp == 'prev':
			if line > 0:
				line -= 1
	except:
		pass







print(head)
		
	
		
