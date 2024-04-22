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

    void testCppGenerateFile_data();
    void testCppGenerateClass_data();
    void testCppGenerateMethod_data();
    void testCppGenerateField_data();
    void testCppGenerateArg_data();
};

#endif // CPPCODEGENERATORTEST_H
