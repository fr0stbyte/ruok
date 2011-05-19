/*
    jsonval - validates JSON files for well-formedness
    Copyright (C) 2007 Ben Spencer

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include "json_parser.h"
#include "json_scanner.h"

extern int yyparse(yyscan_t scanner);
int parse_and_report(const char *);

int parse_and_report(const char *filename)
{
	yyscan_t scanner;
	FILE *f;
	int err = 0;

	f = fopen(filename, "r");
	if(!f) {
	  perror(filename);
	  return 1;
	}

	yylex_init(&scanner);
	yyset_in(f, scanner);
	yyset_extra((void *) filename, scanner);
#ifdef DEBUG
	yyset_debug(1, scanner);
#endif

	switch(yyparse(scanner)) {
	case 0:
	  err = 0;
	  break;
	case 1:
	  err = 1;
	  break;
	case 2:
	  fprintf(stderr, "Out of memory\n");
	  err = 1;
	  break;
	}
	
	yylex_destroy(scanner);

	return err;
}

void yyerror(yyscan_t scanner, const char *error)
{
}
