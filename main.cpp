#include <QCoreApplication>
#include "cppcodegeneratortest.h"
#include <QtTest/QTest>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Instantiate your test class
    CppCodeGeneratorTest testObject;

    // Execute the tests
    int result = QTest::qExec(&testObject);

    // Return the test result
    return result;
}
