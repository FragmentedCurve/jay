/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 *
 */

#include <head.h>

const char * l_ext [] = { "c", "cxx", "cpp", "h", "hpp", "pl", "pm", "ph", "py",  "rb", "php", 
						"go","java", "lua", NULL }; 
EArray * extensions ()
{
	EArray * ret = new EArray(10);
	int i;

	i = 0;
	while (l_ext[i]) {
		(*ret) << strdup (l_ext[i]);
		++i;
	}

	return ret;
}

bool ext_check_c (char * ext)
{
	if (EQ (ext, "c"))
		return true;
	return false;
}

bool ext_check_cxx (char * ext)
{
	if (EQ (ext, "h"))
		return true;
	if (EQ (ext, "hpp"))
		return true;
	if (EQ (ext, "cxx"))
		return true;
	if (EQ (ext, "cpp"))
		return true;
	return false;
}

bool ext_check_perl (char * ext)
{
	if (EQ (ext, "pl"))
		return true;
	if (EQ (ext, "pm"))
		return true;
	if (EQ (ext, "ph"))
		return true;
	return false;
}

bool ext_check_python (char * ext)
{
	if (EQ (ext, "py"))
		return true;
	return false;
}

bool ext_check_ruby (char * ext)
{
	if (EQ (ext, "rb"))
		return true;
	return false;
}

bool ext_check_php (char * ext)
{
	if (EQ (ext, "php"))
		return true;
	return false;
}

bool ext_check_go (char * ext)
{
	if (EQ (ext, "go"))
		return true;
	return false;
}

bool ext_check_java (char * ext)
{
	if (EQ (ext, "java"))
		return true;
	return false;
}

bool ext_check_lua (char * ext)
{
	if (EQ (ext, "lua")) 
		return true;
	return false;
}

int file_lang_id (char * f_name)
{
	char *S;
		
	if (! f_name)
		return 0;

	S = fext (f_name);
	if (! S)
		return 0;
	++S;

	if (ext_check_c (S) || ext_check_cxx (S))
		return LCXX;
	if (ext_check_perl (S))
		return LPERL;
	if (ext_check_python (S))
		return LPYTHON;
	if (ext_check_ruby (S))
		return LRUBY;
	if (ext_check_php (S))
		return LPHP;
	if (ext_check_java (S))
		return LJAVA;
	if (ext_check_go (S))
		return LGO;
	if (ext_check_lua (S))
		return LLUA;

	return 0;
}

