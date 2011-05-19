ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

ruok_CXXFLAGS = $(libcurl_CFLAGS) $(libxml2_CFLAGS) -I$(top_srcdir)/libs
ruok_LDFLAGS = $(libcurl_LIBS) $(libxml2_LIBS) 
ruok_LDADD = ../libs/libjson.la

bin_PROGRAMS = ruok
ruok_SOURCES = main.cc options.cc options.h requestthread.cc requestthread.h responseprinter.cc responseprinter.h
