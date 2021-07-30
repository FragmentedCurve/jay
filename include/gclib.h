/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_GCLIB_H
#define DEFINE_GCLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <gclib_c.h>

typedef unsigned char uchar_t;

#define PUT(arga) DROP(arga)
#define Dmemused mallinfo().uordblks
#define LPCHAR(arga) (char *) arga
#define DARRAY(arga) ((DArray *) arga)
#define container_of(var, type) ((type) var)
#define OBJECT(object,type) ((type) object)
#define DRET(type) ((type) cur_ret)
#define DSRET(ret) cur_ret = (long) ret;

#ifndef min
#define min(arga,argb) (arga < argb ? arga : argb)
#define max(arga,argb) (arga > argb ? arga : argb)
#endif

#define MAXSTRSIZE 256

typedef void (*Dfunc_t)(void *);
typedef void (*Dhash_f)(char *, char *);

enum DHASH_TYPES
{
	DHASH_LPCHAR,
	DHASH_INT,
	DHASH_OTHER
};

#include "dhash.h"
#include "darray.h"
#include "earray.h"
#include "dlist.h"
#include "elist.h"
#include "ehash.h"
#include "dheapsort.h"
#include "djobs.h"

struct dkey_t {
	char *KEY;
	char *VALUE;
};

#ifdef I_IN_DLIB_CODE
struct dirent * cur_dir_ent = NULL;
int cur_dir_fd = 0;
int cur_ret = 0;
#endif

DArray * Dfiles (char * path);
DArray * Dsplit (char * STR, char *ch); // 2005.
int Dsplit (char * STR, char *ch, DList * to_list); 
int Dsplit (char * buf, size_t buflen, char *str, EList * to_list);
int Dposix_fadvise (int fd, int offset, int len, int advice);

#undef __export
#define __export

#endif

