/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dheapsort.h>

DHeapSort::DHeapSort (int n) 
{
	size = 0;
	h = (char **) malloc (sizeof (char *) * n+10);
}

DHeapSort::~DHeapSort() 
{
	if (h)
		free (h);
}

char * DHeapSort::add(char * x) 
{
	h[++size]=x;
	checkup(size);
	return x;
}

char * DHeapSort::extract_min() 
{
	char *x;
	if(size <= 0)
		return NULL;
	x = h[1];
	h[1] = h[size--];
	checkdown (1);
	return x;
}

void DHeapSort::checkup(int c) {
	int p;
	char * tmp;
	p = c>>1;
	if (! p)
		return;
	if (strcmp (h[p], h[c]) > 0) {
		tmp = h[p]; h[p] = h[c]; h[c] = tmp;
		checkup(p);
	}
}
    
void DHeapSort::checkdown(int p) 
{
	int c;
	char * tmp;
	c = p<<1;
	if (c > size) 
		return;
	if ( ((c+1) <= size) && strcmp (h[c + 1], h[c]) < 0) 
		++c;
	if (strcmp (h[c], h[p]) < 0) {
		tmp =  h[c]; 
		h[c] = h[p];
		h[p] = tmp;
		checkdown (c);
	}
}

