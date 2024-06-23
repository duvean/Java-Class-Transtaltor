#include <QCoreApplication>
#include "pythoncodegeneratortest.h"
#include "cppcodegeneratortest.h"
#include <QtTest/QTest>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CodeGenerator generator;
    CppLexer lexer;
    PythonLexer pyLexer;
    vector<string> tokens;
    string generatedCode;


    //pyLexer.tokenize(class12, tokens);
    generator.generate(tokens, generatedCode);

    for(const auto token : tokens)
        if (token == "\n")
            cout << "\"_n\", ";
        else cout << '"' << token << "\", ";
    cout << endl << endl;
    cout << generatedCode << endl;

    cout << endl << endl;
    cout << endl << endl;
    CppCodeGeneratorTest testObjectCpp;
    PythonCodeGeneratorTest testObjectPy;
    int resultCpp = QTest::qExec(&testObjectCpp);
    int result = QTest::qExec(&testObjectPy);

    return (result);
    return (0);
}
