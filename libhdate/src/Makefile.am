libhdate_la_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = libhdate.la

libhdate_la_SOURCES = \
	hdate_strings.c\
	hdate_julian.c\
	hdate_holyday.c\
	hdate_parasha.c\
	hdate_sun_time.c\
	hdate.h\
	support.h

libhdate_la_LDFLAGS = -version-info $(VERSION_INFO)

libhdate_la_LIBADD = 

include_HEADERS = hdate.h hdatepp.h

