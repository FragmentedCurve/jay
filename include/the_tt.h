/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_THE_TT_H
#define DEFINE_THE_TT_H

/*** ./the_tt.cpp ***/
namespace THE_TT {
	void tt_skip () ;
	void tt_comment () ;
	int the_tt_main (char * f_name) ; 
	char * do_tt_file (tt_state_t * d_tt_state);
}
#define do_tt THE_TT::do_tt_file
void free_tt_state (struct tt_state_t * S);
int tt_map (tt_state_t *tt);

#endif
