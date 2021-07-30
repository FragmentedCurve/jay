
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DHEAP_SORT_H
#define DHEAP_SORT_H

class DHeapSort 
{
	public:
		char ** h;
		int size;
	
		DHeapSort (int);
		~DHeapSort ();

		char * add (char *x);
		char * extract_min ();

	private:
		void checkup (int c);
		void checkdown (int c);
	
};

#endif

