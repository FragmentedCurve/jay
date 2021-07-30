/*
 * (c) Oleg Puchinin 2006,2007
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>
#include <wit.h>
#include <the_tt.h>
#include <dbg.h>
#include <fcntl.h>
#include <TT.h>

int call_tags_file (char * fileName, FILE * of)
{
	char * functionName = NULL;
	char m_name[256];
	char * oldName = NULL;
	char * ptr = NULL;
	char * S = NULL;
	int i;
	TT * tt = NULL;
	DArray * d_words = NULL;

	tt = new TT;
	if (tt->loadFile (fileName) < 0) {
		delete tt;
		return -1;
	}

	tt->init ();

	while (true) {
		tt->nextOperator ();
		if (tt->ch == 0)
			break;

		ptr = tt->op ();

		if ((ptr[0] == '#') && def_test (ptr)) {
			if (macro_name (ptr, m_name)) 
				functionName = m_name;
		}

		if (! tt->bracketDepth) {
			if (tt->ch == '\n' && ptr[strlen(ptr) - 1] != '\\') {
				functionName = oldName;
			}
		}

		if (tt->wit () == OT::Function ) {
			functionName = c_last_word (tt->op ());
			oldName = functionName;
		}

		if (! tt->bracketDepth)
			continue;

		d_words = split_to_words (ptr);
		for (i = 0; i < d_words->get_size (); i++) {
			S  = cts ((c_word *) d_words->get (i));
			if (! S) 
				continue;

			if (functionName != NULL) {
				fprintf (of, "%s\t%s\t%i\t;\tby\t%s\n", S, 
						tt->tt->fileName,
						tt->tt->attachment[get_env()->t_op_no].pair_line+1,
						functionName);
			} else {				
				fprintf (of, "%s\t%s\t%i\n", S,
					tt->tt->fileName,
					tt->tt->attachment[get_env()->t_op_no].pair_line+1);
			}
		}

		d_words->foreach ((Dfunc_t) free_cword);
		delete d_words; 
	}

	fflush (of);
	delete tt;
	return 0;
}


