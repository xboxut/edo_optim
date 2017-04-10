/********************************************************
******* SCRIPT_INTERPRETEUR *****************************
********************************************************/

/* cette classe permet de charger les données importantes d'un fichier
de type .edo pour résoudre par la suite des equation differentielles.

Je me tate toujours pour placer une convertisseur infix postfiw ici.
Cela sera a décider par la suite 
*/



class script_interpreter
{
	public:
	
	char * script;// var pour contenir le script entier.
	
	//------------------DONNEE POUR L OPTIMISATION
	bool optimisation; // booleen pour specifier si on optimise ou si on résoud pour tester
	char optim_algo[32]; // nom de l'algo d'optim a utiliser -> a integrer par le suite.
	unsigned int optim_max_run; //nombre d'iteration pour l'optimisation -> utile pour methode MC ou GA ect
	double optim_epsilon; // ecart simu exp max pour l'optim
	
	//-----------------DONNEE POUR LA SAUVEGARDE
	char optim_filename[512];
	char optim_solu_filename[512];
	
	char var_name[256][32]; // nom des variable
	int var_nb; // nombre de var deja charge
	
	char param_name[256][32];// nom des parametres
	double param[256]; // valeur des param
	int param_nb;
	
	double param_optim[256][2]; // domaine d'optim des param
	
//	char coef[256][512]; //coefficient des equations différentielles
//	int coef_nb;
	
	char equation[256][2048];// equations différentielles
	int eq_nb;
	
	// ajout pour traiter les fonctions de l'utilisateur
	char function[32][2048];
	char function_name[32][32];
	char function_arg[32][10][32];
	int arg_nb[32];
	int fct_nb;
	
	
	double init_cond[256];
	int initcond_nb;
	
	double *exp_data[256];
	double *exp_x[256];
	int exp_size[256];
	
	
	script_interpreter();
	~script_interpreter();
	
	
	
	/*int load_script(const char *filename);
	Routine de chargement du script en memoire. A appeler avant
	toute operation.
	La routine retourne :
	1 en cas de succès
	0 en cas d'échec (fichier non existant ou autre)
	*/
	int load_script(const char *filename);
	
	
	/*	int interpret();
	
	Fonction permettant d'interpreter le script precedement charge
	*/
	int interpret();
	
/***************************************************
*** Routine de traitement des donnees script*****
***************************************************/

//Ces routines traitent les mots clés du script et charge les équations sous format
// ascii.
// les equations seront processés par la suite pour le rendre calculable
	
	char *process_var(char *cptr); //ok
	char *process_param(char *cptr); //ok
//	char *process_coef(char *ptr);
	char *process_init_cond(char *cptr); //ok
	char *process_optim_bound(char *cptr); //ok
	char *process_equation(char *cptr); //ok
	char *process_function(char *cptr); //ok
	char *process_experimental_data(char *ptr); //ok
	char *process_max_optim_run(char *ptr);
	char *process_optim_max_epsilon(char *ptr);
	char *process_optim_algo(char *ptr);
	
	
/****************************************************
****** Routines de gestion des equations ***********
****************************************************/

	/*	int process_unary_eq();

		fonction qui permet de corriger les equations avec un moin unaire.
		On ajoute un 0- dans les equation pour les processer
	*/
	int process_unaryop_eq();
	
////////////////////////////////////////////////	
	
	/*char *script_interpreter::jump_comment(char *cptr)

	Routine permettant de sauter les commentaires lorsqu'un % est trouv�.
	La routine prend en param�tre le pointeur en position % et
	retourne la position d'apres %.
	En cas d'erreur, un pointeur NULL est retourn�.
	*/
	char *jump_comment(char *ptr);
	
	/*int var_index(char *ptr);
	Routine pour verifier si la variable a ete declaree.
	Retourne l'index de la variable si celle ci existe, sinon retourne -1;
	
	AJOUT POUR LA GESTION DE LA VAR IMPLICITE T: la routine retourne l indice 10000, ce qui ne doit arriver que lors des conversion postfix des eq.
	*/ 	
	int var_index(char *ptr);
	
	
	/*	int param_index(char ptr);	
	Routine pour verifier si le parametre a ete declaree.
	Retourne l'index du param si celui ci existe, sinon retourne -1;
	*/
	int param_index(char *ptr);
	
	/*
	int func_index(char *ptr);
	Routine pour verifier sir la fonction a ete declare
	retourne l index de la fonction si celui ci existe, sinon -1s
	*/
	int func_index(char *ptr);
	
	/*
	int func_arg_index(char *ptr,func_nb);
	Routine pour verifier si l'aarg  de lafonction a ete declare
	retourne l index de la fonction si celui ci existe, sinon -1s
	*/
	int func_arg_index(char *ptr,int func_nb);
	
	
	
//////////////////////////////////////////////////
////////////test des donnee///////////////////////

	/*bool check_eq_syntax(char *ptr);
	
	routine pour valider la syntaxe des equations
	*/
	bool check_eq_syntax(char *ptr);
	
	bool check_equation_number();
	bool check_initcond_number();
	

	
	
};

