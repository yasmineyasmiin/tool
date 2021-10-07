################
# Programm name #
################

PROGRAM = StructAnal

##################
# Names of file  #
##################

##################### source ########################################

# Source files .c
FILES = main.cpp MDGraph.cpp  Modular_Obs_Graph.cpp RdPBDD.cpp 


# library name
LIBNAMES = RdP bdd

######################
# Names of directory #
######################

# Bin directory: where to put program
BINDIR = .

# Source directory: where to find .c
SRCDIR = ./Causality ./src

# Include directory: where to find .h
INCDIR =  ./Causality/src ./parser ./buddy22/src ./src  ./buddy22/include 

# Library directory: where to find library
LIBDIR =./parser  ./buddy22/src ./buddy22/lib

# Working directory
OBJDIR = ./obj

###############
# extra flags #
############### 

# Linker extra flag
LD_EXTRA_FLAGS =

# Preprocessor defines -D
DFLAGS =
# -DNDEBUG

# compiler flag
#CFLAGS = -g -Wall
CFLAGS = -O9 -Wall

#################
# Program names #
#################

CC = gcc
CXX = g++
# CXX =g++
LINKER = $(CXX)

############## Automatic ########################################## 
# Every Makefile should contain
SHELL = /bin/sh

# Compute the file list for each type

CCFILES =$(filter %.c,$(FILES))
CPPFILES =$(filter %.cpp,$(FILES))

# OFILES = $(CCFILES:.c=.o) 
DFILES = $(addprefix $(OBJDIR)/,$(CCFILES:.c=.d)) $(addprefix $(OBJDIR)/,$(CPPFILES:.cpp=.d))
OFILES = $(addprefix $(OBJDIR)/,$(CCFILES:.c=.o)) $(addprefix $(OBJDIR)/,$(CPPFILES:.cpp=.o))
# Constructs path to search for 

vpath %.cpp $(addsuffix :,$(SRCDIR)).
vpath %.c $(addsuffix :,$(SRCDIR)).
vpath %.h $(addsuffix :,$(INCDIR)).
vpath $(PROGRAM) $(BINDIR)
vpath %.o .
vpath %.d
vpath %.a $(LIBDIR)

# Constructs real names for all used libraries

REAL_LIBS	= $(addprefix -l, $(LIBNAMES))
LIB_FILES	= $(addprefix lib, $(addsuffix .a, $(LIBNAMES)))

# Constructs real name options for directory

REAL_LIBDIR     = $(addprefix -L, $(LIBDIR))
REAL_INCDIR     = $(addprefix -I,$(INCDIR))

# Compute flags for cpp ld c

CPPFLAGS	= $(DFLAGS) $(REAL_INCDIR)
LDFLAGS		= $(LD_EXTRA_FLAGS) $(REAL_LIBDIR) $(REAL_LIBS)

# Simple targets

.PHONY:all clean depend parsers parsersclean

all : $(PROGRAM)
	@echo "all Done"

depend: $(DFILES)
	@echo "all Done"

clean:
	@echo -n "Cleaning directory..."
	@-/bin/rm -f $(OBJDIR)/*.d
	@-/bin/rm -f $(OBJDIR)/*.o	
	@-/bin/rm -f $(BINDIR)/$(PROGRAM)	
	@echo clean Done

# Link object files

$(PROGRAM): $(OFILES) $(REAL_LIB)
	@echo -n "Linking $(PROGRAM) ..."
	$(LINKER) $(OFILES) $(LDFLAGS) -o $(BINDIR)/$(PROGRAM)
	@echo "Done"

# Implicite rules
.SUFFIXES : .tab.c .lex.c .tab.d .lex.d .c .o .d .l .y .cpp

# Rule to compile .c files (handle correctly C++ // comments)

$(OBJDIR)/%.o : %.c 
	@echo -n "Compiling $< :... "
	$(CC) $(CFLAGS) -c -I$(<D) $(CPPFLAGS) $<
	@mv $*.o $(OBJDIR)/.
	@echo "Done"

$(OBJDIR)/%.o : %.cpp 
	@echo -n "Compiling $< :... "
	$(CXX) $(CFLAGS) -c -I$(<D) $(CPPFLAGS) $<
	@mv $*.o $(OBJDIR)/.
	@echo "Done"

# Rule to build dependencied for each file when necessary

$(OBJDIR)/%.d : %.c 
	@echo -n "Computing dependencies for $< ... "
	@$(SHELL) -ec '$(CC) -M -I$(<D) $(CPPFLAGS) $< | sed '\''s,$*\.o,$(OBJDIR)/$*.d $(OBJDIR)/$*.o,g'\''>$@'
	@echo "Done"

$(OBJDIR)/%.d : %.cpp 
	@echo -n "Computing dependencies for $< ... "
	@$(SHELL) -ec '$(CPP) -M -I$(<D) $(CPPFLAGS) $< | sed '\''s,$*\.o,$(OBJDIR)/$*.d $(OBJDIR)/$*.o,g'\''>$@'
	@echo "Done"

# include dependance files
include $(DFILES)










