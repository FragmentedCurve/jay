
/*
 * (c) Oleg Puchinin 2006,2007
 * graycardinalster@gmail.com
 * 
 */ 

void terminal_dump_tags (char * f_name, int n_trip) ;
void opMacro (char ** d_ptr, char ** d_out, char ch) ;
int c_begin_line (struct tt_state_t * d_tt_state, char *d_out, int d_offset) ;
int entry_point ();

#include <inline.h>

/* sblib */
bool def_test (char * d_op) ;
int what_is_this (char * d_op, char ch) ;
char * sstrend (char * d_ptr) ;
void free_cword (c_word * S) ;
inline struct c_word * new_cword (char *word, char sym) ;
DArray * split_to_words (char * d_op) ;
char * cts (struct c_word * d_word) ;
char * c_last_word (char *d_op) ;
char * ruby_last_word (char * op) ;
int words_count (char *S) ;
struct tt_state_t * get_onett_tag (char * f_name, char ** d_tt_buf) ;
int sblib_find (const char * path, const char * name, const char * f_outname) ;
int find_cfiles () ;
int split_tmp_files () ;
int remove_tmp_files () ;
void join_tmp_tags () ;

EHash * bob_init_compile_env () ;
bool validOption (char *Name) ;
bool builtin_language ();
int newPluginID () ;
int move_file (char * src, char * dest);
int language_id ();
int tabs2spaces (char * buf) ;

/**/

/*** ./cgrep.cxx ***/
namespace cgrep {
	bool init () ;
	int cgrep (char * fileName, FILE * of);
}

int replace ();

/*** ./flist.cxx ***/
char * flist_iter (char * d_ptr, char * d_name, bool b_repair) ;
void flist_main (char * d_file) ;

/*** ./globals.cxx ***/
void globals_main (struct tt_state_t * d_tt_state, int d_type) ;
void bob_globals (char * d_file, int d_type) ;

/*** ./kinds.cxx ***/
int kindsFile (char * fileName, int type) ;

/*** ./get_structs.cxx ***/
void get_structs (char * d_file) ;

/*** ./got_tag.cxx ***/
int parse_regexp (char * d_file, char * S) ;
bool bob_tag (char *d_str, char * d_name, d_tag_t * d_tag) ;
EArray * got_tag (char * d_tag) ;

/*** ./init.cxx ***/
int sb_init () ;

/*** ./main.cxx ***/
int sb_prname (char *arg) ;
int main (int argc, char ** argv) ;

/*** ./make_tags.cxx ***/
void mk_tags (char *f_name, DArray *d_in) ;
void make_ctags (EArray * d_files) ;

/*** ./mk_tag.cxx ***/
char * macro_name (char * d_op, char * d_macro_name) ;
char * mk_tag_macro (char * d_op, char * d_file, int t_line) ;
void mk_tag (char * d_op, char * d_file, int line, int d_found_type, FILE * of) ;

/*** ./modding.cxx ***/
env_t * sb_getenv (void) ;
char modding_optparse (int * i, int step) ;
int linux_modding_init () ;
void mods_short_info ();

/*** ./nogui_fdump.cxx ***/
bool brace_count (char * d_str, int * d_count, bool b_force_block) ;
void terminal_dump_function (struct fdump_param_t * d_param) ;

/*** ./nogui_indent.cxx ***/
void nogui_indent () ;

/*** ./tags.cxx ***/
void tags (DArray * d_names, char * d_file_output) ;

/*** ./interactive.cxx ***/
void tags_interactive () ;

/*** ./t_op.cxx ***/
char t_op (char ** d_in, char ** d_prev, bool b_no_operators = true) ;

/*** ./tree.cxx ***/
inline bool tree_immune (char * m_fname) ;
void rctree_iter (char * f_name, int n_trip) ;
void reverse_calltree (char * f_name) ;
void tree_echo (int n_trip, char *S) ;
inline void tree_mkpos (int N) ;
void call_tree (char * f_name, int n_trip) ;

int kinds (char * m_kinds) ;

/*** ./sb_prnmame.cxx **/
int sb_prname (char *arg) ;

int bob_cfiles ();
int logToFile (char * fileName, char * fmt, ...);

/*** src/searchSources.cxx ***/
EArray * searchSources ();

/**8 src/sources.cxx ***/
int sources () ;

/*** src/extensions.cxx ***/
EArray * extensions () ;
bool ext_check_c (char * ext) ;
bool ext_check_cxx (char * ext) ;
bool ext_check_perl (char * ext) ;
bool ext_check_python (char * ext) ;
bool ext_check_ruby (char * ext) ;
bool ext_check_php (char * ext) ;
bool ext_check_java (char * ext);
bool ext_check_lua (char * ext);
bool ext_check_go (char * ext);
int file_lang_id (char * f_name) ;

/*** src/bob_commands.cxx ***/
void bob_commands () ;
void bob_commands2 () ;
void bob_commands_nofiles ();

void cxx_print (tt_state_t * tt, char * d_out, int d_found_type) ;
int files ();

int cxx_ctags (char * f_name, FILE * of) ;
int perl_ctags (char * f_name, FILE * of) ;
int python_ctags (char * f_name, FILE * of) ; 
int ruby_ctags (char * f_name, FILE * of) ;
int php_ctags (char * f_name, FILE * of) ;
int java_ctags (char * f_name, FILE * of) ;
int go_ctags (char * f_name, FILE * of) ;
int lua_ctags (char * f_name, FILE * of) ;

int cache_load ();
int cache_free ();
char * editor_option (char *);
void try_editor ();
int bob_kinds ();


int call_tags_file (char * fileName, FILE * of);
int grepFile (char * fileName, FILE * of);
int ml_ctags (char * f_name, FILE *of);

