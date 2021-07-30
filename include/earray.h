
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_EARRAY_H
#define DEFINE_EARRAY_H

class EArray : public DArray
{
	public:
		EArray (int asize = 0);

		int sfind (char * d_find);
		int snfind (char * d_find);
		int add (long d_one);

		int find (long d_one);
		int del (long d_one);

		int inline add (char * d_one) {
			return add ((long) d_one); 
		}		

/// \note "find (char *)" search a pointer, not a string. Use sfind (or snfind) instead.

		inline char * operator [] (int n) {
			return get (n);
		}

		void push (char ** d_push);
		int snfind_fast (char *str, int n);
		char * join (char * mid = NULL, int *len_out = NULL);

		int from_pipe (int fd);
                int strings_to_file (char *f_name);
		int expand (EArray * e);
		int expand (char *S, char * split);
		int dump (char * d_join);

		EArray & operator <<(char *);

};

#endif

