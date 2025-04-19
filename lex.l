%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

int line_no = 1;
yylType yylval;
#define YY_DECL int yylex()
%}

%option noyywrap
%x COMMENT INSTR

%%

"//"  { BEGIN COMMENT; }
<COMMENT>.
<COMMENT>\n {++line_no; BEGIN INITIAL;}

[ \t\r]+
\n        { line_no++;}

zah      { return ZAH; }
flt      { return FLT; }
input    { return INPUT; }
print    { return PRINT; }

[a-zA-Z][a-zA-Z0-9]* {
    yylval.string = strdup(yytext);
    return ID;
}

[0-9]+ {
    yylval.integer = atoi(yytext);
    return IC;
}

[0-9]+\.[0-9]+([eE][+-]?[0-9]+)? {
    yylval.real = atof(yytext);
    return FC;
}

[\+\-\*/] {
    if (strlen(yytext) == 1) {
        yylval.string = strdup(yytext);
        return BIN_OP;
    }
}

\{  { return '{'; }
\}  { return '}'; }
:   { return ':'; }
=   { return '='; }
\(  { return '('; }
\)  { return ')'; }
\,    { return ','; }


.   { return NOTOK; }

%%

//int yywrap() {
//    return 1;
//}
