/*
 * (c) Oleg Puchinin 2006.
 * graycardinalster@gmail.com
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <gclib.h>
#include <mod.h>
#include <head.h>
#include <dbg.h>
#include <the_tt.h>
#include <the_fly.hpp>

void __perl_files (char * f_name);
int perl_ctags (char * f_name, FILE * of);
char pl_op (char ** d_in, char ** d_prev);

extern struct env_t *ENV;
FILE * perl_out_file;

void pl_sub (struct tt_state_t *tt, int line, char * d_out, char ch)
{
	char *S;
	char *part1, *part2;

	S = strchr (d_out, '(');
	if (S)
		*S = '\0';
	
	part1 = strchr (d_out, ' ');
	++part1;
	part2 = strchr (part1, ':');

	if (part2) {
		*part2 = '\0';
		++part2;
		part2 = strip (part2);
		strip2 (part2);
	}

	strip2 (part1);
	if (part2) 
		fprintf (perl_out_file, "%s:%s\t%s\t%i\t; f\n", part1, part2, tt->fileName, line);
	else
		fprintf (perl_out_file, "%s\t%s\t%i\t; f\n", part1, tt->fileName, line);
}

void pl_package (struct tt_state_t *tt, int line, char * d_out, char ch)
{
	char * S;
	char * ptr;

	S = strchr (d_out, ' ');
	if (! S)
		return;

	++S;
	strip2 (S);

	ptr = rindex (S, ':');
	if (ptr) {
		++ptr;
		fprintf (perl_out_file, "%s\t%s\t%i\t; p\n", ptr, tt->fileName, line);
	}
	fprintf (perl_out_file, "%s\t%s\t%i\t; p\n", S, tt->fileName, line);
}

void perl_make_tag (struct tt_state_t *tt, char * d_out, char ch) 
{
	int line;

	line = tt->attachment[ENV->t_op_no].pair_line+1;
	if (*d_out == ' ')
		++d_out;

	if ((ch == '{') && (! strncmp (d_out, "sub ", 4))) {
		pl_sub (tt, line, d_out, ch);
		return;
	}
	
	if ((ch == ';') && (! strncmp (d_out, "package ", 8))) {
		pl_package (tt, line, d_out, ch);
		return;
	}
}

int perl_ctags (char * f_name, FILE * of) 
{
	char ch;
	char *d_ptr,*d_out; // for pl_op
	int block_depth = 0;
	struct tt_state_t *tt;

	perl_out_file = of;
	
	ENV->t_op_no = 0;
	tt = CNEW (tt_state_t, 1);
	memset (tt, 0, sizeof (struct tt_state_t));
	tt->fileName = strdup (f_name);
	THE_FLY::fly_for_file (tt);

	d_out = tt->result;
	d_ptr = d_out;
	while (true) {
		ch = pl_op (&d_ptr, &d_out);
		ENV->t_op_no++;

		if (ch == '\0')
			break;

		if (! block_depth)
			perl_make_tag (tt, d_out, ch);

		if (ch == '{') 
			++block_depth;

		if (ch == '}')
			--block_depth;
		
		if (block_depth < 0)
			block_depth = 0;
	}
	
	fflush (perl_out_file);
	free_tt_state (tt);
	return 0;
}

char pl_op (char ** d_in, char ** d_prev)
{
	char *d_ptr = *d_in;
	char ch = 0;
	
	if (*d_prev)
		*d_prev = *d_in;

	while (*d_ptr) {
		if (*d_ptr == '{' || *d_ptr == ';' ||
				*d_ptr == '}') {
			if (d_ptr[-1] == '\\') {
				++d_ptr;
				continue;
			}
			ch = *d_ptr;
			*d_ptr = 0;
			++d_ptr; // space
			break;
		}
		++d_ptr;
	}

	++d_ptr;
	*d_in = d_ptr;
	return ch;
}

