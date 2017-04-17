
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// INCLUSION DE  LIBRAIRIES NON STANDARD
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <mpi.h>

#include "script_interpreter.h"
#include "optimiser.h"
#include "edo_system.h"
 
 optimiser::optimiser()
 {
	 
	 scptint=NULL;
	

	seed=NULL;//graines pour le gene de nombre alea
	
	rnd_numb_gen=NULL;// gene de nombre aleatoire
	optim_save=NULL; 
	numsol_save=NULL; 
	continue_compute=NULL;
	param_array_size[0]=0;
	param_array_size[1]=0;
	
	param_array=NULL;
	 
 }

int optimiser::init_optimiser(script_interpreter *scpinta)
{
	 // Routine pour initialiser l'optimiser

	scptint=scpinta;
}

optimiser::~optimiser()
{
	scptint=NULL;// le destructeur ne tue pas l'interpreteur de script.
	
	if(seed!=NULL)delete[] seed;
	
	if(rnd_numb_gen!=NULL) gsl_rng_free(rnd_numb_gen);
	
	if(optim_save!=NULL)fclose(optim_save);
	if(numsol_save!=NULL)fclose(numsol_save);
	if(continue_compute!=NULL)fclose(continue_compute);
	
	
	if(param_array!=NULL)
	{
		
		delete [] param_array;
	}

	
}


/* int init_mc_optimiser(int rank)
 * 
 * routine pour initialiser la routine d'optimisation par MC 

*/
int optimiser::init_mc_optimiser(int rank,int numtask)
{

	// initialisation du generateur de nombre aleatoire
	rnd_numb_gen=gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(rnd_numb_gen, seed[rank]);
	
	if(rank==0)// si le processus principal execute la fction
	{
		param_array=new double[numtask*(scptint->param_nb+1)];// creation de l'array dans lequel stocker les resultats de chaque processus.
		
			
	}
	else
	{
		
	}

return 0;	

}

int optimiser::init_mc_optimiser_debug(int rank,int numtask)
{
	// initialisation du generateur de nombre aleatoire
	rnd_numb_gen=gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(rnd_numb_gen, seed[rank]);
	
	if(rank==0)// si le processus principal execute la fction
	{
		param_array=new double[numtask*(2+1)];// creation de l'array dans lequel stocker les resultats de chaque processus.	
			
	}
	else
	{
		
	}
	
	return 0;
}

int optimiser::run_mc_optimiser(int rank)
{
	return 0;
}

int optimiser::run_mc_optimiser_debug(int rank)
{
	double param_save[3];
	double param[3];
	int loopcount=0;
	int i=0;

	while(1)
	{	
			
			for(i=0;i<comput_per_proc;i++)
			{
			param[0]=gsl_rng_uniform (rnd_numb_gen);	
			param[1]=gsl_rng_uniform (rnd_numb_gen);	
			param[2]=edo_system::rosenbrock2(param[0],param[1]);
			}

		// Recuperation de toute les donnee vers le master -> a tester		
		  MPI_Gather(param_save, 3, MPI_DOUBLE, param_array, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);		  
		
		loopcount++;
		
		if(loopcount>=scptint->optim_max_run)
			break;
		
		
	}	
	

	return 0;
}


int optimiser::mc_optimiser(int argc, char *argv[],int comput_per_proca=1000, int debug=0)
{
	
	comput_per_proc=comput_per_proca;
	int taskid;
	int numtasks;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	
	//initialisation de l'optimiser monte carlo
	if(debug)init_mc_optimiser_debug(taskid,numtasks);
	else init_mc_optimiser(taskid,numtasks);
	
	if(debug)run_mc_optimiser_debug(taskid);
	else run_mc_optimiser(taskid);
	
	
	MPI_Finalize();	
	
	return 0;
}
