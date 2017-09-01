/* @Copyright Fred Zhang Rose-Hulman Instuti
   A simple stricp that simplies your life in OS after Milestone 3.
   Simple compile it with gcc and run it.
   It will gives two exmaple of how to use it.
   It generates C code that construct local char array and fill it.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** args) {
	char *name;
	int i = 0, len;
	char* content;
	char outstrs[50][3];
	FILE* outFile;
	if (argc == 3) {
		outFile = stdout;
	} else if (argc == 4){
		outFile = fopen(args[3], "w");
	} else {
		fprintf(stderr, "%s <VarialbeName> <StringContent>\n", args[0]);
		fprintf(stderr, "%s <VarialbeName> <StringContent> <OutputFile>\n", args[0]);
		exit(1);
	}
	name = args[1];
	content = args[2];

	while(*content != '\0') {
		if(*content == '\\'){
			content++;
			sprintf(outstrs[i], "\\%c", *content);
		} else {
			sprintf(outstrs[i], "%c", *content);
		}
		content++;
		i++;
	}
	len = i;

	fprintf(outFile, "char %s[%u];\n", name, len + 1);
	for (i = 0; i < len ; i++)
		fprintf(outFile, "%s[%d] = '%s';\n", name, i, outstrs[i]);
	fprintf(outFile, "%s[%d] = '\\0';\n", name, len);
	fclose(outFile);
	return 0;
}
