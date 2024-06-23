#ifndef LEXER_H
#define LEXER_H
#include "javasourcedef.h"

class Lexer
{
public:
     Lexer();
     virtual void tokenize(const JavaSourceDef &fileDef, vector<string> &tokens) = 0;
     virtual void tokenize(const ClassDef &classDef, vector<string> &tokens) = 0;
     virtual void tokenize(const MethodDef &method, vector<string> &tokens) = 0;
     virtual void tokenize(const VarDef &field, vector<string> &tokens) = 0;
};

#endif // LEXER_H
