#include "pythoncodegeneratortest.h"
#include "javasourcedef.h"
#include <QtTest/QtTest>
#include <iostream>

Q_DECLARE_METATYPE(JavaSourceDef)
Q_DECLARE_METATYPE(ClassDef)
Q_DECLARE_METATYPE(MethodDef)
Q_DECLARE_METATYPE(VarDef)

PythonCodeGeneratorTest::PythonCodeGeneratorTest(QObject *parent) : QObject(parent)
{

}

void PythonCodeGeneratorTest::testPythonGenerateFile()
{
    PythonCodeGenerator generator;

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

void PythonCodeGeneratorTest::testPythonGenerateFile_data()
{
    QTest::addColumn<JavaSourceDef>("fileDef");
    QTest::addColumn<QString>("expectedOutput");

    VarDef arg;
    arg.name = "argName";
    arg.type = "int";

    MethodDef publicMethod;
    publicMethod.name = "methodName";
    publicMethod.type = "int";
    publicMethod.arguments = { arg };
    publicMethod.modifiers |= PUBLIC;

    VarDef publicField;
    publicField.name = "fieldName";
    publicField.type = "int";

    ClassDef baseClass;
    baseClass.name = "ClassName";
    baseClass.isInterface = false;
    baseClass.fields = { publicField };
    baseClass.methods = { publicMethod };

    JavaSourceDef javaFile;
    javaFile.name = "filename";
    javaFile.package = "com.example";
    javaFile.classes = { baseClass };
    QString expected1 =
            R"(
            class ClassName:
                a: int
                def methodName(self, argName):
                    pass
            )";
    QTest::newRow("Public Method") << javaFile << expected1;
}

void PythonCodeGeneratorTest::testPythonGenerateClass()
{
    PythonCodeGenerator generator;

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

void PythonCodeGeneratorTest::testPythonGenerateClass_data()
{
    QTest::addColumn<ClassDef>("classDef");
    QTest::addColumn<QString>("expectedOutput");

    VarDef arg;
    arg.name = "argName";
    arg.type = "int";

    MethodDef publicMethod;
    publicMethod.name = "methodName";
    publicMethod.type = "int";
    publicMethod.arguments = { arg };
    publicMethod.modifiers |= PUBLIC;

    VarDef publicField;
    publicField.name = "fieldName";
    publicField.type = "int";

    ClassDef class1;
    class1.name = "ClassName";
    class1.isInterface = false;
    class1.fields = { publicField };
    class1.methods = { publicMethod };

    QString expected1 =
            R"(
            class ClassName:
                a: int
                def methodName(self, argName):
                    pass
            )";
    QTest::newRow("Public Method") << class1 << expected1;
}

void PythonCodeGeneratorTest::testPythonGenerateMethod()
{
    PythonCodeGenerator generator;

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

void PythonCodeGeneratorTest::testPythonGenerateMethod_data()
{
    QTest::addColumn<MethodDef>("method");
    QTest::addColumn<QString>("expectedOutput");

    VarDef arg;
    arg.name = "argName";
    arg.type = "int";

    MethodDef method1;
    method1.name = "methodName";
    method1.type = "int";
    method1.arguments = { arg };
    method1.modifiers |= PUBLIC;
    QString expected1 =
            R"(
            def methodName(self, argName):
                pass
            )";
    QTest::newRow("Public Method") << method1 << expected1;
}

void PythonCodeGeneratorTest::testPythonGenerateField()
{
    PythonCodeGenerator generator;

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
}

void PythonCodeGeneratorTest::testPythonGenerateField_data()
{
    QTest::addColumn<VarDef>("field");
    QTest::addColumn<QString>("expectedOutput");

    VarDef field1;
    field1.name = "fieldName";
    field1.type = "int";
    field1.modifiers |= PUBLIC;
    QTest::newRow("Public Field") << field1 << "fieldName: int";

    VarDef field2;
    field2.name = "fieldName";
    field2.type = "int";
    field2.value = "378";
    field2.modifiers |= PROTECTED;
    QTest::newRow("Protected Field") << field2 << "_fieldName: int";

    VarDef field3;
    field3.name = "fieldName";
    field3.type = "int";
    field3.value = "378";
    field3.modifiers |= PRIVATE;
    QTest::newRow("Private Field") << field3 << "__fieldName: int";

    VarDef field4;
    field4.name = "fieldName";
    field4.type = "int";
    field4.value = "378";
    field4.modifiers |= DEFAULT;
    QTest::newRow("Default Field") << field4 << "fieldName: int";

    VarDef field5;
    field5.name = "fieldName";
    field5.type = "int";
    field5.value = "378";
    field5.modifiers |= STATIC;
    QTest::newRow("Static Field") << field5 << "fieldName: int = 378";

    VarDef field6;
    field6.name = "fieldName";
    field6.type = "int";
    field6.modifiers |= FINAL;
    QTest::newRow("Final Field") << field6 << "fieldName: Final[int] = 378";
}

void PythonCodeGeneratorTest::testPythonGenerateArg()
{
    // Test PythonCodeGenerator::generateArg(const VarDef &arg, string &dest)
}

void PythonCodeGeneratorTest::testPythonGenerateArg_data()
{
    // Test PythonCodeGenerator::generateArg(const VarDef &arg, string &dest)
}
