/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>
#include <gclib_c.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

DJobs::DJobs ()
{

}

DJobs::~DJobs ()
{
        
}

__djob_t * DJobs::add (int pid, char * user_data)
{
	__djob_t * j;      
	j = alloc_job ();
	j->pid = pid;
	j->user_data = user_data;
	add_tail (LPCHAR(j));
	return j;
}

__djob_t * DJobs::attach (__djob_t * j)
{
	if (! j)
		return NULL;
	
	return (__djob_t *) add_tail (LPCHAR(j));	
}

__djob_t * DJobs::zombie ()
{
	pid_t pid;
	int status;

	if (! first ())
		return NULL;

	pid = waitpid (-1, &status, WNOHANG);
	if (pid <= 0)
		return NULL;

	return __do_zombie (pid, status);	
}	

__djob_t * DJobs::__do_zombie (pid_t pid, int exit_status)
{
	__djob_t * j = by_pid (pid);
	if (j) {
		rm ();
		j->exit_status = exit_status;
	} 
	return j;
}

__djob_t * DJobs::wait_all ()
{
	__djob_t * j;
	int status;
	pid_t pid;
	
	while (first ()) {
		pid = waitpid (-1, &status, 0);
		if (pid <= 0) 
			break;
		j = __do_zombie (pid, status);	
		return j;
	}

	return NULL;
}

__djob_t * DJobs::alloc_job ()
{
	__djob_t * j;
	j = CNEW (__djob_t, 1);
	Djob_init (j);
	return j;
}

__djob_t * DJobs::by_pid (pid_t pid)
{
	__djob_t * j;
	first ();
	while ((j = (__djob_t *) get ()) && j) {
		if (j->pid == pid)
			return j;
		next ();
	}
	return NULL;
}

__djob_t * DJobs::fork ()
{
	pid_t pid;
	__djob_t *j;
	
	pid = ::fork ();
	if (pid < 0)
		return NULL;

	j = alloc_job ();
	Djob_init (j);
	if (pid > 0) 
		j->pid = pid;
	else {
		j->pid = getpid ();
		j->child = 1;
	}
			
	attach (j);
	return j;
}



