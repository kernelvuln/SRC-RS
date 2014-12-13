#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define OLOG_FILE "/path/file.log"
void logg(const char *fmt, ...);

void logg(const char *fmt, ...)
{
    FILE* olog;
    va_list ap;
    time_t t;
    struct tm* ctm;

    
    if (NULL == (olog = fopen(OLOG_FILE, "a+"))){
	fprintf(stderr, "cant's open log!\n");
	return ;
    }

    t = time(NULL);
    ctm = localtime(&t);
    fprintf(olog, "%4d-%02d-%02d %02d:%02d:%02d",
	    ctm->tm_year + 1900, ctm->tm_mon + 1, ctm->tm_mday,
	    ctm->tm_hour, ctm->tm_min, ctm->tm_sec);

    va_start(ap, fmt);
    vfprintf(olog, fmt, ap);
    va_end(ap);

    fclose(olog);
}
