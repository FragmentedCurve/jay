
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 *
 */

#ifndef DEFINE_LOG_H
#define DEFINE_LOG_H

#ifndef DEFINE_THE_TT_H
struct tt_state_t;
#endif

namespace Log {
	int init ();
	int globals (char *file, int type, int size);
	int tt (tt_state_t * tt);
	int write (char *logName, char *logStr);
	int done (char *logName);
}

#endif

