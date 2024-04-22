#ifndef PYTHONCODEGENERATORTEST_H
#define PYTHONCODEGENERATORTEST_H
#include "pythoncodegenerator.h"
#include <QObject>

class PythonCodeGeneratorTest : public QObject
{
    Q_OBJECT

public:
    explicit PythonCodeGeneratorTest(QObject *parent = nullptr);

private slots:
    void testPythonGenerateFile();
    void testPythonGenerateClass();
    void testPythonGenerateMethod();
    void testPythonGenerateField();
    void testPythonGenerateArg();

    void testPythonGenerateFile_data();
    void testPythonGenerateClass_data();
    void testPythonGenerateMethod_data();
    void testPythonGenerateField_data();
    void testPythonGenerateArg_data();
};

#endif // PYTHONCODEGENERATORTEST_H
