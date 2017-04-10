
/*
Note: 
On a besoin du temps de maniere explicite dans les equations.
On va donc le gerer comme une variable standard.
Les fonction is_var vont etre modifie pour reconnaitre la variable t.


*/
#define POSTFIX_TYPE_NULL			0
#define POSTFIX_TYPE_NUMBER			1
#define POSTFIX_TYPE_PARAM			2
#define POSTFIX_TYPE_VAR			3 
#define POSTFIX_TYPE_OPERAT			4
#define POSTFIX_TYPE_FUNCTION		5
#define POSTFIX_TYPE_USER_FUNC		6
#define POSTFIX_TYPE_USER_FUNC_ARG	7



#define FUNC_SIN	0
#define FUNC_COS	1
#define FUNC_EXP	2
#define FUNC_LOG	3
#define FUNC_TAN	4
#define FUNC_TANH	5

/* postfix_type

Classe decrivant les differents type d'element postfix que l'on peu utiliser.
*/
class postfix_type
{
	public:
	char type;
	
	double num;	
	double *ptr;
	int var_ind; // 30 03 on utilise cet index pou trouver les fonction utilisateurs aussi
	char operat;
	char function;
	//char precedance;
	postfix_type()
	{
	type=POSTFIX_TYPE_NULL;
	num=0;
	ptr=NULL;
	var_ind=-1;
	operat=0;
	function=0;	
//	precedance=0;
	}
	~postfix_type(){}
	
		
};

/*Classe pour construire des piles d objet postfix.
 Cet objets un peu foireux seront utilises pour convertir les equations de l infix vers le postfix.
*/
class postfix_pile
{
	public:
		
	postfix_type pile[512];
	int top;
	
	
	int pop(postfix_type *dst)
	{
		if(top==0)return 0;
			top--;
			dst->function=pile[top].function;
			dst->num=pile[top].num;
			dst->operat=pile[top].operat;
			dst->ptr=pile[top].ptr;
			dst->type=pile[top].type;
			dst->var_ind=pile[top].var_ind;
			//dst->precedance=pile[top].precedance;
			
			
		return 1;
	}
	
	int w_nopop(postfix_type *dst)
	{
		if(pop(dst) ==0) return 0;
		top++;
		return 1;
	}
	
	int push(postfix_type *dst)
	{
		if(top>=512)return 0;
		
			pile[top].function=dst->function;
			pile[top].num=dst->num;
			pile[top].operat=dst->operat;
			pile[top].ptr=dst->ptr;
			pile[top].type=dst->type;
			pile[top].var_ind=dst->var_ind;
			//pile[top].precedance=dst->precedance;	
		top++;
		return 1;
	}
	
	int push(double dat)
	{
		pile[top].type=POSTFIX_TYPE_NUMBER;
		pile[top].num=dat;
	}
	
	postfix_pile()
	{
		top=0;
	}
	
	int print_from_bottom()
	{
		printf("\n*AFFICHAGE DE LA PILE POSTFIX DEPUIS LE BAS: \n");
		for(int i=0;i<top;i++)
		{
			switch (pile[i].type)
			{
				case POSTFIX_TYPE_NUMBER:
					printf("POSTFIX_TYPE_NUMBER   %e", pile[i].num);
					break;
				case POSTFIX_TYPE_PARAM:
					printf("POSTFIX_TYPE_PARAM   %p",pile[i].ptr);
					break;
				case POSTFIX_TYPE_VAR:
					printf("POSTFIX_TYPE_VAR   %d",pile[i].var_ind);
					break;
				case POSTFIX_TYPE_OPERAT:
					printf("POSTFIX_TYPE_OPERAT   %c",pile[i].operat);
					break;
				case POSTFIX_TYPE_FUNCTION:
					printf("POSTFIX_TYPE_FUNCTION   %c",pile[i].function);
					break;
				case POSTFIX_TYPE_USER_FUNC:
					printf("POSTFIX_TYPE_USER_FUNCTION   %d",pile[i].var_ind);
					break;
				default:
					printf("OTHER %d",pile[i].type);
					break;
					
			}
		
		printf("\n");
		}
		
		
	}
	
	int print_from_top()
	{
		printf("\n*AFFICHAGE DE LA PILE POSTFIX DEPUIS LE HAUT: \n");
		 for(int i=top-1;i>=0;i--)
		 {

			switch (pile[i].type)
			{
				case POSTFIX_TYPE_NUMBER:
					printf("POSTFIX_TYPE_NUMBER   %e", pile[i].num);
					break;
				case POSTFIX_TYPE_PARAM:
					printf("POSTFIX_TYPE_PARAM   %p",pile[i].ptr);
					break;
				case POSTFIX_TYPE_VAR:
					printf("POSTFIX_TYPE_VAR   %d",pile[i].var_ind);
					break;
				case POSTFIX_TYPE_OPERAT:
					printf("POSTFIX_TYPE_OPERAT   %c",pile[i].operat);
					break;
				case POSTFIX_TYPE_FUNCTION:
					printf("POSTFIX_TYPE_FUNCTION   %c",pile[i].function);
					break;
			}
		
			printf("\n");
		

		 }
	}
};

class edo_system
{
	public:
		
	static	script_interpreter *scptint;	
	
	// conversion des equations en postfix
	static postfix_type **postfix;
	static int *postfix_nb;
	
	// Conversion des fonction en postfix AJOUT DU 26 03 
	static postfix_type **function;
	static int *functionpostfix_nb;
	
	//parametre de résolution
	static double param[256];
	
	
	
	/********************CONVERSION EN POST FIX ****************
	************************************************************/
	
/*	static int setup_postfix(script_interpreter *sint);

routine pour initialiser les donnees de l edo-system
*/
	static int setup_postfix(script_interpreter *sint);
	
	
	static int	destroy_postfix();

/*static int convert_equations();

Fonction pour convertir automatiquement les equations et les fonctions 
utilisateurs.
*/
	static int convert_equations();
	static int convert_function();
	
	/*int eq_conversion(script_interpreter *sint, int mode);

	routine pour convertir les equations en postfix interpretable par la suite.
	Le mode peut prendre la valeur 0 -> pointeur sur les variables sont via des index
								   1 -> pointeur sur tout. (A IMPLEMENTER)
	*/
	static postfix_type * eq_conversion( int mode,int eq_nb,int *pile_size);
	
	//AJOUT DU 30/03
	static postfix_type * func_conversion( int mode,int func_nb,int *pile_size);
	
	/* Routine de detection des donnees de l equation.*/ 
	static int is_number(char *prt);
	static int is_function(char *ptr);
	static int is_usrfunction(char *ptr);
	static int is_var(char *ptr);
	static int is_param(char *ptr);
	static int is_operator(char *ptr);

	//ajout du 30 03 2017 pour determiner si une donnee est un argument ou pas
	static int is_usrfuncarg(char *ptr,int f_nb);
	
	
	/* int operator_precedance(char *ptr);
	
	Routine fournissant le score d'importance de chaque operateur.
	
	*/
	static int operator_precedance(char *ptr);
	
	static int operator_associat(char *ptr);
	
	/* static char * extract_next_eqpar(char *ptr, char *dest);
	
	routine pour extraire le prochain param/operateur d une aquation.
	La donnée extraite est stockée dans dest tandis que la fonction retourne un pointeur
	sur la suite de l equation. 
	*/
	static char * extract_next_eqpar(char *ptr, char *dest);
	
//	static int conv_eq(char *ptr, int mode);
	
	/******************************************************
	*****EVALUATION DES EQUATION EN POSTFIX **************/
	
	
	/*static double evaluate_eq(int eq_nb,double *var);
	Routine permettant d'evaluer l'équation eq_nb deja convertie en postfix.
	Comme la conversion est deja faite, l interpreteur est tres rapide.
	ON DEVRA CEPENDANT L OPTIMISER AVEC LE PROFILAGE
	On fournit les variable juste calculees 
	*/
	static double evaluate_eq(int eq_nb,double *var, double *cn,double *t);
	

	static double evaluate_usr_fcn(int fct_nb,double *var, double *cn,double *t,double *fctarg,int argnb);

	
};
