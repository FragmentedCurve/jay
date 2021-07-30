/*
 * (c) Oleg Puchinin 2006,2007,2008
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>
#include <sys/mman.h>
#include <the_tt.h>

extern FILE * d_stream_dbg;

namespace THE_TT {

#define IF(arg) if (t_map[ipos] == arg) 
#define IF2(arga, argb) if (t_map[ipos] == arga && t_map[ipos+1] == argb)
#define is_space(arg) (arg == ' ' || arg == '\t' || arg == '\n')
#define IF_breaker if (t_map[ipos]==';' || t_map[ipos] == '}' || t_map[ipos] == '{')
#define T t_map[ipos]
#define T2 t_map[ipos+1]
#define Toff(arg) t_map[ipos+arg]
#define ATTACH t_attach (); 

int ipos; 
int opos;
int t_size;
char * t_map;
char * t_new;
int t_current_line;
int t_op_no;
int brace_depth = 0; // for '()'
int block_depth = 0; // for '{}'
bool b_make_attachment;
bool t_in_macro;
pair_t * d_attachment = NULL;
char * tt_out_buf = NULL;
int tt_outbuf_size = 0;
unsigned int t_spaces;
bool b_mustlined;
bool b_instring;
bool b_newline;
int d_slash_count;
bool b_lined;
char ch_last;
bool b_sharp;

inline void t_attach ()
{
	t_op_no++;
	d_attachment[t_op_no].pair_op = t_op_no;
	d_attachment[t_op_no].pair_line = t_current_line;
	d_attachment[t_op_no].offset = ipos;
	brace_depth = 0; // Paranoid
}

inline void TN (char arg)
{
	t_new[opos] = arg; 
	++opos;
	if (arg == '\n' || 
		arg == '{' || arg == '}' ||
		(arg == ';' && !brace_depth)) {

		if (arg == '\n') {
			--t_current_line;
			ATTACH;
			++t_current_line;
		} else
			ATTACH;
	}
}

#define TNs(arg) do { t_new[opos] = arg; ++opos; } while (0)

inline void tt_comment ()
{
	while (ipos < t_size) {
		IF2('*',  '/')
			break;
		IF ('\n') {
			t_current_line++;
		}
		++ipos;
	}
	++ipos;
}

inline void tt_skip ()
{
	while (T != '\n' && ipos < t_size)
		++ipos;
	--ipos;
}

void tt_init (char * d_input, int t_new_size, char * d_output)
{
	t_spaces = 1;
	b_mustlined = false;
	b_instring = false;
	b_newline = true;
	d_slash_count = 0;
	b_lined = true;
	ch_last = 0;
	b_sharp = false;
	
	ipos = 0; 
	opos = 0;
	b_lined = true;
	b_mustlined = false;
	b_newline = true;
	t_size = t_new_size;
	t_map = d_input;
	t_new = d_output;
	t_current_line = 0;
	t_op_no = 0;
	brace_depth = 0;
	block_depth = 0;
}

bool tt_process_line ()
{
	if (b_instring && ch_last != T)	{
		TN(T);
		return true;
	}
	
	if (b_instring)	{	
		if (b_sharp || (Toff(-1) != '\\')) { // \" and... \\" :)
				b_instring = false;
				b_sharp = false;
		} else {
			d_slash_count = 1;
			while (Toff (-(d_slash_count)) == '\\') // Yes, I'm don't like this.
				d_slash_count++;
			
			if (d_slash_count & 1) {
				b_instring = false;
				b_sharp = false;
			}
		}
	} else { // !b_instring
		ch_last = T;
		b_instring = true;
		if (Toff (-1) == '@')
			b_sharp = true;
	}

	return false;
}

void tt_process_space ()
{
	if (T == '\n') {
		b_newline = true;
		if (Toff(1) == '#' && !b_lined)	{
			TN ('\n');
			++t_spaces;
			b_lined = true;
			b_mustlined = false;
			return;
		}
		
		if (b_mustlined) {
			TN('\n');
			++t_spaces;
			if (!(Toff (-1) == '\\')) {
				b_mustlined = false;
				b_lined = true;
			}
		}
		
		if (t_spaces == 0 && !b_lined) {
			++t_spaces; 
			TN(' ');
		}
		return;
	} else {
		++t_spaces;
		if (t_spaces == 1)
			TN(' ');
	}

	return;
}

// NOTE: you _must_ allocate d_input and d_output before call this.
int the_tt_for_buffers (char * d_input,
		int t_new_size, 
		char * d_output)
{
	tt_init (d_input, t_new_size, d_output);

	if (t_map[0] == '\n') {
		++ipos; // "mmap BUG" fixed :))
		++t_current_line;
	}
	
	if (Toff (t_size-1) == '\n')
		t_size--;	// mmap'ed TT do not work without this !

	for (; ipos < t_size; ++ipos)	{
		if (if_abc (&t_map[ipos]) || if_digit (&t_map[ipos]))	{
			b_lined = false;
			t_spaces = 0;
			goto abc;
		}

		IF ('\n')
			++t_current_line;
		
		if (T == '\'' || T == '\"') {
			if (tt_process_line ())
				continue;
		}
		
		if (b_instring)	{
			if (T != '\n')
				TNs (T);
			else {
				if (Toff(-1) == '\\')
					--opos;
			}
			continue;
		}

		if (T == '\n') {
			if (Toff(-1) == '\\') {
				if (t_in_macro)
					TN(T);
				else
					--opos;
				continue;
			} else
				t_in_macro = false;
		}
			
		IF2('/','/') {
			tt_skip ();
			continue;
		}

		IF2('/', '*') {
			tt_comment ();
			continue;
		}
	
		switch (T) {
			case '(':
				++brace_depth;
				break;
			case ')':
				--brace_depth;
				break;
		}
		
		if (brace_depth < 0)
			brace_depth = 0; 
		
		if (is_space (T)) {
			tt_process_space ();
			continue;
		}
			
		b_lined = false;
		if (T == '(' && t_spaces == 0 && b_mustlined == 0) {
			++t_spaces; // No space after '('.
			TN(' ');
			TN('(');
			continue;
		}
		
		if (T == ')' && t_spaces == 0) {
			++t_spaces;
			TN(')');
			TN(' ');
			continue;
		}
				
		t_spaces = 0;
		IF_breaker {
			TN(T);
			TN(' ');
			++t_spaces;
			continue;
		}
	
		IF('#' && b_newline) {
			TN ('#');
			t_in_macro = true;
			b_mustlined = true;
			continue;
		}
	
		IF (0x0D)
			continue;

abc:
		b_newline = false;
		TNs(T);
	} // for

	return opos;
}

char * do_tt_file (tt_state_t * d_tt_state)
{
	char * t_output = NULL;	
	char * f_name;
	int t_Ret;

	d_attachment = NULL;
	
	if (! d_tt_state)
		return 0;

	f_name = d_tt_state->fileName;
	
	if (EQ(f_name, "-")) {
		d_tt_state->fileData = Dread_to_eof (fileno (stdin), &d_tt_state->fileDataSize);
		if (d_tt_state->fileDataSize <= 0) 
			exit (1);
	} else { 
		if (tt_map (d_tt_state) < 0)
			return NULL;
	}
	
	t_size = d_tt_state->fileDataSize;
	t_output = CNEW (char, t_size<<1); // Paranoid.

	if (t_size < 4096) 
		d_attachment = CNEW (pair_t, 4096);
	else
		d_attachment = CNEW (pair_t, t_size>>1); 
	d_tt_state->attachment = d_attachment;
	
	t_Ret = the_tt_for_buffers (d_tt_state->fileData, t_size, t_output);
	t_output[t_Ret] = 0; // Required.
	d_tt_state->resultSize = t_Ret;
	d_tt_state->result = t_output;

	return t_output;
}

// $ silent_bob --the-tt
int the_tt_main (char * f_name) 
{
	char * t_output;
	tt_state_t * d_tt_state;	
	
	d_tt_state = CNEW (tt_state_t, 1);
	memset (d_tt_state, 0, sizeof (tt_state_t));
	d_tt_state->fileName = f_name;
	t_output = do_tt_file (d_tt_state);

	if (! SB_FLGET (SB_FLSIMULATE)) {
		if (write (fileno (stdout), t_output, d_tt_state->resultSize) < 1) {
			perror ("write to stdout");
		}
	}

	free_tt_state (d_tt_state);
	return EXIT_SUCCESS;
}

} // namespace THE_TT

void free_tt_state (struct tt_state_t * S)
{
	if (S->mmaped)
		munmap (S->fileData, S->fileDataSize);
	else
		DROP (S->fileData);

	if (S->fd)
		close (S->fd);

	DROP (S->result);
	DROP (S->attachment);
	DROP (S);
}

int tt_map (tt_state_t *tt) 
{
	tt->mmaped = true;
	tt->fileData = DFMAP (tt->fileName, &tt->fd, &tt->fileDataSize);
	
	if (tt->fileData == NULL) {
		tt->fileDataSize = fsize (tt->fileName);
		tt->mmaped = false;
		tt->fileData = CNEW (char, tt->fileDataSize);
		if (Dfnread (tt->fileName, tt->fileData, tt->fileDataSize) < 0)
			DROP (tt->fileData);
	}	

	if (! tt->fileData) {
		fprintf (stderr, "can't open/mmap file %s\n", tt->fileName);
		perror ("open/mmap");
		return -1;
	}

	return 0;
}

