#ifndef CPPCODEGENERATORTEST_H
#define CPPCODEGENERATORTEST_H
#include "cppcodegenerator.h"
#include <QObject>

class CppCodeGeneratorTest : public QObject
{
    Q_OBJECT

public:
    explicit CppCodeGeneratorTest(QObject *parent = nullptr);

private slots:
    void testCppGenerateFile();
    void testCppGenerateClass();
    void testCppGenerateMethod();
    void testCppGenerateField();
    void testCppGenerateArg();
};

#endif // CPPCODEGENERATORTEST_H
