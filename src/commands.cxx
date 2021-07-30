/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#include <head.h>
#include <the_tt.h>
#include <cmds.h>

void bob_commands ()
{
	switch (get_env()->sb_cmd) {
		case cmd_cgrep:
			cgrep::init ();
		case cmd_grep:
		case cmd_makectags:
		case cmd_call_tags:
			entry_point ();
			exit (0);

		case cmd_tree:
			if (!SB_FLGET (SB_FLRTREE))
				call_tree (get_env()->d_files->get (0), 0);
			else
				reverse_calltree (get_env()->d_files->get (0));
			exit (0);
		
		case cmd_indent:
			nogui_indent ();
			exit (0);		
		case cmd_replace:
			replace ();
			exit (0);
		case cmd_tags:
			tags (get_env()->d_files, NULL);
			exit (0);		
		case cmd_files:
			files ();
			exit (0);
		case cmd_kinds:
			bob_kinds ();
			exit (0);
	}
}

void bob_commands2 ()
{
	int i;
	int n_files;

	n_files = get_env()->d_files->get_size ();
	for (i = 0; i < n_files; i++) {
		switch (get_env()->sb_cmd) {
			case cmd_file:
				flist_main (get_env()->d_files->get (i));
				break;
			case cmd_the_tt:
				THE_TT::the_tt_main (get_env()->d_files->get(i));	
				break;
			case cmd_give_structs:
				get_structs (get_env()->d_files->get (i));	
				break;
		}
	}
}

void bob_commands_nofiles ()
{
	char d_buf[1024];
	
	if (get_env()->d_files->get_size ())
		return;

	if (get_env()->sb_cmd == cmd_the_tt) // THE_TT for stdin
		THE_TT::the_tt_main ((char *) "-");

	else if (get_env()->sb_cmd == cmd_give_structs) {
		get_structs ((char *) "-");
		exit (0);

	} else if (get_env()->sb_cmd == cmd_file) {

		while (fgets (d_buf, 1023, stdin)) {
			chomp (d_buf);
			switch (get_env()->sb_cmd) {
				case cmd_file:
					flist_main (d_buf);
					break;
			}

		}

	}
	
}


