
/* 
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#include <head.h>
#include <dbg.h>
#include <wit.h>

void terminal_dump_function (struct fdump_param_t * d_param)
{
	int d_count = 0;
	DArray d_array;
	FILE * d_file;
	int d_size;
	char * S;
	int i,a;

	if (!d_array.from_file (d_param->d_file_name))
		return;

	if (d_param->d_file_output == NULL)
		d_file = stdout;
	else
		d_file = fopen (d_param->d_file_output, "w");
	
	if (! d_file)
		return;
	
	if (! d_param->linear) {
		for (a = 0; a	< d_param->n_trip; a++)
			fprintf (d_file, "\t");
		fprintf (d_file, "//<***>\n");
	}
	
	i = d_param->d_line-1;
	d_size = d_array.get_size ();
	
	if (d_array.get (i)[0] != '#') {
		while (i < d_size) {
			if (!d_param->linear) {
				for (a = 0; a	< d_param->n_trip; a++)
					fprintf (d_file, "\t");
			}
			fprintf (d_file, "%s", d_array.get(i));
				
			if (brace_count (d_array.get(i), &d_count, d_param->b_force_block) && !d_count)
				break;
			
			i++;
		}
	} else {
		do {
			S = d_array.get (i);
			fprintf (d_file, "%s", S);
			S = &S[strlen (S)-2];
			while ((*S == ' ') || (*S == '\t'))
				S--;
			
			if (*S != '\\')
				break;
			i++;
		} while (i < d_size);
	}
	
	if (!d_param->linear) {
		for (a = 0; a	< d_param->n_trip; a++)
			fprintf (d_file, "\t");
		fprintf (d_file, "//</***>\n");
	}

	if (d_param->d_file_output != NULL)
		fclose (d_file);

	d_array.foreach (free); 

}

/* code for "linear" functionality, */
void terminal_dump_tags (char * f_name, int n_trip) {
	DArray * d_tags;			
	d_tag_t * d_tag;
	struct fdump_param_t d_param;
	int a,i;
	
	d_tags = got_tag (f_name);
	
	assert (d_tags->get_size () == 0, "HimTeh 1");
	for (i = 0; i < d_tags->get_size (); i++) {
		d_tag = (d_tag_t *) d_tags->get (i);
		if (i != 0)
			fputc ('\n', stdout);

		if (!SB_FLGET(SB_FLLINEAR)) {
			for (a = 0; a < n_trip; a++)
				fputc ('\t', stdout);
		}

		printf ("// file %s line %i\n", d_tag->d_file, d_tag->d_line);
		
		memset (&d_param, 0, sizeof (struct fdump_param_t));
		d_param.n_trip = n_trip;
		d_param.d_file_name = d_tag->d_file;
		d_param.d_line = d_tag->d_line;
		d_param.linear = SB_FLGET (SB_FLLINEAR);
		if (d_tag->d_type & OT::Function)
			d_param.b_force_block = true;
		terminal_dump_function (&d_param);
	}
	
	d_tags->foreach (free);
	delete d_tags;
	printf ("\n");
}
