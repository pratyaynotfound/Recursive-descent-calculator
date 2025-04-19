// y.tab.h
#ifndef _Y_TAB_H
#define _Y_TAB_H

#define ID 300
#define IC 301
#define FC 302
#define FLT 305
#define ZAH 306
#define INPUT 309
#define PRINT 310
#define NOTOK 400
#define BIN_OP 311
#define NEWLINE 312

int yylex(void);
typedef union {
    int integer ;
    double real;
    char *string;
} yylType;

#endif
