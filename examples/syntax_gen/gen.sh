#! /bin/bash

# Generates N Edsger programs in the 'progs' dir.
#   (N is a command-line argument.)
#
# To use it, you need to first  declare the dirs
# where Erlang/OTP and PropEr are in your system.

# Ubuntu systems
# apt-get install erlang
# git clone https://github.com/proper-testing/proper.git
#   make all
if [ -z ${ERL} ] || [ -z ${ERL_LIBS} ]
then
  echo "❗ Did not set ERL and ERL_LIBS!!"
  echo "❗ Will set to default"
  ERL=/opt/homebrew/bin/erl
  export ERL_LIBS=/opt/homebrew/opt/proper/proper-1.4
fi

${ERL}c -pa ${ERL_LIBS}/ebin +debug_info *.erl

mkdir -p progs
for i in $(seq 1 $1)
do
  $ERL -noshell -noinput -s edsger_pp p -s erlang halt > progs/p$i.eds
  printf "."
done
printf "\n"