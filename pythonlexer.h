#ifndef PYTHONLEXER_H
#define PYTHONLEXER_H
#include "lexer.h"
#include "javasourcedef.h"

class PythonLexer : public Lexer
{
public:
    PythonLexer();

    void tokenize(const JavaSourceDef &fileDef, vector<string> &dest);
    void tokenize(const ClassDef &classDef, vector<string> &dest);
    void tokenize(const MethodDef &method, vector<string> &dest);
    void tokenize(const VarDef &field, vector<string> &dest);
};

#endif // PYTHONLEXER_H
