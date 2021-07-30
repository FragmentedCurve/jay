/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#include <head.h>

int php_ctags (char * f_name, FILE * of)
{
	FILE * f;
	char * m_buf;
	char *S;
	char * dclass;
	char * dfunction;
	int line;
	char ch;

	f = fopen (f_name, "r");
	if (! f)
		return -1;
	
	m_buf = CNEW (char, 4096);
	line = 0;
	while (fgets (m_buf, 4096, f)) {
		++line;
		chomp (m_buf);

		S = strstr (m_buf, "//");
		if (S)
			*S = '\0';

		while (true) {
			S = strchr (m_buf, '\t');
			if (! S)
				break;
			*S = ' ';
		}

		dfunction = strstr (m_buf, "function ");
		dclass = strstr (m_buf, "class ");

		if (dfunction) { 
			strcpy (m_buf, dfunction);
			ch = 'f';
		} else if (dclass) {
			strcpy (m_buf, dclass);
			ch = 'c';
		} else
			continue;

		if (dfunction) {
			S = strchr (m_buf, '(');
			if (! S)
				continue;
			*S = '\0';
		} else if (dclass) {
			S = strstr (m_buf, " extends ");
			if (S)
				*S = '\0';
		} else 
			continue;

		S = strchr (m_buf, '{');
		if (S)
			*S = '\0';

		strip (m_buf);
		strip2 (m_buf);

		if (!strncmp (m_buf, "function ", 9)) 
			S = &m_buf[9];
		else if (! strncmp (m_buf, "class ", 6)) 
			S = &m_buf[6];
		else 
			continue;
		while (*S == ' ')
			++S;
		
		if (strchr (S, ' '))
			continue;

		fprintf (of, "%s\t%s\t%d\t; %c\n", S, f_name, line, ch);
	}

	DROP (m_buf);
	return 0;
}


