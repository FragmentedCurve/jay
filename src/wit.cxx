
#include <head.h>
#include <wit.h>

int what_is_this (char * d_op, char ch)
{
	bool b_local_ftest;
	int d_words_count;
	char d_last_ch;
	char * S;
	int Ret = 0;

	if (*d_op == '#' || ch == '\n') {
		if (def_test (d_op))
			return OT::Define;	
		return OT::Macro;
	}

	d_words_count = words_count (d_op);

	if (words_count <= 0)
		return OT::Other;
	
	if (d_words_count == 1) {
		if (ww_call_cmp (d_op, (char *) "if", 2) ||
				ww_call_cmp (d_op, (char *) "else", 4) ||
				ww_call_cmp (d_op, (char *) "do", 2) ||
				ww_call_cmp (d_op, (char *) "while", 5) ||
				ww_call_cmp (d_op, (char *) "switch", 6) ||
				ww_case_cmp (d_op, (char *) "case", 4))
			return OT::Operator;

		if (ww_after_word (d_op) == '(')
			return OT::Call;

		return OT::Other; // Macro or operations (e.g. "+-=*/%^" etc...)		
	}

	if (! strncmp (d_op, "else if ", 8))
		return OT::Operator;
	if (! strncmp (d_op, "class ", 6) && ch == ';') // class definition
		return OT::Other;

	d_last_ch = last_ch (d_op);
	if (d_last_ch == '=')
		return OT::Variable;
	
	b_local_ftest = local_ftest (d_op);
	if ((ch == '{' && d_last_ch == ')')) {
		if (b_local_ftest) // Paranoid.
			return OT::Function;
	}

	if (! strncmp (d_op, "extern ", 7))
		return OT::Extern;
	
	if (ch == '{') {
		Ret = 0;
		if (! strncmp (d_op, "typedef ", 8)) {
			Ret |= OT::Typedef;
			d_op += 8;
		}
			
		if (! strncmp (d_op, "static ", 7))
			d_op += 7;
		
		if (! strncmp (d_op, "const ", 6)) // "static const struct"
			d_op += 6;
				
		if (! strncmp (d_op, "union ", 6))
			return Ret | OT::Other; 

		if (! strncmp (d_op, "enum ", 5))
			return Ret | OT::Other; 
		
		if (! strncmp (d_op, "struct ", 7)) 
			return Ret | OT::Struct;

		if (! strncmp (d_op, "class ", 6))
			return OT::Class;

		if (! strncmp (d_op, "namespace ", 10))
			return OT::Namespace;

		if ((words_count (d_op) > 1) && !b_local_ftest)
			return OT::Variable;
		
		if (Ret)
			return Ret;
		return OT::Other;
	}
	
	if (ch == ';') {
		if (!strncmp (d_op, "typedef ", 8))
			return OT::Typedef;

		if (b_local_ftest) {
			S = strchr (d_op, '(');
			if (! S)
				return OT::Other;
			
			S++;
			if (words_count (S) <= 1) {
				S = strchr_r (S, ')', 0);
				S++;
				if (words_count (S) > 1)
					return OT::Other; // declaration... or not ?
			}
		} else {
			if (d_words_count <= 1)
				return OT::Other;

			if (!strncmp (d_op, "struct ", 7) && d_words_count == 2)  
				return OT::Other;
			
			if (!strncmp (d_op, "return ", 7))
				return OT::Other;
			
			if (!strncmp (d_op, "delete ", 7))
				return OT::Other;
			
			return OT::Variable;
		}
		// Function defenition, callback defenition... it's all ?
	}

	return OT::Other;
}

