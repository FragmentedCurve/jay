
/*
 * (c) Oleg Puchinin 2012
 * graycardinalster@gmail.com
 *
 */

#include <head.h>

char * strreplace (char * where, const char * from, const char * to)
{
	char * S;
	char * end;
	int len;

	if (!where || !from || !to)
		return NULL;

	if (EQ (from, to))
		return where;

	len = strlen (from);
	while (true) {
		S = strstr (where, from);
		if (! S)
			break;
		end = S + len;
		end = strdup (end);
		strcpy (S, to);
		strcat (where, end);
		free (end);
	}

	return where;
}

int replace_file (char * f_name, char * from, char * to)
{
	FILE * fi;
	FILE * fo;
	char * m_buf;
	char * fo_name;
	
	fi = fopen (f_name, "r");
	if (! fi)
		return -1;

	m_buf = CNEW(char, 512);
	strcpy (m_buf, f_name);
	strcat (m_buf, ".tmp");

	fo = fopen (m_buf, "w");
	fo_name = strdup (m_buf);

	if (! fo) {
		DROP (m_buf);
		fclose (fi);
		return -1;
	}

	while (fgets (m_buf, 512, fi)) {
		strreplace (m_buf, from, to);
		fprintf (fo, "%s", m_buf);
	}
	
	fclose (fi);
	fclose (fo);
	unlink (f_name);
	move_file (fo_name, f_name);
	DROP (fo_name);
	DROP (m_buf);

	return 0;
}

int replace ()
{
	int i;
	char *S;
	int count;
	char m_buf[256];

	strcpy (m_buf, get_env()->replace_exp);
	S = strchr (m_buf, ',');
	if (! S)
		return -1;
	*S = '\0';
	++S;
	count = get_env()->d_files->get_size ();
	if (count <= 0) {
		fprintf (stderr, "No such files.\n");
		return -1;
	}
	for (i = 0; i < count; ++i) 
		replace_file (get_env()->d_files->get (i), m_buf, S);

	return 0;
}


