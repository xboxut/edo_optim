





class optimiser
{
	public:
	
	script_interperter *scptint;
	
	int n_proc;//nombre de processus
	
	unsigned long int *seed;//graines pour le gene de nombre alea
	
	gsl_rng * rnd_numb_gen;// gene de nombre aleatoire
	
	
	FILE *optim_save; //fichier pour sauvegarder les resultats (resumes) de l'optimisation;
	FILE *numsol_save; //fichier pour sauvegarder des solutions numeriques des equa diff (pour verification)
	FILE *continue_compute;// fichier binaire pour redemarrer un calcul en cours a voir comment on l utilise!
	
	
	
	double **param_array;// tableau de parametre qui peut servir a l'optimisation de maniere generale.
	double *score;// tableau pour stocker le score, peut être utile pour l'algo d'optim.
	
	 
	
	/*********OPTIMISATION MONTE CARLO ***********/
	
	int init_mc_optimiser(int rank);
	int run_mc_optimiser(int rank);
	int mc_optimiser();
	
	
	/*********************************************/
	optimiser();
	~optimiser();
	
	
};
