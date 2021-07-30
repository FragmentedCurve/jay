/*
 * (c) Oleg Puchinin 2007
 * graycardinalster@gmail.com
 * 
 */

#include <head.h>
#include <the_tt.h>
#include <TT.h>

TT::TT ()
{
	tt = NULL;
	d_out = NULL;
	d_ptr = NULL;
}

TT::~TT ()
{
	if (tt)
		free_tt_state (tt);
}

int TT::loadFile (char * fileName)
{
	if (! fileName)
		return -1;
	tt = CNEW (tt_state_t, 1);
	memset (tt, 0, sizeof (tt_state_t));
	tt->fileName = strdup (fileName);
	if (do_tt (tt) == NULL) {
		DROP (tt->fileName);
		DROP (tt);
		return -1;
	}

	return 0;
}

int TT::drop ()
{
	if (tt) {
		free_tt_state (tt);
		tt = NULL;
	}
	return 0;
}

int TT::init ()
{
	if (! tt)
		return -1;
	d_out = tt->result;
	d_ptr = tt->result;
	get_env()->t_op_no = 0;
	bracketDepth = 0;
	depthModif = 0;
	return 0;
}

char * TT::op ()
{
	return d_out;
}

char * TT::nextOperator ()
{
	if (depthModif > 0) {
		++bracketDepth;
		depthModif = 0;
	} else if (depthModif < 0) {
		if (--bracketDepth < 0)
			bracketDepth = 0;
		depthModif = 0;
	}
	
	ch = t_op (&d_ptr, &d_out);
	if (! ch)
		return NULL;

	if (ch == '{')
		depthModif = 1;
	if (ch == '}')
		depthModif = -1;

	get_env()->t_op_no ++;
	return d_out;
}

int TT::wit ()
{
	return what_is_this (d_out, ch);
}

