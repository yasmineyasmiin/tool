LIB = libRdP.a

FILES = Net.cpp RdPMonteur.tab.cpp RdPMonteur.lex.c

CCFILES =$(filter %.c,$(FILES))
CPPFILES =$(filter %.cpp,$(FILES))

OBJS = $(CPPFILES:.cpp=.o) $(CCFILES:.c=.o)
DEPS = $(CPPFILES:.cpp=.d) $(CCFILES:.c=.d)

CC =gcc
CXX = g++
CXXFLAGS= -O3 -Wall

all: $(LIB)

$(LIB): $(OBJS)
	ar ru $(LIB) $(OBJS)
	ranlib $(LIB)

# implicit rules

.cpp.o: RdPMonteur.tab.cpp RdPMonteur.lex.c
	$(CXX) -c $(CXXFLAGS) $<

.c.o: RdPMonteur.tab.cpp RdPMonteur.lex.c
	$(CC) -c $(CXXFLAGS) $<

clean:
	rm -f $(LIB) *.o *.d *~ *.tab.h *.tab.cpp *.lex.c

%.d: %.cpp RdPMonteur.tab.cpp RdPMonteur.lex.c
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $(INCLUDES) $< \
                | sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
                [ -s $@ ] || rm -f $@'

%.d: %.c RdPMonteur.tab.cpp RdPMonteur.lex.c
	@$(SHELL) -ec '$(CC) -M $(CXXFLAGS) $(INCLUDES) $< \
                | sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
                [ -s $@ ] || rm -f $@'

%.tab.cpp: %.y
	bison -d  $<
	mv $*.tab.c $*.tab.cpp

%.lex.c: %.l %.tab.cpp
	flex  -o$*.lex.c $<

-include $(DEPS)        












