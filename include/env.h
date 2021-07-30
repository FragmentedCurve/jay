
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

#ifndef DEFINE_ENV_H
#define DEFINE_ENV_H

struct env_t
{
	EArray * d_tags_file; // tags array
	EArray * immune_list;
	EArray * full_list;
	EArray * modding;
	EArray * d_cops;
	DArray * d_opts;
	EArray * d_files;
	DJobs * proc_list;
	EList * listOptions; // available options
	
	FILE *d_stream_dbg;
	
	char * home_dir;
	char * tmp_files;
	char * tmp_tags;
	char * debug_file;
	char * home_plugins;
	char * shared_plugins;
	char * language;
	char * editor;
	char * cgrep_exp;
	char * grep_exp;
	char * replace_exp;

	int t_op_no;
	int d_depth;
	int cgrep_A;
	int cgrep_B;
	int sb_cmd;
	int max_proc;
	int nproc;
	char * d_kinds;
	uint32_t sb_flags;

	char * file_name;
};

env_t *get_env ();
#endif
