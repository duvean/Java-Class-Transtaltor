#include <QCoreApplication>
#include "cppcodegeneratortest.h"
#include <QtTest/QTest>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CppCodeGeneratorTest testObject;

    int result = QTest::qExec(&testObject);

    return result;
}
