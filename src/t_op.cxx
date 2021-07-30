/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

char t_op (char ** d_in, char ** d_prev, bool b_no_operators)
{
	bool b_instring = false;
	int brace_depth = 0; // for '(' !
	char *d_ptr = *d_in;
	int slash_count;
	char ch_last = 0;
	char ch = 0;
	
	if (*d_prev)
		*d_prev = *d_in;

	while (*d_ptr) {
		if (*d_ptr == '\'' || *d_ptr == '\"') {
			if (b_instring && *d_ptr != ch_last) {
				++d_ptr;
				continue; // Mmm...
			}
			
			if (b_instring) {
				if (d_ptr[-1] == '\\') {
					slash_count = 1;
					while (d_ptr [-(slash_count)] == '\\') 
							++slash_count;
				
					if (slash_count & 1) 
						b_instring = false;
				} else {
					++d_ptr;
					b_instring = false;
					continue;
				}
			} else {
				ch_last = *d_ptr;
				b_instring = true;
			}
		}
		
		if (b_instring) {
			++d_ptr;
			continue;
		}
	
		if (d_ptr[0] == '(')
			++brace_depth;

		if (d_ptr[0] == ')')
			--brace_depth;
	
		if (brace_depth < 0)
			brace_depth = 0;  
		
		if (b_no_operators && (*d_ptr == '{' || *d_ptr == '}' || *d_ptr == '\n')) {
			ch = *d_ptr;
			*d_ptr = 0;
			if (ch != '\n')
				++d_ptr;
			break;
		}

		if (d_ptr[0] == '{' ||
				((*d_ptr == ';') &&
				 (! brace_depth)) ||
				d_ptr[0] == '}' ||
				d_ptr[0] == '\n') {
			ch = *d_ptr;
			*d_ptr = 0;
			if (ch != '\n')
				++d_ptr; // Skip space
			break;
		}
		++d_ptr;
	}

	d_ptr++;
	*d_in = d_ptr;
	return ch;
}

