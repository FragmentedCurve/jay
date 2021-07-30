
/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#include <head.h>
#include <mod.h>
#include <dbg.h>

int ruby_ctags (char * f_name, FILE * of);

char ruby_check_spec_words (char * S, char ** str)
{
	if (!S || !str)
		return 0;

	if (! strncmp (S, "def ", 4)) {
		*str = &S[4];
		return 'f';
	} else if (! strncmp (S, "class ", 6)) {
		*str = &S[6];
		return 'c';
	} else if (! strncmp (S, "module ", 7)) {
		*str = &S[7];
		return 'm';
	}

	return	0;
}

void ruby_tag (char * filename, char * str, int line, FILE * of)
{
	char * S;
	char ch;

	if (!filename || !str || !of)
		return;

	S = strchr (str, '#');
	if (S)
		*S = '\0';
	
	chomp (str);
	tabs2spaces (str);
	strip(str);
	strip2(str);

	ch = ruby_check_spec_words (str, &S);
	if (! ch)
		return;
	
	while (*S && *S == ' ')
		++S;

	str = strchr (S, '(');
	if (str)
		*str = '\0';

	str = strchr (S, '{');
	if (str)
		*str = '\0';

	str = strchr (S, ' ');
	if (str)
		*str = '\0';
	
	fprintf (of, "%s\t%s\t%d\t; %c\n", S, filename, line, ch);
}

int ruby_ctags (char * f_name, FILE * of)
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
		ruby_tag (f_name, buf, line, of);
	}

	fclose (myfile);
	DROP (buf);

	return 0;
}

