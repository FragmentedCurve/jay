/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>
#include <gclib_c.h>
#include <sys/syscall.h>
#include <stdarg.h>

__export int Dfnread (char * f_name, void * p_lp_buffer, int int_size)
{
	int n_bytes = int_size > fsize (f_name) ? fsize (f_name) : int_size;
	int fd;

	fd = open (f_name, O_RDONLY);
	if (fd < 0)
		return fd;
	
	if (read (fd, p_lp_buffer, n_bytes) < 0) 
		n_bytes = -1;
		
	close (fd);
	return n_bytes;
}

__export char * Dread_to_eof (int fd, int *d_out_size)
{
	char * d_buf = (char *) malloc (4096);
	int d_size = 4096;
	int d_pos = 0;
	int d_ret = 0;

	if (fd < 0)
		return NULL;

	if (d_out_size)
		*d_out_size = 0;

	while (-1) {
		d_ret = read (fd, &d_buf[d_pos], d_size - d_pos - 1);
		if (d_ret == -1)
			return NULL;

		if (d_ret == 0) //EOF
			break;

		d_pos += d_ret;
		if ((d_size - d_pos) < 4096) {
			d_buf = gc_realloc (d_buf, d_size, d_size << 1);
			d_size<<=1;
			if (d_buf == NULL) {
				if (d_out_size)
					*d_out_size = 0;
				return NULL;
			}
		}
	}

	if (d_out_size)
		*d_out_size = d_pos;

	d_buf[d_pos] = 0;
	return d_buf;
}

// 24/06/06 
__export DArray * Dfiles (char * path)
{
	DIR * m_dir;
	DArray * Ret;
	struct dirent * m_dirent;

	if (! path) 
		return NULL;
	
	m_dir = opendir (path);
	if (! m_dir)
		return NULL;
	
	Ret = new DArray;
	while (true) {
		m_dirent = readdir (m_dir);
		if (! m_dirent)
			break;
		m_dirent = (struct dirent *) memdup (m_dirent, sizeof (struct dirent));
		Ret->add (LPCHAR (strdup (m_dirent->d_name)));
		DROP (m_dirent);
	}

	closedir (m_dir);
	return Ret;
}

__export int logToFile (char * fileName, char * fmt, ...)
{
	va_list alist;
	FILE * myfile;
	myfile = fopen (fileName, "a");
	if (! myfile) {
		myfile = fopen (fileName, "w");
		if (! myfile) 
			return -1;
	}
	va_start (alist, fmt);
	vfprintf (myfile, fmt, alist);
	va_end (alist);
	fclose (myfile);
	return 0;
}

#ifdef __linux
int Dposix_fadvise (int fd, int offset, int len, int advice)
{
	return posix_fadvise (fd, offset, len, advice);
//	return syscall (__NR_fadvise64, fd, 0, offset, len, advice);
}
#else
int Dposix_fadvise (int fd, int offset, int len, int advice)
{
#warning "Dposix_fadvise is broken.\n"
	return 0; 
} 
#endif

