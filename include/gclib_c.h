
/*
 * (c); Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_GCLIBC_H
#define DEFINE_GCLIBC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>

#include "djob_t.h"
#include "dexec.h"

void print_the_time (FILE * file_my);
int close_pipe (int *fds);
void Dfastsort_s(char ** a, long N);
void Dtimer ();
struct timeval *the_time ();
int Dterm_one_kick (int fd);
char *Dversion ();
int Dfnwrite (char * p_lpsz_filename, void * p_lp_buffer,int int_size);
int Dfnread (char * f_name, void * p_lp_buffer, int int_size);
int Dselect (int FILENO, int SEC, int USEC);
char * DFILE (const char * m_filename, int *rsize);
struct stat * DSTAT (const char * S);
struct stat * DLSTAT (const char * S);
int DIONREAD (int fd);
int fsize (const char * S);
int fdsize (int fd);
char * DFDMAP (int fd);
char * DFMAP (const char *d_file, int *out_fd, int *d_out_size);
char * Dread_to_eof (int fd, int *d_out_size);
int move_stream (int fd_in, int fd_out);
int Dnonblock(int fd);
char * gc_realloc (char * PTR, int old_size, int new_size);
void * memdup (void * PTR, int size);
char * Dstrmid(char * lpsz_string,char * param1, char * param2);
char * chomp (char * S);
char * DSTR (FILE * m_file);
char * strchr_r (char * S, char ch, int d_len);
char * strchrs (char *S, char ch, char ch2, char ch3, char ch4);
char * Dstrstr_r (char *where, char * str); 
char * Dstrndup (char *ptr, int n);
char * Dtimestr (char * buf, int max);
int Dsyms (char * from, char * to, char sym);
char * Dmemchr (char *from, int n, char ch);
char * Dmid_strchr (char *ptr, char *end, char ch);
char * Dmid_getstr (char *buf, char *end);
char * Drand_str (char * buf, int count);
char * int2str (int i);
char * Dprogram_read (char *EXEC, int * len_out);
char * stail (char *S);
char * strmov (char *buf, char * S);
char * strip (char *str) ;
char * strip2 (char *str) ;
char * Dmemmem (char *haystack, size_t haystacklen, char *needle, size_t needlelen) ;
char * Dmid_memmem (char * begin, char * last, 	char * needle, size_t needlelen) ;
int Dtmpfd (char *name) ;
int fdclose (int * fd) ;
char * fext (char *name) ;
char * Dsprintf (char * fmt, ...) ;
int logToFile (char * fileName, char * fmt, ...) ;
int copyFile (char * sourceName, char * destName) ;

#define if_digit(S) ((*S >= '0') && (*S <= '9'))
#define if_abc(S) ((*S >= 'a' && *S <= 'z') || (*S >= 'A' && *S <= 'Z'))
#define OR(var,val1,val2) ((var == val1) || (var == val2))

#ifndef EQ
#define EQ(arga, argb) (!strcmp ((char *) arga, (char *) argb))
#endif

#ifndef NE
#define NE(arga, argb) (strcmp ((char *) arga, (char *) argb))
#endif

#ifndef chop
#define chop(arg1) arg1[strlen(arg1) - 1] = 0
#endif

#define DROP(arga) if (arga) { free (arga); arga = NULL; }
#define CNEW(arga,argb) (arga *)malloc (sizeof (arga)*(argb))

#undef gclib_c__export
#endif

