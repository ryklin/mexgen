/* Mexgen lexical analyzer

   Copyright (C) 2004 Center for Perceptual Systems

   jsp Created Tue Feb  3 12:57:11 CST 2004
*/

%option nounput
%x DECL

%{

#ifdef _WIN32
#pragma warning (disable: 4786) // identifier was truncated
#endif

// Bison generated header
#include "mexgen.tab.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> stringargs;
string declcomment;
vector<int> declcommands;
string id;

// Keep track of line number for error diagnostics
int line_number = 1;

%}

%%

[0-9]+ { yylval = atoi (yytext); return TOKENNUMBER; }
minversion { return TOKENMINVERSION; }
handlers { return TOKENHANDLERS; }
sources { return TOKENSOURCES; }
libs { return TOKENLIBS; }
mexcommand { return TOKENMEXCOMMAND; }
addpath { return TOKENADDPATH; }
entrypoint { return TOKENENTRYPOINT; }
"(" { return yytext[0]; }
")" { return yytext[0]; }
"." { return yytext[0]; }

function { BEGIN DECL; return TOKENFUNCTION; }
<DECL>double { return TOKENDOUBLE; }
<DECL>complex { return TOKENCOMPLEX; }
<DECL>logical { return TOKENLOGICAL; }
<DECL>char { return TOKENCHAR; }
<DECL>single { return TOKENSINGLE; }
<DECL>int8 { return TOKENINT8; }
<DECL>uint8 { return TOKENUINT8; }
<DECL>int16 { return TOKENINT16; }
<DECL>uint16 { return TOKENUINT16; }
<DECL>int32 { return TOKENINT32; }
<DECL>uint32 { return TOKENUINT32; }
<DECL>int64 { return TOKENINT64; }
<DECL>uint64 { return TOKENUINT64; }
<DECL>any { return TOKENANY; }
<DECL>varargin { return TOKENVARARGIN; }
<DECL>varargout { return TOKENVARARGOUT; }
<DECL>end { BEGIN INITIAL; return TOKENENDDECL; }
<DECL>[a-zA-Z]+[a-zA-Z0-9_]* { id = yytext; return TOKENID; }
<DECL>"(" { return yytext[0]; }
<DECL>")" { return yytext[0]; }
<DECL>"[" { return yytext[0]; }
<DECL>"]" { return yytext[0]; }
<DECL>"=" { return yytext[0]; }
<DECL>"," { return yytext[0]; }
<DECL>[ \t\r] ; // Ignore whitespace
<DECL>[\%].*[\n] { ++line_number; declcomment += yytext; return TOKENDECLCOMMENT; }
<DECL>[\n] { ++line_number; }
<DECL>. { throw "syntax error"; }

[ \t\r] ; // Ignore whitespace
[\n] { ++line_number; }
[;] ; // Ignore these too

\'[^\'\n]*\' { yytext[yyleng-1]='\0'; stringargs.push_back (yytext+1); return TOKENQSTRING; }
[ \t]*[\%].*[\n] { ++line_number; } // Ignore comments
. { throw "syntax error"; }

%%

int yywrap ()
{
	// This function is called when the lexer reads an EOF.  You may
	// modify this function to open multiple input files and redirect
	// yyin.  Otherwise, just return 1 to indicate that there is no
	// more input.
	return 1;
}
