/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>
#include <wit.h>
#include <the_tt.h>
#include <dbg.h>

extern char * d_lpsz_operators[]; // from nogui_indent.cpp

// $ silent_bob --file
// Get functions list from file.
void flist_main (char * d_file)
{
	struct tt_state_t * d_tt_state;
	EArray d_operators;
	char *d_out,*d_ptr;
	char ch;
	int brace_depth = 0;

	if (access (d_file, R_OK) != F_OK) {
		printf ("/*** %s ***/\n", d_file);
		perror ("");
		return;
	}
	
	d_tt_state = CNEW (tt_state_t, 1);
	d_tt_state->fileName = strdup (d_file);
	d_ptr = THE_TT::do_tt_file (d_tt_state);
	d_ptr = do_tt(d_tt_state);
	
	printf ("/*** %s ***/\n", d_file);
	fflush (stdout);
	while (true) {
		ch = t_op (&d_ptr, &d_out, true);
		
		if (ch == 0)
			break;

		if (!brace_depth && d_operators.snfind (d_out) == -1) {
			if (what_is_this (d_out, ch) == OT::Function) 
				printf ("%s;\n", d_out);
		}

		if (ch == '{')
			brace_depth ++;

		if (ch == '}')
			brace_depth --;

		if (brace_depth < 0)
			brace_depth = 0;
	}

	fputc ('\n', stdout);
	free_tt_state (d_tt_state);
}

