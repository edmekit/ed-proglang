#include <stdio.h>
static char input[2048];

int main(int argc, char const *argv[])
{	
	puts("Lispy Version 0.0.0.0.1");
	puts("Press Ctrl+C to Exit\n");

	while (1) {
		fputs("edlang>", stdout);

		fgets(input,2048,stdin);

		printf("I agree, %s", input );
	}
	return 0;
}
