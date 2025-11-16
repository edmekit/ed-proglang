#include <stdio.h>
#include "mpc.h"
static char input[2048];

long eval(mpc_ast_t* t);
long eval_op(long x, char* op, long y);

int main(int argc, char const *argv[])
{	
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Lispy = mpc_new("lispy");

	mpca_lang(MPCA_LANG_DEFAULT,
		"											\
		number : /-?[0-9]+/ ; 						\
		operator : '+' | '-' | '*' | '/' ; 			\
		expr : <number> | '(' <operator> <expr>+ ')' ;\
		lispy : /^/ <operator> <expr>+ /$/ ; 		\
		",
			Number, Operator, Expr, Lispy);


	puts("Lispy Version 0.0.0.0.1");
	puts("Press Ctrl+C to Exit\n");

	while (1) {
		fputs("edlang>", stdout);

		fgets(input,2048,stdin);

		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Lispy, &r)) {
			mpc_ast_t* a = r.output;
			long result = eval(a);
			printf("%li\n", result);
			mpc_ast_delete(a);
		} else {
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
	}

	mpc_cleanup(4, Number, Operator,Expr, Lispy);
	return 0;
}

long eval(mpc_ast_t* t){
	if (strstr(t->tag, "number")) {
		return atoi(t->contents);
	}

	char* opera = t->children[1]->contents;

	long x = eval(t->children[2]);

	int i = 3;
	while(strstr(t->children[i]->tag, "expr")) {
		x = eval_op(x, opera, eval(t->children[i]));
		i++;
	}
	return x;
}

long eval_op(long x, char* op, long y) {
	if (strcmp(op, "+") == 0) {return x + y;}
	if (strcmp(op, "-") == 0) {return x - y;}
	if (strcmp(op, "*") == 0) {return x * y;}
	if (strcmp(op, "/") == 0) {return x / y;}
	return 0;
}