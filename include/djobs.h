
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_DJOBS_H
#define DEFINE_DJOBS_H
#include "djob_t.h"

class DJobs : public EList
{
	public:
		DJobs ();
		~DJobs ();
		
		__djob_t * add (int pid, char * user_data = NULL);
		__djob_t * attach (__djob_t * j);
		__djob_t * zombie ();
		__djob_t * by_pid (pid_t pid);
		__djob_t * fork ();
		__djob_t * wait_all ();
	
	private:
		__djob_t * alloc_job ();
		void __close_job_fds (__djob_t * j);
		__djob_t * __do_zombie (pid_t pid, int status);
};

#endif


