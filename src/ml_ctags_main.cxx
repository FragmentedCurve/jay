/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#include <head.h>

int ml_ctags (char * S, FILE * of)
{
	switch (file_lang_id (S)) {
		case LCXX:
			cxx_ctags (S, of); 
			return 0;
		case LPERL:
			perl_ctags (S, of);
			return 0;
		case LPYTHON:
			python_ctags (S, of);
			return 0;
		case LRUBY:
			ruby_ctags (S, of);
			return 0;
		case LPHP:
			php_ctags (S, of);
			return 0;
		case LJAVA:
			java_ctags (S, of);
			return 0;
		case LGO:
			go_ctags (S, of);
			return 0;
		case LLUA:
			lua_ctags (S, of);
			return 0;
	}
	return -1;
}

