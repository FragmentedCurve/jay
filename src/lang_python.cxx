
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>
#include <mod.h>
#include <head.h>
#include <dbg.h>
#include <the_tt.h>
#include <wit.h>

int python_ctags (char * f_name, FILE * of);

namespace PYTHON {
	char * py_tt_file (struct tt_state_t * tt);
	char t_op (char ** d_in, char ** d_prev);
}

char * py_prepare (char *ptr)
{
	char *S;

	S = strchr (ptr, '(');
	if (S)
		*S = '\0';

	strip2 (ptr);
	S = strchr (ptr, ' ');
	return ++S;
}

void python_tag (tt_state_t * tt, char *ptr, int type, FILE *of)
{
	int line = tt->attachment[get_env()->t_op_no].pair_line+1;
	char ch;
	
	while (*ptr == ' ' || *ptr == '\t')
		++ptr;
	
	if (type & OT::Class) {
		if (! strncmp (ptr, "def ", 4)) {
			ptr = py_prepare (ptr);
			ch = 'f';
			goto ttry;
		}
	}

	if (type & OT::Function) {
		if (! strncmp (ptr, "class ", 6)) {
			ptr = py_prepare (ptr);
			ch = 'c';
			goto ttry;
		}
	}
	
	return;
ttry:
	fprintf (of, "%s\t%s\t%i\t; %c\n", ptr, tt->fileName, line, ch);
}

int python_ctags (char * f_name, FILE * of)
{
	char ch;
	char *d_ptr,*d_out; // for t_op2
	struct tt_state_t *tt;

	get_env()->t_op_no = 0;
	
	tt = CNEW (tt_state_t, 1);
	memset (tt, 0, sizeof (struct tt_state_t));
	tt->fileName = strdup (f_name);
	PYTHON::py_tt_file (tt);

	d_out = tt->result;
	d_ptr = d_out;
	while (true) {
		ch = PYTHON::t_op (&d_ptr, &d_out);
		++get_env()->t_op_no;

		if (ch == '\0')
			break;

		python_tag (tt, d_out, OT::Function | OT::Class, of);
	}
	
	free_tt_state (tt);
	return 0;
}

