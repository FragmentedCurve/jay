
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_DARRAY_H
#define DEFINE_DARRAy_H

class DArray
{
public:
	DArray (int asize = 0);
	~DArray ();

	bool add (char * VALUE);
	void drop ();
	char * get (int NUM);
	int get_size ();
	int set_size (int);
	inline char ** get_skeleton ()
	{
		return values;
	}
	void foreach (Dfunc_t);
	void set (long, char *);
	void init (int asize);
	bool from_file (char * __from);
	inline int get_real_size () {
		return size;
	}

private:
	char ** values;
	int size;
	int used;

};

#endif
