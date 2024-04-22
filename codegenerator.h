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

#endif // CODEGENERATOR_H
