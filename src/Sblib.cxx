
/*
 * (c) Oleg Puchinin 2006,2007
 * graycardinalster@gmail.com
 *
 */

#include <gclib.h>
#include <env.h>
#include <head.h>
#include <wit.h>
#include <dbg.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <the_tt.h>
#include <cmds.h>

int sb_prname (char *arg)
{
	int i_cmd = 0;
	char * d_ptr;
	d_ptr = arg + strlen (arg);
	d_ptr--;


	while (*d_ptr != '\\' &&
			d_ptr > arg)
		d_ptr--;

	if (*d_ptr == '\\') 
		d_ptr++;
	
	if (EQ (d_ptr, "bob_cxx")) 
		get_env()->language = strdup ("C++");
	else if (EQ (d_ptr, "bob_perl"))
		get_env()->language = strdup ("Perl");
	else if (EQ (d_ptr, "bob_python"))
		get_env()->language = strdup ("Python");
	else if (EQ (d_ptr, "bob_ruby"))
		get_env()->language = strdup ("Ruby");

	if (EQ (d_ptr, "tags"))
		i_cmd = cmd_tags;
	else if (EQ (d_ptr, "the_tt"))
		i_cmd = cmd_the_tt;
	else if (EQ (d_ptr, "gc_indent"))
		i_cmd = cmd_indent;
	else if (EQ (d_ptr, "structs"))
		i_cmd = cmd_give_structs;

	return i_cmd;
}

char * cts (struct c_word * d_word)
{
	char * S;

	if (d_word == NULL)
		return NULL;

	S = d_word->S;
	if (!strncmp (S, "else ", 5))
		S += 5;
	
	if (d_word->ch != '(')
		return NULL;
	
	while (!strncmp (S, "do ", 3))  
		S += 3;
	
	if (!strncmp (S, "return ", 7))
		S += 7;
	
	if (get_env()->d_cops->sfind (S) != -1)
		return NULL;
	
	if (words_count (S) != 1)
		return NULL;
	
	return S;
}

bool def_test (char * d_op)
{
	char * S = d_op;
	++S;
	while (*S == ' ' || *S == '\t')
		++S;

	if (! strncmp (S, "define", 6))
		return true;
	return false;
}

int find_cfiles ()
{
	find_one_ext ((char *) "*.h");
	find_one_ext ("*.hpp");
	find_one_ext ("*.cpp");
	find_one_ext ("*.c");
	find_one_ext ("*.cc");
	find_one_ext ("*.cxx");
        return 0;
}

int sblib_find (const char * path, const char * name, const char * f_outname)
{
	int devnull;
	int fd;
	int pid;
	int status = -1;

	pid = fork ();
	if (pid == 0) {
		devnull = open ("/dev/null", O_WRONLY, 0600);
		fd = open (f_outname, O_WRONLY, 0600);
		if (fd == -1) {
			fd = open (f_outname, O_WRONLY | O_CREAT, 0600);
			if (fd == -1) {
				close (devnull);
				return -1;
			}
		} else
			lseek (fd, 0, SEEK_END);
		dup2 (devnull, 2);
		dup2 (fd, 1);
		execlp ("find", "find", "-name", name, NULL);
	} else if (pid > 0) {
		waitpid (pid, &status, 0);
		return status;
	}

	return status;		
}

# define TMP_FILE_NAME (char *) "./silent_bob.tmp"
struct tt_state_t * get_onett_tag (char * f_name, char ** d_tt_buf) 
{
	DArray d_array;
	tt_state_t * Ret = NULL;
	char * S;

	if (d_tt_buf)
		*d_tt_buf = NULL;	
	
	d_array.add (f_name);
	Ret = CNEW (tt_state_t, 1);

	unlink (TMP_FILE_NAME);
	tags (&d_array, TMP_FILE_NAME);

	while( 1 ) {
		if (access (TMP_FILE_NAME, R_OK) != 0)
			break;
	
		Ret->fileName = strdup (TMP_FILE_NAME);
		S = THE_TT::do_tt_file (Ret);
	
		if (S == NULL)
			break;
	
		if (Ret->mmaped)
			munmap (Ret->fileData, Ret->fileDataSize);

		if (Ret->fd)
			close (Ret->fd);

		if (d_tt_buf)
			*d_tt_buf = S;

		unlink (TMP_FILE_NAME);
		return Ret;
	}

	DROP (Ret);
	unlink (TMP_FILE_NAME);
	return NULL;	
}

#define NOT_VALID 0
#define CALL_BACK 1
#define VALID 2

EArray * got_tag (char * d_tag)
{
	struct d_tag_t *d_new_tag;
	char d_buf[256];
	EArray * d_ret;
	int d_len;
	char *S;
	int i;

	d_ret = new EArray;

	if (get_env()->d_tags_file == NULL) {
		get_env()->d_tags_file = new EArray;
		get_env()->d_tags_file->from_file ((char *) "./tags");	
	}

	snprintf (d_buf, 255, "%s\t", d_tag);
	d_len = strlen (d_buf);

	if (get_env()->d_tags_file->get_size () == 0)
		return d_ret;
	
	i = get_env()->d_tags_file->snfind_fast (d_buf, strlen (d_buf));
	
	if (i == -1) {
		fprintf (get_env()->d_stream_dbg, "\tget_env()->d_tags_file->snfind_fast == -1");	LN;
		return d_ret;
	}
	
	d_len = strlen (d_buf);
	do {
		i++;
		S = get_env()->d_tags_file->get (i);
		if (! S)
			break;
	} while (! strncmp (S, d_buf, d_len));
	--i;
	
	while (true) {
		S = get_env()->d_tags_file->get (i);
		fprintf (get_env()->d_stream_dbg, "\ttag : %s\n", S); fflush (get_env()->d_stream_dbg);

		d_new_tag = CNEW (d_tag_t, 1);
		memset (d_new_tag, 0, sizeof (d_tag_t));
		if (strstr (S, ";\tf"))
			d_new_tag->d_type = OT::Function;

		if (bob_tag (S, d_tag, d_new_tag) == false) {
			DROP (d_new_tag);
			fprintf (get_env()->d_stream_dbg, "\tBAD tag : %s\n", S);
			return d_ret;
		}
		
		d_ret->add ((long) d_new_tag);
		
		i--;
		if (i < 0)
			break;
		
		S = get_env()->d_tags_file->get (i);
		if (strncmp (S, d_tag, strlen (d_tag)))	 	
			break;
	}	

	return d_ret;
}

void mk_tags (char *f_name, DArray *d_in)
{
	DHeapSort * heap;
	DArray * d_array = NULL;
	char *S;
	FILE * my_file;
	int d_size;
	int i;

	if (d_in == NULL) {
		d_array = new DArray (1024);
		d_array->from_file (get_env()->tmp_tags);
	} else 
		d_array = d_in;

	d_size = d_array->get_size ();
	heap = new DHeapSort (d_size);

	for (i = 0; i < d_size; ++i) 
		heap->add (d_array->get (i));

	my_file = fopen (f_name, "w");
	if (my_file == NULL) {
		fprintf (stderr, "file %s:\n", f_name);
		perror ("fopen");
		return;
	}

	fprintf (my_file, "!_TAG_FILE_FORMAT\t2\n");
	fprintf (my_file, "!_TAG_FILE_SORTED\t1\n");
	fprintf (my_file, "!_TAG_PROGRAM_AUTHOR\tOleg Puchinin (graycardinalster@gmail.com)\n");
	fprintf (my_file, "!_TAG_PROGRAM_NAME\tSilent Bob\n");
	fprintf (my_file, "!_TAG_PROGRAM_URL\thttp://sf.net/projects/silentbob\n");
	fprintf (my_file, "!_TAG_PROGRAM_VERSION\t2.0\n");

	while ((S = heap->extract_min ()) && S) 
		fprintf (my_file, "%s", S);
		
	if (d_in == NULL) {
		d_array->foreach (free);
		delete d_array;
	}

	fclose (my_file);
	delete heap;
}

bool b_in_comment;

bool brace_count (char * d_str, int * d_count, bool b_force_block) // "nice"
{
	bool Ret = false;

	if (! d_str || ! d_count)
		return false; 
	
	while (*d_str != 0) {	
		if (!strncmp (d_str, "/*", 2)) {
			b_in_comment = true;
			d_str += 2;
			continue;
		}
		
		if (b_in_comment) {
			if (strncmp (d_str, "*/", 2)) {
				d_str ++;
				continue;
			} else {
				d_str += 2;
				b_in_comment = false;
				continue;
			}
		}		
		
		if (!strncmp (d_str, "//", 2)) 
			break;

		if (*d_str == '\"' || *d_str == '\'') {
			d_str = sstrend (d_str);
			if (d_str == NULL || *d_str == 0) {
				assert (true, "HimTeh 4");
				break;
			}
		}
		
		if (*d_str == '{') {
			Ret = true;
			(*d_count)++;
		}
		
		if (*d_str == '}') {
			Ret = true;
			(*d_count)--;
		}

		if (*d_str == ';' && *d_count == 0 && !b_force_block) {
			Ret = true;
			break;
		}
		d_str++;
	}
	
	return Ret;
}

void opMacro (char ** d_ptr, char ** d_out, char ch)
{
	char * d_my;
	int n = 0;
	
	d_my = *d_out;
	while (true) {
		if (ch == '\n')
			n++;

		if ((ch == '\n') && (d_my[strlen (d_my) - 1] != '\\'))
			break;
//		if (n > 300) 
//			break;
		ch = t_op (d_ptr, d_out);
		get_env()->t_op_no++;		
		if (ch == 0)
			break;
		d_my = *d_out;
	}
}

void join_tmp_tags ()
{
	char m_buf[512];
	int i;

	unlink (get_env()->tmp_tags);
	for (i = 0; i < get_env()->max_proc; ++i) {
		sprintf (m_buf, "cat %s%i >>%s", get_env()->tmp_tags, i, get_env()->tmp_tags);
		if( system (m_buf) == 1) {
			perror ("Jeany lose boy code 513");
		}
	}
}

DArray * split_to_words (char * d_op)	
{
	DArray * d_Ret = new DArray (16);
	char * d_old = strdup (d_op);
	bool b_done = false;
	char * S = d_old;
	char * d_end;
	char ch;
	
	if (d_Ret == NULL || d_old == NULL)
		return NULL;
	
	while (true) {
		b_done = false;
		d_end = S;
		
		if (*d_end == ' ')
			d_end++;
		
		while (*d_end) {
			if (!(if_abc(d_end) ||
				  if_digit (d_end) ||
				  *d_end == '_' || 
				  *d_end == ' ') )
				break;
			d_end ++;
		}

		if (! *d_end) {
			ch = 0;
			b_done = true;
			goto split_to_words_L1;
		}

		ch = *d_end;
		if (d_end[-1] == ' ')
			d_end[-1] = 0;
		else 
			*d_end = 0;

		while (*S && *S == ' ')
			S++;
		
split_to_words_L1:
		d_Ret->add (LPCHAR(new_cword (S, ch)));

		if (b_done)
			break;
		
		if (ch == '\"' || ch == '\'') {
			*d_end = ch;
			d_end = sstrend (d_end);
			assert (d_end == NULL, "Lena 1");
			if (*d_end == '\0' || *(++d_end) == '\0')
				break;
		}
		
		S = d_end + 1;
	}

	DROP (d_old);	
	return d_Ret;
}

char * sstrend (char * d_ptr)
{
	bool t_instring = false;
	int d_slash_count;
	char ch_last;
	unsigned limit = 1024;
	
	if (! d_ptr)
		return (char *) 0;

	if (!(*d_ptr))
		return (char *) 0;
	
	ch_last = *d_ptr;
	limit--;
	while (*d_ptr && (limit > 0)) {
		if (*d_ptr == '\'' || *d_ptr == '\"') {
			if (t_instring && *d_ptr != ch_last) {
				d_ptr++;
				continue; // Mmm...
			}
			
			if (t_instring) {
				if (d_ptr[-1] == '\\') {
					d_slash_count = 1;
					while (d_ptr [-(d_slash_count)] == '\\')
							d_slash_count++;
				
					if (d_slash_count & 1) 
						t_instring = false;
				} else {
					d_ptr++;
					t_instring = false;
					continue;
				}
			} else {
				ch_last = *d_ptr;
				t_instring = true;
			}
		}
		
		if (t_instring)	{
			d_ptr++;
			continue;
		} else
			break;
	}

	d_ptr --;
	
	if (*d_ptr == 0)
		return 0;

	return d_ptr;
}

char * sstrkill (char *OP)
{
	char *S;
	char *tail;

	if (! OP)
		return NULL;

	S = OP;
	while (*S) {
		if (*S == '\"' || *S == '\'') {
			tail = sstrend (S);
			if (! tail)
				break;

			if (*tail == '\0' ||*(tail+1) == '\0') {
				*S = '\0';
				break;
			}

			++S;
			strcpy (S, tail);
		}
		++S;
	}
	
	return OP;
}

int words_count (char *S)
{
	bool b_begin = true;
	int d_ret = 0;

	if (S == 0)
		return 0;
	
	while (*S) {
		if (*S == ' ') {
			b_begin = true;
			S++;
			continue;
		}
				
		if (b_begin) {
			if (if_abc (S) ||
					(*S == '_') ||
					(*S == '*') ||
					(*S == '&')) {
				S++; 
				d_ret ++; 
				b_begin = false;
				continue;
			} else
				break;
		} else {
			if (!(if_abc (S) || (*S == '_') 
				|| (*S == '*') || (if_digit (S))))
				break;
		}
		
		S++;
		b_begin = false;
	}

	return d_ret;	
}

int c_begin_line (struct tt_state_t * d_tt_state, char *d_out, int d_offset)
{
	char *S = &d_tt_state->fileData [d_offset] - 1;
	char * d_end = &d_out[strlen (d_out)] - 1;
	int Ret = 0;

	while (d_end > d_out) {		
		if (*d_end == ' ' || *d_end == '\t') {
			while ((S >= d_tt_state->fileData) && (*S == ' ' || *S == '\t'))
				--S;
			
			if (S < d_tt_state->fileData)
				return Ret;

			while ((d_end >= d_out) && (*d_end == ' ' || *d_end == '\t'))
				--d_end;

			if (d_end <= d_out)
				return Ret;

			continue;
		}
		
		if (*S == '\n')
			--Ret;

		if (*S == *d_end)
			--d_end;

		--S;
	}

	return Ret;
}

char * c_last_word (char *d_op)
{
	char * S = d_op;
	char * d_word;
	
	while (*S) {
		if (*S == '(' || *S == '=' || *S == '[')
			break;
		S++;
	}

	if (S[-1] == ' ')
		S--;

	*S = 0;
	d_word = d_op;
	while (true) {
		S = strchr (d_word, ' ');
		if (S == NULL)
			break;
		d_word = S+1;
	}

	while (*d_word == '*' ||
			*d_word == '&' ||
			*d_word == ' ')
		d_word++;
	
	return d_word;
}

char * ruby_last_word (char * op)
{
	char * d_word;
	char * S;
	
	S = op;
	while (*S) {
		if (*S == '(' || *S == '=' || *S == '[' || *S == '<')
			break;
		S++;
	}

	while (S[-1] == ' ')
		S--;

	*S = 0;
	d_word = op;
	while (true) {
		S = strchr (d_word, ' ');
		if (S == NULL)
			break;
		d_word = S+1;
	}

	return d_word;
}

bool validOption (char *Name)
{
	char *S;
	if (! Name)
		return false;

	S = get_env()->listOptions->first ();
	while (S) {
		if (EQ (Name, S))
			return true;
		S = get_env()->listOptions->next ();
	}
	return false;
}

bool builtin_language ()
{
	if (EQ (get_env()->language, "C") ||
			EQ (get_env()->language, "C++") ||
			EQ (get_env()->language, "Perl") || 
			EQ (get_env()->language, "Python") ||	
			EQ (get_env()->language, "Ruby") ||
	  		EQ (get_env()->language, "PHP") || 
			EQ (get_env()->language, "Java") ||
			EQ (get_env()->language, "Go")  ||
			EQ (get_env()->language, "Lua"))
		return true;
	return false;
}

int lastPluginID = 0;
int newPluginID ()
{
	return ++lastPluginID;
}

int move_file (char * src, char * dest)
{
	char m_buf[512];
	if ((! src) || (! dest))
		return -1;
	sprintf (m_buf, "mv %s %s", src, dest);
	return system (m_buf);
}

int language_id ()
{
	int lang = 0;

	if (EQ (get_env()->language, "ALL"))
		lang = LALL;
	else if (EQ (get_env()->language, "C") || EQ (get_env()->language, "C++")) 
		lang = LCXX;
	else if (EQ (get_env()->language, "Perl")) 
		lang = LPERL;
	else if (EQ (get_env()->language, "Python"))
		lang = LPYTHON;
	else if (EQ (get_env()->language, "Ruby"))
		lang = LRUBY;
	else if (EQ (get_env()->language, "PHP"))
		lang = LPHP;
	else if (EQ (get_env()->language, "Java"))
		return LJAVA;
	else if (EQ (get_env()->language, "Go"))
		return LGO;
	else if (EQ (get_env()->language, "Lua"))
		return LLUA;

	return lang;
}

int tabs2spaces (char * buf)
{
	if (! buf)
		return -1;

	while (*buf) {
		if (*buf == '\t')
			*buf = ' ';
		++buf;
	}
	return 0;
}

