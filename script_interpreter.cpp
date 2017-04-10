

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "script_interpreter.h"


script_interpreter::script_interpreter()
{
	script=NULL;
	optimisation=false;
	optim_max_run=0;
	optim_epsilon=10000.0;
	optim_algo[0]='\0';
	var_nb=0;
	param_nb=0;
	initcond_nb=0;
	eq_nb=0;
	
	for(int i=0;i<256;i++)
	{
	exp_data[i]=NULL;
	exp_x[i]=NULL;
	exp_size[i]=0;
	}
	
	for(int i=0;i<32;i++)
	{
		arg_nb[i]=0;
	}
	fct_nb=0;
}

script_interpreter::~script_interpreter()
{
	if(script!=NULL)
		delete [] script;
}

int script_interpreter::load_script(const char *filename)
{
	int pos=0;	
	FILE* fich=fopen(filename,"rb");
	
	if(fich==NULL)
	{
		printf("ERREUR: impossible d'ouvrir le fichier %s\n",filename);
		return 0;
	}
	
	fseek(fich,0,SEEK_END);
	pos=ftell(fich);
	fseek(fich,0,SEEK_SET);
	
	script=new char[pos+1];
	
	fread(script,1,pos,fich); //lecture du script;
	
	script[pos]='\0';
	
}


	/*	int interpret();
	
	Fonction permettant d'interpreter le script precedement charge
	*/
int script_interpreter::interpret()
{
	
	char *ptr=script;
	
	while(*ptr!='\0')
	{
		
		if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
		}
		else if(strncmp("var",ptr,3)==0)
		{
			ptr=process_var(ptr);
			
		}
		else if(strncmp("param",ptr,5)==0)
		{
			ptr=process_param(ptr);
		}
		else if(strncmp("equa",ptr,4)==0)
		{
			ptr=process_equation(ptr);
		}
		else if(strncmp("func",ptr,4)==0)
		{
			ptr=process_function(ptr);
		}
		else if(strncmp("init_cond",ptr,9)==0)
		{
			printf("GGGGGGGGGGGGGGGGGGGGGGG");
			ptr=process_init_cond(ptr);
		}
		else if(strncmp("optim_dom",ptr,9)==0)
		{
	 		ptr=process_optim_bound(ptr);	
	
		}
		else if(strncmp("optim_max_run",ptr,13)==0)
		{
		 ptr=process_max_optim_run(ptr);	
		}
		else if(strncmp("optim_max_epsilon",ptr,17)==0)
		{
			
		}
		else if(strncmp("optim_use_jac",ptr,13)==0)
		{
			
		}
		else if(strncmp("optim_algo",ptr,10)==0)
		{
			
		}
		else if(strncmp("optim_save_data",ptr,15)==0)
		{
			
		}		
		else if(strncmp("optim_save_numsol",ptr,17)==0)
		{
			
		}		
		else if(strncmp("exec_optim",ptr,10)==0)
		{
			
		}
		else if(strncmp("exec_run",ptr,8)==0)
		{
			
		}
		else if(strncmp("experim_data",ptr,12)==0)
		{
			ptr=process_experimental_data(ptr);
		}
		else 
			ptr++;
			
		if(ptr==NULL)return 0;
		
	}
	process_unaryop_eq();
	
}

char *script_interpreter::process_var(char *cptr)
{
	char *ptr=cptr+strlen("var");// on se positionne apres le var
	char *cp_ptr=NULL;
	while(1)
	{	
		if(*ptr==' ')
			ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}
		else 
		 break;
	    		
	}
	
	//demarrage de la copie du nom de variable
	cp_ptr=var_name[var_nb];
	
	while(*ptr!=' ' && *ptr!=';' && *ptr!='%')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de variable est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	//#ifdef DEBUG
	printf("OK:var %s definie %d\n",var_name[var_nb],var_nb);
	//#endif
	var_nb++;
	return ptr;
}


char *script_interpreter::process_param(char *cptr)
{
	char buff[32];
	char *ptr=cptr+strlen("param");// on se positionne apres le var
	char *cp_ptr=NULL;
	while(1)
	{	
		if(*ptr==' ')
			ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}
		else 
		 break;
	    		
	}
	
	
	//demarrage de la copie du nom du parametre
	cp_ptr=param_name[param_nb];
	
	while(*ptr!=' ' && *ptr!='=' && *ptr!='%')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de parametre est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	//positionnement du pointeur de recopie apres le egal
	
	while(*ptr!='-' && *ptr!='+' && *ptr!='0' && *ptr!='1' && *ptr!='2' && *ptr!='3' && *ptr!='4' && *ptr!='5' && *ptr!='6' && *ptr!='7' && *ptr!='8' && *ptr!='9')
	{
	
		 if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}

		ptr++;
	}
	
	
	// recopie de la valeu du parametre
	cp_ptr=buff;
	while(*ptr!=' ' && *ptr!=';' && *ptr!='%')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de variable est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';

	param[param_nb]=atof(buff); // conversion des paramètres
	
	printf("OK:ajout du parametre %s d un valeur de %f\n", param_name[param_nb],param[param_nb]);
	param_nb++;	
	
	return ptr;
}

char *script_interpreter::process_init_cond(char *cptr)
{
	char buff[32];
	int index=0;
	char *ptr=cptr+strlen("init_cond");// on se positionne apres le var
	char *cp_ptr=NULL;
	printf("INNNNNNNIIIT");
	while(1)
	{	
		if(*ptr==' ')
			ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}
		else 
		 break;
	    		
	}
	
	//demarrage de la copie du nom du parametre
	cp_ptr=buff;
	
	while(*ptr!=' ' && *ptr!='=' && *ptr!='%')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de parametre est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	
	printf("OK: nom de la condition initiale chargee: %s\n",buff);
	
	//VERIFICATION DU NOM DE LA VARIABLE
	index=var_index(buff);
	if(index==-1)
	{
		printf("ERREUR: la condition initiale est definie pour une variable non connue\n");
		return NULL;
	}
	
	//positionnement du pointeur de recopie apres le egal	
	while(*ptr!='-' && *ptr!='+' && *ptr!='0' && *ptr!='1' && *ptr!='2' && *ptr!='3' && *ptr!='4' && *ptr!='5' && *ptr!='6' && *ptr!='7' && *ptr!='8' && *ptr!='9')
	{
	
		 if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}

		ptr++;
	}
	
	
	// recopie de la valeur du parametre
	cp_ptr=buff;
	while(*ptr!=' ' && *ptr!=';' && *ptr!='%')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de variable est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';

	
	init_cond[index]=atof(buff);
	
	return ptr;
}



char *script_interpreter::process_optim_bound(char *cptr)
{
	char *ptr=cptr+strlen("optim_dom");
	char *cp_ptr;
	int index=0;
	char buff[32];
	
	
	while(1)
	{	
		if(*ptr==' ')
			ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}
		else 
		 break;
	    		
	}
	
	//demarrage de la copie du nom du parametre
	cp_ptr=buff;
	
	while(*ptr!=' ' && *ptr!='=' && *ptr!='%')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de domaine d'optimisation est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	
	printf("OK: nom du domaine d'optimisation charge: %s\n",buff);
	
	
	//VERIFICATION DU NOM DU PARAMETRE
	index=param_index(buff);
	if(index==-1)
	{
		printf("ERREUR: le domaine d'optimisation est defini pour un parametre non connu\n");
		return NULL;
	}
	
	
//positionnement du pointeur de recopie apres le egal	
	while(*ptr!='-' && *ptr!='+' && *ptr!='0' && *ptr!='1' && *ptr!='2' && *ptr!='3' && *ptr!='4' && *ptr!='5' && *ptr!='6' && *ptr!='7' && *ptr!='8' && *ptr!='9')
	{
	
		 if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}

		ptr++;
	}
	
	
	// recopie de la valeur du parametre
	cp_ptr=buff;
	while(*ptr!=' ' && *ptr!=':')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de variable est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
		
	param_optim[index][0]=atof(buff);
	
	ptr++;
	cp_ptr=buff;
	while(*ptr!=' ' && *ptr!=';')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de variable est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	param_optim[index][1]=atof(buff);
	
	printf("   domaine %e : %e\n",	param_optim[index][0],	param_optim[index][1]);
	
	return ptr;
}


char *script_interpreter::process_equation(char *cptr)
{
	char *ptr=cptr+strlen("equa");
	char *cp_ptr;
	int index=0;
	char buff[32];
	
	
	
	while(1)
	{	
		if(*ptr==' ')
			ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}
		else 
		 break;
	    		
	}
	
	//demarrage de la copie du nom du parametre
	cp_ptr=buff;
	
	while(*ptr!=' ' && *ptr!='=' && *ptr!='%')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration d equation est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	
	printf("OK: equation charge: %s\n",buff);
	
	//VERIFICATION DU NOM DE LA VARIABLE
	index=var_index(&buff[1]);
	if(index==-1)
	{
		printf("ERREUR: l equation est definie pour une variable non connue\n");
		return NULL;
	}
	
	
	//decalage jusqu'a l equation:
	while(1)
	{	
		if(*ptr==' ')
			ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}
		else 
		 break;
	    		
	}
	
	
	//recopie de l'equation
	cp_ptr=equation[index];
	while(*ptr!=';')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration d equation est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	eq_nb++;
	printf(" %s \n",equation[index]);
	
	return ptr;	
}


char *script_interpreter::process_function(char *cptr)
{
	char *ptr=cptr+strlen("func");
	char *cp_ptr;
	int index=0;
	char buff[32];
	
	
	
	while(1)
	{	
		if(*ptr==' ')
			ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
			if(ptr==NULL)return NULL;//remontee de l erreur
		}
		else 
		 break;
	    		
	}
	
	//demarrage de la copie du nom de la fonction
	cp_ptr=function_name[fct_nb];
	
	while(*ptr!=' ' && *ptr!='(' && *ptr!='%')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de fonction est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	
	printf("OK: fonction chargee: %s\n",function_name[fct_nb]);
	/*
		char function[128][2048];
	char function_name[128][32];
	char function_arg[128][128];
	int fct_nb;
	*/
	// RECUPERATION DES DIFFERENTS ARGUMENTS
	ptr++; //deplacement du pointeur a droite
	
	while(1)
	{
		cp_ptr=function_arg[fct_nb][arg_nb[fct_nb]];
		 while(*ptr!=',' && *ptr!=')') 
		 {
		 	*cp_ptr=*ptr;
		 	ptr++;
		 	cp_ptr++;
		 	
		 }	
		 *cp_ptr='\0';
		 arg_nb[fct_nb]++;
		 if(*ptr==')')break;
		 ptr++;
	}
	
	while(*ptr!='=')
	{
		ptr++;
	}
	
	
/*	index=var_index(&buff[1]);
	if(index==-1)
	{
		printf("ERREUR: l equation est definie pour une variable non connue\n");
		return NULL;
	}
*/	
	ptr++;
			
	//recopie du corps de la fonction
	cp_ptr=function[fct_nb];
	while(*ptr!=';')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration d equation est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	eq_nb++;
	printf(" %s \n",function[fct_nb]);
	fct_nb++;
	return ptr;	
}


char *script_interpreter::process_experimental_data(char *cptr)
{
	char buff[32];
	char *ptr=cptr+strlen("experim_data");
	char *cp_ptr;
	bool copy=false;
	int nb_cnt=0;
	double *tab=new double[10000];
	int var_ind=0;
	
	while(1)//decalage de pointeur vers le nom  des var exp
	{
		if(*ptr==' ')
		ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
		}
		else
		break;
	}
	
	cp_ptr=buff;
	
	while(*ptr!=' ')
	{	*cp_ptr=*ptr;	
		ptr++;
		cp_ptr++;
	}
	
	*cp_ptr='\0';
	
	var_ind=var_index(buff);
	
	if(var_ind==-1)
	{
		printf("ERREUR: la donnee experimentale ne correspond a aucune variable: %s \n",buff);
		delete [] tab;
		return NULL;
	}
	// recuperation des donnes
	 do// on boucle tant que toute les donnees n ont pas ete traite
     {
          //Si le charactere pointé par le pointeur est un nombre 
          // ou une "puissance"
          if((*ptr>='0' && *ptr<='9')|| *ptr=='.' || *ptr=='-'
                || *ptr=='e' || *ptr=='E')
          {
             copy=true; // on signale que la copie de nombre est en marche
             *cp_ptr=*ptr; //on copie chaque chiffre dans le buffer
             cp_ptr++;//on incremente le pointeur du buff se déplacer dans la chaine  
                
          }
          else if(*ptr=='%')
          {
          	ptr=jump_comment(ptr);
		  }
          else //sinon
          {
             if(copy==true)//si l'on a plus rien a copier mais que l'on vient de le faire  
             {
                  copy=false;// on arrete la copie
                  *cp_ptr='\0';//on cloture la chaine contenue dans buffer
                  tab[nb_cnt]=atof(buff);// et on la transforme en double
                  //printf(buffer);
                  //printf("\n%d\n",nb_cnt);
                  nb_cnt++;//on incrémente le nombre de nombre déjà convertis
                  cp_ptr=buff;// on replace le pointeur au début du buffer pour réécrire dessus    
             } 
          }                           
                               
          ptr++; 
          //printf("%c\n",*ptr_fich);                
     }while(*(ptr-1)!=';');
      
	if(nb_cnt%2==1)
	{
		printf("ERREUR: une donnee est mal exprime.\n");
		delete [] tab;
		return NULL;
	}
	
		
		
	exp_size[var_ind]=nb_cnt/2;
	exp_data[var_ind]=new double[exp_size[var_ind]];;
	exp_x[var_ind]=new double[exp_size[var_ind]];
	
	for(int i=0;i<exp_size[var_ind];i+=2)
	{
		exp_x[var_ind][i]=tab[i];
		exp_data[var_ind][i]=tab[i+1];
	}	
	
	printf("OK: donne experimentale charge %d, nombre de donnee %d\n",var_ind,exp_size[var_ind]);
		delete [] tab;
	
	return ptr;
}


char *script_interpreter::process_max_optim_run(char *cptr)
{
	char *ptr=cptr+strlen("optim_max_run");
	char buff[32];
	char *cp_ptr;
	
	while(1)//decalage de pointeur vers le nombre de run max
	{
		if(*ptr==' ')
		ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
		}
		else
		break;
	}
	
	
    	// recopie de la valeur du parametre
	cp_ptr=buff;
	while(*ptr!=' ' && *ptr!=';')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de variable est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	optim_max_run=atoi(buff);
	
	printf("OK: chargement du nombre de run maximum %d\n",optim_max_run);
	
	return ptr;
}

char *script_interpreter::process_optim_max_epsilon(char *cptr)
{
	char *ptr=cptr+strlen("optim_max_epsilon");
	char buff[32];
	char *cp_ptr;
	
	while(1)//decalage de pointeur vers le nombre de run max
	{
		if(*ptr==' ')
		ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
		}
		else
		break;
	}
	
	
    	// recopie de la valeur du parametre
	cp_ptr=buff;
	while(*ptr!=' ' && *ptr!=';')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de variable est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';
	
	optim_epsilon=atof(buff);
	
	printf("OK: chargement du epsilon %e\n",optim_epsilon);
	
	return ptr;
}

char *script_interpreter::process_optim_algo(char *cptr)
{

	char *ptr=cptr+strlen("optim_algo");
	char buff[32];	
	char *cp_ptr;
	
	while(1)//decalage de pointeur vers le nom  des var exp
	{
		if(*ptr==' ')
		ptr++;
		else if(*ptr=='%')
		{
			ptr=jump_comment(ptr);
		}
		else
		break;
	}

	    	// recopie de la valeur du parametre
	cp_ptr=buff;
	while(*ptr!=' ' && *ptr!=';')
	{
		*cp_ptr=*ptr;
		cp_ptr++;
		ptr++;
		if(*ptr=='\0')
		{
			printf("ERREUR: une declaration de variable est mal faite\n");
			return NULL;
		}
	}
	*cp_ptr='\0';

    strcpy(optim_algo,buff);
	printf("OK: methode d'optimisation: %s",optim_algo);
	
	return ptr;
}


/****************************************************
****** Routines de gestion des equations ***********
****************************************************/

	/*	int process_unary_eq();

		fonction qui permet de corriger les equations avec des operateurs unaires
		On ajoute un 0- dans les equation pour les processer
	*/
int script_interpreter::process_unaryop_eq()
{
	char *ptr;
	bool unary=false;
	bool wait_op=false;
			
		for(int i=0;i<eq_nb;i++)
		{
			
			ptr=equation[i];				
			
			while(*ptr!='\0')
			{
				
				if(*ptr=='+' ||*ptr=='-' || *ptr=='^' || *ptr=='*' || *ptr=='/')
				{
					if(wait_op==false)
					{
						if(*ptr=='-') *ptr='#';
						else if(*ptr=='+') *ptr='!';
						
					}
					else
					{
						wait_op=false;
					}
					ptr++;
				}
				else if(*ptr=='(' || *ptr==')')
				ptr++;
				else
				{
					wait_op=true;
					ptr++;
				}
			}	
		printf("EQUATION %s\n",equation[i]);
		}
		
	
	return 1;
}

/////////////////////////////////////////////////////////////

/*char *script_interpreter::jump_comment(char *cptr)

Routine permettant de sauter les commentaires lorsqu'un % est trouvé.
La routine prend en paramètre le pointeur en position % et
retourne la position d'apres %.
En cas d'erreur, un pointeur NULL est retourné.
*/
char *script_interpreter::jump_comment(char *cptr)
{
	
	 char *ptr=cptr+1;
	 
	 while(*ptr!='%')
	 {
		 ptr++;
	
		if(*ptr=='\0')
		{
		printf("ERREUR: un  commentaire n'est pas ferme\n");
		return NULL;		
		}
	 }
	
	ptr++;
	return ptr;
}


/*int var_index(char *ptr);
	Routine pour verifier si la variable a ete declaree.
	Retourne l'index de la variable si celle ci existe, sinon retourne -1;
	
	AJOUT POUR LA GESTION DE LA VAR IMPLICITE T: la routine retourne l indice 10000, ce qui ne doit arriver que lors des conversion postfix des eq.
	*/ 	
int script_interpreter::var_index(char *ptr)
{
	for(int i=0;i<var_nb;i++)
	{
		if(strcmp(var_name[i],ptr)==0)
		return i;
	}
	if(strcmp("t",ptr)==0)return 10000; // AJOUT POUR LE TRAITEMENT IMPLICITE DU TEMPS. CELA NE DEVRAIT ETRE UTILISE QUE DANS LA CONVERSION DES EQUATIONS EN POSTFIX
	
	return -1;
}
	

/*	int param_index(char ptr);	
	Routine pour verifier si le parametre a ete declaree.
	Retourne l'index du param si celui ci existe, sinon retourne -1;
	*/
int script_interpreter::param_index(char  *ptr)
{
	
	for(int i=0;i<param_nb;i++)
	{
		if(strcmp(param_name[i],ptr)==0)
		return i;
	}
	
	
	return -1;
}
	
	/*
	int func_index(char *ptr);
	Routine pour verifier sir la fonction a ete declare
	retourne l index de la fonction si celui ci existe, sinon -1s
	*/
int script_interpreter::func_index(char *ptr)
{
	
	for(int i=0;i<fct_nb;i++)
	{
		if(strcmp(function_name[i],ptr)==0)
		return i;
	}
	
	
	return -1;
	
}


/*
	int func_arg_index(char *ptr,func_nb);
	Routine pour verifier si l'aarg  de lafonction a ete declare
	retourne l index de la fonction si celui ci existe, sinon -1s
*/
int script_interpreter::func_arg_index(char *ptr,int func_nb)
{
	
	for(int i=0;i<arg_nb[func_nb];i++)
	{
		
		if(strcmp(function_arg[func_nb][i],ptr)==0)
		return i;
	}
	
	return -1;
}


