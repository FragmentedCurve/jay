/*
 * (c) Oleg Puchinin 2006
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

namespace PYTHON {
	
char * t_map;
char * t_new;
char pch;
int i;
int ii;
int t_size;
int spaces;
int nlines;
int t_op_no;
pair_t * d_attachment = NULL;

#define T t_map[i]
#define TN(ch) do { t_new[ii] = ch; ++ii; } while (0)

inline void t_attach ()
{
	t_op_no++;
	d_attachment[t_op_no].pair_op = t_op_no;
	d_attachment[t_op_no].pair_line = nlines;
	d_attachment[t_op_no].offset = i;
}

inline void tt_skip ()
{
	while (T != '\n' && i < t_size)
		++i;
	--i;
}

void doc_skip ()
{
	char * ptrn;

	if (T == '\'')
		ptrn = (char *) "'''";
	else
		ptrn = (char *) "\"\"\"";
	
	i += 3;
	if (i >= t_size)
		return;
	
	for (; i < t_size; ++i) {
		if (T == '\n')
			++nlines;

		if (! strncmp (&t_map[i], ptrn, 3))
			break;
	}

	i+=2;
}

int py_tt (char * d_input, int size, char * d_output)
{
	char ch;
	char * S;
	bool b_newline = true;

	i = 0;
	ii = 0;
	t_size = size;
	t_map = d_input;
	t_new = d_output;
	spaces = 0;
	nlines = 0;
	t_op_no = 0;

	for (i = 0; i < t_size; ++i) {
		if (isalnum (T))
			goto abc;
		
		if (! strncmp (&t_map[i], "\"\"\"", 3) || 
		    ! strncmp (&t_map[i], "'''", 3)) {
			doc_skip ();
			continue;
		}

		if (T == '\"' || T == '\'') {			
			ch = T;
			S = sstrend (&t_map[i]);
			assert (S == NULL, "\n\nSibadi 2006\n\n");
			nlines += Dsyms (&t_map[i], S, '\n'); 
			i += S - &t_map[i];
			TN (ch);
			TN (ch);
			continue;
		}

		if (T == '#') {
			tt_skip ();
			continue;
		}
		
		if (T == ' ' || T == '\t') {
			if (b_newline) {
				++spaces;
				TN (T);
			}
			if (! spaces) {
				++spaces;
				TN (' ');
			}

			continue;
		}

		if (T == '\n') {
			t_attach ();
			++nlines;
			TN (T);
			continue;
		}
		
		if (T == ':') 
			t_attach ();
		
abc:
		b_newline = false;
		spaces = 0;
		TN(T);
	}

	TN ('\0');
	return ii+1;
}

char * py_tt_file (struct tt_state_t * tt)
{
	if (! tt)
		return NULL;

	if (access (tt->fileName, R_OK) != F_OK) {
		fprintf (stderr, "No such file %s\n", tt->fileName);
		return NULL;
	}

	tt_map (tt);
	
	tt->result = CNEW (char, 1024*1024);
	tt->result[0] = 0;
	d_attachment = (pair_t *) CNEW (char, 1024*1024);
		
	tt->attachment = d_attachment;
	tt->resultSize = py_tt (tt->fileData, tt->fileDataSize, tt->result);
	return tt->result;
}

char t_op (char ** d_in, char ** d_prev)
{
	char *d_ptr = *d_in;
	char ch = 0;
	
	if (*d_prev)
		*d_prev = *d_in;

	while (*d_ptr) {
		if (*d_ptr == '\n' || *d_ptr == ':') {
			ch = *d_ptr;
			*d_ptr = 0;
			break;
		}
		++d_ptr;
	}

	++d_ptr;
	*d_in = d_ptr;
	return ch;
}

} // namespace PYTHON
