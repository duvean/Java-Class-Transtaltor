# Java to C++ and Python Classes Translator

## Introduction

This program translates Java classes into C++ and Python code. It takes an input project file that lists Java source files in XML format, parses the XML files to generate an internal representation of the Java classes, and then uses this representation to generate corresponding C++ header files and Python scripts.

## Installation

### Step 1: Download executable file

Download and install .exe file and .dll dependencies from [here](https://github.com/duvean/JavaClassTranstaltor/)

## Usage

### Running the Program

To run the program, use the following command:
```sh
./JavaClassTranslator.exe <project_file_path> <output_directory>
```

### Example

```sh
./JavaClassTranslator.exe project.txt output
```

The `project.txt` should contain paths to Java XML files, one per line. The generated C++ and Python files will be saved in the `output/C++` and `output/Python` directories respectively.

## Unit Tests

The program includes unit tests for different components. To run the tests, uncomment the test execution lines in the main function:

```cpp
// int result1 = QTest::qExec(&testObjectCpp);
// int result2 = QTest::qExec(&testObjectPy);
// int result3 = QTest::qExec(&testObjectValidate);
// int result4 = QTest::qExec(&testObjectXML);
```

## Files

### Main File

The main file is `main.cpp` which orchestrates the overall translation process and error handling.

### C++ Code Generation Files

- `cppcodegeneratortest.h`
- `cpplexer.h`

### Python Code Generation Files

- `pythoncodegeneratortest.h`
- `pythonlexer.h`

### XML Parsing Files

- `xmlparser.h`
- `xmlparsertest.h`
- `pugixml.hpp`

### Validation Files

- `validatefiletest.h`

## Authors

This program was developed by Alexandr Kulakov.

## License

This program is licensed under the... Idk.
