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
};

#endif // PYTHONCODEGENERATORTEST_H
