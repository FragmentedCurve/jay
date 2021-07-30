/*
 * (c) Oleg Puchinin 2009
 * graycardinalster@gmail.com
 * 
 */ 

#include <stdio.h>

void usage ()
{
	printf ("usage:\t silentbob [<function>|<files>] [<options>]\n");
	printf ("\t options:\n"
			"\t\t -C <dir>\t\t-\tchdir to <dir>\n"
			"\t\t--cgrep\t\t\t-\tsearch in code\n"
			"\t\t--grep\t\t\t-\tsearch text\n"
			"\t\t--replace\t\t-\treplace text\n"
			"\t\t--call-tags\t\t-\tcreate \"call_tags\" file\n"
			"\t\t -f\t\t\t-\tcreate \"files\" file\n"
			"\t\t--ctags-append\t\t-\tdon't rewrite \"tags\" file\n"
			"\t\t--depth <arg>\t\t-\tset reentrant level\n"
			"\t\t--file <files>\t\t-\tshow file functions\n"
			"\t\t -fn\t\t\t-\techo filenames\n"
			"\t\t--structs\t\t-\tshow all structs in stream\n"
			"\t\t -i\t\t\t-\tinteractive mode \"tags\"\n"
			"\t\t--indent\t\t-\tindention\n"
			"\t\t -j\t\t\t-\tnumber of jobs\n"
			"\t\t--kinds\t\t\t-\t(c - classes, s - structs, f - functions,\n"
		   "\t\t\t\t\t\tm - modules, n - namespaces, p - packages)\n"
			"\t\t -L <file>\t\t-\tread files list from file\n"
			"\t\t -c\t\t\t-\tcreate \"tags\" file\n"
			"\t\t--tags\t\t\t-\taccess to tags\n"
			"\t\t--the-tt\t\t-\tC preprocessor\n"
			"\t\t -ts\t\t\t-\tforce ctags style\n"
			"\t\t -u\t\t\t-\treverse call-tree\n"
			);
	
	printf ("\nCONTACTS\nOleg Puchinin <graycardinalster@gmail.com>\n");
}
