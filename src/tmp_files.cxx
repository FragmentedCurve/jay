
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>
#include <env.h>
#include <errno.h>

int split_tmp_files ()
{
	FILE * f_tmpfiles;
	FILE ** proc_files;
	char m_buf[512];
	int i = 0;

	f_tmpfiles = fopen (get_env()->tmp_files, "r");
	if (! f_tmpfiles)
		return -1;
	
	proc_files = CNEW (FILE *, get_env()->max_proc);
	memset (proc_files, 0, sizeof (FILE *) * get_env()->max_proc);
	errno = 0;
	for (i = 0; i < get_env()->max_proc; ++i) {
		sprintf (m_buf, "%s%i", get_env()->tmp_files, i);
		unlink (m_buf);
		proc_files[i] = fopen (m_buf, "w");
		if (! proc_files[i]) {
			perror ("fopen");
			return -1;
		}
	}	
	
	i = 0;
	while (fgets (m_buf, 512, f_tmpfiles)) {
		fprintf (proc_files[i], "%s", m_buf);
		if (++i >= get_env()->max_proc)
			i = 0;		
	}
	
	for (i = 0; i < get_env()->max_proc; ++i) 
		fclose (proc_files[i]);
	
	return get_env()->max_proc;
}

int remove_tmp_files ()
{
	char m_buf[512];
	int i = 0;

	for (i = 0; i < get_env()->max_proc; ++i) {
		sprintf (m_buf, "%s%i", get_env()->tmp_files, i);
		unlink (m_buf);
		sprintf (m_buf, "%s%i", get_env()->tmp_tags, i);
		unlink (m_buf);
	}	
	
	unlink (get_env()->tmp_files);
	unlink (get_env()->tmp_tags);
	return get_env()->max_proc;
}

