/*
 * (c) Oleg Puchinin 2006,2007
 * graycardinalster@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/ioctl.h>
#include <stdarg.h>
#include <gclib_c.h>

#define __export
struct timeval *cur_tv = NULL;
struct stat *cur_stat = NULL;

__export void Djob_init (struct __djob_t * ctx)
{
	memset (ctx, 0, sizeof (struct __djob_t));
	ctx->stdIn = -1;
	ctx->stdOut = -1;
	ctx->stdErr = -1;
	ctx->pipe_in[0] = -1;
	ctx->pipe_in[1] = -1;
	ctx->pipe_out[0] = -1;
	ctx->pipe_out[1] = -1;
	ctx->pipe_err[0] = -1;
	ctx->pipe_err[1] = -1;
	ctx->otmp_name = (char *) malloc (128);
	ctx->etmp_name = (char *) malloc (128);
	ctx->otmp_name[0] = '\0';
	ctx->etmp_name[0] = '\0';
	ctx->shared_mem = NULL;
}

__export int Dexec_done (struct __djob_t *ctx)
{
	if (! ctx)
		return 0;

	if (ctx->otmp_name)
		free (ctx->otmp_name);

	if (ctx->etmp_name)
		free (ctx->etmp_name);

	if (ctx->shared_mem)
		munmap (ctx->shared_mem, ctx->shm_size);
	
	fdclose (&ctx->stdIn);
	fdclose (&ctx->stdOut);
	fdclose (&ctx->stdErr);
	free (ctx);
	return 0;
}

__export int Dfnwrite (char * p_lpsz_filename, void * p_lp_buffer,int int_size)
{
	int result;
	FILE * myfile;
	myfile = fopen(p_lpsz_filename,"w");
	if(!myfile)
		return 0;
	result = fwrite(p_lp_buffer,1,int_size,myfile);
	fclose(myfile);
	return result;
}

__export char * DFILE (const char * m_filename, int *rsize)
{
	char * m_file;
	struct stat m_stat;
	char * ptr;
	int count;
	int len;
	int fd;

	if (m_filename == NULL)
		return NULL;

	fd = open (m_filename, O_RDONLY);
	if (fd < 0)
		return NULL;

	if (lstat (m_filename, &m_stat) < 0)
		return NULL;

	m_file = (char *) malloc (m_stat.st_size); 
	if (m_file == NULL)
		return NULL;

	ptr = m_file;
	len = m_stat.st_size;
	while (-1) {
		count = read (fd, ptr, len);
		if (count <= 0)
			break;
		ptr+=count;
		len-=count;
	}	

	if (rsize)
		*rsize = m_stat.st_size;

	close (fd);
	return m_file;
}

__export struct stat * DSTAT (const char * S)
{
	if (!cur_stat)
		cur_stat = (struct  stat *) malloc (sizeof (struct stat));
	stat (S, cur_stat);
	return cur_stat;
}

__export struct stat * DLSTAT (const char * S)
{
	if (! cur_stat)
		cur_stat = (struct stat *) malloc (sizeof (struct stat));
	lstat (S, cur_stat);
	return cur_stat;
}

__export int DIONREAD (int fd)
{
	int ret = -1;

	if (ioctl (fd, FIONREAD, &ret) != 0)
		return -1;

	return ret;
}

__export int fsize (const char * S)
{
	struct stat m_stat;

	if (lstat (S, &m_stat) < 0)
		return -1;

	return m_stat.st_size;
}

__export int fdsize (int fd)
{
	struct stat m_stat;

	if (fstat (fd, &m_stat) < 0)
		return -1;

	return m_stat.st_size;
}

__export char * DFDMAP (int fd)
{
	return (char *) mmap (NULL, fdsize (fd), PROT_READ, MAP_SHARED, fd, 0);
}

__export char * DFMAP (const char *d_file, int *out_fd, int *d_out_size)
{
	char *S = NULL;
	int d_size;
	int fd;

	fd = open (d_file, O_RDONLY);
	if (fd < 0)
		return NULL;

	d_size = fdsize (fd);

	if (d_out_size)
		*d_out_size = d_size;

	if (out_fd)
		*out_fd = fd;

	S = (char *) mmap (NULL, d_size, PROT_READ, MAP_SHARED, fd, 0);

	if ((long) S == -1) {
		close (fd);
		return NULL;
	}

	return S;
}

__export int close_pipe (int *fds)
{
	int Ret1 = 0;
	int Ret2 = 0;

	if (fds[0] != -1) {
		Ret1 = close (fds[0]);
		fds[0] = -1;
	}

	if (fds[1] != -1) {
		Ret2 = close (fds[1]);
		fds[1] = -1;
	}

	return Ret1 ? Ret1 : Ret2;	
}

__export int Dtmpfd (char *name)
{
	char m_buf[128];
	char tmpstr[64];
	int fd;

	Drand_str (tmpstr, 63);
	sprintf (m_buf, "/tmp/%s", tmpstr);
	fd = open (m_buf, O_CREAT | O_RDWR, 0666);
	if (name) {
		if (fd >= 0) 
			strcpy (name, m_buf);
		else 			
			name[0] = '\0';
	}
	
	return fd;
}

__export int fdclose (int * fd)
{
	if (! fd)
		return 0;
	
	if (*fd != -1) {
		close (*fd);
		*fd = -1;
	}
	return 0;
}

__export char * fext (char *name)
{
	if (! name)
		return NULL;
	return rindex (name, '.');
}

__export void Dtimer ()
{
	if (! cur_tv)
		cur_tv = (struct timeval *) malloc (sizeof (struct timeval));
	gettimeofday(cur_tv, NULL);
}

__export struct timeval *the_time ()
{
	struct timeval new_tv;

	if (cur_tv == NULL)
		return NULL;
	
	gettimeofday (&new_tv, NULL);
	cur_tv->tv_sec = new_tv.tv_sec - cur_tv->tv_sec;
	if (new_tv.tv_usec >= cur_tv->tv_usec)
		cur_tv->tv_usec = new_tv.tv_usec - cur_tv->tv_usec;
	else {
		cur_tv->tv_sec--;
		cur_tv->tv_usec = cur_tv->tv_usec - new_tv.tv_usec;
	}

	return cur_tv;
}

__export void print_the_time (FILE * file_my)
{
	if (! the_time ()) 
		return;
	
	if (file_my)
		fprintf (file_my, "The time : %i.%06i\n",
				(int) cur_tv->tv_sec,
				(int) cur_tv->tv_usec);
	else
		printf ("The time : %i.%06i\n",
			(int) cur_tv->tv_sec, (int) cur_tv->tv_usec);
	
}

__export int Dterm_one_kick (int fd)
{
	struct termios ttystate;
	tcgetattr (fd, &ttystate);
	ttystate.c_lflag &= -ICANON;
	ttystate.c_cc[VMIN] = 1;
	return tcsetattr (fd, TCSANOW, &ttystate);
}

__export char *Dversion ()
{
	return (char *) "1.6";
}

__export char * Dtimestr (char * buf, int max)
{
	time_t t;
	time (&t);
	if (! buf)
		return NULL;
	strftime (buf, max, "%H:%M:%S %d.%m.%Y", localtime (&t)); 
	return buf;
}

__export char * gc_realloc (char * PTR, int old_size, int new_size)
{
	int i;
	char * S = (char *) malloc (new_size);
	if (S == NULL)
		return NULL;

	i = (new_size >= old_size) ? old_size : new_size;
	memcpy (S, PTR, i);
	free (PTR);
	return S;
}

__export void * memdup (void * PTR, int size)
{
	char * Ret;
	Ret = (char *) malloc (size);
	memcpy (Ret, PTR, size);
	return (void *) Ret;
}

/// lpsz_string =~ m/param1(.*)param2/
__export char * Dstrmid(char * lpsz_string,char * param1, char * param2)
{
	char * Result;
	char *S;
	char *S2;
	int int_strsize;

	if(! strlen (param1))
		return 0;

	S = strstr (lpsz_string,param1);
	if(! S)
		return 0; 
	S += strlen (param1);
	S2 = strstr (S,param2); 
	if(! S2)
		return 0;

	int_strsize = S2 - S;
	if(! int_strsize)
		return 0;
	
	Result = (char *) malloc (int_strsize + 1);
	memcpy (Result, S, int_strsize);
	Result [int_strsize] = 0;
	return Result;
}

__export char * chomp (char * S)
{
	if (S == NULL)
		return NULL;

	while (*S) {
		if (*S == 0x0D || *S == 0x0A) {
			*S = '\0';
			break;
		}
		++S;
	}
	return S;
}

__export char * DSTR (FILE * m_file)
{
        char *S;
	if (m_file == NULL)
		return NULL;

	S = (char *) malloc (256);
	if (fgets (S, 256, m_file) != S)
		return NULL;

	return S;
}

__export char * strchr_r (char * S, char ch, int d_len)
{
	if (! d_len)
		d_len = strlen (S);

	S += d_len - 1;
	while (d_len > 0) {
		if (*S == ch)
			break;
		--S;
		--d_len;
	}

	return S;
} 

__export char * strchrs (char *S, char ch, char ch2, char ch3, char ch4)
{
	while (*S) {
		if (*S == ch)
			break;

		if (*S == ch2)
			break;

		if (*S == ch3)
			break;

		if (*S == ch4)
			break;

		S++;
	}

	if (*S == ch || *S == ch2 || *S == ch3 || *S == ch4)
		return S;

	return NULL;
}

/* 2006-05-25 */
__export char * Dstrstr_r (char *where, char * str) 
{
	char * S;
	int len;	

	if (! where || ! str || strlen (where) == 0)
		return NULL;

	S = &where[strlen (where)];
	len = strlen (str);

	while (--S != where) {
		if (! strncmp (S, str, len))
			return S;
	}

	return NULL;
}

__export int Dsyms (char * from, char * to, char sym)
{
	int count = 0;
	if (from == to) {
		if (*from == sym)
			return 1;
		return 0;
	}
	do {
		if (*from == sym)
			++count;
	} while (++from != to);
	return count;
}

__export char * Dmemchr (char * from, int n, char ch)
{
	while (n--) {
		if (*from == ch)
			return from;
		++from;
	}
	return NULL;
}

__export char * Dstrndup (char *ptr, int n)
{
	char *S;
	char *buf;
	
	if (ptr == NULL || n <= 0)
		return NULL;

	buf = (char *) malloc (n+1);
	S = buf;
	while (*ptr && n--) {
		*S = *ptr;
		++S; ++ptr;
	}
	*S = '\0';
	
	return buf;
}

__export char * Dmid_strchr (char *ptr, char *end, char ch)
{
        while (ptr <= end) {
                if (*ptr == ch) 
                        return ptr;
                ++ptr;
        }
        return NULL;
}

__export char * Dmid_getstr (char *buf, char *end)
{
        char *S;
        char *out;
        int s_len;
        
        if (! buf || ! end)
                return NULL;
        
        S = Dmid_strchr (buf, end, '\n');
        if (! S)
                S  = end;
        
        s_len = S-buf+1;
        out = (char *) malloc (s_len+1);
        memcpy (out, buf, s_len);
        out[s_len] = '\0';

        return out;
}

__export char * Drand_str (char * buf, int count)
{
        int i;
        unsigned char ch;
	
        if (! buf)
                return NULL;

        --count;
        for (i = 0; i < count; ++i) {
                ch = rand () % ('z' - 'a' - 1);
                buf[i] = ch + 'a';
        }                

        buf[i] = 0;
        return buf;
}

__export char * int2str (int i)
{
        char buf[64];
        sprintf (buf, "%i", i);
        return strdup (buf);
}

__export char * stail (char *S)
{
	if (! S)
		return NULL;
	return &S[strlen (S)];
}

__export char * strmov (char *buf, char * S)
{
	if (! buf || ! S)
		return NULL;
	strcpy (buf, S);
	return buf+strlen (S);
}

__export char * strip (char *str)
{
	char *S;
	if (! str)
		return NULL;
	S = str;
	while (*S && (*S == '\t' || *S == ' '))
		++S;
	if (S != str)
		strcpy (str, S);
	return str;
}

__export char * strip2 (char *str)
{
	char *S;
	if (! str)
		return NULL;
	S = stail (str);
	--S;
	while (S != str && (*S == ' ' || *S == '\t'))
		--S;
	++S;
	*S = '\0';
	return str;
}

__export char * Dmemmem (char *haystack, size_t haystacklen,
                    char *needle, size_t needlelen)
{
	char * ptr;
	char * end;

	if (! haystack || ! needle)
		return NULL;

	if (haystacklen < needlelen)
		return NULL;
	ptr = haystack;
	end = &haystack[haystacklen] - needlelen;
	while (ptr != end && memcmp (ptr, needle, needlelen)) 
		++ptr;
	if (ptr == end)
		return NULL;

	return ptr;
}

__export char * Dmid_memmem (char * begin, char * last, 
		char * needle, size_t needlelen)
{
	char * ptr;

	if (! begin || ! needle)
		return NULL;

	if ((last - begin - 1) < (int) needlelen)
		return NULL;

	last -= needlelen;
	++last;
	ptr = begin;
	while (ptr <= last && memcmp (ptr, needle, needlelen)) 
		++ptr;

	if (ptr > last)
		return NULL;

	return ptr;
}

__export char * Dsprintf (char * fmt, ...)
{
	char m_buf[512];
	va_list ap;
	m_buf[511] = '\0';
	va_start (ap, fmt);
	vsnprintf (m_buf, 511, fmt, ap);
	va_end (ap);
	return strdup (m_buf);
}

