#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "script_interpreter.h"
#include "edo_system.h"

script_interpreter * edo_system::scptint=NULL;
postfix_type ** edo_system::postfix=NULL;
int *edo_system::postfix_nb=NULL;
	
// Conversion des fonctions en postfix AJOUT DU 26 03 
	 postfix_type **edo_system::function=NULL;
	 int *edo_system::functionpostfix_nb=NULL;
	
		
	//parametre de résolution
double edo_system::param[256];


int edo_system::setup_postfix(script_interpreter *sint)
{
	
	//sint->eq_nb;
	printf("create postfix representation\n");

	
	scptint=sint;
	postfix= new postfix_type*[sint->eq_nb];
	postfix_nb=new int[sint->eq_nb];
	
	function= new postfix_type*[sint->fct_nb];
	functionpostfix_nb=new int[sint->fct_nb];
	
	
	for(int i=0;i<sint->eq_nb;i++)
	{
		postfix_nb[i]=-1;
		postfix[i]=NULL;
	}
	
	for(int i=0;i<sint->fct_nb;i++)
	{
		functionpostfix_nb[i]=-1;
		function[i]=NULL;
	}	
	
	return 0;
}

int edo_system::destroy_postfix()
{
	printf("destroy postfix representation\n");
	for(int i=0;i<scptint->eq_nb;i++)
	{
		printf("eq %d: %p\n",i,postfix[i]);
		delete[] postfix[i];
	}
	delete [] postfix;
	delete [] postfix_nb;
	for(int i=0;i<scptint->fct_nb;i++)
	{
		printf("function %d: %p\n",i,function[i]);
		delete[] function[i];
	}
	delete [] postfix;
	delete [] functionpostfix_nb;
	return 0;
}
/*static int convert_equations();

Fonction pour convertir automatiquement les equations et les fonctions 
utilisateurs.
*/
int edo_system::convert_equations()
{
	for(int i=0;i<scptint->eq_nb;i++)
	{
		
	edo_system::postfix[i]=edo_system::eq_conversion(0, i,&edo_system::postfix_nb[i]); 	
	}
	
	return 0;
}

int edo_system::convert_function()
{

	for(int i=0;i<scptint->fct_nb;i++)
	{
		
	edo_system::function[i]=edo_system::func_conversion(0, i,&edo_system::functionpostfix_nb[i]); 	
	}
	
	return 0;
	
	
}



int edo_system::is_number(char *ptr)
{
	if(*ptr=='.' || (*ptr>='0' && *ptr<='9'))return 1;
	
	return 0;
}

	/* Routine de detection des donnees de l equation.*/ 
int edo_system::is_function(char *ptr)
{
	
	if(strcmp("cos",ptr)==0 || strcmp("sin",ptr)==0 || strcmp("tan",ptr)==0 )
	{
		return 1;
	}
	if(strcmp("tanh",ptr)==0 )
	{
		return 1;
	}
	if(strcmp("exp",ptr)==0 || strcmp("log",ptr)==0  )
	{
		return 1;
	}
	return 0;
}

int edo_system::is_usrfunction(char *ptr)
{
	
	for(int i=0;i<scptint->fct_nb;i++)
	{
		
		if(strcmp(ptr,scptint->function_name[i])==0)return 1;
	}
	return 0;
}

int edo_system::is_var(char *ptr)
{

	for(int i=0;i<scptint->var_nb;i++)
	{
		
		if(strcmp(ptr,scptint->var_name[i])==0)return 1;
	}
	
	if(strcmp(ptr,"t")==0)return 1;//modification pour prendre en compte le temps
	return 0;
}

int edo_system::is_param(char *ptr)
{
	for(int i=0;i<scptint->var_nb;i++)
	{
		
		if(strcmp(ptr,scptint->param_name[i])==0)return 1;
	}
	return 0;	
}

int edo_system::is_operator(char *ptr)
{
	if(strcmp("-",ptr)==0 || strcmp("+",ptr)==0 || strcmp("*",ptr)==0 || strcmp("/",ptr)==0 || strcmp("^",ptr)==0 || strcmp("#",ptr)==0 || strcmp("!",ptr)==0 || strcmp(",",ptr)==0)
	{
		return 1;
	}

	if(strcmp("(",ptr)==0 || strcmp(")",ptr)==0) return 1;
return 0;
}

	//ajout du 30 03 2017 pour determiner si une donnee est un argument ou pas
int edo_system::is_usrfuncarg(char *ptr,int f_nb)
{
	
	for(int i=0;i<scptint->arg_nb[f_nb];i++)
	{
		
		if(strcmp(ptr,scptint->function_arg[f_nb][i])==0)return 1;
	}
	return 0;	
	
}
	


	
	/* int operator_precedance(char *ptr);
	
	Routine fournissant le score d'importance de chaque operateur.
	
	*/
int edo_system::operator_precedance(char *ptr)
{
		//ajout du 20 03 17
		if(*ptr==',')return 5;
		//
		if(*ptr=='^' || *ptr=='#' || *ptr=='!')return 4;
		if(*ptr=='*' || *ptr=='/')return 3;
		if(*ptr=='+' || *ptr=='-')return 2;
		if(*ptr=='(' || *ptr==')')return 1;
		return 0;
			
	
}	

int edo_system::operator_associat(char *ptr)
{
		if(*ptr=='^' || *ptr=='#' || *ptr=='!')return 1;
		
		return 0;
}
	
/* static char * extract_next_eqpar(char *ptr, char *dest);
	
routine pour extraire le prochain param/operateur d une aquation.
La donnée extraite est stockée dans dest tandis que la fonction retourne un pointeur
sur la suite de l equation. 
*/
 char * edo_system::extract_next_eqpar(char *ptr, char *dest)
{
	

	char *c=ptr;
	char *cp=dest;
		
	if(*ptr=='\0')return NULL;
	

		if(*c=='-' || *c=='+' || *c=='*' || *c=='/' || *c=='^' || *c=='(' || *c==')'|| *c=='#' || *c=='!' || *c==',')
		{
			*cp=*c;
			cp++;
			c++;
			*cp='\0';	
		}
		else
		{
		
			while(*c!='-' && *c!='+' && *c!='*' && *c!='/' && *c!='^' && *c!='(' && *c!=')' && *c!='#' && *c!='!' && *c!=',' && *c!='\0')
			{
			
				*cp=*c;
				cp++;
				c++;
	
			}
			*cp='\0';
		}
		
	
 return c;
}

postfix_type * edo_system::eq_conversion( int mode,int eq_nb,int *pile_size)
{
	
	postfix_type *res=NULL;
	 postfix_pile res_pile;
	 postfix_pile operat_pile;
	 
	 postfix_type oper_buff;
	 postfix_type pop_buff;
	 
	 int associat=0;
	 int precedance=0;
	 int associat_pop=0;
	 int precedance_pop=0;
	 // retour des fonctions
	 int ret=0;
	 
	 char buffer[32];
	 char *cptr=scptint->equation[eq_nb];
	 
	 while(1)
	 {
		 //ETAPE 1 EXTRACTION DE LA PROCHAINE DONNEE ********************	
		 cptr=extract_next_eqpar(cptr, buffer);
		 if(cptr==NULL)break;
		 
		 printf("param %s\n",buffer);
		 
		 //ETAPE 2 DECODAGE DE LA DONNEE ********************************
		 if(is_number(buffer))
		 {
		 	oper_buff.num=atof(buffer);
		 	oper_buff.type=POSTFIX_TYPE_NUMBER;
		 	
		 }else if(is_function(buffer))
		 {
		 	
			if(strcmp(buffer,"sin")==0)oper_buff.function=FUNC_SIN;
			else if(strcmp(buffer,"cos")==0)oper_buff.function=FUNC_COS;
			else if(strcmp(buffer,"exp")==0)oper_buff.function=FUNC_EXP;
			else if(strcmp(buffer,"log")==0)oper_buff.function=FUNC_LOG;
		 	else if(strcmp(buffer,"log")==0)oper_buff.function=FUNC_TAN;
		 	else if(strcmp(buffer,"log")==0)oper_buff.function=FUNC_TANH;
		 	
		 	oper_buff.type=POSTFIX_TYPE_FUNCTION;
		 	precedance=1;
		 	associat=1;
			
		 }else if(is_usrfunction(buffer))
		 {
		 	oper_buff.type=POSTFIX_TYPE_USER_FUNC;
		 	oper_buff.var_ind=scptint->func_index(buffer);
		 	precedance=1;
		 	associat=1;
		 }
		 else if(is_var(buffer))
		 {
		
			oper_buff.var_ind=scptint->var_index(buffer);
		 	oper_buff.type=POSTFIX_TYPE_VAR;
		 	
		 }else if(is_param(buffer))
		 {
			
			
			oper_buff.ptr=&param[scptint->param_index(buffer)];
		 	oper_buff.type=POSTFIX_TYPE_PARAM;
		 	
					 	
		 }else if(is_operator(buffer))
		 {
		 	oper_buff.operat=buffer[0];
		 	oper_buff.type=POSTFIX_TYPE_OPERAT;
			//calcul de la precedance de l operateur
			precedance=operator_precedance(buffer);
			associat=operator_associat(buffer);
			
		 }
		 
		//ETAPE 3 CONVERSION EN POST FIX ********************************
	 	//traitement des parametres des donnees etc...
		if(oper_buff.type!=POSTFIX_TYPE_OPERAT && !(oper_buff.type==POSTFIX_TYPE_FUNCTION || oper_buff.type==POSTFIX_TYPE_USER_FUNC))
	 	{
	 		
	 		res_pile.push(&oper_buff);
	 		
		}
		else
		{
			//Traitement des operateurs parentheses
			if(oper_buff.type==POSTFIX_TYPE_OPERAT && oper_buff.operat==')')
			{
				// operat_pile.print_from_bottom();
					while(1)
					{
						//lecture du haut de la pile d'operateur
						ret=operat_pile.w_nopop(&pop_buff);
						if(ret==0)return 0; //erreur 
						if(pop_buff.type==POSTFIX_TYPE_OPERAT && pop_buff.operat=='(')
						{
							operat_pile.top--;
							break;
						}
						operat_pile.top--;
						res_pile.push(&pop_buff);
						
					}
					
				//TRANSFERT DES TOKEN DE FONCTION SUR LA PILE RESULTAT:
						ret=operat_pile.w_nopop(&pop_buff);
						if(pop_buff.type==POSTFIX_TYPE_FUNCTION || pop_buff.type==POSTFIX_TYPE_USER_FUNC)
						{
						operat_pile.top--;
						res_pile.push(&pop_buff);
						}	
								
			}
			else if(oper_buff.type==POSTFIX_TYPE_OPERAT && oper_buff.operat=='(')//GESTION DES PARENTHESES
			{
					operat_pile.push(&oper_buff);
			}
			else if(oper_buff.type==POSTFIX_TYPE_FUNCTION || oper_buff.type==POSTFIX_TYPE_USER_FUNC)//GESTION DES FONCTIONS
					operat_pile.push(&oper_buff);
			else if(oper_buff.type==POSTFIX_TYPE_OPERAT && oper_buff.operat==',') // ajout du 20/03/2017
			{
					while(1)
					{
						//lecture du haut de la pile d'operateur
						ret=operat_pile.w_nopop(&pop_buff);
					
						if(ret==0)return 0; //erreur
						if(pop_buff.type==POSTFIX_TYPE_OPERAT && pop_buff.operat=='(')
						{
							break;
						}
						operat_pile.top--;
						res_pile.push(&pop_buff);
						
						
					}
			}
			else//GESTION STANDARD DES OPERATEUR
			{
			
			//traitement des operateurs standards.
			
				while(1)
				{
					//lecture du haut de la pile d'operateur
					ret=operat_pile.w_nopop(&pop_buff);
					
					if(ret==0)break;
					
					if(pop_buff.type==POSTFIX_TYPE_FUNCTION || pop_buff.type==POSTFIX_TYPE_USER_FUNC)
					{
					associat_pop=1;
					precedance_pop=1;
					}
					else
					{
					precedance_pop=operator_precedance(&pop_buff.operat);
					associat_pop=operator_associat(&pop_buff.operat);
					}
					
					
					if(associat==0 && precedance_pop>=precedance)
					{
							operat_pile.top--;
							res_pile.push(&pop_buff);
					}
					else if(associat==1 && precedance_pop>precedance)
					{
							operat_pile.top--;
							res_pile.push(&pop_buff);
					}
					else
					break;
					
				}
				//push de l'operateur en cours
				operat_pile.push(&oper_buff);
				
			}
			
			
		}

	 }
	 printf("argggg");
	 // ETAPE 4 TRAITEMENT FINI, TRAITEMENT DE LA PILE D OPERATEURs
	 	while(1)
		{
					//lecture du haut de la pile d'operateur
			ret=operat_pile.pop(&pop_buff);
			if(ret==0)break;
			res_pile.push(&pop_buff);		
			
		}
	 operat_pile.print_from_bottom();
	 
	 res_pile.print_from_bottom();
	 
	 
	 //CREATION DU RESULTAT
	 res=new postfix_type[res_pile.top];
	 *pile_size=res_pile.top;
	 
	 //COPIE 
	 for(int i=0;i<res_pile.top;i++)
	 {
	 	res[i].function=res_pile.pile[i].function;	
	 	res[i].num=res_pile.pile[i].num;		
	 	res[i].operat=res_pile.pile[i].operat;		
	 	res[i].ptr=res_pile.pile[i].ptr;
	 	res[i].type=res_pile.pile[i].type;
	 	res[i].var_ind=res_pile.pile[i].var_ind;
	 }
	 
	 return res;
}


//


postfix_type * edo_system::func_conversion( int mode,int func_nb,int *pile_size)
{
	
	postfix_type *res=NULL;
	 postfix_pile res_pile;
	 postfix_pile operat_pile;
	 
	 postfix_type oper_buff;
	 postfix_type pop_buff;
	 
	 int associat=0;
	 int precedance=0;
	 int associat_pop=0;
	 int precedance_pop=0;
	 // retour des fonctions
	 int ret=0;
	 
	 char buffer[32];
	 char *cptr=scptint->function[func_nb];
	 
	 while(1)
	 {
		 //ETAPE 1 EXTRACTION DE LA PROCHAINE DONNEE ********************	
		 cptr=extract_next_eqpar(cptr, buffer);
		 if(cptr==NULL)break;
		 
		 printf("param %s\n",buffer);
		 
		 //ETAPE 2 DECODAGE DE LA DONNEE ********************************
		 if(is_number(buffer))
		 {
		 	oper_buff.num=atof(buffer);
		 	oper_buff.type=POSTFIX_TYPE_NUMBER;
		 	
		 }else if(is_function(buffer))
		 {
		 	
			if(strcmp(buffer,"sin")==0)oper_buff.function=FUNC_SIN;
			else if(strcmp(buffer,"cos")==0)oper_buff.function=FUNC_COS;
			else if(strcmp(buffer,"exp")==0)oper_buff.function=FUNC_EXP;
			else if(strcmp(buffer,"log")==0)oper_buff.function=FUNC_LOG;
		 	else if(strcmp(buffer,"log")==0)oper_buff.function=FUNC_TAN;
		 	else if(strcmp(buffer,"log")==0)oper_buff.function=FUNC_TANH;
		 	
		 	oper_buff.type=POSTFIX_TYPE_FUNCTION;
		 	precedance=1;
		 	associat=1;
			
		 }else if(is_usrfunction(buffer))
		 {
		 	oper_buff.type=POSTFIX_TYPE_USER_FUNC;
		 	oper_buff.var_ind=scptint->func_index(buffer);
		 	precedance=1;
		 	associat=1;
		 }
		 else if(is_usrfuncarg(buffer,func_nb))
		 {
		 	oper_buff.type=POSTFIX_TYPE_USER_FUNC_ARG;
		 	oper_buff.var_ind=scptint->func_arg_index(buffer,func_nb);
		 }
		 else if(is_var(buffer))
		 {
		
			oper_buff.var_ind=scptint->var_index(buffer);
		 	oper_buff.type=POSTFIX_TYPE_VAR;
		 	
		 }else if(is_param(buffer))
		 {
			
			oper_buff.ptr=&param[scptint->var_index(buffer)];
		 	oper_buff.type=POSTFIX_TYPE_PARAM;
		 	
					 	
		 }else if(is_operator(buffer))
		 {
		 	oper_buff.operat=buffer[0];
		 	oper_buff.type=POSTFIX_TYPE_OPERAT;
			//calcul de la precedance de l operateur
			precedance=operator_precedance(buffer);
			associat=operator_associat(buffer);
			
		 }
		 
		//ETAPE 3 CONVERSION EN POST FIX ********************************
	 	//traitement des parametres des donnees etc...
		if(oper_buff.type!=POSTFIX_TYPE_OPERAT && !(oper_buff.type==POSTFIX_TYPE_FUNCTION || oper_buff.type==POSTFIX_TYPE_USER_FUNC))
	 	{
	 		
	 		res_pile.push(&oper_buff);
	 		
		}
		else
		{
			//Traitement des operateurs parentheses
			if(oper_buff.type==POSTFIX_TYPE_OPERAT && oper_buff.operat==')')
			{
				// operat_pile.print_from_bottom();
					while(1)
					{
						//lecture du haut de la pile d'operateur
						ret=operat_pile.w_nopop(&pop_buff);
						if(ret==0)return 0; //erreur 
						if(pop_buff.type==POSTFIX_TYPE_OPERAT && pop_buff.operat=='(')
						{
							operat_pile.top--;
							break;
						}
						operat_pile.top--;
						res_pile.push(&pop_buff);
						
					}
					
				//TRANSFERT DES TOKEN DE FONCTION SUR LA PILE RESULTAT:
						ret=operat_pile.w_nopop(&pop_buff);
						if(pop_buff.type==POSTFIX_TYPE_FUNCTION ||pop_buff.type==POSTFIX_TYPE_USER_FUNC )
						{
						operat_pile.top--;
						res_pile.push(&pop_buff);
						}	
								
			}
			else if(oper_buff.type==POSTFIX_TYPE_OPERAT && oper_buff.operat=='(')//GESTION DES PARENTHESES
			{
					operat_pile.push(&oper_buff);
			}
			else if(oper_buff.type==POSTFIX_TYPE_FUNCTION || oper_buff.type==POSTFIX_TYPE_USER_FUNC)//GESTION DES FONCTIONS
					operat_pile.push(&oper_buff);
			else if(oper_buff.type==POSTFIX_TYPE_OPERAT && oper_buff.operat==',') // ajout du 20/03/2017
			{
					while(1)
					{
						//lecture du haut de la pile d'operateur
						ret=operat_pile.w_nopop(&pop_buff);
					
						if(ret==0)return 0; //erreur
						if(pop_buff.type==POSTFIX_TYPE_OPERAT && pop_buff.operat=='(')
						{
							break;
						}
						operat_pile.top--;
						res_pile.push(&pop_buff);
						
						
					}
			}
			else//GESTION STANDARD DES OPERATEUR
			{
			
			//traitement des operateurs standards.
			
				while(1)
				{
					//lecture du haut de la pile d'operateur
					ret=operat_pile.w_nopop(&pop_buff);
					
					if(ret==0)break;
					
					if(pop_buff.type==POSTFIX_TYPE_FUNCTION)
					{
					associat_pop=1;
					precedance_pop=1;
					}
					else
					{
					precedance_pop=operator_precedance(&pop_buff.operat);
					associat_pop=operator_associat(&pop_buff.operat);
					}
					
					
					if(associat==0 && precedance_pop>=precedance)
					{
							operat_pile.top--;
							res_pile.push(&pop_buff);
					}
					else if(associat==1 && precedance_pop>precedance)
					{
							operat_pile.top--;
							res_pile.push(&pop_buff);
					}
					else
					break;
					
				}
				//push de l'operateur en cours
				operat_pile.push(&oper_buff);
				
			}
			
			
		}

	 }
	 printf("argggg");
	 // ETAPE 4 TRAITEMENT FINI, TRAITEMENT DE LA PILE D OPERATEURs
	 	while(1)
		{
					//lecture du haut de la pile d'operateur
			ret=operat_pile.pop(&pop_buff);
			if(ret==0)break;
			res_pile.push(&pop_buff);		
			
		}
	 operat_pile.print_from_bottom();
	 
	 res_pile.print_from_bottom();
	 
	 
	 //CREATION DU RESULTAT
	 res=new postfix_type[res_pile.top];
	 *pile_size=res_pile.top;
	 
	 //COPIE 
	 for(int i=0;i<res_pile.top;i++)
	 {
	 	res[i].function=res_pile.pile[i].function;	
	 	res[i].num=res_pile.pile[i].num;		
	 	res[i].operat=res_pile.pile[i].operat;		
	 	res[i].ptr=res_pile.pile[i].ptr;
	 	res[i].type=res_pile.pile[i].type;
	 	res[i].var_ind=res_pile.pile[i].var_ind;
	 }
	 
	 return res;
}


	/******************************************************
	*****EVALUATION DES EQUATION EN POSTFIX **************/
	
 double edo_system::evaluate_eq(int eq_nb,double *var, double *cn,double *t)
 {
 	
 	postfix_pile num_pile;
 	postfix_type operat;
 	postfix_type num;
 	
 	postfix_type buffer;
 	double dat1;
 	double dat2;
 	int ibuff;//buffer d entier, multiusages
 	double usr_func_arg[32];
 	
 	for(int i=0;i<postfix_nb[eq_nb];i++)
 	{
 		
 		
			printf("toto %d",postfix_nb[eq_nb]);
				switch(postfix[eq_nb][i].type)
				{
					case POSTFIX_TYPE_PARAM:
						
						//recuperation des param de la classe edo system et envoi su le pile num.
						
						buffer.num=*postfix[eq_nb][i].ptr;
						buffer.type=POSTFIX_TYPE_NUMBER;
						num_pile.push(&buffer);
							
						break;
					case POSTFIX_TYPE_VAR:
						
						if(postfix[eq_nb][i].var_ind==10000) //traitement de la variable implicite temps
						{
						buffer.num=*t;
						}
						else
						buffer.num=var[postfix[eq_nb][i].var_ind];// traitement du reste
						
						buffer.type=POSTFIX_TYPE_NUMBER;
						num_pile.push(&buffer);
						
						break;
					case POSTFIX_TYPE_NUMBER:
						
						num_pile.push(&postfix[eq_nb][i]);
						num_pile.print_from_bottom();
						break;				
					case POSTFIX_TYPE_OPERAT:
						
						switch(postfix[eq_nb][i].operat)
						{
							case '#':
							num_pile.pop(&num);// pop
							num.num=-num.num;// conversion				
							num_pile.push(&num);//push
							break;
							
							case '!':
								//rien a faire, c'est un plus unaire
							break;
							
							case '*':
								num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num*=dat1;
								num_pile.push(&num);//push
							
																
							break;
							
							case '+':
								num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num+=dat1;
								num_pile.push(&num);//push
								
							break;
							
							case '-':
								num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num-=dat1;
								num_pile.push(&num);//push
								
							break;
							
							case '/':
									num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num/=dat1;
								num_pile.push(&num);//push
							break;
							
							case '^':
								num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num=pow(num.num,dat1);
								num_pile.push(&num);//push
							break;
							
						}
						
						break;
					case POSTFIX_TYPE_FUNCTION:
						//traitement des fonctions. Le pop dans le switch est ok pour gerer les fonctions plus complexes
						switch(postfix[eq_nb][i].function)
						{
							case FUNC_SIN:
								num_pile.pop(&num);
								num.num=sin(num.num);
								num_pile.push(&num);
								break;
							
							case FUNC_COS:
								num_pile.pop(&num);
								num.num=cos(num.num);
								num_pile.push(&num);
								break;
							
							case FUNC_EXP:
								num_pile.pop(&num);	
								num.num=exp(num.num);
								num_pile.push(&num);
								break;				
							
							case FUNC_LOG:
								num_pile.pop(&num);
								num.num=log(num.num);
								num_pile.push(&num);
								break;
							
							case FUNC_TAN:
								num_pile.pop(&num);
								num.num=tan(num.num);
								num_pile.push(&num);
								break;
								
							case FUNC_TANH:
								num_pile.pop(&num);	
								num.num=tanh(num.num);
								num_pile.push(&num);
								break;
						}
						
						
						
						break;
					case POSTFIX_TYPE_USER_FUNC:
						
						//Calcul du nombre d'argument a poper
						// D apres les tests du 03/04/17, les arguments a poper sont dans l'ordre
						ibuff=scptint->arg_nb[postfix[eq_nb][i].var_ind];
						
						for(int j=0;j<ibuff;j++)
						{
							num_pile.pop(&num);	
							usr_func_arg[j]=num.num;
							printf("\nARG DE FONCTION UTILISATEUR N %d: %e\n",j,num.num);
						}
						
						num.num=evaluate_usr_fcn(postfix[eq_nb][i].var_ind,var, cn,t,usr_func_arg,ibuff);
						num_pile.push(&num);
						
						break;
				} 	
				 
	}
		  
	 num_pile.print_from_bottom();
 	
 	return num_pile.pile[0].num;
 }



	
 double edo_system::evaluate_usr_fcn(int fct_nb,double *var, double *cn,double *t,double *fctarg,int argnb)
{

	postfix_pile num_pile;
 	postfix_type operat;
 	postfix_type num;
 	
 	postfix_type buffer;
 	double dat1;
 	double dat2;
 	int ibuff;//buffer d entier, multiusages
 	double usr_func_arg[32];
 	
 	for(int i=0;i<functionpostfix_nb[fct_nb];i++)
 	{
 		
 		
			printf("toto %d",functionpostfix_nb[fct_nb]);
				switch(function[fct_nb][i].type)
				{
					case POSTFIX_TYPE_PARAM:
						
						//recuperation des param de la classe edo system et envoi su le pile num.
						
						buffer.num=*function[fct_nb][i].ptr;
						buffer.type=POSTFIX_TYPE_NUMBER;
						num_pile.push(&buffer);
							
						break;
					case POSTFIX_TYPE_VAR:
						
						if(function[fct_nb][i].var_ind==10000) //traitement de la variable implicite temps
						{
						buffer.num=*t;
						}
						else
						buffer.num=var[function[fct_nb][i].var_ind];// traitement du reste
						
						buffer.type=POSTFIX_TYPE_NUMBER;
						num_pile.push(&buffer);
						
						break;
					case POSTFIX_TYPE_NUMBER:
						
						num_pile.push(&function[fct_nb][i]);
						num_pile.print_from_bottom();
						break;				
					case POSTFIX_TYPE_OPERAT:
						
						switch(function[fct_nb][i].operat)
						{
							case '#':
							num_pile.pop(&num);// pop
							num.num=-num.num;// conversion				
							num_pile.push(&num);//push
							break;
							
							case '!':
								//rien a faire, c'est un plus unaire
							break;
							
							case '*':
								num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num*=dat1;
								num_pile.push(&num);//push
							
																
							break;
							
							case '+':
								num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num+=dat1;
								num_pile.push(&num);//push
								
							break;
							
							case '-':
								num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num-=dat1;
								num_pile.push(&num);//push
								
							break;
							
							case '/':
									num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num/=dat1;
								num_pile.push(&num);//push
							break;
							
							case '^':
								num_pile.pop(&num);// pop premier nombre
								dat1=num.num;
								num_pile.pop(&num);// pop second nombre
								
								num.num=pow(num.num,dat1);
								num_pile.push(&num);//push
							break;
							
						}
						
						break;
					case POSTFIX_TYPE_FUNCTION:
						//traitement des fonctions. Le pop dans le switch est ok pour gerer les fonctions plus complexes
						switch(function[fct_nb][i].function)
						{
							case FUNC_SIN:
								num_pile.pop(&num);
								num.num=sin(num.num);
								num_pile.push(&num);
								break;
							
							case FUNC_COS:
								num_pile.pop(&num);
								num.num=cos(num.num);
								num_pile.push(&num);
								break;
							
							case FUNC_EXP:
								num_pile.pop(&num);	
								num.num=exp(num.num);
								num_pile.push(&num);
								break;				
							
							case FUNC_LOG:
								num_pile.pop(&num);
								num.num=log(num.num);
								num_pile.push(&num);
								break;
							
							case FUNC_TAN:
								num_pile.pop(&num);
								num.num=tan(num.num);
								num_pile.push(&num);
								break;
								
							case FUNC_TANH:
								num_pile.pop(&num);	
								num.num=tanh(num.num);
								num_pile.push(&num);
								break;
						}
						
						
						
						break;
					case POSTFIX_TYPE_USER_FUNC:
						
						//Calcul du nombre d'argument a poper
						ibuff=scptint->arg_nb[function[fct_nb][i].var_ind];
						
						for(int j=0;j<ibuff;j++)
						{
							num_pile.pop(&num);	
							usr_func_arg[j]=num.num;
						}
						
						num.num=evaluate_usr_fcn(function[fct_nb][i].var_ind,var, cn,t,usr_func_arg,ibuff);
						num_pile.push(&num);

						
						break;
						
					case POSTFIX_TYPE_USER_FUNC_ARG:
						
						buffer.num=fctarg[argnb-1-function[fct_nb][i].var_ind];
						buffer.type=POSTFIX_TYPE_NUMBER;
						num_pile.push(&buffer);
						
						break;
				} 	
				 
	}
		  
	printf("\n\n resultat user func\n");	  
	num_pile.print_from_bottom();
 	
 	return num_pile.pile[0].num;


}
