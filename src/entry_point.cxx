/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#include <vector>
#include <head.h>
#include <cmds.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

bool supported ()
{
	switch (get_env()->sb_cmd) {
		case cmd_call_tags:
		case cmd_cgrep:
			if ((language_id () != LCXX) && (language_id () != LALL)) {
				perror ("Available for \"C/C++\" only\n");
				return false;
			}
	}

	return true;
}

FILE * prepare_of ()
{
	char m_buf[512];

	switch (get_env()->sb_cmd) {
		case cmd_makectags:
		case cmd_call_tags:
			if (!SB_FLGET(SB_FLTHREAD))
				return fopen (get_env()->tmp_tags, "w");
			else {
				sprintf (m_buf, "%s%i", get_env()->tmp_tags, get_env()->nproc);
				return fopen (m_buf, "w");
			}
			break;
		case cmd_grep:
		case cmd_cgrep:
			return stdout;
	}
	return stdout;
}

bool ccheck(char * f)
{
	char * ext;
	if (! f)
		return false;
	ext = fext (f);
	if (! ext)
		return false;
	++ext;

	if (!ext_check_c(ext) && !ext_check_cxx(ext))
		return false;

	return true;
}

void process_cmd (char * f, FILE * of)
{
	switch (get_env()->sb_cmd) {
		case cmd_grep:
			grepFile (f, of);
			break;
		case cmd_cgrep:
			if (! ccheck(f))
				break;
			cgrep::cgrep (f, of);
			break;
		case cmd_makectags:
			ml_ctags (f, of);
			break;
		case cmd_call_tags:
			if (! ccheck(f))
				break;
			call_tags_file (f, of);
			break;
	}
}

int foreach_file ()
{
	FILE * of;
	int i;
	char * n;

	of = prepare_of ();
	for (i = 0; i < get_env()->d_files->get_size (); ++i) {
		n = get_env()->d_files->get (i);
		get_env()->file_name = strdup (n);
		process_cmd (n, of);
		DROP(n);
	}
	if (of != stdout)
		fclose (of);

	return 0;
}

EArray * split_files ()
{
	EArray * ret;
	int count;
	int i;
	int k;

	ret = new EArray(16);
	for (i = 0; i < get_env()->max_proc; ++i)
		(*ret).add (LPCHAR(new EArray(16)));

	k = 0;
	count = get_env()->d_files->get_size ();	
	for (i = 0; i < count; ++i) {
		((EArray *) ret->get(k))->add (get_env()->d_files->get (i));
		if (++k >= get_env()->max_proc)
			k = 0;
	}

	return ret;
}

int child (EArray * files)
{
	if (! files)
		return -1;
	get_env()->d_files = files;
	foreach_file ();
	return 0;
}

int processes_done (std::vector<pid_t> * pids)
{
	unsigned int i;
	int status;

	if (! pids)
		return -1;
	for (i = 0; i < pids->size (); ++i) 
		waitpid (pids->at (i), &status, 0);
	delete pids;
	return 0;
}

int processes ()
{
//	EArray * pids;
	std::vector<pid_t> * pids;
	EArray * spl;
	int i;
	int pid;

	spl = split_files ();
	pids = new std::vector<pid_t>;
	for (i = 0; i < get_env()->max_proc; ++i) {
		pid = fork ();
		if (pid < 0) { // error
			perror ("fork");
			exit (1);
		} else if (pid == 0) { // child
			get_env()->nproc = i;
			SB_FLSET (SB_FLTHREAD);
			child ((EArray *) spl->get (i));
			exit (0);
		} else {
			pids->push_back (pid);
		}
	}

	processes_done (pids);
	return 0;
}

int entry_point_done ()
{
	if (get_env()->max_proc != 1) 
		join_tmp_tags ();

	if (get_env()->sb_cmd == cmd_makectags) 		
		mk_tags ((char *)"./tags", NULL);
	else if (get_env()->sb_cmd == cmd_call_tags)
		mk_tags ((char *)"./call_tags", NULL);
	
	return 0;
}

int entry_point ()
{
	if (! supported ())
		return -1;

	if (get_env()->d_files->get_size () == 0) {
		delete get_env()->d_files;
		get_env()->d_files = searchSources ();
	}

	if (get_env()->max_proc == 1) 
		foreach_file ();	
	else 
		processes ();

	entry_point_done ();
	remove_tmp_files ();
	return 0;
}

