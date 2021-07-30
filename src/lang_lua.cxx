/*
 * (c) Oleg Puchinin 2010
 * graycardinalster@gmail.com
 *
 */

#include <head.h>

void print_lua_tag (char * f_name, int lnum, char * line, FILE *of)
{
	char *S;
	char *S2;
	strip(line);

	if (strncmp (line, "function", 8))
		return;
	if ((line[8] != ' ') && (line[8] != '\t'))
		return;
	S = strchr (line, '(');
	if (! S)
		return;

	*S = '\0';
	S = &line[8];
	strip (S);
	strip2 (S);

	S2 = strchr (S, '.');
	if (S2) 
		S = S2+1;	
	if (!strlen (S))
		return;

	fprintf (of, "%s\t%s\t%d\t; %c\n", S, f_name, lnum, 'f');
}

int lua_ctags (char * f_name, FILE *of) 
{
	FILE * myfile;
	char * buf;
	int line = 0;

	myfile = fopen (f_name, "r");
	if (! myfile)
		return -1;

	buf = CNEW (char, 4096);
	while (fgets (buf, 4096, myfile)) {
		++line;
		if (strstr (buf, "function"))
			print_lua_tag (f_name, line, buf, of);
	}

	fclose (myfile);
	DROP (buf);

	return 0;
}

