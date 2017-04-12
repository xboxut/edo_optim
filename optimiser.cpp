
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// INCLUSION DE  LIBRAIRIES NON STANDARD
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <mpi.h>

#include "optimiser.h"

 
 optimiser::optimiser()
 {
	 
	 scptint=NULL;
	
	n_proc=0;//nombre de processus
	
	seed=NULL;//graines pour le gene de nombre alea
	
	rnd_numb_gen=NULL;// gene de nombre aleatoire
	optim_save=NULL; 
	numsol_save=NULL; 
	continue_compute=NULL;
	param_array_size[0]=0;
	param_array_size[1]=0;
	
	param_array=NULL;
	score=NULL;
	 
 }

optimiser::~optimiser()
{
	scptint=NULL;// le destructeur ne tue pas l'interpreteur de script.
	
	if(seed!=NULL)delete[] seed;
	
	if(rnd_numb_gen!=NULL) gsl_rng_free(rnd_num_gen);
	
	if(optim_save!=NULL)fclose(optim_save);
	if(numsol_save!=NULL)fclose(numsolve_save);
	if(continue_compute!=NULL)fclose(continue_compute);
	
	
	if(param_array!=NULL)
	{
		for(int i=0;i<param_array_size[0];i++)
		{
			delete [] param_array[i];
		}
		
		delete [] param_array;
	}
	
	if(score!=NULL)delete[] score;
	
}


/* int init_mc_optimiser(int rank)
 * 
 * routine pour initialiser la routine d'optimisation par MC 

*/
int optimiser::init_mc_optimiser(int rank)
{

	// initialisation du generateur de nombre aleatoire
	rnd_numb_gen=gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(rnd_numb_gen, seed[rank]);
	
	if(rank==0)// si le processus principal execute la fction
	{
		
		
	}
	
}


int optimiser::run_mc_optimiser()
{
	
}

int optimiser::run_mc_optimiser_debug()
{
	
}


int optimiser::mc_optimiser(int argc, char *argv[],int sync_counta=1000, int debug=0)
{
	
	sync_count=sync_counta;
	int taskik;
	int numtask;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	
	//initialisation de l'optimiser monte carlo
	init_mc_optimiser(taskid);
	
	if(debug)run_mc_optimiser(taskid);
	else run_mc_optimiser_debug(taskid);
	
	
	
}
