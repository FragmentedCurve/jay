/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>
#undef __export
#define __export

/* 2006-06-07 */
__export int Dsplit (char * STR, char *ch, DList * to_list)
{
	DList * ret = to_list;
	char * S;
	char * ptr = STR;
	int ch_len;
	int count = 0;

	if (!STR || !ch || !ret) 
		return -1;

	ch_len = strlen (ch);
	while((S = strstr (ptr, ch)) && S) {
		ret->add_tail (Dstrndup (ptr, S - ptr));
		S += ch_len;
		ptr = S;
		++count;
	}

	if (strlen (ptr)) { 
		ret->add_tail (strdup (ptr));
		++count;
	}

	return count;
}

__export int Dsplit (char * buf, size_t buflen, char *str, EList * to_list)
{
	char * S;
	char * ptr = buf;
	int str_len;
	int count = 0;
	char * last;

	if (!buf || !str || !to_list) 
		return -1;

	last = &buf[buflen-1];
	str_len = strlen (str);
	while((S = Dmid_memmem (ptr, last, str, str_len)) && S) {
		*to_list << Dstrndup (ptr, S - ptr);
		S += str_len;
		ptr = S;
		++count;
	}

	if (ptr <= last) { 
		*to_list << Dstrndup (ptr, last-ptr+1);
		++count;
	}

	return count;
}

__export DArray * Dsplit (char * STR, char *ch)
{
	DArray * ret = new DArray(128);
	char * S;
	char * buf = strdup (STR);
	char * ptr = buf;
	int ch_len;

	if (buf == NULL)
		return NULL;

	if (!STR || !ch) {
		delete ret;
		return NULL;
	}

	ch_len = strlen (ch);
	while((S = strstr (ptr, ch)) && S) {
		*S = 0;
		S += ch_len;
		ret->add (strdup (ptr));
		ptr = S;
	}

	if (strlen (ptr)) 
		ret->add (strdup (ptr));

	free (buf);
	return ret;
}


