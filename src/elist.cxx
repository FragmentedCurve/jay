/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>

EList::EList ()
{
        pos = NULL;
}

EList::~EList ()
{
}

char * EList::get ()
{
        if (! pos)
                return NULL;
        else
                return pos->data;
}

char * EList::first ()
{
        pos = get_head ();
        return get ();
}

char * EList::last ()
{
        pos = get_tail ();
        return get ();
}

char * EList::next ()
{
        if (! pos->next)
                pos = NULL;
        else 
                pos = pos->next;
        return get ();
}

char * EList::prev ()
{
        if (! pos->prev)
                pos = NULL;
        else 
                pos = pos->prev;
        return get ();
}

char * EList::rm ()
{
        __dlist_entry_t *ptr;
        char * Ret;
        
        if (! pos)
                return NULL;

        ptr = pos;
        Ret = ptr->data;
        
        if (ptr->next)
                pos = ptr->next;
        else if (ptr->prev)
                pos = ptr->prev;	
        else 
                pos = NULL;
        
        DList::rm (ptr);
        return Ret;
}

EList & EList::operator << (char * S)
{
	add_tail (S);
	return *this;
}

bool EList::eol () 
{
	return pos ? false : true;
}

