
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_EHASH_H
#define DEFINE_EHASH_H

class EHash : public EList 
{
	public:
		EHash ();
		~EHash ();

		char * set (char *KEY, char *SET);
		char * get (char *KEY);
		inline char * get (const char *KEY) {
			return get ((char *) KEY);
		}
		char * del (char *KEY);
		int from_file (char *f_name);
		void foreach (Dhash_f f); 
		char * operator [] (const char *S);

		inline char * get () {
			return EList::get ();
		}
};

#endif

