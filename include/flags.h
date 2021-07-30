
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#ifndef BOB_FLAGS_H
#define BOB_FLAGS_H

enum {
	SB_FLVERBOSE = 0,
	SB_FLLINEAR,
	SB_FLNOLINKS,
	SB_FLSIMULATE,
	SB_FLTAGSTYLE,
	SB_FLFNAMES,
	SB_FLALL,
	SB_FLTEST,
	SB_FLRTREE,
	SB_FLCTAGSAPPEND,
	SB_FLTHREAD,
	SB_FLDEBUG
};

#define SB_FLSET(arg) (get_env()->sb_flags |= (1<<arg))
#define SB_FLGET(arg) (get_env()->sb_flags & (1<<arg))

#endif

