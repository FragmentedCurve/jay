/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>
#include <mod.h>
#include <temp_names.h>

#define SB_IMMUNE_FILE ".silent_bob/silent_bob_immune"
const char * c_ops [] = { "if", "else", "do", "while",
	"switch", "case", "for", "return", NULL };

env_t *ENV;

env_t * get_env () 
{
	return ENV;
}

int editors_init ();

void mk_dirs ()
{
	char m_buf[4096];
	sprintf (m_buf, "%s/.silent_bob", get_env()->home_dir);
	mkdir (m_buf, 0700);
	sprintf (m_buf, "%s/.silent_bob/plugins", get_env()->home_dir);
	mkdir (m_buf, 0700);
	get_env()->home_plugins = strdup (m_buf);
	get_env()->shared_plugins = strdup ("/usr/lib/silent_bob/");
}

int sb_init ()
{
	char * sb_immune_path = CNEW (char, 256);
	char * stdout_vbuf;

	stdout_vbuf = CNEW (char, 4096 * 100);
	setvbuf (stdout, stdout_vbuf, _IOFBF, 4096*100);

	ENV = CNEW(env_t, 1);
	memset (ENV, 0, sizeof (env_t));
	
	ENV->d_cops = new EArray(10);
	ENV->d_cops->push ((char **) c_ops);
	ENV->d_depth = 6;
	ENV->sb_flags = 0;

	ENV->d_tags_file = NULL;
	ENV->immune_list = NULL;
	ENV->cgrep_A = 0;
	ENV->cgrep_B = 0;

	ENV->home_dir = getenv ("HOME");
	ENV->tmp_files = CNEW (char, strlen (ENV->home_dir) + strlen (TMPNAME_FILES) + 20);
	sprintf (ENV->tmp_files, "%s/.silent_bob/%s", ENV->home_dir, TMPNAME_FILES);
	ENV->tmp_tags = CNEW (char, strlen (ENV->home_dir) + strlen (TMPNAME_TAGS) + 20);
	sprintf (ENV->tmp_tags, "%s/.silent_bob/%s", ENV->home_dir, TMPNAME_TAGS);
	ENV->debug_file =  (char *) "/tmp/silent_bob.log";

	mk_dirs ();
	sprintf (sb_immune_path, "%s/.silent_bob/%s", getenv ("HOME"), SB_IMMUNE_FILE);

	ENV->immune_list = new EArray;
	ENV->immune_list->from_file (sb_immune_path);
	ENV->immune_list->foreach ((Dfunc_t) chomp);
	ENV->full_list = new EArray;
	
	ENV->d_stream_dbg = fopen ("/dev/null", "w");	
	ENV->max_proc = 1;
	ENV->nproc = 1;
	ENV->proc_list = new DJobs;
	ENV->listOptions = new EList;
	ENV->language = strdup ("ALL");

	return 0;
}

