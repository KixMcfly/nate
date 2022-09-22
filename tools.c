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

//void
//log_print_map (MAP *m)
//{
//	int x, y;
//	int nl = map_get_nl (m);
//	int w = map_get_w (m);
//	int h = map_get_h (m);
//	FILE *ol = fopen ("log.txt", "a");
//	
//	for (y = 0; y < h; y++)
//	{
//		for (x = 0; x < w; x++)
//		{
//			fprintf (ol, "%02x ", map_get_tile_flags (m, 0, x, y));
//		}
//		
//		fprintf (ol, "\n");
//	}
//	
//	fclose (ol);
//}

char *
strtmp (char *str)
{
	static char tmp[100];
	
	strcpy (tmp, str);
	
	return tmp;
}
