#include "cppcodegeneratortest.h"
#include "cppcodegenerator.h"
#include <QtTest/QtTest>

CppCodeGeneratorTest::CppCodeGeneratorTest(QObject *parent) : QObject(parent)
{

}

void CppCodeGeneratorTest::testCppGenerateFile()
{
    // Test CppCodeGenerator::generate(const JavaSourceDef &fileDef, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateClass()
{
    // Test CppCodeGenerator::generate(const ClassDef &classDef, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateMethod()
{
    // Test CppCodeGenerator::generate(const MethodDef &method, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateField()
{
    // Test CppCodeGenerator::generate(const VarDef &field, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateArg()
{
    // Test CppCodeGenerator::generateArg(const VarDef &arg, string &dest)
}
