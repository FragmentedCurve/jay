/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 * 
 */ 

#include <head.h>
#include <the_tt.h>
#include <dbg.h>
#include <bugs.h>

EArray * d_calltags_file;

inline bool tree_immune (char * m_fname)
{
	return get_env()->immune_list->sfind (m_fname) == -1 ? false : true;
}

/* "SilentBob -u" functionality. */
void rctree_iter (char * f_name, int n_trip)
{
	EArray * d_donelist;
	char m_buf[256];
	char * S;
	int len;
	int i;
	int a;

	n_trip ++;
	if (n_trip > get_env()->d_depth)
		return;

	/* 
	 * 1. Find record in d_calltags_file.
	 * 2. Find _first_ record in d_calltags_file.
	 * 3. Parse record.
	 * 4. Check d_donelist list.
	 * 5. If f_name not in get_env()->full_list :
	 * 5.1 Add f_name to get_env()->full_list.
	 * 5.2 New iteration.
	 *
	 */
	
	snprintf (m_buf, 255, "%s\t", f_name);
	m_buf[255] = 0;
	len = strlen (m_buf);	
	i = d_calltags_file->snfind_fast (m_buf, strlen (m_buf));
	d_donelist = new EArray (32);
	
	do {
		i--;
		if (i < 0)
			break;
		S = d_calltags_file->get (i);
		if (! S)
			break;
	} while (! strncmp (S, m_buf, len));

	while (true) {
		++i;
		S = d_calltags_file->get (i);
		if (! S || strncmp (S, m_buf, len))
			break;

		S = strstr (S, "\tby\t");
		if (S == NULL) {
			fprintf (get_env()->d_stream_dbg, "\tBUG (no \"by\") : %s\n",
					d_calltags_file->get (i));
			continue;
		}
							
		S += 4;
		chomp (S);

		if (d_donelist->sfind (S) != -1)
			continue;
					
		d_donelist->add (strdup (S));	
		for (a = 0; a < n_trip; a++) 
			fputc ('\t', stdout);
		
		tree_echo (n_trip, S);

		if (get_env()->full_list->sfind (S) == -1) {
			get_env()->full_list->add (strdup (S));
			rctree_iter (S, n_trip);
		}
	} 

	d_donelist->foreach (free);
	delete d_donelist;
}

void reverse_calltree (char * f_name)
{
	assert (f_name == NULL, "Lena 2");
	if (! f_name)
		return;
	
	tree_echo (0, f_name);
	d_calltags_file = new EArray;
	d_calltags_file->from_file ((char *) "./call_tags");
	if (d_calltags_file->get_size () == 0) {
		bug_nocalltags ();
	}

	rctree_iter (f_name, 0);
	
	d_calltags_file->foreach (free);
	delete d_calltags_file;
	fflush (stdout);
}

void tree_echo (int n_trip, char *S) 
{
	struct d_tag_t * tag;
	EArray * tags;
	
	if (! SB_FLGET (SB_FLFNAMES)) 
		goto out_min;
	else {
		tags = got_tag (S);
		if (! tags || (tags->get_size () == 0)) 
			goto out_min;
		
		tag = (struct d_tag_t *) tags->get (0);
		if (! tag)
			goto out_min;
		
		printf ("// %i // %s\t%s", n_trip, S, tag->d_file);
		if (tags->get_size () > 1)
			printf (" (more)");
		fputc ('\n', stdout);
	}

	return;

out_min:	
	printf ("// %i // %s\n", n_trip, S);	
}

inline void tree_mkpos (int N)
{
	while (N--) 
		fputc ('\t', stdout);
}

/* SilentBob call-tree functionality. */
void call_tree (char * f_name, int n_trip)
{
	struct tt_state_t * d_tt_state;
	char * d_ptr, *d_out;
	EArray * d_donelist; 
	DArray * d_words;
	DArray * d_vars; // hash_t ** 	
	char * S;
	char ch;
	int i;

	if (n_trip > get_env()->d_depth) 
		return;

	if (n_trip == 0) {
		tree_echo (0, f_name);
		if (SB_FLGET (SB_FLLINEAR) || SB_FLGET (SB_FLVERBOSE))
			terminal_dump_tags (f_name, 0);
		++n_trip;
	}
	
	d_tt_state = get_onett_tag (f_name, &d_ptr);
	if (! d_tt_state) {
		if (get_env()->full_list->get_size () == 0)
			bug_nosuch_tag (f_name);
		return;
	}		
		
	d_out = d_ptr;
	d_donelist = new EArray (256);
	d_vars = new DArray (32);
	
	ch = t_op (&d_ptr, &d_out);
	if (ch == 0)
		return;

	fault (! d_out);
	
	while (true) {
		ch = t_op (&d_ptr, &d_out);
		
		if (ch == 0)
			break;

		d_words = split_to_words (d_out);
		assert (d_words == NULL, "Lena 28");
		
		for (i = 0; i < d_words->get_size (); i++) {
			S = cts ((struct c_word *) d_words->get (i));
			
			if (S == NULL)
				continue;
			
			if (d_donelist->sfind (S) != -1) 
				continue;
						
			if (! SB_FLGET (SB_FLLINEAR)) 			
				tree_mkpos (n_trip);
			
			d_donelist->add (strdup (S));
			tree_echo (n_trip, S);
					
			if ((SB_FLGET (SB_FLVERBOSE) || SB_FLGET (SB_FLLINEAR))
				 	&& !tree_immune (S)) {
				terminal_dump_tags (S, n_trip);
			}
			
			if (get_env()->full_list->sfind (S) == -1) {
				get_env()->full_list->add (strdup (S));
				if (!tree_immune (S))
					call_tree (S, n_trip+1);
			}
		}
		
		d_words->foreach ((Dfunc_t) free_cword);
		delete d_words;			
	}

	delete d_donelist;
	if (d_vars)
		d_vars->foreach (free);
	delete d_vars;
}

