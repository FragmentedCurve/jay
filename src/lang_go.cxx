
/*
 * (c) Oleg Puchinin 2010
 * graycardinalster@gmail.com
 *
 */

#include <head.h>

char * __go_skip_spaces (char *S)
{
	if (S == NULL)
		return NULL;
	while ((*S) && ((*S == ' ') || (*S == '\t')))
		++S;
	return S;
}

char * __go_first_space (char *S)
{
	if (S == NULL)
		return NULL;
	while ((*S) && !((*S == ' ') || (*S == '\t')))
		++S;
	return S;
}

int go_ctags (char * f_name, FILE * of) 
{
	FILE *f;
	char *m_buf;
	char *begin;
	int line = 1;
	char *S;

	f = fopen (f_name, "r");
	if (! f)
		return -1;
	
	m_buf = CNEW (char, 4096);
	while (fgets (m_buf, 4096, f)) {
		S = strstr (m_buf, "//");
		if (S) 
			*S = '\0';		

		S = __go_skip_spaces(m_buf);		
		begin = S;
		if ((S[4] != ' ') && (S[4] != '\t')) {
			++line;
			continue;
		}
	
		if (strncmp (begin, "func", 4) && 
			strncmp (begin, "type", 4)) {
			++line;
			continue;
		}
		
		if (strstr (begin, "func")) {
			S = &begin[5];
			begin = __go_skip_spaces(S);
			if (*begin == '(') {
				S = strchr (begin, ')');
				++S;
				S = __go_skip_spaces(S);
				begin = S;
			}
			S = strchr (begin, '(');
			if (S) {
				*S = '\0';
			}
			strip2(begin);
			fprintf (of, "%s\t%s\t%d\t; %c\n", begin, f_name, line, 'f');
		} else if (strstr (begin, "type")) {
			S = &begin[5];
			S = __go_skip_spaces(S);
			begin = S;
			S = __go_first_space (S);
			if (S)
				*S = '\0';
			fprintf (of, "%s\t%s\t%d\t; %c\n", begin, f_name, line, 't');
		}
		++line;
	}
	
	DROP (m_buf);
	fclose (f);
	return 0;
}

