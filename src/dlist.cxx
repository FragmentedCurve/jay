/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

/* 2006-05-25 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gclib.h>

DList::~DList ()
{
	__dlist_entry_t * one;
	__dlist_entry_t * d_del;
	if (! head || dont_free)
		return;

	one = head;
	while (one) {
		d_del = one;
		one = one->next;
		free (d_del);
	}
	head = NULL;
	tail = NULL;
}

char * DList::init (__dlist_entry_t * one)
{
	head = one;
	tail = one;
	return one->data;
}

char * DList::add_head (char * one)
{
	__dlist_entry_t *entry;

	if (head == NULL) 
		return init (one);
	
	entry = new_one (one);
	head->prev = entry;
	entry->next = head;
	head = entry;	

	return one;
}

char * DList::add_tail (char * one)
{
	__dlist_entry_t * entry;
	if (tail == NULL) 
		return init (one);

	entry = new_one (one);
	tail->next = entry;
	entry->prev = tail;
	tail = entry;

	return one;
}

void DList::rm (__dlist_entry_t * one)
{
	if (! one)
		return;

	--ecount;
	if (one == tail && one == head) {
		free (one);
		head = NULL;
		tail = NULL;
		return;
	}
	
	if (one == head) {
		head = one->next;
		head->prev = NULL;
		free (one);
		return;
	}
	
	if (one == tail) {
		tail = one->prev;
		tail->next = NULL;
		free (one);
		return;
	}

	one->prev->next = one->next;
	one->next->prev = one->prev;
	free (one);
}

void DList::detach (__dlist_entry_t * one)
{
	if (! one)
		return;
	
	if (one == tail && one == head) {
		head = NULL;
		tail = NULL;
		return;
	}
	
	if (one == head) {
		head = one->next;
		head->prev = NULL;
		return;
	}
	
	if (one == tail) {
		tail = one->prev;
		tail->next = NULL;
		return;
	}

	one->prev->next = one->next;
	one->next->prev = one->prev;
}


char * DList::add_after (__dlist_entry_t *two, char * one)
{
	__dlist_entry_t * entry;

	if (! two || ! one)
		return NULL;

	if (two == tail) 
		return add_tail (one);

	entry = new_one (one);
	if (two->next) {
		entry->prev = two;
		entry->next = two->next;
		two->next->prev = entry;
		two->next = entry;
	} else  
		return NULL;

	return one;
}

char * DList::add_before (__dlist_entry_t *two, char * one)
{
	__dlist_entry_t * entry;

	if (! two || ! one)
		return NULL;

	if (two == head) 
		return add_head (one);

	entry = new_one (one);
	if (two->prev) {
		entry->next = two;
		entry->prev = two->prev;
		two->prev->next = entry;
		two->prev = entry;
	} else 
		return NULL;	

	return one;
}

void DList::foreach (Dfunc_t f)
{
	__dlist_entry_t * entry;
	entry = head;
	while (entry) {
		f((void *) entry->data);
		entry = entry->next;
	}
}

char * DList::add_entry_head (__dlist_entry_t * one)
{
	if (head == NULL) 
		return init (one);
	head->prev = one;
	one->next = head;
	one->prev = NULL;
	head = one;
	return one->data;
}

char * DList::add_entry_tail (__dlist_entry_t * one)
{
	if (tail == NULL) 
		return init (one);
	tail->next = one;
	one->prev = tail;
	one->next = NULL;
	tail = one;
	return one->data;
}

__dlist_entry_t * DList::get_entry_at (int N)
{
        __dlist_entry_t * one;
        one = get_head ();
        if (! one || ! N)
                return NULL;
        while ((one = one->next) && one && --N);
        return NULL;               
}

char * DList::get_at (int N)
{
        __dlist_entry_t * one;
        one = get_entry_at (N);
        if (! one)
                return NULL;
        else
                return one->data;
}

