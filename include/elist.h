
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_ELIST_H
#define DEFINE_ELIST_H

class EList : public DList 
{
        public:
                EList ();
                ~EList ();

                char * get ();
                char * first ();
                char * last ();
                char * next ();
                char * prev ();
                char * rm ();
		bool eol ();
                inline char * del () {
                        return rm ();
                }
		inline char * add (const char *S) {
			return add_tail ((char *) S);
		}
		EList & operator << (char *);
                
        private:
                __dlist_entry_t * pos;
};

#endif

