/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <gclib_c.h>
#include <gclib.h>

/* Extendent array */
EArray::EArray (int asize)
{
}

int EArray::add (long one)
{
	return DArray::add (LPCHAR (one));
}

int EArray::sfind (char * d_find)
{
	int i;
	int d_size = get_size ();
	char ** d_array = get_skeleton ();

	for (i = 0; i < d_size; i++) {
		if (EQ (d_find, d_array[i]))
			return i;
	}

	return -1;
}

int EArray::snfind (char * d_find)
{
	int i;
	int d_size = get_size ();
	char ** d_array = get_skeleton ();

	for (i = 0; i < d_size; i++) {
		if (!strncmp (d_find, d_array[i], strlen (d_array[i])))
			return i;
	}

	return -1;
}

void EArray::push (char ** d_push)
{
	int i = 0;
	if (d_push == NULL)
		return;

	while (d_push[i]) {
		add ((long) d_push[i]);
		++i;
	}
}

int EArray::snfind_fast (char *str, int n)
{
	int first;
	int last;
	int fact;
	int check;
	int move;

	first = 0;
	last = get_size ();
	--last;
	if (last < 0)
		return -1;
	
	if (strcmp (get (0), str) > 0)
		return -1;

	while (true) {
		move = (last - first) >> 1;
		if (! move) {
			if (EQ (get (first), str))
				return first;
			if (EQ (get (last), str))
				return last;
			return -1;
		}

		check = first + move;
		if (n)
			fact = strncmp (get (check), str, n);
		else
			fact = strcmp (get (check), str);

		if (fact < 0) { // [check] < str
			first = check;
		} else if (! fact) { // [check] == str
			return check;
		} else if (fact > 0) { // [chec] > str
			last = check;
		}
	}

	if (EQ (get (first), str))
		return first;

	return -1;
}

int EArray::from_pipe (int fd)
{
	char * buf;
	FILE * m_file;

	m_file = fdopen (fd, "r");
	if (m_file == NULL)
		return false;

	buf = CNEW (char, 256);
	while (true) {
		if (fgets (buf, 256, m_file) == NULL)
			break;

		add (strdup (buf));
	}

	fclose (m_file);
	DROP (buf);
	return get_size ();
}

char * EArray::join (char * mid, int *len_out)
{
	char *Ret;
	char *ptr;
	int count;
	int len = 0;
	int mid_len;
	int i;

	if (! mid) {
		mid_len = 0;
		mid = (char *) "";
	} else
		mid_len = strlen (mid);

	count = get_size ();
	for (i = 0; i < count; i++)
		len += strlen (get (i));

	if (mid)
		len += count * mid_len;

	len++;
	Ret = CNEW (char, len);
	ptr = Ret;
	for (i = 0; i < count; i++) {
		strcpy (ptr, (char *) get (i));
		ptr += strlen (get (i));
		strcpy (ptr, mid);
		ptr += mid_len;
	}

	if (len_out)
		*len_out = len;

	return Ret;
}

int EArray::strings_to_file (char * f_name)
{
        FILE * d_file;
        int d_len;
        int i;
        
        d_file = fopen (f_name, "w");
        if (! d_file)
                return -1;
        
        d_len = get_size ();
        for (i = 0; i < d_len; ++i) 
                fprintf (d_file, "%s\n", get (i));

        fclose (d_file);        
        return d_len;
}

EArray & EArray::operator << (char *S)
{
	add (S);
	return *this;
}

int EArray::expand (EArray * e)
{
	int count;
	int i;

	if (! e)
		return get_size ();

	count = e->get_size ();
	if (! count)
		return get_size ();

	for (i = 0; i < count; ++i) 
		add (e->get (i));

	return get_size ();
}

int EArray::expand (char *buf, char * split)
{
	char * S;
	char * ptr = buf;
	int split_len;
	char * last;

	if (!buf || !split || !(*buf)) 
		return -1;

	last = &buf[strlen (buf)-1];
	split_len = strlen (split);
	while((S = Dmid_memmem (ptr, last, split, split_len)) && S) {
		add (Dstrndup (ptr, S - ptr));
		S += split_len;
		ptr = S;
	}

	if (ptr <= last)  
		add (Dstrndup (ptr, last-ptr+1));

	return get_size ();
}

int EArray::dump (char * d_join)
{
	int i;
	int count;
	
	count = get_size ();
	for (i = 0; i < count; ++i) 
		printf ("%s%s", get (i), d_join);
	
	return count;
}
