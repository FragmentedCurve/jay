/*
 * (c) Oleg Puchinin 2005,2006
 * graycardinalster@gmail.com
 *
 * June 2006 - [DEPRECATED] Use EHash instead.
 */
#if 0
#include <gclib.h>
#include <gclib_c.h>

DHash::DHash (int asize)
{
	if (asize)
		size = asize;
	else
		size = 256;
	
	keys = CNEW(char *, size);
	values = CNEW(char *, size);
	types = CNEW(int, size);
	memset (keys, 0, sizeof (char *) * size);
	memset (values, 0, sizeof (char *) * size);
	memset (types, 0, (sizeof (int)) * size);
}

DHash::~DHash ()
{
	int i;
	
	for (i = 0; i < size; i++) {
		if (keys[i] != NULL)
			free (keys[i]);
	}
	DROP (keys);
	DROP (values);
	DROP (types);
}

bool DHash::set (char * KEY, char * VALUE, int TYPE)
{
	int i;

	for (i = 0; i < size; i++) {
		if (keys[i] && EQ (keys[i], KEY)) {
			values[i] = VALUE;
			types[i] = TYPE;
			return true;
		}
	}

	for (i = 0; i < size; i++) {
		if (keys[i] == NULL) {
			keys[i] = strdup (KEY);
			values[i] = VALUE;
			types[i] = TYPE;
			return true;
		}
	}

	return false;
}

bool DHash::set (char * KEY, long VALUE, int TYPE)
{
	return set (KEY, LPCHAR (VALUE), DHASH_INT);
}

char * DHash::get (char * KEY)
{
	int i;

	for (i = 0; i < size; i++) {
		if (keys[i] && EQ (keys[i], KEY)) {
			return values[i];
		}
	}

	return NULL;
}

char * DHash::del (char * KEY)
{
	int i;

	for (i = 0; i < size; i++) {
		if (keys[i] && EQ (keys[i], KEY)) {
			keys[i] = NULL;
			return values[i];
		}
	}
	
	return NULL;
}

void DHash::foreach (Dhash_f func)
{
	int i;

	for (i = 0; i < size; i++) 
		func (keys[i], values[i]);
}

int DHash::from_file (char *f_name)
{
	FILE * f;
	char * m_buf;
	char * d_key;
	char * d_set;

	m_buf = CNEW (char, 4096);
	f = fopen (f_name, "r");
	if (! f)
		return -1;

	while (true) {
		if (fgets (m_buf, 256, f) == NULL)
			break;
		
		if (m_buf[0] == '#')
			continue;

		chomp (m_buf);
		d_key = strchr (m_buf, '=');
		if (! d_key)
			continue;

		d_set = d_key;
		*d_set = '\0';
		while ((*(--d_key) == ' ' || *d_key == '\t') 
				&& d_key != m_buf)
			*d_key = '\0';

		while (*(++d_set) && (*d_set == ' ' || *d_set == '\t'));
		
		set (m_buf, strdup (d_set));
	}

	fclose (f);
	DROP (m_buf);
	return 0;
}

#endif
