/*
 * (c) Oleg Puchinin 2006,2007,2008
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
#include "the_fly.hpp"

namespace THE_FLY {
	
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

inline void t_attach ()
{
	t_op_no++;
	d_attachment[t_op_no].pair_op = t_op_no;
	d_attachment[t_op_no].pair_line = nlines;
	d_attachment[t_op_no].offset = i;
}

inline void TN (char ch) 
{
	t_new[ii] = ch;
	++ii;
	if ((i && t_map[i-1] != '\\') && (ch == '{' || ch == '}' || ch == ';'))
		t_attach ();
}

inline char * fly_fw (char * word) 
{
	char * Ret;
	char *S = word;
	while (true) {
		if (!if_abc (S) &&
			       	!if_digit (S) &&
				*S != '_' &&
				*S != '*')
			break;
		++S;
	}
	Ret = CNEW (char, S - word + 1);
	memcpy (Ret, word, S - word); 
	Ret[S-word] = '\0';
	return Ret;
}

void fly_skip ()
{
	char * word = NULL;
	char * S = NULL;
	char * ptrn = NULL;
	
	word = &t_map[i+2];
	if (*word == '\'' || *word == '\"')
		++word;

	word = fly_fw (word);
	if (! word)
		return;
	
	ptrn = CNEW (char, strlen (word)+3);
	sprintf (ptrn, "\n%s\n", word);
	S = (char *) Dmemmem (&t_map[i+2], t_size-i-2, ptrn, strlen (ptrn));
	assert (! S, "fly_skip fail !");
	if (! S) {
		i = t_size;
		goto out;
	}

	nlines += Dsyms (&t_map[i+2], S, '\n');
	nlines+=2;
	i += S - &t_map[i] + strlen (ptrn) + 1;
	
out:
	DROP (ptrn);
	DROP (word);
}

void doc_skip ()
{
	char *S;
	char * ptrn = (char *) "\n=cut\n";

	S = (char *) Dmemmem (&t_map[i], t_size-i, ptrn, 6);
	if (! S) {
		i = t_size;
		return;
	}

	nlines += Dsyms (&t_map[i], S, '\n');
	++nlines;
	i += S - &t_map[i] + 4; 
}

void regexp_skip ()
{
	char ss;
	int count;
	char * ptr;
	int old;
	
	count = 1;
	old = i;
	switch (T) {
		case 't':
			i+=2;
			break;
		case 's':
			++i;
			count = 2;
			break;
		case 'm':
			++i;
			break;
	}	

	ss = T;
	if (ss == '{')
		ss = '}';
	++i;
	ptr = &t_map[i];
	while (true) {
		for (; i < t_size; ++i) {
			if (T == '\n') 
				break;

			if (t_map[i] == ss && !(ww_nsyms_r (ptr, &t_map[i-1], '\\') % 2))
				break;
		}
		
		if (T == '\n')
			break;

		assert (i >= t_size, "Sibadi 2006.2");
		if (i >= t_size) 
			break;
		
		if (--count == 0)
			break;
		++i;
	}

	if (T == '\n') {
		i = old;
		TN(T);
	}
}

bool regexp_test ()
{
	char * ptr;
	char ch = T;
	char ss;

	if (isalnum (pch) || pch == '_' || pch == ')')
		return false;

	if (! (ch == '/' || ch == 's' || ch == 'm' || !strncmp (&t_map[i], "tr", 2))) 
		return false;
	
	ptr = &t_map[i];
	++ptr;
	switch (ch) {
		case 't':
			++ptr;
		case 's':
		case 'm':
			ss = *ptr;
			if (! (ss == '/' || ss == '?' || ss == '#'
				|| ss == '!' || ss == '&' || ss == '|' || ss == '{'))
				return false;
			break;
	}

	return true;
}

inline void tt_skip ()
{
	while (T != '\n' && i < t_size)
		++i;
	--i;
}

int fly_for_buffers (char * d_input, int size, char * d_output)
{
	char ch;
	char * S;

	i = 0;
	ii = 0;
	t_size = size;
	t_map = d_input;
	t_new = d_output;
	spaces = 0;
	nlines = 0;
	pch = 'a';
	t_op_no = 0;

	i = 0;
	if (T == '#' || T == '=') {
		tt_skip ();	
		++i;
	}

	for (; i < size; i++) {
		ch = T;
		if (ch == '\n')
			++nlines;

		if (regexp_test ()) {
			regexp_skip ();
			continue;
		}

		if (if_digit (&t_map[i]) || if_abc (&t_map[i])) {
			spaces = 0;
			goto abc;
		}

		if ((ch == '\'' || ch == '\"') && t_map[i-1] != '$' && t_map[i-1] != '\\') {
			if (ch == '\'' && (isalnum (t_map[i-1]) || t_map[i-1] == '&'))
				goto abc;
			
			S = sstrend (&t_map[i]);
			assert (S == NULL, "\n\nSibadi 2006\n\n");
			nlines += Dsyms (&t_map[i], S, '\n'); 
			i += S - &t_map[i];
			pch = ch;
			TN (ch);
			TN (ch);
			continue;
		}

		if (ch == '\n' || ch == ' ' || ch == '\t') {
			if (spaces == 0) {
				TN (' ');
				spaces++;
				continue;
			} else 
				continue;
		}
	
		if (ch == '<' && t_map[i+1] == '<') {
			fly_skip ();
			if (i >= t_size) {
				TN ('\0');
				break;
			}
			TN (';');
			TN (' ');
			spaces = 0;
			pch = ch;
			continue;
		}
		
		if (ch == ';' || ch == '}' || ch == ')' || 
				ch == '{' || ch == '(') {
			if (! spaces)
				TN (' ');
			TN (ch);
			TN (' ');
			spaces = 1;
			pch = ch;
			continue;
		}

		if (ch == '=' && t_map[i-1] == '\n') {
			doc_skip ();
			pch = ch;
			continue;
		}

		if (ch == '#' && t_map[i-1] != '$') {
			tt_skip ();
			pch = ch;
			continue;
		}

abc:
		pch = ch;
		TN(T);
	}
	
	TN('\0');
	return ii;
}

char * fly_for_file (struct tt_state_t * tt)
{
	int m_size;

	if (! tt)
		return NULL;

	if (access (tt->fileName, R_OK) != F_OK) {
		fprintf (stderr, "No such file %s\n", tt->fileName);
		return NULL;
	}

	tt_map (tt);
	
	m_size = fsize (tt->fileName);
	tt->result = CNEW (char, m_size << 2);
	tt->result[0] = 0;
	d_attachment = (pair_t *) CNEW (char, m_size << 2);
	tt->attachment = d_attachment;
	tt->resultSize = fly_for_buffers (tt->fileData, tt->fileDataSize, tt->result);

	return tt->result;
}

} // namespace THE_FLY

