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

%{
#include "json_parser.h"
#include "json_scanner.h"

extern void yyerror(yyscan_t, const char *);
%}

%defines
%error-verbose
%pure-parser
%parse-param {yyscan_t scanner}
%lex-param {yyscan_t scanner}

%token TSTRING
%token TNUMBER
%token TBOOL
%token TNULL
%token TINVALID

%%

text: object
    | array
;

object: '{' '}'
      | '{' members  '}'
;

members: member
       | member ',' members
;

member: TSTRING ':' value
;

array: '[' ']'
     | '[' elements ']'
;

elements: value
        | value ',' elements
;

value: TSTRING
     | TNUMBER
     | object
     | array
     | TBOOL
     | TNULL
;

