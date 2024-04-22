#ifndef PYTHONCODEGENERATOR_H
#define PYTHONCODEGENERATOR_H
#include "codegenerator.h"

class PythonCodeGenerator : public CodeGenerator
{
public:
    void generate(const JavaSourceDef &fileDef, string &dest);
    void generate(const ClassDef &classDef, string &dest);
    void generate(const MethodDef &method, string &dest);
    void generate(const VarDef &field, string &dest);
    void generateArg(const VarDef &arg, string &dest);
};

#endif // PYTHONCODEGENERATOR_H
