/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <gclib.h>
#include <gclib_c.h>
#include <mod.h>
#include <head.h>
#include <dbg.h>

struct grep_env_t {
	bool ignore_case;
	char * f_name;
};
struct grep_env_t ENV_grep;

void grepBuf (char * buf, int size, char * search, int s_len)
{
	int nlines;
	char *s_line;
	char *ptr;
	char *end;
	char *end2;
	char * line_ptr;

	if (size < s_len)
		return;
	
	nlines = 1;
	ptr = buf;
	end = &buf[size - s_len - 1];
	end2 = &buf[size - 1];
	line_ptr = ptr;
	
	while (ptr <= end) {
		if (*ptr == '\n') {
			line_ptr = ptr+1;
			++nlines;
		}

		if (! memcmp (ptr, search, s_len)) {
			s_line = Dmid_getstr (line_ptr, end2);
			if (! s_line) 
				break;
			chomp (s_line);
			if (SB_FLGET (SB_FLTAGSTYLE)) 
				printf ("%s\t%s\t%i\n", search, ENV_grep.f_name, nlines);
			else
				printf ("%s:%i: %s\n", ENV_grep.f_name, nlines, s_line);
			fflush (stdout);
			free (s_line);
			ptr += s_len;
			continue;
		}
		++ptr;
	}
}

int grepFile (char * fileName, FILE * of)
{
	char * ptr;
	int size;

	ptr = DFILE (fileName, &size);
	if (! ptr) {
		perror ("DFILE");
		return -1;
	}
	ENV_grep.f_name = fileName;
	grepBuf (ptr, size, get_env()->grep_exp, strlen (get_env()->grep_exp));
	DROP (ptr);
	return 0;
}

