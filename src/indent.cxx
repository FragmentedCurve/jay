/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>
#include <dbg.h>
#include <the_tt.h>

// \033[01;32m
extern FILE * d_stream_dbg;

const char * d_lpsz_operators [] = { "if", "else", "for", "while", "do", "switch", "#define",
	"#include", "#else", "#if", "#elif","typedef", "struct", "class", 
	"case", "return", NULL };

const char * d_lpsz_oneopops [] = {"if", "else", "for", "while", "do", NULL};
const char * d_lpsz_mustlined_ops [] =  {"if", "for", "while", "do", NULL};
const char * d_lpsz_greenpeace [] = {"struct", 
	"class", 
	"extern", 
	"typedef",
	"int",
	"char",
	"long",
	"double",
	"short",
	NULL
};

// $ silent_bob --indent
void nogui_indent () // "UN-TT" ;)
{
	struct tt_state_t * d_tt_state;
	bool b_inmacro = false;
	char * d_out, *d_ptr;
	int brace_depth = 0;
	bool t_lined = true;
	EArray d_must_lined;
	EArray d_operators;
	EArray d_array;
	char ch;
	int i;

	d_tt_state = CNEW (tt_state_t, 1);
	d_tt_state->fileName = strdup ("-");
	d_ptr = THE_TT::do_tt_file (d_tt_state);
	d_out = d_ptr;

	d_array.push ((char **) d_lpsz_oneopops);
	d_must_lined.push ((char **) d_lpsz_mustlined_ops);
	d_operators.push ((char **) d_lpsz_operators);
	
	while (true) {
		ch = t_op (&d_ptr, &d_out); // t_op on top
		
		if (! ch)
			break;

		if (*d_out == '#')
			b_inmacro = true;
		
		if (ch == '\n' && d_ptr[-1] != '\\') {
			printf ("%s\n", d_out);
			b_inmacro = false;
			continue;
		}
		
		if (ch == '}')
			brace_depth--;

		if (d_must_lined.snfind (d_out) != -1 && !t_lined && !b_inmacro) {
			fputc ('\n', stdout);
			t_lined = true;
		}

		if (ch != '\n' && !b_inmacro) {
			for (i = 0; i < brace_depth; i++)
				printf ("  ");
		}
		
		if (ch == '{')
			brace_depth++;

		printf ("%s", d_out);

		fputc (ch, stdout);
		t_lined = false;
		
		if (b_inmacro && ch == '}')
			fputc (' ', stdout);
		
		if (!b_inmacro) {
			if (ch == '}' && brace_depth == 0) {
				if (*d_ptr == '#' || *d_ptr == '\n')
					fputc ('\n', stdout);
				else if (*d_ptr && *d_ptr != ';') {
					fputc ('\n', stdout);
					fputc ('\n', stdout);
				}
				else if (*d_ptr && *d_ptr == ';') {
					d_ptr+=2;
					printf (";\n\n");
				}
			} else {
				if (ch != '\n')	{
					fputc ('\n', stdout);
				}
				if (ch == '{')
					t_lined = true;
			} 
		}
	}	

	free_tt_state (d_tt_state);
	dbg_out;
}

