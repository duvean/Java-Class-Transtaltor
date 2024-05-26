#ifndef CPPLEXER_H
#define CPPLEXER_H
#include "lexer.h"
#include "javasourcedef.h"

class CppLexer : public Lexer
{
public:
    CppLexer();

    void tokenize(const JavaSourceDef &fileDef, vector<string> &dest);
    void tokenize(const ClassDef &classDef, vector<string> &dest);
    void tokenize(const MethodDef &method, vector<string> &dest);
    void tokenize(const VarDef &field, vector<string> &dest);
};

#endif // CPPLEXER_H
