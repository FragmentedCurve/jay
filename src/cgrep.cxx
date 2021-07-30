/* 
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#include <head.h>
#include <structs.h>
#include <the_tt.h>
#include <dbg.h>

namespace cgrep {
	
/// смещение (в строках) строки str в буфере d_buf.
int stringOffset (char ** d_buf, char *str) 
{
	char * S;
	int len = strlen (str);
	int fix = 0;;

	S = *d_buf;
	while (true) {
		if (! strncmp (S, str, len))
			break;
		if (*S == '\n')
			fix--;
		--S;
	}

	*d_buf = S;
	return fix;
}
	
int fixPosition (tt_state_t *tt, int t_op_no, DArray * exp) 
{
	int fix = 0;
	char * d_buf;
	int i;
	
	if (! tt || ! exp)
		return 0;

	d_buf = tt->fileData + tt->attachment[t_op_no].offset - 1;
	for (i = exp->get_size () - 1; i >= 0; --i) 
		fix += stringOffset (&d_buf, exp->get (i));

	return fix;
}

void print (tt_state_t * tt, DArray * d_array, DArray * d_lines, FILE * of) 
{
	int line;
	line = tt->attachment[get_env()->t_op_no].pair_line + 1;
	line += fixPosition (tt, get_env()->t_op_no, d_array);
	if (SB_FLGET (SB_FLTAGSTYLE)) 
		fprintf (of, "%s\t%s\t+%i\n", get_env()->cgrep_exp,
				tt->fileName, line);
	else 
		fprintf (of, "%s +%i: %s", tt->fileName, line, d_lines->get (line-1));
	fflush (of);
}

bool scan (char *S, DArray * d_array) 
{
	char * one;
	int size;
	int i;

	size = d_array->get_size ();
	for (i = 0; i < size; i++) {
		one = d_array->get (i);
		S = strstr (S, one);
		if (! S)  
			break;
		S += strlen (one);
	}

	if (i == size)
		return true;
	return false;
}

tt_state_t * prepare_tt (char * fileName)
{
	tt_state_t * tt;
	tt = CNEW (tt_state_t, 1);
	memset (tt, 0, sizeof (tt_state_t));
	tt->fileName = strdup (fileName);	
	
	if (THE_TT::do_tt_file (tt) == NULL) {
		free_tt_state (tt);
		return NULL; // broken file 
	}

	return tt;
}

int cgrep (char * fileName, FILE * of) 
{
	char * d_ptr, *d_out;
	DArray * exp_arr;	
	DArray * d_lines = NULL;
	tt_state_t * tt;
	char ch;
	
	if (! of)
		return -1;

	tt = prepare_tt (fileName);
	if (! tt)
		return -1;

	d_out = tt->result;
	d_ptr = tt->result;
	get_env()->t_op_no = 0;
	
	exp_arr = Dsplit (get_env()->cgrep_exp, (char *) ",");
	if (exp_arr == NULL) 
		return -2;
	
	if (! SB_FLGET (SB_FLTAGSTYLE)) {
		d_lines = new DArray (1024);
		d_lines->from_file (tt->fileName);
	}

	while (true) {
		ch = t_op (&d_ptr, &d_out);
		get_env()->t_op_no++;
		if (! ch)
			break;
		if (! scan (d_out, exp_arr))
			continue;
		print (tt, exp_arr, d_lines, of);
	}

	if (d_lines) {
		d_lines->foreach (free);
		delete d_lines;
	}

	free_tt_state (tt);
	return 0;
}

bool valid_exp ()
{
	if (get_env()->cgrep_exp == NULL)
	   return false;	

	if (strchr (get_env()->cgrep_exp, ' ') ||
	    strchr (get_env()->cgrep_exp, '\t')) 
		return false;
	return true;
}

bool init () 
{
	if (! valid_exp ()) {
		fprintf (stderr, "Invalid expression.\n");
		exit (0);
	}		
	return true;
}

} // namespace cgrep

