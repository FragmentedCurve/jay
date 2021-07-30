/*
 * (c) Oleg Puchinin 2008
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>

void tags_interactive ()
{
	DArray * t;
	char m_buf[512];

	m_buf[511] = '\0';
	while (true) {
		printf ("Tag: ");
		fflush (stdout);
		
		if (! fgets (m_buf, 511, stdin))
			break;

		chomp (m_buf);
		t = Dsplit (m_buf, (char *) " ");
		if (! t) 
			continue;
		tags (t, NULL);
		t->foreach (free);
		delete t;
	}
}

