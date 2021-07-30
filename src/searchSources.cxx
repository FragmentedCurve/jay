
/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>

EArray * searchSources ()
{
	EArray * fs;
	EArray * exts;
	EArray * ret;
	char * S;
	int i;

	exts = extensions ();

	unlink (get_env()->tmp_files);
	find_one_ext ("*");
	
	fs = new EArray(64);
	fs->from_file (get_env()->tmp_files);
	fs->foreach ((Dfunc_t)chomp);

	ret = new EArray;
	for (i = 0; i < fs->get_size (); ++i) {
		S = (*fs)[i];
		S = fext (S);
		if (! S)
			continue;
		++S;
		if (exts->sfind(S) >= 0) {
			(*ret) << strdup ((*fs)[i]);
		} else
			continue;
	}

	fs->foreach (free);
	delete fs;
	return ret;
}

