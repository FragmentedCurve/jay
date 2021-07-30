/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_DLIST_H
#define DEFINE_DLIST_H

struct __dlist_entry_t;
struct __dlist_entry_t {
	char * data;
	__dlist_entry_t * next;
	__dlist_entry_t * prev;
};

class DList {
	public:
		DList () {
			head = NULL;
			tail = NULL;
			dont_free = false;
			ecount = 0;
		}

		~DList ();
		
		inline char * init (char * one) {
			head = new_one (one);
			tail = head;
			return one;
		}

		char * init (__dlist_entry_t * one);
		char * add_head (char *);
		char * add_tail (char *);
		void rm (__dlist_entry_t *);

		inline void del (__dlist_entry_t *entry) {
			rm (entry); 
		}
		
		inline void remove (__dlist_entry_t *entry) {
			rm (entry); 
		}
	
		void detach (__dlist_entry_t * one);
		char * add_after (__dlist_entry_t *, char *);
		char * add_before (__dlist_entry_t *, char *);
		char * add_entry_head (__dlist_entry_t * one);
		char * add_entry_tail (__dlist_entry_t * one);
		char * add_entry_after (__dlist_entry_t *, __dlist_entry_t *);
		char * add_entry_before (__dlist_entry_t *, __dlist_entry_t *);
		void foreach (Dfunc_t);

		inline __dlist_entry_t * new_one (char * one) {
			__dlist_entry_t *entry;
			entry = new __dlist_entry_t;
			memset (entry, 0, sizeof (__dlist_entry_t));
			entry->data = one;
			++ecount;
			return entry;
		}

		inline int count () {
			return ecount;
		}

		inline __dlist_entry_t * get_head () {
			return head; 
		}
		
		inline __dlist_entry_t * get_tail () { 
			return tail;
		}

                __dlist_entry_t * get_entry_at (int N);
		char * get_at (int N);
		
		bool dont_free;

	private:
		__dlist_entry_t * head;
		__dlist_entry_t * tail;
		int ecount;
};
#endif

