/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <gclib.h>
#include <sys/wait.h>
#include <mod.h>
#include <head.h>
#include <dbg.h>

#define TMP_FILE "/tmp/.silent_bob.tmp.cpp"

void try_editor ()
{
	int pid;
	int fd;
	int status;
	char m_buf[1024];
	
	pid = fork ();
	if (pid < 0)
		return;

	if (pid == 0) {
		unlink (TMP_FILE);
		fd = open (TMP_FILE, O_WRONLY | O_CREAT, 0666);
		fchmod (fd, S_IREAD | S_IWRITE);
		if (fd == -1) {
			perror ("open");
			exit (1);
		}
		dup2 (fd, fileno (stdout));
		return;
	} else {
		if ((waitpid (pid, &status, 0) < 0) ||
			status != 0) 
			exit (status);
		
		sprintf (m_buf, "%s %s", get_env()->editor, TMP_FILE);
		exit (system (m_buf));
	}
}

char * editor_option (char * name)
{
	if (! name)
		return NULL;

	if (EQ(name, "-g") || EQ(name, "--gvim")) 
		get_env()->editor = (char *) "gvim";

	if (EQ(name, "--emacs"))
		get_env()->editor = (char *) "emacs";

	if (EQ(name, "--nano"))
		get_env()->editor = (char *) "nano";

	return get_env()->editor;
}

