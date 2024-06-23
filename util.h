#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

static string normalize(const string& input);

template<typename T>
std::vector<T>& operator+=(std::vector<T>& lhs, const std::vector<T>& rhs);

template<typename T>
std::vector<std::string>& operator+=(std::vector<std::string>& vec, const std::string& element);

string extractPackageImport(const string& import);

string extractClassName(const string& import);

string replaceAllSubstrings(const string& str, const string& from, const string& to);

#endif // UTIL_H
