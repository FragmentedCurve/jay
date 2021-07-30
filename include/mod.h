/* 
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_MOD_H
#define DEFINE_MOD_H

struct mod_t {
	int Type;
	int ID;
	char * Version;
	char * Name;
	char Enabled;
        char * mod_file;	
	void (*short_info) ();
	void (*long_info) ();
	void (*usage) ();
	char (*opt) (DArray * d_opts, int * pos);
	char (*opt2) (DArray * d_opts, int * pos);
	char *language;
	char *(*the) (struct tt_state_t *d_tt_state); // --the-tt
	int (*make_ctags) (char *f_name, FILE *ofile); // SilentBob --make-ctags code body.
	int (*call_tags) (char *f_name); // SilentBob --call-tags code body.
	int (*print_tags) (); // tags <tag>
	void (*file) (char * f_name); // bob <files> --file
	bool internal;
};

int modding_init ();
int modding_start (int i_cmd);
int modding_load_plugin (char * name, char * path);
void mod_plugin_info (int ID);
void plugins_viewer ();

#endif

