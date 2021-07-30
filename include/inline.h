
#ifndef DEFINE_INLINE_H
#define DEFINE_INLINE_H

inline bool ww_call_cmp (char * d_op, char * d_cmp, int d_len)
{
	if (!strncmp (d_op, d_cmp, d_len)
			&& (d_op[d_len] == '(' || d_op[d_len+1] == '('))
		return true;

	return false;
}

inline bool ww_case_cmp (char * d_op, char * d_cmp, int d_len)
{
	if (!strncmp (d_op, d_cmp, d_len) 
			&& (d_op[d_len] == ':' || d_op[d_len+1] == ':'))
		return true;

	return false;
}

inline bool local_ftest (char *S) // test for pair "()"
{
	int d_count1 = 0;
	int d_count2 = 0;
	
	while (*S) {
		if (*S == '(')
			d_count1++;
		
		if (*S == ')')
			d_count2++;
		
		if (!d_count1 || (d_count1 == d_count2)) {
			if (*S == '[')
				return false;
			if (*S == ']')
				return false;
			if (*S == '=')
				return false;
		}
		S++;
	}

	return d_count1 && d_count2;
}

inline char last_ch (char *S)
{
	S = &S[strlen (S)] - 1;
	while (if_digit (S) || if_abc(S) || *S == '_')
		S--;
	if (*S == ' ')
		S--;
	return *S;
}

inline char ww_after_word (char *S)
{
	while (true) {
		if (! (if_abc (S) || *S =='_' || if_digit (S)))
			break;
		S++;		
	}
	if (*S == ' ')
		S++;

	return *S;
}

inline void free_cword (c_word * word) 
{
	free (word->S);
	free (word);
}

inline struct c_word * new_cword (char *word, char sym) 
{
	c_word * Ret = CNEW (c_word, 1);
	Ret->S = strdup (word);
	Ret->ch = sym;
	return Ret;
}

inline char * ww_first_word (char * word) 
{
	char *S = word;

	while (true) {
		if (!if_abc (S) &&
			       	!if_digit (S) &&
				*S != '_')
			break;
		S++;
	}

	return Dstrndup (word, S-word);
}

inline int ww_nsyms_r (char * begin, char * end, char sym)
{
	char * S;
	int count = 0;
	S = end;
	while (S != begin && *S == sym) {
		++count;
		--S;
	}
	return count;
}

#define find_one_ext(ext) sblib_find ((char *) ".", (char *) ext, get_env()->tmp_files)

#endif

