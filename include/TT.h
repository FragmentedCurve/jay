/*
 * (c) Oleg Puchinin 2007
 * graycardinalster@gmail.com
 * 
 */

#ifndef DEFINE_TT_H
#define DEFINE_TT_H
class TT
{
	public:
		TT ();
		~TT ();
		
		int loadFile (char * fileName);
		int init ();
		int drop ();
		char * op ();
		char * nextOperator ();
		int wit ();

		inline int line () {
			if (! tt)
				return -1;
			return tt->attachment[get_env ()->t_op_no].pair_line + 1;
		}

		char ch;
		tt_state_t * tt;
		int bracketDepth;

	private:
		char *d_out, *d_ptr;
		int depthModif;
};

#endif

