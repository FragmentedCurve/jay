
/*
 * (c) Oleg Puchinin March 2012
 * graycardinalster@gmail.com
 *
 */

#include <head.h>

void bug_longmacro ()
{
	printf ("Too big macro."
			"If your macro have more than 300 lines, please "
			"contact <graycardinal@pisem.net>\n"
			"Program stopped.\n");
	
	exit (0);
}

void bug_nosuch_tag (char * f_name)
{
	printf ("Tag \"%s\" not found. Broken \"tags\" file ? "
			"Try \"silent-bob --make-ctags\".\n", f_name);
}

void bug_nocalltags ()
{
	printf ("File \"call_tags\" not found. "
			"Try \"silent-bob --call-tags [-L] <files>\"\n");
	exit (1);
}

void bug_system ()
{
	printf ("Can't make tags file. Maybe you do not have write permissions ?\n");
	exit (1);
}

void bug_fork ()
{
	perror ("fork");
	exit (1);
}

void bug_plugin (char *name)
{
	fprintf (stderr, "Can't load plugin (%s)\n", name);
}

void bug_notsupported ()
{
	printf ("SilentBob (or language plugin)"
	       " don't support this feature !\n");
}

bool bob_tag (char *d_str, char * d_name, d_tag_t * d_tag)
{
	char m_buf[256];
	char *d_file;
	char *S;

	strcpy (m_buf, d_str);
	d_str = m_buf;
	
	strncpy (d_tag->d_name, d_name, 255);
	d_tag->d_name[255] = 0;
	S = strchr (d_str, '\t');
	if (! S)
		return false;

	S++;
	d_file = S;
	S = strchr (d_file, '\t');
	if (! S)
		return false;
	
	*S = 0;
	strncpy (d_tag->d_file, d_file, 255);
	d_tag->d_file[255] = 0;
	
	S++;

	if (if_digit (S)) 
		d_tag->d_line = atoi (S);
	else 
		return false;

	return true;
}

