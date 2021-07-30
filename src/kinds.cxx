/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#include <head.h>
#include <wit.h>

int bob_kinds ()
{
	DArray * ar;
	char * S;
	int i;

	if (! get_env()->d_kinds)
		return -1;

	ar = new DArray(256);
	ar->from_file ((char *) "./tags");
	for (i = 0; i < ar->get_size (); ++i) {
		S = ar->get (i);
		chomp (S);
		if (*S == '!')
			continue;	
		S = rindex (S, ';');
		if (! S) {
			fprintf (stderr, "error in tag: %s\n", ar->get(i));
			exit (1);
		}

		S += 2;
		if (! strchr (get_env()->d_kinds, *S))
			continue;

		printf ("%s\n", ar->get (i));
	}

	ar->foreach (free);
	delete ar;
	return -1;
}


