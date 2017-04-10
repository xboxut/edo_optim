############################################################################################
########### MAKEFILE POUR LA LIBRAIRIE SIG_SIMU ###########
###########################################################
##
## pour que le make file fonctionne, il est necessaire d'a
## voir les librairie GSL et MPI d'installees.
## 
## Ce make fonctionne sous les OS linux/unix et sous windows.
##
## Il est conseillé d'utiliser MingW et l'environnement msys
## pour windows afin de compiler ces sources. Msys fournit un
## bash ainsi que les outils de bases necessaire a la compila
## -tion. make autoconf etc...
##
## FONCTIONNEMENT DU MAKEFILE
##
## Pour construire tous les elements relatifs a la bibliotheque
## sig_simu:
## 
## make all
##
##
##########
############################
##########
## Plusieurs options  peuvent etre utilisees pour compiler
## la librairie.  Un mode debugage existe, il fait apparaitre
## de nombreux message d'etat sur les operation en cours dans la lib
## -rairie.  Pour activer ce mode, on utilisera l'option :
## 
##  FLAGS=-D VERBOSE
##
## D'autre options de compilations relatives a GCC peuvent etre fournis 
## a flag pour optimiser le code compile. On peut par exemple passer:
## FLAGS=-msse4 pour compiler avec le support des instruction SSE4.
##
## Par defaut, le code est compile avec une optimisation O3, si des informations
## de debug doivent etre integre au code source, on modifiera la ligne CFLAGS de
## makefile.
##
## 
## l'inclusion de openmp se fait de maniere statique, on veillera donc a bien posseder la 
## librairie pthread.
############################################################################################
CC=g++
CFLAGS=-O3 ${FLAGS}
LIBS=-lgsl 

S_INTERP_EDO=edo_optim
EXAMPLE_EDO=edo_optim_example
	
all: default  
#example
	
default: $(S_INTERP_EDO)

#example: $(EXAMPLE_EDO)


$(S_INTERP_EDO): script_interpreter.o edo_system.o main.o
ifeq	($(OPTION),OMP)
	$(CC) $(CFLAGS) -static -fopenmp script_interpreter.o edo_system.o main.o  -Wall -lgomp $(LIBS) -o $(S_INTERP_EDO)
else
	$(CC) $(CFLAGS) -static script_interpreter.o edo_system.o main.o -Wall $(LIBS) -o $(S_INTERP_EDO)
endif


## COMPILATION DES EXEMPLES
#$(EXAMPLE_SSIMU): sig_simu.o data_treatment.o measurement.o scripting.o main_script.o main.cpp sig_simu.h 
#ifeq	($(OPTION),OMP)
#	$(CC) $(CFLAGS) -static -fopenmp main.cpp sig_simu.o  data_treatment.o measurement.o  -Wall -lgomp $(LIBS) -o $(EXAMPLE_SSIMU)
#else
#	$(CC) $(CFLAGS) -static  main.cpp sig_simu.o  data_treatment.o measurement.o  -Wall $(LIBS) -o $(EXAMPLE_SSIMU)
#endif



edo_system.o: edo_system.cpp edo_system.h
ifeq	($(OPTION),OMP)
	$(CC) $(CFLAGS) -fopenmp -c edo_system.cpp -lgomp -o edo_system.o
else
	$(CC) $(CFLAGS) -c edo_system.cpp -o edo_system.o
endif

script_interpreter.o: script_interpreter.cpp script_interpreter.h
ifeq	($(OPTION),OMP)
	$(CC) $(CFLAGS) -fopenmp -c script_interpreter.cpp -lgomp -o script_interpreter.o
else
	$(CC) $(CFLAGS) -c script_interpreter.cpp -o script_interpreter.o
endif

main.o: main.cpp script_interpreter.h edo_system.h
ifeq	($(OPTION),OMP)
	$(CC) $(CFLAGS) -fopenmp -c main.cpp -lgomp -o main.o
else
	$(CC) $(CFLAGS) -c main.cpp -o main.o
endif
	
##COMPILATION POUR LE LANGAGE DE SCRIPT	
#scripting.o: scripting.cpp scripting.h data_treatment.h sig_simu.h
#ifeq	($(OPTION),OMP)
#	$(CC) $(CFLAGS) -fopenmp -c scripting.cpp -lgomp -o scripting.o
#else
#	$(CC) $(CFLAGS) -c scripting.cpp -o scripting.o
#endif

#main_script.o: 	scripting.h sig_simu.h measurement.h
#ifeq	($(OPTION),OMP)
#	$(CC) $(CFLAGS) -fopenmp -c main_script.cpp -lgomp -o main_script.o
#else
#	$(CC) $(CFLAGS) -c main_script.cpp -o main_script.o
#endif



	
	
	
clean: 
	-rm -f *.o
	-rm -f *.a
