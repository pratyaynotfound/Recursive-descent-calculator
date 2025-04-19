#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

typedef enum
{
    INT_TYPE,
    FLOAT_TYPE
} typeTag;

typedef struct
{
    typeTag type;
    union
    {
        int iVal;
        double fVal;
    } val;
} valType;

typedef struct sym
{
    char *varP;
    struct sym *next;
    char type; // 'i': int, 'f': float
    char init; // 't': initialized, 'f': not initialized
    valType value;
} *symP;

int token;
extern int yylex();
extern int line_no;
extern FILE *yyin;
yylType yylval;

symP symTable = NULL, symTable_head = NULL, symTable_tail = NULL;

int getToken()
{
    token = yylex();
    return 0;
}

int match(int expected)
{
    if (token == expected)
    {
        getToken();
        return 0;
    }
    else
    {
        printf("Syntax error: Expected %c(%d) but found %c(%d) at line %d\n", expected, expected, token, token, line_no);
        exit(1);
    }
}

symP findSymbol(char *varP)
{
    symP p = symTable;
    while (p)
    {
        if (strcmp(p->varP, varP) == 0)
            return p;
        p = p->next;
    }
    return NULL;
}

int addSymbol(char *varP, char type)
{
    if (findSymbol(varP))
    {
        printf("Redeclaration: %s : %d\n", varP, line_no);
        return 1;
    }
    symP newSym = (symP)malloc(sizeof(struct sym));
    newSym->varP = strdup(varP);
    newSym->type = type;
    newSym->init = 'f';
    newSym->next = NULL;
    if (!symTable)
    {
        symTable = symTable_head = symTable_tail = newSym;
    }
    else
    {
        symTable_tail->next = newSym;
        symTable_tail = newSym;
    }
    return 0;
}

int checkVars(char *varP, valType *v)
{
    symP sym = findSymbol(varP);
    if (sym == NULL)
    {
        printf("Variable %s not declared: (%d)\n", varP, line_no);
        getToken();
        return 0;
    }
    if (sym->init == 'f')
    {
        printf("Variable %s not initialized: (%d)\n", varP, line_no);
        return 1;
    }
    *v = sym->value;
    return 0;
}

int F(valType *svP);
int T(valType *svP);
int E(valType *svP);
int AC(), RC(), PC();
int C(), CL();
int VL(char type);
int TY(char *type);
int D();
int DL();
int P();

int F(valType *svP)
{
    if (token == IC)
    {
        svP->type = INT_TYPE;
        svP->val.iVal = yylval.integer;
        getToken();
        return 0;
    }
    else if (token == FC)
    {
        svP->type = FLOAT_TYPE;
        svP->val.fVal = yylval.real;
        getToken();
        return 0;
    }
    else if (token == ID)
    {
        if (checkVars(yylval.string, svP))
            return 1;
        getToken();
        return 0;
    }
    else if (token == '(')
    {
        getToken();
        if (E(svP))
            return 1;
        return match(')');
    }
    else
    {
        printf("F() error: Unexpected token %d at line %d\n", token, line_no);
        return 1;
    }
}

int T(valType *svP)
{
    valType left;
    if (F(&left))
        return 1;

    while (token == BIN_OP && (*yylval.string == '*' || *yylval.string == '/'))
    {
        char op = *yylval.string;
        getToken();
        valType right;
        if (F(&right))
            return 1;

        if (left.type == FLOAT_TYPE || right.type == FLOAT_TYPE)
        {
            double l = (left.type == INT_TYPE) ? left.val.iVal : left.val.fVal;
            double r = (right.type == INT_TYPE) ? right.val.iVal : right.val.fVal;
            left.type = FLOAT_TYPE;
            left.val.fVal = (op == '*') ? l * r : l / r;
        }
        else
        {
            left.val.iVal = (op == '*') ? left.val.iVal * right.val.iVal : left.val.iVal / right.val.iVal;
        }
    }

    *svP = left;
    return 0;
}

int E(valType *svP)
{
    valType left;
    if (T(&left))
        return 1;

    while (token == BIN_OP && (*yylval.string == '+' || *yylval.string == '-'))
    {
        char op = *yylval.string;
        getToken();
        valType right;
        if (T(&right))
            return 1;

        if (left.type == FLOAT_TYPE || right.type == FLOAT_TYPE)
        {
            double l = (left.type == INT_TYPE) ? left.val.iVal : left.val.fVal;
            double r = (right.type == INT_TYPE) ? right.val.iVal : right.val.fVal;
            left.type = FLOAT_TYPE;
            left.val.fVal = (op == '+') ? l + r : l - r;
        }
        else
        {
            left.val.iVal = (op == '+') ? left.val.iVal + right.val.iVal : left.val.iVal - right.val.iVal;
        }
    }
    *svP = left;
    return 0;
}

int AC()
{
    // printf("Inside AC.\n"); //debug
    if (token != ID)
    {
        return 1;
    }
    symP var = findSymbol(yylval.string);
    if (!var)
    {
        printf("Variable %s not declared: (%d)\n", yylval.string, line_no);
        getToken();
        return 0;
    }
    match(ID);
    if (match('='))
        return 1;

    valType result;
    if (E(&result))
        return 1;

    if (var->type != result.type)
    {
        printf("Var-Data type mismatch: %s (%d)\n", var->varP, line_no);
        return 0;
    }

    var->init = 't';
    var->value = result;
    return 0;
}

int RC()
{
    if (match(INPUT))
        return 1;
    if (token != ID)
        return 1;
    symP sym = findSymbol(yylval.string);
    if (!sym)
    {
        printf("Variable %s not declared: (%d)\n", yylval.string, line_no);
        getToken();
        return 0;
    }

    if (sym->type == 'i')
    {
        scanf("%d", &sym->value.val.iVal);
        sym->value.type = INT_TYPE;
    }
    else
    {
        scanf("%lf", &sym->value.val.fVal);
        sym->value.type = FLOAT_TYPE;
    }
    sym->init = 't';
    return match(ID);
}

int PC()
{
    if (match(PRINT))
        return 1;
    valType result;
    if (E(&result))
        return 1;

    if (result.type == INT_TYPE)
        printf("%d\n", result.val.iVal);
    else
        printf("%f\n", result.val.fVal);
    return 0;
}

int C()
{
    // printf("Inside C (before PC).\n"); //debug
    if (token == PRINT)
        return PC();
    // printf("Inside C (before RC).\n"); //debug
    if (token == INPUT)
        return RC();
    // printf("Inside C (after RC).\n"); //debug
    if (token == ID)
        return AC();
    // printf("Inside C (after AC).\n"); //debug
    printf("C() error: Unexpected token %d at line %d\n", token, line_no);
    return 1;
}

int CL()
{
    while (token != '}' && token != 0)
    {
        // printf("Inside while before C.\n"); //debug
        if (C())
            return 1;
        // printf("Inside while after C.\n"); //debug
    }
    return 0;
}

int VL(char type)
{
    if (token != ID)
        return 1;
    if (addSymbol(yylval.string, type))
        return 1;
    getToken();
    while (token == ID)
    {
        if (addSymbol(yylval.string, type))
            return 1;
        getToken();
    }
    return 0;
}

int TY(char *type)
{
    if (token == ZAH)
    {
        *type = 'i';
    }
    else if (token == FLT)
    {
        *type = 'f';
    }
    else
    {
        printf("’flt’/’zah’ missing:%d\n", line_no);
        return 1;
    }
    getToken();
    return 0;
}

int D()
{
    char type;
    if (TY(&type))
        return 1;
    return VL(type);
}

int DL()
{
    while (token == ZAH || token == FLT)
    {
        if (D())
            return 1;
    }
    return 0;
}

int P()
{
    if (match('{'))
        return 1;
    if (DL())
        return 1;
    if (match(':'))
        return 1;
    if (CL())
        return 1;
    return match('}');
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
        perror("Cannot open input file");
        return 1;
    }

    getToken();
    P();

    fclose(yyin);
    return 0;
}