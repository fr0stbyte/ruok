BUILT_SOURCES = json_parser.c json_scanner.c
AM_YFLAGS = -dt

json_scanner.c :
	flex json_scanner.l
# include_HEADERS = json.h
noinst_LTLIBRARIES = libjson.la
libjson_la_SOURCES =  jsonmain.c json_parser.y json_scanner.l 
