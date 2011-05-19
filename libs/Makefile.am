BUILT_SOURCES = json_parser.c json_scanner.c
AM_YFLAGS = -dt

json_scanner.c :
	flex json_scanner.l

clean-local :
	$(RM) json_scanner.c json_parser.c json_scanner.h json_parser.h

# include_HEADERS = json.h
noinst_LTLIBRARIES = libjson.la
libjson_la_SOURCES =  jsonmain.c json_parser.y json_scanner.l 
