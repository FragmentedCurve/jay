
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_DJOB_T_H
#define DEFINE_DJOB_T_H

#include <sys/types.h>
#include <unistd.h>

struct __djob_t
{
	pid_t pid;
	int exit_status;
	char status_ready;
	char * user_data;
	int stdIn;
	int stdOut;
	int stdErr;	
	int pipe_in[2];
	int pipe_out[2];
	int pipe_err[2];
	char * shared_mem;
	int shm_size;
	char child;
	char *otmp_name;
	char *etmp_name;
};

#endif

