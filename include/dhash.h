/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_DHASH_H
#define DEFINE_DHASH_H
class DHash
{
public:
	DHash (int asize = 256);
	~DHash ();
	bool set (char * KEY, char * DATA, int TYPE = DHASH_LPCHAR);
	bool set (char * KEY, long DATA, int TYPE = DHASH_INT);
	char * get (char * KEY);
	char * del (char * KEY);
	inline char ** get_keys () { return keys; }
	inline char ** get_values () { return values; }
	inline int * get_types () { return types; }
	inline int get_size () { return size; }
	void foreach (Dhash_f func);
	int from_file (char *f_name);

private:
	char ** keys;
	char ** values;
	int * types;
	int size;
};

#endif

