#ifndef CPPCODEGENERATOR_H
#define CPPCODEGENERATOR_H
#include "codegenerator.h"

class CppCodeGenerator : public CodeGenerator
{
public:
    void generate(const JavaSourceDef &fileDef, string &dest);
    void generate(const ClassDef &classDef, string &dest);
    void generate(const MethodDef &method, string &dest);
    void generate(const VarDef &field, string &dest);
    void generate(const vector<string> &tokens, string &dest);

};

#endif // CPPCODEGENERATOR_H
