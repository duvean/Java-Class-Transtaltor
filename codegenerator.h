#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H
#include "javasourcedef.h"

class CodeGenerator
{
public:
     virtual void generate(const JavaSourceDef &fileDef, string &dest) = 0;
     virtual void generate(const ClassDef &classDef, string &dest) = 0;
     virtual void generate(const MethodDef &method, string &dest) = 0;
     virtual void generate(const VarDef &field, string &dest) = 0;
     virtual void generateArg(const VarDef &arg, string &dest) = 0;
};

class CppCodeGenerator : public CodeGenerator
{
public:
    void generate(const JavaSourceDef &fileDef, string &dest);
    void generate(const ClassDef &classDef, string &dest);
    void generate(const MethodDef &method, string &dest);
    void generate(const VarDef &field, string &dest);
    void generateArg(const VarDef &arg, string &dest);
};

class PythonCodeGenerator : public CodeGenerator
{
public:
    void generate(const JavaSourceDef &fileDef, string &dest);
    void generate(const ClassDef &classDef, string &dest);
    void generate(const MethodDef &method, string &dest);
    void generate(const VarDef &field, string &dest);
    void generateArg(const VarDef &arg, string &dest);
};

#endif // CODEGENERATOR_H
