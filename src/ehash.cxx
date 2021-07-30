/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

/* 2006-06-07 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gclib.h>
#include <gclib_c.h>

EHash::EHash ()
{
        
}

EHash::~EHash ()
{

}

char * EHash::set (char *KEY, char *VALUE)
{
	__dlist_entry_t *entry;
	dkey_t * one;
	
	entry = get_head ();
	while (entry) {
		one = (dkey_t *) entry->data;
		if (EQ (KEY, one->KEY)) {
			one->VALUE = VALUE;
			break;
		}
		entry = entry->next;
	}

	if (! entry) {
		one = CNEW (dkey_t, 1);
		one->KEY = strdup (KEY);
		one->VALUE = VALUE;
		add_tail (LPCHAR (one));
	}

	return KEY;
}

char * EHash::get (char *KEY)
{
	char * ret = NULL;
	__dlist_entry_t * one;
	one = get_head ();

	if (! one)
		return NULL;

	while (true) {
		if (EQ (KEY, ((dkey_t *) one->data)->KEY)) {
			ret = ((dkey_t *) one->data)->VALUE;
			break;
		}
		if (! one->next)
			break;
                one = one->next;
	}
	return ret;
}

char * EHash::del (char *KEY)
{
	char * ret = NULL;
	__dlist_entry_t * one;
	
	one = get_head ();
	while (one) {
		if (EQ (KEY, ((dkey_t *) one->data)->KEY)) {
			ret = ((dkey_t *) one->data)->VALUE;
			DList::del (one);
			break;
		}
		one = one->next;
	}

	return ret;
}

int EHash::from_file (char *f_name)
{
	FILE * f;
	char * m_buf;
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
		d_set = strchr (m_buf, '=');
		if (! d_set)
			continue;
		
		*d_set = '\0';
		++d_set;
		strip (d_set);
		strip2 (m_buf);
		printf ("%s = %s\n", m_buf, d_set);
		set (m_buf, strdup (d_set));
	}

	fclose (f);
	DROP (m_buf);
	return 0;
}

void EHash::foreach (Dhash_f f)
{
	__dlist_entry_t * one;
        dkey_t * key;
        
	one = get_head ();
	while (one) {
		key = (dkey_t *) one->data;
		f (key->KEY, key->VALUE);
		one = one->next;
	}
}

char * EHash::operator [] (const char *S)
{
	return get (S);
}

