/*
    This file is part of ruok - a program that measures timings of transferring data with URL syntax
    Copyright (C) 2011 by Radu Brumariu [brum76@gmail.com]
    
    ruok is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ruok is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ruok.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include "json_parser.h"
#include "json_scanner.h"

extern int yyparse(yyscan_t scanner);
int parse_and_report(FILE *f);

int parse_and_report(FILE *f)
{
	yyscan_t scanner;
	int err = 0;

	if(!f) {
	  perror("failed");
	  return 1;
	}

	yylex_init(&scanner);
	yyset_in(f, scanner);
	//	yyset_extra((void *) filename, scanner);
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
