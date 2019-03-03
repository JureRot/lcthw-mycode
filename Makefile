CFLAGS=-Wall -g -DNDEBUG -lm
#-Wall is to display "all" warnings, -g to have backtrac in gdb and lldb
#-DNDEBUG is to define NDEBUG var to ignore debug outputs from dbg.h
#-lm is for ex25 log10() from math.h to link it correctly

#this should include all .c files in this directory (optimally)
all: ex1 ex3

#again, should include all .c files (optimally)
clean:
	rm -f ex1
	rm -f ex3

#would need a debug rule or target so it would not use the -DNDEBUG flag (something like make debug ./ex#)
#something like this will probably be explaned in ex28

#creates object file from ex22.c and complies ex22_main with that object file
combine_ex22:
	rm ex22.o ex22_main
	cc -Wall -g -DNDEBUG -c -o ex22.o ex22.c
	cc -Wall -g -DNDEBUG ex22_main.c ex22.o -o ex22_main
