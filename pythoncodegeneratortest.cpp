#include "pythoncodegeneratortest.h"

PythonCodeGeneratorTest::PythonCodeGeneratorTest(QObject *parent) : QObject(parent)
{

}

void PythonCodeGeneratorTest::testPythonGenerateFile()
{
    // Test PythonCodeGenerator::generate(const JavaSourceDef &fileDef, string &dest)
}

void PythonCodeGeneratorTest::testPythonGenerateClass()
{
    // Test PythonCodeGenerator::generate(const ClassDef &classDef, string &dest)
}

void PythonCodeGeneratorTest::testPythonGenerateMethod()
{
    // Test PythonCodeGenerator::generate(const MethodDef &method, string &dest)
}

void PythonCodeGeneratorTest::testPythonGenerateField()
{
    // Test PythonCodeGenerator::generate(const VarDef &field, string &dest)
}

void PythonCodeGeneratorTest::testPythonGenerateArg()
{
    // Test PythonCodeGenerator::generateArg(const VarDef &arg, string &dest)
}
