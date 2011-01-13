#ifndef JPEGTRAN_H
#define JPEGTRAN_H
#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "transupp.h"		/* Support routines for jpegtran */
#include "jversion.h"		/* for version message */

static const char * progname;	/* program name for error messages */
static char * outfilename;	/* for -outfile switch */
static char * scaleoption;	/* -scale switch */
static JCOPY_OPTION copyoption;	/* -copy switch */
static jpeg_transform_info transformoption; /* image transformation options */

void select_transform (JXFORM_CODE transform);

int parse_switches (j_compress_ptr cinfo, int argc, char **argv,
		int last_file_arg_seen, boolean for_real);

int main_tran (int argc, char **argv, JStegoData stg);
#endif //JPEGTRAN_H
			   