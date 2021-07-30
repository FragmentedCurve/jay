
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>
#include <the_tt.h>

// $ silent_bob --structs
void get_structs (char * d_file)
{
	struct tt_state_t * d_tt_state = CNEW (tt_state_t, 1);
	char *d_ptr, *d_out;
	EArray d_operators;
	int brace_depth;
	bool b_typedef;
	bool b_struct;
	char ch;
	int i;
	
	memset (d_tt_state, 0, sizeof (struct tt_state_t));
	d_tt_state->fileName = strdup (d_file);
	d_ptr = do_tt (d_tt_state);
	
	while (true) {
		ch = t_op (&d_ptr, &d_out);
		if (ch == 0)
			break;
	
		b_typedef = false;
		b_struct = false;
		if (!strncmp (d_out, "typedef", 7))
			b_typedef = true;
		else if (!strncmp (d_out, "struct", 6))
			b_struct = true;

		if (! b_typedef && ! b_struct)
			continue;
		
		if (strchr (d_out, '('))
			continue;

		if (ch == ';' && b_typedef && !SB_FLGET (SB_FLNOLINKS)) // Link... 
			printf ("%s;\n", d_out); 

		if (ch != '{') 
			continue;

		brace_depth = 1;
		printf ("%s{\n", d_out);
		
		while (brace_depth > 0) {
			ch = t_op (&d_ptr, &d_out);
			if (ch == '\n')	{
				if (*d_out == '#')
					printf ("%s\n", d_out);
				continue;
			}

			if (ch == '}')
				--brace_depth;
			
			for (i = 0; i < brace_depth; i++)
				fputc ('\t', stdout);

			if (ch == '{')
				++brace_depth;
			
			if (ch != '}' && ch != '{' && strlen (d_out)) 
				printf ("%s;\n", d_out);
			else if (brace_depth) {
				printf ("%s", d_out);
				fputc (ch, stdout);
				if (ch == '}') {
					t_op (&d_ptr, &d_out);
					if (strlen (d_out))
						printf (" %s;", d_out);
					else
						fputc (';', stdout);
					printf ("\n");
				}
				printf ("\n");
			}
		} // while (brace_deph > 0)
		ch = t_op (&d_ptr, &d_out);
		if (strlen (d_out))
			printf ("} %s;\n\n", d_out);
		else
			printf ("};\n\n");
	} // if (!strncmp (d_out, "typedef", 7))
	free_tt_state (d_tt_state);
}

