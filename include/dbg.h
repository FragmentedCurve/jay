
/*
 * (c) Oleg Puchinin 2006
 * graycardinalster@gmail.com
 * 
 */ 

#ifndef DEFINE_DBG_H
#define DEFINE_DBG_H

#define dbg { fputc ('*', get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); }
#define dbg_if { fputc ('>', get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); }
#define dbg_else { fputc ('<', get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); }
#define dbg_s(ptr,str) { if (strstr (ptr,str)) { fprintf (get_env()->d_stream_dbg, "//SDBG %s %i %s\n", __FUNCTION__, __LINE__, ptr); \
    d_dbg_SDBG_active = true; } else { d_dbg_SDBG_active = false; } }
#define dbg_s2 { if (d_dbg_SDBG_active) { fputc ('*', get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); } }
#define dbg_sif { if (d_dbg_SDBG_active) { fputc ('>', get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); } }
#define dbg_selse { if (d_dbg_SDBG_active) { fputc ('<', get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); } }

#define LN { fputc ('\n', get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); }

#define THE_TT_DBG { fputc (T, get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); }
#define THE_TT_DBG_SPEC { fputc ('!', get_env()->d_stream_dbg); fputc (T, get_env()->d_stream_dbg); fflush (get_env()->d_stream_dbg); }
#define FDBG(arg) { fprintf (get_env()->d_stream_dbg, "%s", arg); fflush (get_env()->d_stream_dbg); }

#define assert(arga,argb) if(arga) { fprintf (get_env()->d_stream_dbg, "\tBUG at file %s line %i \"%s\"\n",  \
		__FILE__, __LINE__, argb); fflush (get_env()->d_stream_dbg); }

#define fault(arga) if(arga) { fprintf (get_env()->d_stream_dbg, "\tBUG at file %s line %i\n", __FILE__, __LINE__); exit (2); }

#define dbg_in { fprintf (get_env()->d_stream_dbg, "%s in\n", __FUNCTION__); fflush (get_env()->d_stream_dbg); }
#define dbg_out { fprintf (get_env()->d_stream_dbg, "%s out\n", __FUNCTION__); fflush (get_env()->d_stream_dbg); }

#endif
