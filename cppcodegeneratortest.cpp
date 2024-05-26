#include "cppcodegeneratortest.h"
#include "javasourcedef.h"
#include <QtTest/QtTest>


/* ПЕРЕНОСЫ СТРОК НЕ КАК ТОКЕНЫ, А ПРИ ГЕНЕРАЦИИ КОДА */


Q_DECLARE_METATYPE(JavaSourceDef)
Q_DECLARE_METATYPE(ClassDef)
Q_DECLARE_METATYPE(MethodDef)
Q_DECLARE_METATYPE(VarDef)
Q_DECLARE_METATYPE(vector<string>)

CppCodeGeneratorTest::CppCodeGeneratorTest(QObject *parent) : QObject(parent)
{

}

void CppCodeGeneratorTest::testCppGenerateFile()
{
    CppLexer          lexer;
    CodeGenerator     generator;

    vector<string>    tokens;
    string            generatedCode;
    string            expectedCode;

    QFETCH(JavaSourceDef, javaFile);
    QFETCH(vector<string>, expectedOutput);

    lexer.tokenize(javaFile, tokens);
    generator.generate(tokens, generatedCode);
    generator.generate(expectedOutput, expectedCode);

    if (tokens != expectedOutput)
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                .arg(QString::fromStdString(generatedCode))
                .arg(QString::fromStdString(expectedCode));
        QWARN(message.toStdString().c_str());
        QVERIFY(false);
    }
}

void CppCodeGeneratorTest::testCppGenerateFile_data()
{
    QTest::addColumn<JavaSourceDef>("javaFile");
    QTest::addColumn<vector<string> >("expectedOutput");

    VarDef publicField;
    publicField.name = "fieldName";
    publicField.type = "int";
    publicField.modifiers = PUBLIC;
    MethodDef publicMethod;
    publicMethod.name = "methodName";
    publicMethod.type = "int";
    publicMethod.modifiers = PUBLIC;
    ClassDef baseClass;
    baseClass.name = "baseClass";
    baseClass.fields = { publicField };
    baseClass.methods = { publicMethod };

    JavaSourceDef file1;
    file1.name = "filename";
    file1.package = "com.example";
    file1.classes = { baseClass };
    vector<string> expected1 = { "namespace", "com.example", "\n", "{", "\n", "class", "", "baseClass", "\n", "{", "\n", "public", ":",
                                 "$_START", "\n", "int", "fieldName", ";", "\n", "virtual", "int", "methodName", "(", ")", ";", "$_END", "$_END",
                                 "\n", "}", ";", "\n", "\n", "\n" };
    QTest::newRow("1. One Base Class No Imports") << file1 << expected1;

    JavaSourceDef file2;
    file2.name = "filename";
    file2.package = "com.example";
    file2.classImports = { "com.example.class1" };
    file2.classes = { baseClass };
    vector<string> expected2 = { "#", "include", "example", ".", "h", "\n", "using", "namespace", "com::example::class1", ";", "\n", "\n",
                                 "namespace", "com.example", "\n", "{", "\n", "class", "", "baseClass", "\n", "{", "\n", "public", ":",
                                 "$_START", "\n", "int", "fieldName", ";", "\n", "virtual", "int", "methodName", "(", ")", ";", "$_END", "$_END",
                                 "\n", "}", ";", "\n", "\n", "\n" };
    QTest::newRow("2. One Class Import") << file2 << expected2;

    JavaSourceDef file3;
    file3.name = "filename";
    file3.package = "com.example1";
    file3.packageImports = { "com.example2" };
    file3.classes = { baseClass };
    vector<string> expected3 = { "#", "include", "example2", ".", "h", "\n", "using", "namespace", "com::example2", ";", "\n", "\n",
                                 "namespace", "com.example1", "\n", "{", "\n", "class", "", "baseClass", "\n", "{", "\n", "public", ":",
                                 "$_START", "\n", "int", "fieldName", ";", "\n", "virtual", "int", "methodName", "(", ")", ";",
                                 "$_END", "$_END", "\n", "}", ";", "\n", "\n", "\n" };
    QTest::newRow("3. One Package Import") << file3 << expected3;

    JavaSourceDef file4;
    file4.name = "filename";
    file4.package = "com.example1";
    file4.classImports = { "com.example.class1" };
    file4.packageImports = { "com.example2" };
    file4.classes = { baseClass };
    vector<string> expected4 = { "#", "include", "example2", ".", "h", "\n", "#", "include", "example", ".", "h", "\n", "using", "namespace",
                                 "com::example2", ";", "\n", "using", "namespace", "com::example::class1", ";", "\n", "\n", "namespace",
                                 "com.example1", "\n", "{", "\n", "class", "", "baseClass", "\n", "{", "\n", "public", ":", "$_START", "\n",
                                 "int", "fieldName", ";", "\n", "virtual", "int", "methodName", "(", ")", ";", "$_END", "$_END", "\n", "}",
                                 ";", "\n", "\n", "\n" };
    QTest::newRow("4. Several Classes Several Imports") << file4 << expected4;
}

void CppCodeGeneratorTest::testCppGenerateClass()
{
    CppLexer          lexer;
    CodeGenerator     generator;

    vector<string>    tokens;
    string            generatedCode;
    string            expectedCode;

    QFETCH(ClassDef, classDef);
    QFETCH(vector<string>, expectedOutput);

    lexer.tokenize(classDef, tokens);
    generator.generate(tokens, generatedCode);
    generator.generate(expectedOutput, expectedCode);

    if (tokens != expectedOutput)
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                .arg(QString::fromStdString(generatedCode))
                .arg(QString::fromStdString(expectedCode));
        QWARN(message.toStdString().c_str());
        QVERIFY(false);
    }
}

void CppCodeGeneratorTest::testCppGenerateClass_data()
{
    QTest::addColumn<ClassDef>("classDef");
    QTest::addColumn<vector<string> >("expectedOutput");

    VarDef privateField;
    privateField.name = "privateField";
    privateField.type = "int";
    privateField.modifiers = PRIVATE;
    VarDef protectedField;
    protectedField.name = "protectedField";
    protectedField.type = "int";
    protectedField.modifiers = PROTECTED;
    VarDef publicField;
    publicField.name = "publicField";
    publicField.type = "int";
    publicField.modifiers = PUBLIC;
    VarDef defaultField;
    defaultField.name = "defaultField";
    defaultField.type = "int";
    defaultField.modifiers = DEFAULT;
    VarDef staticField;
    staticField.name = "staticField";
    staticField.type = "int";
    staticField.value = "378";
    staticField.modifiers = PUBLIC|STATIC;

    MethodDef privateMethod;
    privateMethod.name = "privateMethod";
    privateMethod.type = "int";
    privateMethod.modifiers = PRIVATE;
    MethodDef protectedMethod;
    protectedMethod.name = "protectedMethod";
    protectedMethod.type = "int";
    protectedMethod.modifiers = PROTECTED;
    MethodDef publicMethod;
    publicMethod.name = "publicMethod";
    publicMethod.type = "int";
    publicMethod.modifiers = PUBLIC;
    MethodDef defaultMethod;
    defaultMethod.name = "defaultMethod";
    defaultMethod.type = "int";
    defaultMethod.modifiers = DEFAULT;
    MethodDef abstractMethod;
    abstractMethod.name = "abstractMethod";
    abstractMethod.type = "int";
    abstractMethod.modifiers = PUBLIC|ABSTRACT;

    ClassDef privateClass;
    privateClass.name = "privateClass";
    privateClass.modifiers = PRIVATE;
    privateClass.fullPath = "ClassName::";
    ClassDef protectedClass;
    protectedClass.name = "protectedClass";
    protectedClass.modifiers = PROTECTED;
    protectedClass.fullPath = "ClassName::protectedMembers::";
    ClassDef publicClass;
    publicClass.name = "publicClass";
    publicClass.modifiers = PUBLIC;
    publicClass.fullPath = "ClassName::";
    ClassDef defaultClass;
    defaultClass.name = "defaultClass";
    defaultClass.modifiers = DEFAULT;
    defaultClass.fullPath = "ClassName::defaultMembers::";
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
    vector<string> expected1 = { "class", "", "ClassName", "\n", "{", "\n", "private", ":", "$_START", "\n", "int", "privateField", ";", "\n",
                                 "virtual", "int", "privateMethod", "(", ")", ";", "$_END", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("1. Private Field And Method") << class1 << expected1;

    ClassDef class2;
    class2.name = "ClassName";
    class2.isInterface = false;
    class2.fields = { protectedField };
    class2.methods = { protectedMethod };
    vector<string> expected2 = { "class", "", "ClassName", "\n", "{", "\n", "public", ":", "$_START", "\n", "class", "protectedMembers", "\n",
                                 "{", "\n", "protected", ":", "$_START", "\n", "int", "protectedField", ";", "\n", "virtual", "int", "protectedMethod",
                                 "(", ")", ";", "$_END", "\n", "}", ";", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("2. Protected Field And Method") << class2 << expected2;

    ClassDef class3;
    class3.name = "ClassName";
    class3.isInterface = false;
    class3.fields = { publicField };
    class3.methods = { publicMethod };
    vector<string> expected3 = { "class", "", "ClassName", "\n", "{", "\n", "public", ":", "$_START", "\n", "int", "publicField", ";", "\n",
                                 "virtual", "int", "publicMethod", "(", ")", ";", "$_END", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("3. Public Field And Method") << class3 << expected3;

    ClassDef class4;
    class4.name = "ClassName";
    class4.isInterface = false;
    class4.fields = { defaultField };
    class4.methods = { defaultMethod };
    vector<string> expected4 = { "class", "", "ClassName", "\n", "{", "\n", "public", ":", "$_START", "\n", "class", "defaultMembers", "\n", "{",
                                 "\n", "$_START", "public", ":", "\n", "int", "defaultField", ";", "\n", "virtual", "int", "defaultMethod", "(", ")",
                                 ";", "$_END", "\n", "}", ";", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("4. Default Field And Method") << class4 << expected4;

    ClassDef class5;
    class5.name = "IName";
    class5.isInterface = true;
    class5.fields = { staticField };
    class5.methods = { abstractMethod };
    vector<string> expected5 = { "class", "", "IName", "\n", "{", "\n", "public", ":", "$_START", "\n", "static", "int", "staticField", "=", "378",
                                 ";", "\n", "virtual", "int", "abstractMethod", "(", ")", "=", "0", ";", "$_END", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("5. Interface") << class5 << expected5;

    ClassDef class6;
    class6.name = "IName";
    class6.isInterface = true;
    class6.methods = { abstractMethod };
    class6.annotations = { "@FunctionalInterface" };
    vector<string> expected6 = { "/*", "@FunctionalInterface", "*/", "\n", "class", "", "IName", "\n", "{", "\n", "public", ":", "$_START", "\n",
                                 "virtual", "int", "abstractMethod", "(", ")", "=", "0", ";", "$_END", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("6. Functional Interface") << class6 << expected6;

    ClassDef class7;
    class7.name = "ClassName";
    class7.isInterface = false;
    class7.methods = { abstractMethod, publicMethod };
    class7.modifiers = ABSTRACT;
    vector<string> expected7 = { "class", "", "ClassName", "\n", "{", "\n", "public", ":", "$_START", "\n", "virtual", "int", "abstractMethod",
                                 "(", ")", "=", "0", ";", "\n", "virtual", "int", "publicMethod", "(", ")", ";", "$_END", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("7. Abstract Class") << class7 << expected7;

    ClassDef class8;
    class8.name = "ClassName";
    class8.isInterface = false;
    class8.fields = { publicField };
    class8.methods = { publicMethod };
    class8.modifiers = STATIC;
    vector<string> expected8 = { "class", "", "ClassName", "\n", "{", "\n", "public", ":", "$_START", "\n", "static", "int", "publicField",
                                 ";", "\n", "static", "int", "publicMethod", "(", ")", ";", "$_END", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("8. Static Class") << class8 << expected8;

    ClassDef class9;
    class9.name = "ClassName";
    class9.isInterface = false;
    class9.modifiers = FINAL;
    vector<string> expected9 = { "class", "", "ClassName", "final", "\n", "{", "$_END", "\n", "}", ";", "\n", "\n"};
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
    vector<string> expected10 = { "class", "", "ClassName", "\n", "{", "\n", "private", ":", "$_START", "\n", "class", "privateClass", ";", "$_END", "\n",
                                  "public", ":", "$_START", "\n", "class", "publicClass", ";", "\n", "class", "protectedMembers", "\n", "{", "\n", "protected",
                                  ":", "$_START", "\n", "class", "protectedClass", ";", "$_END", "\n", "}", ";", "\n", "class", "defaultMembers", "\n", "{", "\n",
                                  "$_START", "public", ":", "\n", "class", "defaultClass", ";", "$_END", "\n", "}", ";", "$_END", "\n", "}", ";", "\n", "\n", "class",
                                  "ClassName::", "privateClass", "\n", "{", "$_END", "\n", "}", ";", "\n", "\n", "class", "ClassName::protectedMembers::", "protectedClass",
                                  "\n", "{", "$_END", "\n", "}", ";", "\n", "\n", "class", "ClassName::", "publicClass", "\n", "{", "$_END", "\n", "}", ";", "\n", "\n", "class",
                                  "ClassName::defaultMembers::", "defaultClass", "\n", "{", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("10. Nested Classes Of Each Privacy Modifier") << class10 << expected10;

    ClassDef class11;
    class11.name = "ClassName";
    class11.isInterface = false;
    class11.nestedClasses = {
        Nested1
    };
    vector<string> expected11 = { "class", "", "ClassName", "\n", "{", "\n", "public", ":", "$_START", "\n", "class", "Nested1", ";", "$_END", "$_END", "\n", "}", ";",
                                  "\n", "\n", "class", "ClassName::", "Nested1", "\n", "{", "\n", "public", ":", "$_START", "\n", "class", "Nested2", ";", "$_END", "$_END",
                                  "\n", "}", ";", "\n", "\n", "class", "ClassName::Nested1::", "Nested2", "\n", "{", "$_END", "\n", "}", ";", "\n", "\n",  };
    QTest::newRow("11. Twice Nested Class") << class11 << expected11;

    ClassDef class12;
    class12.name = "ClassName";
    class12.isInterface = false;
    class12.extends = "ParentClass";
    vector<string> expected12 = { "class", "", "ClassName", ":", "public", "ParentClass", "\n", "{", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("12. Inheritance From Class") << class12 << expected12;

    ClassDef class13;
    class13.name = "ClassName";
    class13.isInterface = false;
    class13.implements = { "IName1", "IName2" };
    vector<string> expected13 = { "class", "", "ClassName", ":", "public", "IName1", ",", "public", "IName2", "\n", "{", "$_END", "\n", "}", ";", "\n", "\n" };
    QTest::newRow("13. Inheritance From Several Interfaces") << class13 << expected13;
}

void CppCodeGeneratorTest::testCppGenerateMethod()
{
    CppLexer          lexer;
    CodeGenerator     generator;

    vector<string>    tokens;
    string            generatedCode;
    string            expectedCode;

    QFETCH(MethodDef, method);
    QFETCH(vector<string>, expectedOutput);

    lexer.tokenize(method, tokens);
    generator.generate(tokens, generatedCode);
    generator.generate(expectedOutput, expectedCode);

    if (tokens != expectedOutput)
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                .arg(QString::fromStdString(generatedCode))
                .arg(QString::fromStdString(expectedCode));
        QWARN(message.toStdString().c_str());
        QVERIFY(false);
    }
}

void CppCodeGeneratorTest::testCppGenerateMethod_data()
{
    QTest::addColumn<MethodDef>("method");
    QTest::addColumn<vector<string> >("expectedOutput");

    VarDef arg;
    arg.name = "argName";
    arg.type = "int";

    VarDef arg2;
    arg2.name = "secArg";
    arg2.type = "string";

    MethodDef method1;
    method1.name = "method";
    method1.type = "int";
    method1.arguments.push_back(arg);
    vector<string> expected1 = { "virtual", "int", "method", "(", "int", "argName", ")", ";" };
    QTest::newRow("1. No Modifiers") << method1 << expected1;

    MethodDef method2;
    method2.name = "methodName";
    method2.type = "int";
    method2.arguments.push_back(arg);
    method2.modifiers = ABSTRACT;
    vector<string> expected2 = { "virtual", "int", "methodName", "(", "int", "argName", ")", "=", "0", ";" };
    QTest::newRow("2. Abstract") << method2 << expected2;

    MethodDef method3;
    method3.name = "methodName";
    method3.type = "int";
    method3.arguments.push_back(arg);
    method3.modifiers = FINAL;
    vector<string> expected3 = { "virtual", "int", "methodName", "(", "int", "argName", ")", "final", ";" };
    QTest::newRow("3. Final") << method3 << expected3;

    MethodDef method4;
    method4.name = "methodName";
    method4.type = "int";
    method4.arguments.push_back(arg);
    method4.modifiers = STATIC;
    vector<string> expected4 = { "static", "int", "methodName", "(", "int", "argName", ")", ";" };
    QTest::newRow("4. Static") << method4 << expected4;

    MethodDef method5;
    method5.name = "ClassName";
    method5.arguments.push_back(arg);
    method5.isConstructor = true;
    vector<string> expected5 = { "ClassName", "(", "int", "argName", ")", ";" };
    QTest::newRow("5. Constructor") << method5 << expected5;

    MethodDef method6;
    method6.name = "finalize";
    method6.type = "void";
    method6.arguments.push_back(arg);
    method6.modifiers = PROTECTED;
    method6.isDestructor = true;
    vector<string> expected6 = { "~", "finalize", "(", "int", "argName", ")", ";" };
    QTest::newRow("6. Deconstructor") << method6 << expected6;
    /* невозможно задать имя деструктора не зная имени класса, оно задастся при генерации класса */

    MethodDef method7;
    method7.name = "methodName";
    method7.type = "int";
    method7.arguments.push_back(arg);
    method7.annotations.push_back("@Override");
    vector<string> expected7 = { "virtual", "int", "methodName", "(", "int", "argName", ")", "override", ";" };
    QTest::newRow("7. Overriding") << method7 << expected7;

    MethodDef method8;
    method8.name = "methodName";
    method8.type = "int";
    method8.arguments.push_back(arg);
    method8.arguments.push_back(arg2);
    vector<string> expected8 = { "virtual", "int", "methodName", "(", "int", "argName", ",", "string", "secArg", ")", ";" };
    QTest::newRow("8. Many arguments") << method8 << expected8;
}

void CppCodeGeneratorTest::testCppGenerateField()
{
    CppLexer          lexer;
    CodeGenerator     generator;

    vector<string>    tokens;
    string            generatedCode;
    string            expectedCode;

    QFETCH(VarDef, field);
    QFETCH(vector<string>, expectedOutput);

    lexer.tokenize(field, tokens);
    generator.generate(tokens, generatedCode);
    generator.generate(expectedOutput, expectedCode);

    if (tokens != expectedOutput)
    {
        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
                .arg(QString::fromStdString(generatedCode))
                .arg(QString::fromStdString(expectedCode));
        QWARN(message.toStdString().c_str());
        QVERIFY(false);
    }
}

void CppCodeGeneratorTest::testCppGenerateField_data()
{
    QTest::addColumn<VarDef>("field");
    QTest::addColumn<vector<string> >("expectedOutput");

    VarDef field1;
    field1.name = "fieldName";
    field1.type = "int";
    vector<string> expected1 = { "int", "fieldName", ";" };
    QTest::newRow("1. No Modifiers") << field1 << expected1;

    VarDef field2;
    field2.name = "fieldName";
    field2.type = "int";
    field2.modifiers = STATIC;
    vector<string> expected2 = { "static", "int", "fieldName", ";" };
    QTest::newRow("2. Static Field") << field2 << expected2;

    VarDef field3;
    field3.name = "fieldName";
    field3.type = "int";
    field3.modifiers = FINAL;
    vector<string> expected3 = { "const", "int", "fieldName", ";" };
    QTest::newRow("3. Final Field") << field3 << expected3;

    VarDef field4;
    field4.name = "fieldName";
    field4.type = "int";
    field4.value = "378";
    vector<string> expected4 = { "int", "fieldName", "=", "378", ";" };
    QTest::newRow("4. Field With Init") << field4 << expected4;
}
