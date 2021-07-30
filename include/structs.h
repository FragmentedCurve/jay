
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

#ifndef DEFINE_STRUCTS_H
#define DEFINE_STRUCTS_H
struct d_tag_t
{
	char d_name[256];
	char d_file[256];
	int d_op;
	int d_line;
	int d_type; // Minimal support.	
};

struct pair_t
{
	int pair_op;
	int pair_line;
	int offset;
};

struct tt_state_t 
{
	char *fileName;
	int fd;
	char * fileData;
	int fileDataSize;
	bool mmaped;
	char * result;
	int resultSize;	
	pair_t * attachment;
};

struct fdump_param_t 
{
	int n_trip;
	int d_line;
	bool linear;
	bool b_force_block;
	char * d_file_name; 
	char *d_file_output;
};

struct c_word
{
	char * S;
	char ch;
};

void free_cword (c_word *);

enum {
	LALL = 1,
	LCXX,
	LPERL,
	LPYTHON,
	LRUBY,
	LPHP,
	LJAVA,
	LGO,
	LLUA
};

#endif
