#include "cppcodegeneratortest.h"
#include "cppcodegenerator.h"
#include "javasourcedef.h"
#include <QtTest/QtTest>
#include <iostream>

Q_DECLARE_METATYPE(JavaSourceDef)
Q_DECLARE_METATYPE(ClassDef)
Q_DECLARE_METATYPE(MethodDef)
Q_DECLARE_METATYPE(VarDef)

//void CppCodeGeneratorTest::testCppGenerateFile()
//{
//    CppCodeGenerator generator;

//    QFETCH(JavaSourceDef, fileDef);
//    QFETCH(QString, expectedOutput);

//    string generatedCode;
//    generator.generate(fileDef, generatedCode);

//    if (util::normalize(generatedCode) != util::normalize(expectedOutput.toStdString()))
//    {
//        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
//                            .arg(QString::fromStdString(generatedCode))
//                            .arg(expectedOutput);
//        QWARN(message.toStdString().c_str());
//        QVERIFY2(false, "");
//    }
//}

void CppCodeGeneratorTest::testCppGenerateFile_data()
{
    QTest::addColumn<ClassDef>("classDef");
    QTest::addColumn<QString>("expectedOutput");

    VarDef publicField;
    publicField.name = "fieldName";
    publicField.type = "int";
    publicField.modifiers |= PUBLIC;
    MethodDef publicMethod;
    publicMethod.name = "methodName";
    publicMethod.type = "int";
    publicMethod.modifiers |= PUBLIC;
    ClassDef baseClass;
    baseClass.name = "baseClass";
    baseClass.fields = { publicField };
    baseClass.methods = { publicMethod };

    JavaSourceDef file1;
    file1.name = "filename";
    file1.package = "com.example";
    file1.classes = { baseClass };
    QString expected1 =
            R"(
            namespace com.example {
            class ClassName
            {
            public:
                int fieldName;
                virtual int methodName();
            };
            }
            )";
    QTest::newRow("One Base Class No Imports") << file1 << expected1;

    JavaSourceDef file2;
    file2.name = "filename";
    file2.package = "com.example";
    file2.classImports = { "com.example.class1" };
    file2.classes = { baseClass };
    QString expected2 =
            R"(
            using com::example2::class1
            namespace com.example {
            class ClassName
            {
            public:
                int fieldName;
                virtual int methodName();
            };
            }
            )";
    QTest::newRow("One Class Import") << file2 << expected2;

    JavaSourceDef file3;
    file3.name = "filename";
    file3.package = "com.example1";
    file3.packageImports = { "com.example2" };
    file3.classes = { baseClass };
    QString expected3 =
            R"(
            using com::example2
            namespace com.example1 {
            class ClassName
            {
            public:
                int fieldName;
                virtual int methodName();
            };
            }
            )";
    QTest::newRow("One Package Import") << file3 << expected3;

    JavaSourceDef file4;
    file4.name = "filename";
    file4.package = "com.example1";
    file4.classImports = { "com.example.class1" };
    file4.packageImports = { "com.example2" };
    file4.classes = { baseClass };
    QString expected4 =
            R"(
            using com::example2
            using com::example2::class1
            namespace com.example1 {
            class ClassName1
            {
            public:
                int fieldName;
                virtual int methodName();
            };
            class ClassName2
            {
            public:
                int fieldName;
                virtual int methodName();
            };
            }
            )";
    QTest::newRow("Several Classes Several Imports") << file4 << expected4;
}

void CppCodeGeneratorTest::testCppGenerateClass()
{
    CppCodeGenerator generator;

    QFETCH(ClassDef, classDef);
    QFETCH(QString, expectedOutput);

    string generatedCode;
    generator.generate(classDef, generatedCode);

    if (util::normalize(generatedCode) != util::normalize(expectedOutput.toStdString()))
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                .arg(QString::fromStdString(generatedCode))
                .arg(expectedOutput);
        QWARN(message.toStdString().c_str());
        QVERIFY2(false, "");
    }
}

void CppCodeGeneratorTest::testCppGenerateClass_data()
{
    QTest::addColumn<ClassDef>("classDef");
    QTest::addColumn<QString>("expectedOutput");

    VarDef privateField;
    privateField.name = "fieldName";
    privateField.type = "int";
    privateField.modifiers = PRIVATE;
    VarDef protectedField;
    protectedField.name = "fieldName";
    protectedField.type = "int";
    protectedField.modifiers = PROTECTED;
    VarDef publicField;
    publicField.name = "fieldName";
    publicField.type = "int";
    publicField.modifiers = PUBLIC;
    VarDef defaultField;
    defaultField.name = "fieldName";
    defaultField.type = "int";
    defaultField.modifiers = DEFAULT;
    VarDef staticField;
    staticField.name = "fieldName";
    staticField.type = "int";
    staticField.value = "378";
    staticField.modifiers = PUBLIC|STATIC;

    MethodDef privateMethod;
    privateMethod.name = "methodName";
    privateMethod.type = "int";
    privateMethod.modifiers = PRIVATE;
    MethodDef protectedMethod;
    protectedMethod.name = "methodName";
    protectedMethod.type = "int";
    protectedMethod.modifiers = PROTECTED;
    MethodDef publicMethod;
    publicMethod.name = "methodName";
    publicMethod.type = "int";
    publicMethod.modifiers = PUBLIC;
    MethodDef defaultMethod;
    defaultMethod.name = "methodName";
    defaultMethod.type = "int";
    defaultMethod.modifiers = DEFAULT;
    MethodDef abstractMethod;
    abstractMethod.name = "methodName";
    abstractMethod.type = "int";
    abstractMethod.modifiers = PUBLIC|ABSTRACT;

    ClassDef privateClass;
    privateClass.name = "privateClass";
    privateClass.modifiers = PRIVATE;
    privateClass.fullPath = "ClassName::";
    ClassDef protectedClass;
    protectedClass.name = "protectedClass";
    protectedClass.modifiers = PROTECTED;
    protectedClass.fullPath = "ClassName::";
    ClassDef publicClass;
    publicClass.name = "publicClass";
    publicClass.modifiers = PUBLIC;
    publicClass.fullPath = "ClassName::";
    ClassDef defaultClass;
    defaultClass.name = "defaultClass";
    defaultClass.modifiers = DEFAULT;
    defaultClass.fullPath = "ClassName::";
    ClassDef Nested2;
    Nested2.name = "Nested2";
    Nested2.modifiers = PUBLIC;
    Nested2.fullPath = "ClassName::Nested1::";
    ClassDef Nested1;
    Nested1.name = "Nested1";
    Nested1.modifiers = PUBLIC;
    Nested1.nestedClasses = { Nested2 };
    Nested1.fullPath = "ClassName::";





    ClassDef class1;
    class1.name = "ClassName";
    class1.isInterface = false;
    class1.fields = { privateField };
    class1.methods = { privateMethod };
    QString expected1 =
            R"(
            class ClassName
            {
            private:
                int fieldName;
                virtual int methodName();
            };
            )";
    QTest::newRow("1. Private Field And Method") << class1 << expected1;

    ClassDef class2;
    class2.name = "ClassName";
    class2.isInterface = false;
    class2.fields = { protectedField };
    class2.methods = { protectedMethod };
    QString expected2 =
            R"(
            class ClassName
            {
            public:
                class protectedMembers
                {
                protected:
                    int fieldName;
                    virtual int methodName();
                };
            };
            )";
    QTest::newRow("2. Protected Field And Method") << class2 << expected2;

    ClassDef class3;
    class3.name = "ClassName";
    class3.isInterface = false;
    class3.fields = { publicField };
    class3.methods = { publicMethod };
    QString expected3 =
            R"(
            class ClassName
            {
            public:
                int fieldName;
                virtual int methodName();
            };
            )";
    QTest::newRow("3. Public Field And Method") << class3 << expected3;

    ClassDef class4;
    class4.name = "ClassName";
    class4.isInterface = false;
    class4.fields = { defaultField };
    class4.methods = { defaultMethod };
    QString expected4 =
            R"(
            class ClassName
            {
            public:
                class defaultMembers
                {
                public:
                    int fieldName;
                    virtual int methodName();
                };
            };
            )";
    QTest::newRow("4. Default Field And Method") << class4 << expected4;

    ClassDef class5;
    class5.name = "IName";
    class5.isInterface = true;
    class5.fields = { staticField };
    class5.methods = { abstractMethod };
    QString expected5 =
            R"(
            class IName
            {
            public:
                static int fieldName = 378;
                virtual int methodName() = 0;
            };
            )";
    QTest::newRow("5. Interface") << class5 << expected5;

    ClassDef class6;
    class6.name = "IName";
    class6.isInterface = true;
    class6.methods = { abstractMethod };
    class6.annotations = { "@FunctionalInterface" };
    QString expected6 =
            R"(
            /* @FunctionalInterface */
            class IName
            {
            public:
                virtual int methodName() = 0;
            };
            )";
    QTest::newRow("6. Functional Interface") << class6 << expected6;

    ClassDef class7;
    class7.name = "ClassName";
    class7.isInterface = false;
    class7.methods = { abstractMethod, publicMethod };
    class7.modifiers = ABSTRACT;
    QString expected7 =
            R"(
            class ClassName
            {
            public:
                virtual int methodName() = 0;
                virtual int methodName();
            };
            )";
    QTest::newRow("7. Abstract Class") << class7 << expected7;

    ClassDef class8;
    class8.name = "ClassName";
    class8.isInterface = false;
    class8.fields = { publicField };
    class8.methods = { publicMethod };
    class8.modifiers = STATIC;
    QString expected8 =
            R"(
            class ClassName
            {
            public:
                static int fieldName;
                static int methodName();
            };
            )";
    QTest::newRow("8. Static Class") << class8 << expected8;

    ClassDef class9;
    class9.name = "ClassName";
    class9.isInterface = false;
    class9.modifiers = FINAL;
    QString expected9 =
            R"(
            class ClassName final
            {
            };
            )";
    QTest::newRow("9. Final Class") << class9 << expected9;

    ClassDef class10;
    class10.name = "ClassName";
    class10.isInterface = false;
    class10.nestedClasses = {
        privateClass,
        protectedClass,
        publicClass,
        defaultClass
    };
    QString expected10 =
            R"(
            class ClassName
            {
            private:
                class privateClass;
            public:
                class publicClass;

                class protectedMembers
                {
                protected:
                    class protectedClass;
                };
                class defaultMembers
                {
                public:
                    class defaultClass;
                };
            };

            class ClassName::privateClass
            {
            };

            class ClassName::protectedClass
            {
            };

            class ClassName::publicClass
            {
            };

            class ClassName::defaultClass
            {
            };
            )";
    QTest::newRow("10. Nested Classes Of Each Privacy Modifier") << class10 << expected10;

    ClassDef class11;
    class11.name = "ClassName";
    class11.isInterface = false;
    class11.nestedClasses = {
        Nested1
    };
    QString expected11 =
            R"(
            class ClassName
            {
            public:
                class Nested1;
            };

            class ClassName::Nested1
            {
            public:
                class Nested2;
            };

            class ClassName::Nested1::Nested2
            {
            };
            )";
    QTest::newRow("11. Twice Nested Class") << class11 << expected11;

    ClassDef class12;
    class12.name = "ClassName";
    class12.isInterface = false;
    class12.extends = "ParentClass";
    QString expected12 =
            R"(
            class ClassName : public ParentClass
            {
            };
            )";
    QTest::newRow("12. Inheritance From Class") << class12 << expected12;

    ClassDef class13;
    class13.name = "ClassName";
    class13.isInterface = false;
    class13.implements = { "IName1", "IName2" };
    QString expected13 =
            R"(
            class ClassName : public IName1, public IName2
            {
            };
            )";
    QTest::newRow("13. Inheritance From Several Interfaces") << class13 << expected13;
}

void CppCodeGeneratorTest::testCppGenerateMethod()
{
    CppCodeGenerator generator;

    QFETCH(MethodDef, method);
    QFETCH(QString, expectedOutput);

    string generatedCode;
    generator.generate(method, generatedCode);

    if (util::normalize(generatedCode) != util::normalize(expectedOutput.toStdString()))
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                            .arg(QString::fromStdString(generatedCode))
                            .arg(expectedOutput);
        QWARN(message.toStdString().c_str());
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
    /* невозможно задать имя деструктора не зная имени класса, оно задастся при генерации класса */

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

    if (util::normalize(generatedCode) != util::normalize(expectedOutput.toStdString()))
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                            .arg(QString::fromStdString(generatedCode))
                            .arg(expectedOutput);
        QWARN(message.toStdString().c_str());
        QVERIFY(false);
    }
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
