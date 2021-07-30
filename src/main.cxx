/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#include <head.h>
#include <wit.h>
#include <the_tt.h>
#include <mod.h>
#include <cmds.h>

extern "C" struct env_t *ENV;

/* MAIN */
int main (int argc, char ** argv) 
{
	char * opt;
	DArray * d_opts;
	int i;

	sb_init ();

	ENV->sb_cmd = sb_prname (argv[0]);		
	if (argc == 1 && !ENV->sb_cmd) {
		usage ();
		exit (0);
	}

	if (! ENV->sb_cmd)
		ENV->sb_cmd = cmd_tree;

	ENV->d_files = new EArray (128);
	d_opts = new EArray (argc);
	ENV->d_opts = d_opts;
	for (i = 0; i < argc; i++) 
		d_opts->add (argv[i]);
	
	for (i = 1; i < d_opts->get_size (); i++) {
		opt = d_opts->get (i);

		if (opt[0] != '-') {
			ENV->d_files->add (strdup (d_opts->get (i)));
			continue;
		}
		
		if (! bob_options (d_opts, i))
			continue;
		
		if (d_opts->get (i)[0] == '-' && !validOption (d_opts->get(i))) {
			fprintf (stderr, "unknown option : %s\n", d_opts->get (i));
			exit (1);
		}

		//modding_optparse (&i, 1);
	} // for (i = 1; i < argc; i++)

/*	for (i = 0; i < argc; i++) 
		modding_optparse (&i, 2);

	if (! builtin_language ()) {
		i = modding_start (ENV->sb_cmd);
		exit (1);
	}*/

	bob_commands ();
	bob_commands2 ();
	bob_commands_nofiles ();

	if (ENV->immune_list)
		delete ENV->immune_list;
	
	ENV->d_files->foreach (free);
	delete ENV->d_files;
	fflush (stdout);

	return EXIT_SUCCESS;
}

