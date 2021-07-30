/*
 * (c) Oleg Puchinin 2005,2006
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>
#include <gclib_c.h>

DArray::DArray (int asize)
{
	init (asize);
}

DArray::~DArray ()
{
	drop ();
}

char * DArray::get (int NUM)
{
	if (NUM >= size)
		return NULL;
	
	if (NUM < 0)
		return NULL;

	return values[NUM];
}

bool DArray::add (char * VALUE)
{
	if (values == NULL) {
		used = 0;
		size = 16;
		values = CNEW(char *,size);
		memset (values, 0, sizeof (char *) * size);
	}

	if (used < size) {
		values[used] = VALUE;
		used++;
		return true;
	}	else {
		values = (char **) gc_realloc ((char *)values, 
				(sizeof (char *)*size),
				(sizeof (char *) * (size << 1)));
		if (values == NULL)
			return false;
		size <<= 1;
		values[used] = VALUE;
		used++;
		return true;
	}
}

void DArray::drop ()
{
	if (values) {
		delete [] values;
		values = NULL;
	}
	used = 0;
	size = 0;	
}

int DArray::get_size ()
{
	return used;
}

int DArray::set_size (int a_used)
{
	int old = used;
	used = a_used;
	return old;
}

void DArray::foreach (Dfunc_t FUNC)
{
	for (int i = 0; i < used; i++)
		FUNC(values[i]);	
}

void DArray::set (long ARGA, char * S)
{
	values[ARGA] = S;
}

void DArray::init (int asize)
{
	values = NULL;
	used = 0;
	if (asize > 0) {
		size = asize;
	} else {
		size = 16;
		asize = 16;
	}

	values = CNEW(char *, asize);
	memset (values, 0, asize * sizeof (char *));
}

bool DArray::from_file (char * __from)
{
	char * buf;
	FILE * m_file;

	m_file = fopen (__from, "r");
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
	return true;
}

