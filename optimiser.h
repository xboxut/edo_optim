





class optimiser
{
	public:
	
	script_interpreter *scptint;
	
	unsigned long int *seed;//graines pour le gene de nombre alea
	
	gsl_rng * rnd_numb_gen;// gene de nombre aleatoire
	
	/*
	 * Graine pour le générateur de nombre aleatoire.
	 * On ne dispose pas du nombre de proc avant MPI_INIT,
	 * On initie donc 10000 graines à l'aide du temps 
	 * 
	 */
	unsigned long int *rng_seed;
	
	FILE *optim_save; //fichier pour sauvegarder les resultats (resumes) de l'optimisation;
	FILE *numsol_save; //fichier pour sauvegarder des solutions numeriques des equa diff (pour verification)
	FILE *continue_compute;// fichier binaire pour redemarrer un calcul en cours a voir comment on l utilise!
	

	
	
	double *param_array;// tableau de parametre qui peut servir a l'optimisation de maniere generale.
	double param_array_size[2];
	 
	
	int comput_per_proc; //compteur a partir duquel on trasnfere des donnees peut etre utilise par certains algorithmes
	
	
	/*********OPTIMISATION MONTE CARLO ***********/
	int init_mc_optimiser(int rank,int numtask);
	int init_mc_optimiser_debug(int rank,int numtask);
	int run_mc_optimiser(int rank);
	int run_mc_optimiser_debug(int rank);
	int mc_optimiser(int argc, char *argv[],int comput_per_proca, int debug);
	/*********************************************/
	optimiser();// constructeur par defaut -> ne fait rien a part mettre tout les pointeurs a null
	
	/*int init_optimiser(script_interpreter *scpint); // Routine pour initialiser l'optimiser 
	Routine pour initialiser l optimiser en fonction des donnees de l interpreteur de script.
	
	A FAIRE !!!
	
	*/
	int init_optimiser(script_interpreter *scpint); 
	
	
	~optimiser();//destructeur -> supprime la mémoire allouée.
	
	
};
