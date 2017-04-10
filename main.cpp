#include <stdio.h>
#include <stdlib.h>
 #include "script_interpreter.h"
 #include "edo_system.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	
	char buff[32];
	char *ptr;
	script_interpreter interp;
	
	interp.load_script("debug.edo");
	
	printf("%s\n",interp.script);
	printf("\n\n\n");
	interp.interpret();
	
	printf("\n\n");
	
//	interp.process_unary_eq();
	
	printf("\n\n\nvariable premiere du nom%s\n",interp.var_name[0]);
	printf("param premiere du nom %s %f\n",interp.param_name[0],interp.param[0]);
	printf("init cond %d %e\n",interp.initcond_nb,interp.init_cond[0]);
	
	
	
	ptr=interp.equation[1];
	do{
	
	ptr=edo_system::extract_next_eqpar(ptr, &buff[0]);
	if(ptr!=NULL)
	printf("%s\n",buff);
	}while(ptr!=NULL);
	
	
	
// test du parser postfix:
 edo_system::setup_postfix(&interp);	
 
 printf("\n\nConversion d'equation\n");
 int t;
 
// edo_system::postfix[0]=edo_system::eq_conversion(0, 0,&edo_system::postfix_nb[0]); 
// edo_system::function[0]=edo_system::func_conversion(0, 0,&edo_system::functionpostfix_nb[0]); 
edo_system::convert_equations();
edo_system::convert_function();

getchar();
	
 edo_system::param[0]=-10.0;
 edo_system::param[1]=-10.0;
 
printf("evaluation %e", edo_system::evaluate_eq(0,NULL,NULL,NULL));
//printf("evaluation %p",&edo_system::param[0]);
	getchar();
	
	edo_system::destroy_postfix();
	
	return 0;
}
