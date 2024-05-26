#include "pythoncodegeneratortest.h"
#include "javasourcedef.h"
#include <QtTest/QtTest>

Q_DECLARE_METATYPE(JavaSourceDef)
Q_DECLARE_METATYPE(ClassDef)
Q_DECLARE_METATYPE(MethodDef)
Q_DECLARE_METATYPE(VarDef)
Q_DECLARE_METATYPE(vector<string>)

PythonCodeGeneratorTest::PythonCodeGeneratorTest(QObject *parent) : QObject(parent)
{

}

//void PythonCodeGeneratorTest::testPythonGenerateFile()
//{
//    PythonCodeGenerator generator;

//    QFETCH(MethodDef, method);
//    QFETCH(QString, expectedOutput);

//    string generatedCode;
//    generator.generate(method, generatedCode);

//    if (QString::fromStdString(generatedCode) != expectedOutput)
//    {
//        QString message = QString("\n=== Generated code: === \n%1\n=== Expected code: === \n%2")
//                            .arg(QString::fromStdString(generatedCode))
//                            .arg(expectedOutput);
//        cout << message.toStdString() << endl;
//        QVERIFY2(false, "");
//    }
//}

//void PythonCodeGeneratorTest::testPythonGenerateFile_data()
//{
//    QTest::addColumn<JavaSourceDef>("fileDef");
//    QTest::addColumn<QString>("expectedOutput");

//    VarDef arg;
//    arg.name = "argName";
//    arg.type = "int";

//    MethodDef publicMethod;
//    publicMethod.name = "methodName";
//    publicMethod.type = "int";
//    publicMethod.arguments = { arg };
//    publicMethod.modifiers |= PUBLIC;

//    VarDef publicField;
//    publicField.name = "fieldName";
//    publicField.type = "int";

//    ClassDef baseClass;
//    baseClass.name = "ClassName";
//    baseClass.isInterface = false;
//    baseClass.fields = { publicField };
//    baseClass.methods = { publicMethod };

//    JavaSourceDef javaFile1;
//    javaFile1.name = "filename";
//    javaFile1.package = "com.example";
//    javaFile1.classes = { baseClass };
//    QString expected1 =
//            R"(
//            class ClassName:
//                a: int
//                def methodName(self, argName):
//                    pass
//            )";
//    QTest::newRow("Public Method") << javaFile1 << expected1;
//}

void PythonCodeGeneratorTest::testPythonGenerateClass()
{
    PythonLexer       lexer;
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

void PythonCodeGeneratorTest::testPythonGenerateClass_data()
{
    QTest::addColumn<ClassDef>("classDef");
    QTest::addColumn<vector<string> >("expectedOutput");

    VarDef arg;
    arg.name = "argName";
    arg.type = "int";

    VarDef publicField;
    publicField.name = "publicField";
    publicField.type = "int";
    publicField.value = "378";
    publicField.modifiers = PUBLIC;
    VarDef privateFinalField;
    privateFinalField.name = "privateFinalField";
    privateFinalField.type = "int";
    privateFinalField.value = "378";
    privateFinalField.modifiers = PRIVATE|FINAL;
    VarDef annotatedField;
    annotatedField.name = "annotatedField";
    annotatedField.type = "bool";
    annotatedField.value = "true";
    annotatedField.annotations = { "@Test" };

    MethodDef publicMethod;
    publicMethod.name = "publicMethod";
    publicMethod.type = "int";
    publicMethod.modifiers = PUBLIC;
    MethodDef abstractMethod;
    abstractMethod.name = "abstractMethod";
    abstractMethod.type = "int";
    abstractMethod.modifiers = ABSTRACT;
    MethodDef finalMethod;
    finalMethod.name = "finalMethod";
    finalMethod.type = "void";
    finalMethod.arguments = { arg };
    finalMethod.modifiers = FINAL;
    MethodDef annotatedMethod;
    annotatedMethod.name = "methodName";
    annotatedMethod.type = "int";
    annotatedMethod.arguments = { arg };
    annotatedMethod.modifiers = PRIVATE;
    annotatedMethod.annotations = { "@Test2" };

    ClassDef nested;
    nested.name = "Nested";
    nested.isInterface = false;
    ClassDef nested2;
    nested2.name = "Nested1";
    nested2.isInterface = false;
    ClassDef nested1;
    nested1.name = "Nested2";
    nested1.isInterface = false;
    nested1.nestedClasses = { nested2 };

    ClassDef class1;
    class1.name = "ClassName";
    class1.isInterface = false;
    class1.fields = { publicField };
    class1.methods = { publicMethod };
    vector<string> expected1 = { "class", "ClassName", ":", "$_START", "\n", "publicField", ":", "int", "=", "378", "\n",
                          "def", "publicMethod", "(", "self", ")", ":", "$_START", "\n", "pass", "$_END", "$_END" };
    QTest::newRow("1. One Field One Method") << class1 << expected1;

    ClassDef class2;
    class2.name = "IName";
    class2.isInterface = true;
    vector<string> expected2 ={ "class", "IName", "(", "ABC", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("2. Interface") << class2 << expected2;

    ClassDef class3;
    class3.name = "FuncInterface";
    class3.isInterface = true;
    class3.annotations = { "@FunctionalInterface" };
    vector<string> expected3 ={ "#", "@FunctionalInterface", "#", "\n", "class", "FuncInterface", "(", "ABC", ")", ":",
                                "$_START", "\n", "pass", "$_END" };
    QTest::newRow("3. Functional Interface") << class3 << expected3;

    ClassDef class4;
    class4.name = "ClassName";
    class4.isInterface = false;
    class4.modifiers = PRIVATE;
    vector<string> expected4 = { "class", "__ClassName", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("4. Private Class") << class4 << expected4;

    ClassDef class5;
    class5.name = "ClassName";
    class5.isInterface = false;
    class5.modifiers = PROTECTED;
    vector<string> expected5 = { "class", "_ClassName", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("5. Protected Class") << class5 << expected5;

    ClassDef class6;
    class6.name = "AbstractClass";
    class6.isInterface = false;
    class6.modifiers = ABSTRACT;
    class6.fields = { publicField };
    class6.methods = { abstractMethod };
    vector<string> expected6 = { "class", "AbstractClass", "(", "ABC", ")", ":", "$_START", "\n", "publicField", ":", "int", "=", "378", "\n",
                                 "@", "abstractmethod", "\n", "def", "abstractMethod", "(", "self", ")", ":",
                                 "$_START", "\n", "pass", "$_END", "$_END" };
    QTest::newRow("6. Abstract Class") << class6 << expected6;

    ClassDef class7;
    class7.name = "StaticClass";
    class7.isInterface = false;
    class7.modifiers = STATIC;
    class7.fields = { publicField };
    class7.methods = { abstractMethod };
    vector<string> expected7 = { "class", "StaticClass", ":", "$_START", "\n", "publicField", ":", "int", "=", "378", "\n",
                                 "@", "abstractmethod", "\n", "@", "staticmethod", "\n", "def", "abstractMethod", "(", "self", ")", ":",
                                 "$_START", "\n", "pass", "$_END", "$_END" };
    QTest::newRow("7. Static Class") << class7 << expected7;

    ClassDef class8;
    class8.name = "FinalClass";
    class8.isInterface = false;
    class8.modifiers = FINAL;
    vector<string> expected8 = { "@", "final", "\n", "class", "FinalClass", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("8. Static Class") << class8 << expected8;

    ClassDef class9;
    class9.name = "ClassName";
    class9.isInterface = false;
    class9.nestedClasses = { nested };
    vector<string> expected9 = { "class", "ClassName", ":", "$_START", "\n", "class", "Nested", ":", "$_START", "\n", "pass", "$_END", "$_END" };
    QTest::newRow("9. With Nested Class") << class9 << expected9;

    ClassDef class10;
    class10.name = "ClassName";
    class10.isInterface = false;
    class10.nestedClasses = { nested1 };
    vector<string> expected10 = { "class", "ClassName", ":", "$_START", "\n", "class", "Nested2", ":", "$_START", "\n", "class", "Nested1", ":",
                                  "$_START", "\n", "pass", "$_END", "$_END", "$_END" };
    QTest::newRow("10. With Nested Class With Nested Class") << class10 << expected10;

    ClassDef class11;
    class11.name = "ClassName";
    class11.isInterface = false;
    class11.extends = "ParentName";
    vector<string> expected11 = { "class", "ClassName", "(", "ParentName", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("11. Inheritance From Class") << class11 << expected11;

    ClassDef class12;
    class12.name = "ClassName";
    class12.isInterface = false;
    class12.implements = { "IName1", "IName2" };
    vector<string> expected12 = { "class", "ClassName", "(", "IName1", ",", "IName2", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("12. Inheritance From Several Interfaces") << class12 << expected12;

    ClassDef class13;
    class13.name = "ClassName";
    class13.isInterface = false;
    class13.extends = "Parent";
    class13.implements = { "IName1", "IName2" };
    vector<string> expected13 = { "class", "ClassName", "(", "Parent", ",", "IName1", ",", "IName2", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("13. Inheritance From Class And Several Interfaces") << class13 << expected13;

    ClassDef class14;
    class14.name = "ClassName";
    class14.isInterface = false;
    class14.fields = { publicField, privateFinalField };
    class14.methods = { abstractMethod, finalMethod };
    vector<string> expected14 = { "class", "ClassName", ":", "$_START", "\n", "publicField", ":", "int", "=", "378", "\n",
                                  "__privateFinalField", ":", "Final", "[", "int", "]", "=", "378", "\n", "@", "abstractmethod", "\n",
                                  "def", "abstractMethod", "(", "self", ")", ":", "$_START", "\n", "pass", "$_END", "\n",
                                  "@", "final", "\n", "def", "finalMethod", "(", "self", ",", "argName", ")", ":",
                                  "$_START", "\n", "pass", "$_END", "$_END" };
    QTest::newRow("14. Several Fields And Methods") << class14 << expected14;

    ClassDef class15;
    class15.name = "AnnotatedClass";
    class15.isInterface = false;
    class15.fields = { annotatedField };
    class15.methods = { annotatedMethod };
    class15.annotations = { "@Deprecated" };
    vector<string> expected15 = { "#", "@Deprecated", "#", "\n", "class", "AnnotatedClass", ":", "$_START", "\n", "#", "@Test", "#", "\n",
                                  "annotatedField", ":", "bool", "=", "true", "\n", "#", "@Test2", "#", "\n", "def",
                                  "__methodName", "(", "self", ",", "argName", ")", ":", "$_START", "\n", "pass", "$_END", "$_END" };
    QTest::newRow("15. Class, Field and Method with Annotations") << class15 << expected15;

}

void PythonCodeGeneratorTest::testPythonGenerateMethod()
{
    PythonLexer       lexer;
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

void PythonCodeGeneratorTest::testPythonGenerateMethod_data()
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
    method1.name = "publicMethod";
    method1.type = "int";
    method1.modifiers = PUBLIC;
    vector<string> expected1 = { "def", "publicMethod", "(", "self", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("1. Public Method") << method1 << expected1;

    MethodDef method2;
    method2.name = "privateMethod";
    method2.type = "int";
    method2.arguments = { arg };
    method2.modifiers = PRIVATE;
    vector<string> expected2 = { "def", "__privateMethod", "(", "self", ",", "argName", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("2. Private Method") << method2 << expected2;

    MethodDef method3;
    method3.name = "protectedMethod";
    method3.type = "int";
    method3.arguments = { arg };
    method3.modifiers = PROTECTED;
    vector<string> expected3 = { "def", "_protectedMethod", "(", "self", ",", "argName", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("3. Protected Method") << method3 << expected3;

    MethodDef method4;
    method4.name = "defaultMethod";
    method4.type = "int";
    method4.arguments = { arg };
    method4.modifiers = DEFAULT;
    vector<string> expected4 = { "def", "defaultMethod", "(", "self", ",", "argName", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("4. Default Method") << method4 << expected4;

    MethodDef method5;
    method5.name = "abstractMethod";
    method5.type = "int";
    method5.arguments = { arg };
    method5.modifiers = ABSTRACT;
    vector<string> expected5 = { "@", "abstractmethod", "\n", "def", "abstractMethod", "(", "self", ",", "argName", ")", ":",
                                 "$_START", "\n", "pass", "$_END" };
    QTest::newRow("5. Abstract Method") << method5 << expected5;

    MethodDef method6;
    method6.name = "staticMethod";
    method6.type = "int";
    method6.arguments = { arg };
    method6.modifiers = STATIC;
    vector<string> expected6 = { "@", "staticmethod", "\n", "def", "staticMethod", "(", "self", ",", "argName", ")", ":",
                                 "$_START", "\n", "pass", "$_END" };
    QTest::newRow("6. Static Method") << method6 << expected6;

    MethodDef method7;
    method7.name = "finalMethod";
    method7.type = "int";
    method7.arguments = { arg };
    method7.modifiers = FINAL;
    vector<string> expected7 = { "@", "final", "\n", "def", "finalMethod", "(", "self", ",", "argName", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("7. Final Method") << method7 << expected7;

    MethodDef method8;
    method8.name = "ClassName";
    method8.isConstructor = true;
    method8.type = "int";
    method8.arguments = { arg };
    vector<string> expected8 = { "def", "__init__", "(", "self", ",", "argName", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("8. Constructor") << method8 << expected8;

    MethodDef method9;
    method9.name = "finalize";
    method9.isDestructor = true;
    method9.type = "void";
    method9.modifiers = PROTECTED;
    method9.arguments = { arg };
    vector<string> expected9 = { "def", "__del__", "(", "self", ",", "argName", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("9. Destructor") << method9 << expected9;

    MethodDef method10;
    method10.name = "arrayMethod";
    method10.type = "int[]";
    method10.modifiers = PROTECTED;
    vector<string> expected10 = { "def", "_arrayMethod", "(", "self", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("10. Array Return") << method10 << expected10;

    MethodDef method11;
    method11.name = "manyArgsMethod";
    method11.type = "void";
    method11.arguments = { arg, arg2 };
    vector<string> expected11 = { "def", "manyArgsMethod", "(", "self", ",", "argName", ",", "secArg", ")", ":", "$_START", "\n", "pass", "$_END" };
    QTest::newRow("11. Several Args") << method11 << expected11;
}

void PythonCodeGeneratorTest::testPythonGenerateField()
{
    PythonLexer       lexer;
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

void PythonCodeGeneratorTest::testPythonGenerateField_data()
{
    QTest::addColumn<VarDef>("field");
    QTest::addColumn<vector<string> >("expectedOutput");

    VarDef field1;
    field1.name = "publicField";
    field1.type = "int";
    field1.modifiers = PUBLIC;
    vector<string> expected1 = { "publicField", ":", "int" };
    QTest::newRow("1. Public Field") << field1 << expected1;

    VarDef field2;
    field2.name = "protectedField";
    field2.type = "int";
    field2.value = "378";
    field2.modifiers = PROTECTED;
    vector<string> expected2 = { "_protectedField", ":", "int", "=", "378" };
    QTest::newRow("2. Protected Field") << field2 << expected2;

    VarDef field3;
    field3.name = "privateField";
    field3.type = "int";
    field3.value = "378";
    field3.modifiers = PRIVATE;
    vector<string> expected3 = { "__privateField", ":", "int", "=", "378" };
    QTest::newRow("3. Private Field") << field3 << expected3;

    VarDef field4;
    field4.name = "defaultField";
    field4.type = "int";
    field4.value = "378";
    field4.modifiers = DEFAULT;
    vector<string> expected4 = { "defaultField", ":", "int", "=", "378" };
    QTest::newRow("4. Default Field") << field4 << expected4;

    VarDef field5;
    field5.name = "staticField";
    field5.type = "int";
    field5.value = "378";
    field5.modifiers = STATIC;
    vector<string> expected5 = { "staticField", ":", "int", "=", "378" };
    QTest::newRow("5. Static Field") << field5 << expected5;

    VarDef field6;
    field6.name = "finalField";
    field6.type = "int";
    field6.value = "11";
    field6.modifiers = FINAL;
    vector<string> expected6 = { "finalField", ":", "Final", "[", "int", "]", "=", "11" };
    QTest::newRow("6. Final Field") << field6 << expected6;
}
