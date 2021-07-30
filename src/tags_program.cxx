
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>
#include <env.h>
#include <head.h>
#include <wit.h>
#include <dbg.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <the_tt.h>
#include <cmds.h>

// SilentBob --tags
void tags (DArray * d_names, char * d_file_output)
{
	struct fdump_param_t d_param;
	EArray * d_tags;			
	struct d_tag_t * d_tag;
	int a, i, n_names;
	
	if (! d_names)
		return;
	
	n_names = d_names->get_size ();
	
	for (i = 0; i < n_names; i++) {
		d_tags = got_tag (d_names->get (i));
		if (d_tags == NULL) {
			if (d_tags->get_size () == 0) {
				fprintf (get_env()->d_stream_dbg, "Tag not found : %s\n",
					       	d_names->get (i));
				fflush (get_env()->d_stream_dbg);
				delete d_tags;
				continue;
			}
		}
		
		if (! d_tags)
			continue;

		if(! d_tags->get_size ()) {
			delete d_tags;
			d_tags = NULL;
			continue;
		}
		
		fault (! d_tags);

		for (a = 0; a < d_tags->get_size (); a++) {
			d_tag = (d_tag_t *) d_tags->get (a);

			fault (! d_tag);
			
			if (! d_file_output)
				printf ("// file %s line %i\n",
						d_tag->d_file, d_tag->d_line);
			
			memset (&d_param, 0, sizeof (struct fdump_param_t));
			d_param.n_trip = 0;
			d_param.d_file_name = d_tag->d_file;
			d_param.d_line = d_tag->d_line;
			d_param.linear = true;
			d_param.d_file_output = d_file_output;
			if (d_tag->d_type == OT::Function)
				d_param.b_force_block = true;
			
			terminal_dump_function (&d_param);
			if (! d_file_output)
				fputc ('\n', stdout);
	
			DROP (d_tag);				
		}
		
		if (d_tags) {
			d_tags->drop ();
			delete d_tags;
		}
	}
	
	fflush (stdout);
}

