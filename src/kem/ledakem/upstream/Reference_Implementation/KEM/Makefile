ifndef SL
$(warning -------------------------------------------------------------------- )
$(warning Missing definition for the security level, defaulting to SL=1. Define)
$(warning it as an extra make parameter SL=<desired level> where the desired   )
$(warning level is an integer between 1 and 5)
$(warning -------------------------------------------------------------------- )
SL = 1
endif


ifneq ($(SL),1)
ifneq ($(SL),2)
ifneq ($(SL),3)
ifneq ($(SL),4)
ifneq ($(SL),5)
$(warning -------------------------------------------------------------------- )
$(warning Invalid definition for the security level,defaulting to SL=1. )
$(warning Define it as an extra make parameter as ) 
$(warning SL=<desired level> N0=<desired number of blocks> make )
$(warning where the desired security level --category-- is an integer between )
$(warning 1 and 5)
$(warning -------------------------------------------------------------------- )
SL = 1
endif
endif
endif
endif
endif

ifndef N0
$(warning -------------------------------------------------------------------- )
$(warning Missing definition for the number of circulant blocks N0, defaulting )
$(warning to N0=2. Define it as an extra make parameter as ) 
$(warning SL=<desired level> N0=<desired number of blocks> make )
$(warning where the desired number of blocks is an integer between 2 and 4)
$(warning -------------------------------------------------------------------- )
N0 = 2
endif

ifneq ($(N0),2)
ifneq ($(N0),3)
ifneq ($(N0),4)
$(warning -------------------------------------------------------------------- )
$(warning Invalid definition for the number of circulant blocks N0, defaulting )
$(warning to N0=2. Define it as an extra make parameter as ) 
$(warning SL=<desired level> N0=<desired number of blocks> make )
$(warning where the desired number of blocks is an integer between 2 and 4)
$(warning -------------------------------------------------------------------- )
N0 = 2
endif
endif
endif

  CC = gcc 

  CFLAGS =  -fPIC -DCATEGORY=$(SL) -DN0=$(N0) -DCPU_WORD_BITS=64 \
           -std=c99 -Wall -pedantic -Wmaybe-uninitialized -Wuninitialized \
	   -march=native -O3
  LDFLAGS = -lm 
  INCLUDES = -I./include
  SRCDIR = library
  OBJDIR = bin

# Gathers the names of all C files 
    CSRC = $(wildcard $(SRCDIR)/*.c) 
# Produces in $(COBJS) the names of .o object files for all C files
   COBJS = $(CSRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) -c $(CFLAGS) $(INCLUDES)  $< -o $@ 

.PHONY : all clean

all: $(COBJS) 
	ar rcs $(OBJDIR)/libLEDAkem_sl$(SL)_N0$(N0).a $(COBJS) 
clean:
	$(RM) $(OBJDIR)/*.o  $(OBJDIR)/libLEDAkem_sl$(SL)_N0$(N0).a
