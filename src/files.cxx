/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>

int files ()
{
	EArray * src;
	int lang;
	int i;
	int count;
	char * S;

	lang = language_id ();
	src = searchSources ();
	if (! src) 
		return -1;

	count = src->get_size ();	
	for (i = 0; i < count; ++i) {
		S = src->get (i);
		if ((lang == file_lang_id (S)) || (lang == LALL))
			printf ("%s\n", S);
	}

	src->foreach (free);
	return 0;
}

