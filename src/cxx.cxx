/*
 * (c) Oleg Puchinin 2009.
 * graycardinalster@gmail.com
 *
 */

#include <head.h>
#include <TT.h>
#include <wit.h>

char * macro_name (char * d_op, char * d_macro_name)
{
	char *d_begin;
	char *S;
	char m_buf[256];

	strncpy (m_buf, d_op, 255);
	m_buf[255] = 0;
	
	S = strstr (m_buf, "define");
	
	if (! S)
		return NULL;
	
	S = strchr (S, ' ');
	if (! S)
		return NULL;
	
	while (*S == ' ')
		S++;

	d_begin = S;
	S = strchr (d_begin, ' ');
	if (S)
		*S = 0;

	S = strchr (d_begin, '(');
	if (S)
		*S = 0;
	
	strcpy (d_macro_name, d_begin);
	return d_macro_name;
}

char * mk_tag_macro (char * d_op, char * d_file, int t_line)
{
	char S[256];	
	if (! macro_name (d_op, S))
		return NULL;
	return strdup (S);	
}

void mk_tag (char * d_op, char * d_file, int line, int d_found_type, FILE * of)
{
	char * S;
	const char * d_format = "%s\t%s\t%i\t; %c\n";
	char ch;
	
	if (d_found_type & OT::Define) {
		S = mk_tag_macro (d_op, d_file, line);
		if (S) {
			d_format = (char *) "%s\t%s\t%d\t; d\n";
			fprintf (of, d_format, S, d_file, line);
			free (S);
		}
		return;
	}

	if (d_found_type & OT::Class) {
		S = strchr (d_op, ':');
		if (S) 
			*S = 0;		
	}
	
	S = c_last_word (d_op);
	if (! S || !(*S) || (*S == ' '))
		return;

	if (*S == 's') {
		if (! strncmp (S, "static", 6))
			return;
		if (! strncmp (S, "struct", 6))
			return;
	}
	
	if (*S == 'u' && !strncmp (S, "union", 5))
		return;
	if (*S == 'e' && !strncmp (S, "enum", 4))
		return;	
		
	if (d_found_type & OT::Function)
		ch = 'f';
	else if (d_found_type & OT::Class)
		ch = 'c';
	else if (d_found_type & OT::Struct)
		ch = 's';
	else if (d_found_type & OT::Namespace)
		ch = 'n';
	else if (d_found_type & OT::Typedef)
		ch = 't';
	else
		return;

	fprintf (of, d_format, S, d_file, line, ch);
	S = strstr (S, "::");
	if (S) {
		S += 2;
		fprintf (of, d_format, S, d_file, line, ch);
	}
}

void cxx_print (tt_state_t * tt, char * d_out, int d_found_type, FILE * of)
{
	int t_line;

	if (SB_FLGET (SB_FLSIMULATE))
		return;
	
	if (d_found_type & OT::Other)
		return;

	t_line = tt->attachment[get_env()->t_op_no].pair_line+1;
	t_line += c_begin_line (tt, d_out, 
			tt->attachment[get_env()->t_op_no].offset);

	mk_tag (d_out, tt->fileName, t_line, d_found_type, of); 
}

int cxx_ctags (char * f_name, FILE * of) 
{
	TT * tt;
	int m_type;
	int type;
	bool c = false;
	char * S;

	if (! f_name)
		return -1;

	S = fext (f_name);
	if (! S)
		return -1;
	++S;
	if (ext_check_c (S))
		c = true;

	tt = new TT;
	if (tt->loadFile (f_name) < 0) {
		delete tt;
		return -1;
	}

	if (tt->init () < 0) {
		delete tt;
		return -1;
	}

	type = 0xFFFF &
		~OT::Extern & 
		~OT::Other & 
		~OT::Call & 
		~OT::Macro;

	while (true) {
		tt->nextOperator ();
		if (tt->ch == 0)
			break;

		if (c && tt->bracketDepth)
			continue;
		
		m_type = tt->wit ();
		if (m_type & type)
			cxx_print (tt->tt, tt->op (), m_type, of);
	}

	delete tt;
	return 0;
}

