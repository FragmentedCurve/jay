/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#ifdef __linux

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <gclib.h>
#include <sys/wait.h>
#include <mod.h>
#include <head.h>
#include <dbg.h>

void cache_file (char * file, int adv)
{
	int fd;
	
	fd = open (file, O_RDONLY);
	if (fd < 0)
		return;

	Dposix_fadvise (fd, 0, fdsize (fd), adv);
	close (fd);
	return;
}

int cache_load ()
{
	DArray * files = get_env()->d_files;
	for (int i = 0; i < files->get_size (); i++) 
		cache_file (files->get (i), POSIX_FADV_WILLNEED);
	return 0;
}

int cache_free ()
{
	DArray * files = get_env()->d_files;
	for (int i = 0; i < files->get_size (); i++)
		cache_file (files->get (i), POSIX_FADV_DONTNEED);
	return 0;
}

#else
int cache_load ()
{
	return -1;
}

int cache_free ()
{
	return -1;
}
#endif


