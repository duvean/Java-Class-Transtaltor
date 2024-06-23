/*!
 * \mainpage Java to C++ and Python Classes Translator
 *
 * \section intro_sec Introduction
 *
 * This program translates Java classes into C++ and Python code. It takes an input project file 
 * that lists Java source files in XML format, parses the XML files to generate an internal 
 * representation of the Java classes, and then uses this representation to generate corresponding 
 * C++ header files and Python scripts.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Download executable file
 * 
 * Download and install .exe file and .dll dependencies from https://github.com/duvean/JavaClassTranstaltor/
 *
 * \section usage_sec Usage
 *
 * \subsection run_sec Running the Program
 *
 * To run the program, use the following command:
 * \code{.sh}
 * ./JavaClassTranslator.exe <project_file_path> <output_directory>
 * \endcode
 *
 * \subsection example_sec Example
 *
 * \code{.sh}
 * ./JavaClassTranslator.exe project.txt output
 * \endcode
 * 
 * The `project.txt` should contain paths to Java XML files, one per line. The generated C++ and 
 * Python files will be saved in the `output/C++` and `output/Python` directories respectively.
 *
 * \section test_sec Unit Tests
 *
 * The program includes unit tests for different components. To run the tests, uncomment the test 
 * execution lines in the main function:
 *
 * \code{.cpp}
 * // int result1 = QTest::qExec(&testObjectCpp);
 * // int result2 = QTest::qExec(&testObjectPy);
 * // int result3 = QTest::qExec(&testObjectValidate);
 * // int result4 = QTest::qExec(&testObjectXML);
 * \endcode
 *
 * \section file_sec Files
 *
 * \subsection main_file Main File
 *
 * The main file is `main.cpp` which orchestrates the overall translation process and error handling.
 *
 * \subsection cpp_codegen_files C++ Code Generation Files
 *
 * - `cppcodegeneratortest.h`
 * - `cpplexer.h`
 *
 * \subsection python_codegen_files Python Code Generation Files
 *
 * - `pythoncodegeneratortest.h`
 * - `pythonlexer.h`
 *
 * \subsection xml_files XML Parsing Files
 *
 * - `xmlparser.h`
 * - `xmlparsertest.h`
 * - `pugixml.hpp`
 *
 * \subsection validation_files Validation Files
 *
 * - `validatefiletest.h`
 *
 * \section authors_sec Authors
 *
 * This program was developed by Alexandr Kulakov.
 *
 * \section license_sec License
 *
 * This program is licensed under the... Idk.
 *
 */
