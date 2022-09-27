#include "tools.h"

void
log_print (char *log_o, ...)
{
	FILE *ol = fopen ("log.txt", "a");
	va_list ap;
	time_t now = time (NULL);
	struct tm tm = *localtime (&now);
	
	
	fprintf (ol, "%02d-%02d-%d %02d:%02d:%02d> ", 
		tm.tm_mon+1, tm.tm_mday, tm.tm_year + 1900,
		tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	va_start(ap, log_o);
	vfprintf (ol, log_o, ap);
	va_end (ap);
	
	fclose (ol);
	
}

char *
strtmp (char *str)
{
	static char tmp[100];
	
	strcpy (tmp, str);
	
	return tmp;
}
