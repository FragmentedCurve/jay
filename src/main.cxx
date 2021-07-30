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

static const char * pgmName;

static void usage ()
{
	printf ("Usage: %s [<function>|<files>] [<options>]\n", pgmName);
	printf ("Options:\n"
			"   -C <dir>        -  chdir to <dir>\n"
			"  --cgrep          -  search in code\n"
			"  --grep           -  search text\n"
			"  --replace        -  replace text\n"
			"  --call-tags      -  create \"call_tags\" file\n"
			"   -f              -  create \"files\" file\n"
			"  --ctags-append   -  don't rewrite \"tags\" file\n"
			"  --depth <arg>    -  set reentrant level\n"
			"  --file <files>   -  show file functions\n"
			"   -fn             -  echo filenames\n"
			"  --structs        -  show all structs in stream\n"
			"   -i              -  interactive mode \"tags\"\n"
			"  --indent         -  indention\n"
			"   -j              -  number of jobs\n"
			"  --kinds          -  (c - classes, s - structs,    f - functions,\n"
			"                       m - modules, n - namespaces, p - packages)\n"
			"   -L <file>       -  read files list from file\n"
			"   -c              -  create \"tags\" file\n"
			"  --tags           -  access to tags\n"
			"  --the-tt         -  C preprocessor\n"
			"   -ts             -  force ctags style\n"
			"   -u              -  reverse call-tree\n");
}

static void dumpOptions ()
{
	char * S;
	get_env()->listOptions->first ();
	while ((S = get_env()->listOptions->get ())) {
		printf ("%s ", S);
		get_env()->listOptions->next ();
	}
	printf ("\n");
}

static int bob_options (DArray * d_opts, int & i)
{
	char * opt;
	int argc;

	if (! d_opts)
		return -1;

	opt = d_opts->get (i);
	argc = d_opts->get_size ();

	if (opt[0] != '-') {
		get_env()->d_files->add (strdup (opt));
		return 0;
	}
	
	if (EQ (opt, "--help") || EQ (opt, "-h")) {
		usage ();
		exit (0);
	}
	
	if (EQ (opt, "--file")) {
		get_env()->sb_cmd = cmd_file;
		return 0;
	}

	if (EQ (opt, "--the-tt")) {
		get_env()->sb_cmd = cmd_the_tt;
		return 0;
	}
	
	if (EQ (opt, "--structs")) {
		get_env()->sb_cmd = cmd_give_structs;
		return 0;
	}
	
	if (EQ (opt, "--indent")) {
		get_env()->sb_cmd = cmd_indent;
		return 0;
	}
	
	if (EQ (opt, "--tags")) {
		get_env()->sb_cmd = cmd_tags;
		return 0;
	}

	if (EQ (opt, "--make-ctags") ||
			EQ (opt, "-c")) {
		get_env()->sb_cmd = cmd_makectags;
		return 0;	
	}
	
	if (EQ (opt, "--call-tags") ||
			EQ (opt, "-ct")) {
		SB_FLSET (SB_FLTAGSTYLE); 
		get_env()->sb_cmd = cmd_call_tags;
		return 0;
	}
	
	if (EQ (opt, "--cgrep")) {
		if (++i >= d_opts->get_size ())
			return 0;
		
		get_env()->sb_cmd = cmd_cgrep;
		get_env()->cgrep_exp = d_opts->get (i);
		return 0;
	}

	if (EQ (opt, "--grep")) {
		if (++i >= d_opts->get_size ())
			return 0;
		
		get_env()->sb_cmd = cmd_grep;
		get_env()->grep_exp = d_opts->get (i);
		return 0;
	}

	if (EQ (opt, "--replace")) {
		if (++i >= d_opts->get_size ())
			return 0;
		
		get_env()->sb_cmd = cmd_replace;
		get_env()->replace_exp = d_opts->get (i);
		return 0;
	}
	
	if (EQ (opt, "--tags")) {
		get_env()->sb_cmd = cmd_tags;
		return 0;
	}

	if (EQ (opt, "-i")) {
		tags_interactive ();
		exit (0);
	}
	
	if (EQ (opt, "--time-test")) 
		Dtimer ();

	if (EQ (opt, "--options")) {
		dumpOptions ();
		exit (0);
	}
	
	if (EQ (opt, "--verbose")) {
		SB_FLSET (SB_FLVERBOSE);
		return 0;
	}
	
	if (EQ (opt, "-u")) {
		SB_FLSET (SB_FLRTREE);			
		return 0;
	}
	
	if (EQ (opt, "--linear") ||
			EQ (opt, "-l")) {
		SB_FLSET (SB_FLLINEAR);
		return 0;
	}
	
	if (EQ (opt, "-C") && i < argc) {
		++i;
		return chdir (d_opts->get (i));
	}
	
	if (EQ (opt, "--linux")) {
		return chdir ("/usr/src/linux");
	}
		
	if (EQ (opt, "--debug")) {
		SB_FLSET (SB_FLDEBUG);
		return 0;
	}
	
	if (EQ (opt, "--simulate")) {
		Dtimer ();
		SB_FLSET (SB_FLSIMULATE);
		return 0;
	}
		
	if (EQ (opt, "--no-links"))	{
		SB_FLSET (SB_FLNOLINKS);
		return 0;
	}

	if (EQ (opt, "-a") || EQ (opt, "--all")) {
		SB_FLSET (SB_FLALL);
		return 0;
	}
	
	if (EQ (opt, "--test") || EQ (opt, "-t")) {
		SB_FLSET (SB_FLTEST);
		return 0;
	}

	if (EQ (opt, "--ctags-append")) {
		SB_FLSET (SB_FLCTAGSAPPEND);
		return 0;
	}
	
	if (EQ (opt, "-A")) {
		if (++i >= argc)
			return 0;
		
		get_env()->cgrep_A = atoi (d_opts->get (i));
		return 0;
	}

	if (EQ (opt, "-B")) {
		if (++i >= argc) 
			return 0;
		
		get_env()->cgrep_B = atoi (d_opts->get (i));
		return 0;
	}

	if (EQ (opt, "-j")) {
		if (++i >= argc)
			return 0;
		
		get_env()->max_proc = atoi (d_opts->get (i));
		return 0;
	}
	
	if (EQ (opt, "--tag-style") || EQ (opt, "-ts")) {
		SB_FLSET (SB_FLTAGSTYLE); 
		return 0;
	}
	
	if (EQ (opt, "-L") && ((i+1) < argc))	{
		++i;
		get_env()->d_files->from_file (d_opts->get (i));
		get_env()->d_files->foreach ((Dfunc_t) chomp);
		return 0;
	}
	
	if (EQ (opt, "--depth")) {
		if (++i >= argc) 
			return 0;
		
		get_env()->d_depth = atoi (d_opts->get (i));
		return 0;
	}
		
	if (EQ (opt, "-fn")) {
		SB_FLSET (SB_FLFNAMES);
		return 0;
	}

	if (EQ (opt, "--cxx")) {
		get_env()->language = strdup ("C++");
		return 0;
	}

	if (EQ (opt, "--perl")) {
		get_env()->language = strdup ("Perl");
		return 0;
	}

	if (EQ (opt, "--python")) {
		get_env()->language = strdup ("Python");
		return 0;
	}

	if (EQ (opt, "--ruby")) {
		get_env()->language = strdup ("Ruby");
		return 0;
	}

	if (EQ (opt, "--php")) {
		get_env()->language = strdup ("PHP");
		return 0;
	}

	if (EQ (opt, "--java")) {
		get_env()->language = strdup ("Java");
		return 0;
	}

	if (EQ (opt, "--go")) {
		get_env()->language = strdup ("Go");
		return 0;
	}

	if (EQ (opt, "--lua")) {
		get_env()->language = strdup ("Lua");
		return 0;
	}

	if (EQ (opt, "-f")) {
		get_env()->sb_cmd = cmd_files;
		return 0;
	}

	if (EQ (opt, "--cache-load")) {
		cache_load ();
		exit (0);
	}

	if (EQ (opt, "--cache-free")) {
		cache_free ();
		exit (0);
	}

	if (editor_option (opt)) {
		try_editor ();
		return 0;
	}

	if (EQ (opt, "--kinds")) {
		if (++i >= argc) 
			return 0;		
		get_env()->sb_cmd = cmd_kinds;
		get_env()->d_kinds = d_opts->get (i);
		return 0;
	}

	if (EQ (opt, "--thread")) {
		SB_FLSET(SB_FLTHREAD);
		return 0;
	}

	return -1;
}


/* MAIN */
int main (int argc, char ** argv) 
{
	char * opt;
	DArray * d_opts;
	int i;

	pgmName = argv[0];
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
	} 

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

