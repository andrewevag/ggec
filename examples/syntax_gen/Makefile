
PROGSDIR=progs
GENSCIRPT=./gen.sh
GEN?=10
ERL?=/opt/homebrew/bin/erl
ERL_LIBS?=/opt/homebrew/opt/proper/proper-1.4

generate:
	@export ERL
	@export ERL_LIBS
	@$(GENSCIRPT) $(GEN)

clean:
	$(RM) *.beam *.dump
	$(RM) -rd $(PROGSDIR)