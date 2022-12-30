.PHONY: clean distclean default help

##====================================================================================##
## Variables for Building The Compiler                                                ##
##====================================================================================##


CXX=clang++-10
CXXFLAGS=-Wall -std=c++14 -g `$(LLVMCONFIG) --cxxflags` -fexceptions
LDFLAGS=$(shell $(LLVMCONFIG) --ldflags --system-libs --libs all)
BINS=ggec
INCLUDE=-I$(PWD)/inc
INCLUDE+= -I$(PWD)
DEPSOURCE=$(wildcard src/*.cpp)
DEPOBJECTS=$(patsubst %.cpp, %.o, $(DEPSOURCE))
LLVMCONFIG=llvm-config-10

##====================================================================================##
## Variables for Running Tests                                                        ##
##====================================================================================##

export PYTHON3?=$(shell which python3)
export ERL?=$(shell which erl)
export ERL_LIBS?=/home/andreas/other/proper
export PROGRAMGENPATH?=$(PWD)/EdsgerProgramGenerator
export GEN?=10 # Number Of Programs to Generate 100 may take some time to run


##====================================================================================##
## Targets for Building The Compiler                                                  ##
##====================================================================================##
default: $(BINS)

ggec: lexer.o main.o parser.o error.o ast.o tojsonstring.o general.o symbol.o semantical.o codegen.o $(DEPOBJECTS)
	$(CXX) $(INCLUDE) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


lexer.cpp lexer_funcs.hpp: lexer.l 
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp parser.hpp lexer.hpp ast.hpp error.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

parser.cpp parser.hpp: parser.y 
	bison -dv -o parser.cpp $<

parser.o: parser.cpp parser.hpp ast.hpp lexer.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

general.o: general.cpp error.hpp general.hpp lexer_funcs.hpp ast.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

error.o: error.cpp general.hpp error.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

symbol.o: symbol.cpp symbol.hpp general.hpp error.hpp ast.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

semantical.o: semantical.cpp ast.hpp error.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

codegen.o: codegen.cpp symbol.hpp ast.hpp error.hpp general.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

main.o: main.cpp general.hpp ast.hpp parser.hpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $<

ast.hpp: symbol.hpp llvmhead.hpp error.hpp
symbol.hpp: llvmhead.hpp

%.o: %.cpp
	$(CXX) -c $(INCLUDE) $(CXXFLAGS) -o $@ $^


##====================================================================================##
## Targets for Building The Test Suites' Executables                                  ##
##====================================================================================##

lexertest: lexer.o ./tests/lexer/main.o error.o ast.o tojsonstring.o parser.o general.o symbol.o semantical.o codegen.o $(DEPOBJECTS)
	$(CXX) $(CXXFLAGS) -o ./tests/lexer/$@ $^ $(LDFLAGS)

parsertest: lexer.o ./tests/parser/main.o parser.o error.o ast.o tojsonstring.o general.o symbol.o semantical.o codegen.o $(DEPOBJECTS)
	$(CXX) $(CXXFLAGS) -o ./tests/parser/$@ $^ $(LDFLAGS)

semanticstest:  lexer.o parser.o error.o ast.o tojsonstring.o general.o symbol.o semantical.o codegen.o $(DEPOBJECTS) ./tests/semantics/main.o
	$(CXX) $(CXXFLAGS) -o ./tests/semantics/$@ $^ $(LDFLAGS)

codegentest:   lexer.o parser.o error.o ast.o tojsonstring.o general.o symbol.o semantical.o codegen.o $(DEPOBJECTS) ./tests/codegen/main.o
	$(CXX) $(CXXFLAGS) -o ./tests/codegen/$@ $^ $(LDFLAGS)


##====================================================================================##
## Targets for Running The Test suites                                                ##
##====================================================================================##

lexersuite: lexertest
	@echo "🧪 Running Lexer Suite :"
	@$(PYTHON3) ./tests/lexer/runner.sh

parsersuite: parsertest
	@echo "🧪 Running Parser Suite :"
	@$(PYTHON3) ./tests/parser/runner.sh
	@echo "🧪 Running Randomly Generated Edsger Programs :"
	@echo "Generating the input files :"
	@export ERL=$(ERL)
	@export ERL_LIBS=$(ERL_LIBS)
	@$(MAKE) -C examples/syntax_gen generate GEN="$(GEN)" ERL="$(ERL)" ERL_LIBS="$(ERL_LIBS)"
	@echo "Running the input files on the parser : ⛏️"
	@$(PYTHON3) ./tests/parser/runner_gen.sh $(GEN) 

semanticssuite: semanticstest
	@echo "🧪 Running Semantics Suite :"
	@$(PYTHON3) ./tests/semantics/runner.sh
	@echo "🧪 Running Randomly Generated Edsger Programs :"
	@echo "Generating the input files :"
	@$(MAKE) -C  $(PROGRAMGENPATH) generate GEN="$(GEN)"
	@echo "Running the input files on the semantics analyzer : ⛏️"
	@export PROGRAMGENPATH
	@$(PYTHON3) ./tests/semantics/runner_gen.sh $(GEN)

codegenInC:
	$(MAKE) -C ./tests/codegen/inC

codegensuite: codegentest codegenInC libEdsger.a
	@echo "🧪 Running Codegen Suite :"
	@$(PYTHON3) ./tests/codegen/runner.sh

test: lexersuite parsersuite semanticssuite codegensuite

##====================================================================================##
## Targets for Building The Edsger Library                                            ##
##====================================================================================##

EDSGERLIBSOURCE = $(wildcard lib/*.c)
EDSGEROBJ = $(patsubst %.c, %.o, $(EDSGERLIBSOURCE))
CC=clang-10
CFLAGS=-g -Wall

library: libEdsger.a

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^
libEdsger.a: $(EDSGEROBJ)
	$(AR) rcs $@ $^

##====================================================================================##
## Clean                                                                              ##
##====================================================================================##
.PHONY: clean distclean

clean:
	$(RM) *.o lexer.cpp
	$(RM) parser.hpp parser.output parser.cpp
	$(RM) lexer_funcs.hpp
	$(RM) ./src/*.o
	$(RM) $(EDSGEROBJ)
	$(RM) libEdsger.a
	$(RM) out out.ll out.s
	$(MAKE) -C ./tests/lexer clean
	$(MAKE) -C ./tests/parser clean
	$(MAKE) -C ./examples/syntax_gen clean
	$(MAKE) -C ./tests/semantics clean
	$(MAKE) -C $(PROGRAMGENPATH) clean
	$(MAKE) -C ./tests/codegen clean
	$(MAKE) -C ./tests/codegen/inC clean

distclean: clean
	$(RM) $(BINS)

##====================================================================================##
## Help Message Print                                                                 ##
##====================================================================================##

.PHONY: help

BLUE=\e[0;34m
RESET=\e[0m
help:
	@echo "➖ $(BLUE)make: $(RESET)"
	@echo "\tBuilds the compiler"
	@echo "\t     Deps:"
	@echo "\t\t- clang++-10"
	@echo "\t\t- llvm-10"
	@echo "\t\t- flex  >= 2.6.4"
	@echo "\t\t- bison >= 3.5.1"
	@echo "➖ $(BLUE)make library: $(RESET)"
	@echo "\tBuild the Edsger Library"
	@echo "\t     Deps:"
	@echo "\t\t- clang-10"
	@echo "➖ $(BLUE)make pythonModules: $(RESET)"
	@echo "\t Install Python Modules"
	@echo "➖ $(BLUE)make deps: $(RESET)"
	@echo "\t Install Python Modules and Haskell Stack used for testing"
	@echo "➖ $(BLUE)make clean [PROGRAMGENPATH=EdsgerProgramGenerator path]: $(RESET)"
	@echo "\tRemoves all automatically generated files except the final executable"
	@echo "\tWill return non-zero exit code if PROGRAMGENPATH is not defined but will delete anything else"
	@echo "➖ $(BLUE)make distclean: $(RESET)"
	@echo "\tremoves all automatically generated files and the final executable"
	@echo "➖ $(BLUE)make test [GEN=number] [PYTHON3=python3 path] [ERL=erl path] [ERL_LIBS=proper path] [PROGRAMGENPATH=EdsgerProgramGenerator path]: $(RESET)"
	@echo "\tDefine The Variables GEN, PYTHON3, ERL, ERL_LIBS, PROGRAMGENPATH to run either on Makefile or when running make test as shown above"
	@echo "\t- Run lexer suite"
	@echo "\t     Deps:"
	@echo "\t- Run parser suite"
	@echo "\t     Deps:"
	@echo "\t\t- python3"
	@echo "\t\t  - os         module"
	@echo "\t\t  - subprocess module"
	@echo "\t\t  - json       module"
	@echo "\t\t  - deepdiff   module"
	@echo "\t\t  - tqdm       module"
	@echo "\t\t- Erlang/OTP"
	@echo "\t\t- PropEr"
	@echo "\t\t- https://github.com/kostis/ntua_compilers"
	@echo "\t- Run semantics suite"
	@echo "\t     Deps:"
	@echo "\t\t- Haskell Stack"
	@echo "\t\t- https://github.com/andrewevag/EdsgerProgramGenerator"
	@echo "\t\t- python3"
	@echo "\t- Run codegen suite"
	@echo "\t     Deps:"
	@echo "\t\t- python3"
	@echo "\t\t- gcc (any version that support -std=gnu11)"
	

##====================================================================================##
## Install Python modules                                                             ##
##====================================================================================##
.PHONY: deps pythonModules haskellStack

deps: pythonModules haskellStack

pythonModules: 
	$(PYTHON3) -m pip install deepdiff
	$(PYTHON3) -m pip install tqdm
	
# https://docs.haskellstack.org/en/stable/
haskellStack:
	curl -sSL https://get.haskellstack.org/ | sh



##====================================================================================##
## Debug                                                                              ##
##====================================================================================##

debug:
	@echo "python3 = $(PYTHON3)"
	@echo "erl     = $(ERL)"
	@echo "PropEr  = $(ERL_LIBS)"
	@echo "genpath = $(PROGRAMGENPATH)"


##====================================================================================##
## Targets for Creating Tests Fast for the Code Generations                           ##
##====================================================================================##

LASTCODEGENTEST?=globals2
NEWNAME?=newname
newcodegentest:
	cp ./tests/codegen/inC/$(LASTCODEGENTEST).c ./tests/codegen/inC/$(NEWNAME).c
newcodegenfix:
	cp ./tests/codegen/inC/$(NEWNAME).c ./tests/codegen/programs/$(NEWNAME).eds
	sed -i 's/int16_t/int/g' ./tests/codegen/programs/$(NEWNAME).eds
	sed -i 's/long double/double/g' ./tests/codegen/programs/$(NEWNAME).eds
	touch ./tests/codegen/inputs/$(NEWNAME).inp
undonewcodegentest:
	rm ./tests/codegen/inC/$(NEWNAME).c
	rm ./tests/codegen/programs/$(NEWNAME).eds
	rm ./tests/codegen/inputs/$(NEWNAME).inp
