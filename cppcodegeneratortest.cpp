#include "cppcodegeneratortest.h"
#include "cppcodegenerator.h"
#include "javasourcedef.h"
#include <QtTest/QtTest>

Q_DECLARE_METATYPE(JavaSourceDef)
Q_DECLARE_METATYPE(ClassDef)
Q_DECLARE_METATYPE(MethodDef)
Q_DECLARE_METATYPE(VarDef)

CppCodeGeneratorTest::CppCodeGeneratorTest(QObject *parent) : QObject(parent)
{

}

void CppCodeGeneratorTest::testCppGenerateFile()
{
    // Test CppCodeGenerator::generate(const JavaSourceDef &fileDef, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateFile_data()
{
    // Test CppCodeGenerator::generate(const JavaSourceDef &fileDef, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateClass()
{
    // Test CppCodeGenerator::generate(const ClassDef &classDef, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateClass_data()
{
    // Test CppCodeGenerator::generate(const ClassDef &classDef, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateMethod()
{
    // Test CppCodeGenerator::generate(const MethodDef &method, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateMethod_data()
{
    // Test CppCodeGenerator::generate(const MethodDef &method, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateField()
{
    CppCodeGenerator generator;

    QFETCH(VarDef, field);
    QFETCH(QString, expectedOutput);

    string generatedCode;
    generator.generate(field, generatedCode);
    QCOMPARE(QString::fromStdString(generatedCode), expectedOutput);
}

void CppCodeGeneratorTest::testCppGenerateField_data()
{
    QTest::addColumn<VarDef>("field");
    QTest::addColumn<QString>("expectedOutput");

    VarDef field1;
    field1.name = "fieldName";
    field1.type = "int";
    QTest::newRow("__BASE__ No Modifiers") << field1 << "int fieldName;";

    VarDef field2;
    field2.name = "fieldName";
    field2.type = "int";
    field2.modifiers |= STATIC;
    QTest::newRow("Static Field") << field2 << "static int fieldName;";

    VarDef field3;
    field3.name = "fieldName";
    field3.type = "int";
    field3.modifiers |= FINAL;
    QTest::newRow("Final Field") << field3 << "const int fieldName;";

    VarDef field4;
    field4.name = "fieldName";
    field4.type = "int";
    field4.value = "378";
    QTest::newRow("Field With Init") << field4 << "int fieldName = 378;";
}

void CppCodeGeneratorTest::testCppGenerateArg()
{
    // Test CppCodeGenerator::generateArg(const VarDef &arg, string &dest)
}

void CppCodeGeneratorTest::testCppGenerateArg_data()
{
    // Test CppCodeGenerator::generateArg(const VarDef &arg, string &dest)
}
