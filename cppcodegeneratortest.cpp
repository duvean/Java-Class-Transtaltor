#include "cppcodegeneratortest.h"
#include "cppcodegenerator.h"
#include "javasourcedef.h"
#include <QtTest/QtTest>
#include <iostream>

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
    CppCodeGenerator generator;

    QFETCH(MethodDef, method);
    QFETCH(QString, expectedOutput);

    string generatedCode;
    generator.generate(method, generatedCode);

    if (QString::fromStdString(generatedCode) != expectedOutput)
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                            .arg(QString::fromStdString(generatedCode))
                            .arg(expectedOutput);
        cout << message.toStdString() << endl;
        QVERIFY2(false, "");
    }
}

void CppCodeGeneratorTest::testCppGenerateMethod_data()
{
    QTest::addColumn<MethodDef>("method");
    QTest::addColumn<QString>("expectedOutput");

    VarDef arg;
    arg.name = "argName";
    arg.type = "int";

    MethodDef method1;
    method1.name = "methodName";
    method1.type = "int";
    method1.arguments.push_back(arg);
    QTest::newRow("__BASE__ No Modifiers") << method1 << "virtual int methodName(int argName);";

    MethodDef method2;
    method2.name = "methodName";
    method2.type = "int";
    method2.arguments.push_back(arg);
    method2.modifiers |= ABSTRACT;
    QTest::newRow("Abstract") << method2 << "virtual int methodName(int argName) = 0;";

    MethodDef method3;
    method3.name = "methodName";
    method3.type = "int";
    method3.arguments.push_back(arg);
    method3.modifiers |= FINAL;
    QTest::newRow("Final") << method3 << "virtual int methodName(int argName) final;";

    MethodDef method4;
    method4.name = "methodName";
    method4.type = "int";
    method4.arguments.push_back(arg);
    method4.modifiers |= STATIC;
    QTest::newRow("Static") << method4 << "static int methodName(int argName);";

    MethodDef method5;
    method5.name = "ClassName";
    method5.arguments.push_back(arg);
    method5.isConstructor = true;
    QTest::newRow("Constructor") << method5 << "ClassName(int argName);";

    MethodDef method6;
    method6.name = "finalize";
    method6.type = "void";
    method6.arguments.push_back(arg);
    method6.modifiers |= PROTECTED;
    method6.isDestructor = true;
    QTest::newRow("Deconstructor") << method6 << "~finalize(int argName);";
    /* невозможно задать имя деструктора не зная имени класса */

    MethodDef method7;
    method7.name = "methodName";
    method7.type = "int";
    method7.arguments.push_back(arg);
    method7.annotations.push_back("@Override");
    QTest::newRow("Overriding") << method7 << "virtual int methodName(int argName) override;";
}

void CppCodeGeneratorTest::testCppGenerateField()
{
    CppCodeGenerator generator;

    QFETCH(VarDef, field);
    QFETCH(QString, expectedOutput);

    string generatedCode;
    generator.generate(field, generatedCode);

    if (QString::fromStdString(generatedCode) != expectedOutput)
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                            .arg(QString::fromStdString(generatedCode))
                            .arg(expectedOutput);
        cout << message.toStdString() << endl;
        QVERIFY2(false, "");
    }

    //QCOMPARE(QString::fromStdString(generatedCode), expectedOutput);
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
