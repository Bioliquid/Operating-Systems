#include <stdio.h>
#include <unistd.h>

const char rocket[] =
"	   _\n\
	  /^\\\n\
	  |-|\n\
	  |M|\n\
	  |Y|\n\
	  | |\n\
	  |H|\n\
	  |W|\n\
	 /|4|\\\n\
	/ | | \\\n\
	| | | |\n\
	\\-\"\"\"-/\n\
";

int go() {
	for (int i = 0; i < 50; ++i) {
		printf("\n");
	}
	printf("%s", rocket);
	int j = 300000;
	for (int i = 0; i < 50; ++i) {
		usleep(j); 
		j = (int)(j * 0.9);
		printf("\n");
	}
	return 0;
}