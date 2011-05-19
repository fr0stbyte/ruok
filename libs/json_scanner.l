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
%}
%option outfile="json_scanner.c" header="json_scanner.h"
%option 8bit reentrant bison-bridge
%option yylineno noyywrap nounput
%option warn nodefault


%x str

%%
	/* Whitespace */
[ \t\n]

	/* Punctuation */
[\[\]\{\}:,] return (unsigned char) yytext[0];

	/* Numbers */
0|-?([1-9][0-9]*(\.[0-9]+)?|0\.[0-9]+)([Ee][+-]?[0-9]+)? return TNUMBER;

	/* Strings */
\"	BEGIN str;
<str>[^\"\\[:cntrl:]]+
<str>\\u[0-9A-Fa-f]{4}
<str>\\[\"\\/bfnrt]
<str>\" {BEGIN(INITIAL); return TSTRING;}
<str>.|\n return TINVALID;

	/* Booleans and NULL */
(true|false) return TBOOL;
null return TNULL;

	/* Crap */
. return TINVALID;
