/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#include <head.h>
#include <TT.h>

int java_ctags (char * f_name, FILE * of) 
{
	TT * tt;
	char * op;
	char * S;
	char * tag;
	char * dclass;
	char ch;

	if (! f_name)
		return -1;

	tt = new TT;
	if (tt->loadFile (f_name) < 0) {
		delete tt;
		return -1;
	}

	if (tt->init () < 0) {
		delete tt;
		return -1;
	}

	while (true) {
		op = tt->nextOperator ();
		if (tt->ch == 0)
			break;

		tag = NULL;
		if (tt->ch == ';') {
			if (!strncmp (op, "package ", 8)) { // PACKAGE
				S = &op[8];
				tag = S;
				ch = 'p';
			}
		} else if (tt->ch == '{') {
			if (tt->bracketDepth == 0) { // CLASS
				dclass = strstr (op, "class ");
				if (! dclass)
					continue;

				S = strstr (op, "extends");
				if (S)
					*S = '\0';
				S = strstr (op, "implements");
				if (S)
					*S = '\0';

				strip2 (dclass);
				tag = &dclass[6];
				ch = 'c';
			} else if (tt->bracketDepth == 1) { // FUNCTION
				S = strchr (op, '(');
				if (! S)
					continue;
				*S = '\0';
				strip (op);
				strip2 (op);

				tag = c_last_word (op);
				ch = 'f';
			} else
				continue;	
		} 

		if (tag)
			fprintf (of, "%s\t%s\t%d\t; %c\n", tag, f_name, tt->line (), ch);
	}

	delete tt;
	return 0;
}

