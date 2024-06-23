#include "util.h"
#include <QList>

string normalize(const string& input)
{
    std::string res;
    for (char ch : input)
        if (!std::isspace(ch))
            res.push_back(ch);
    return res;
}

template<typename T>
std::vector<T>& operator+=(std::vector<T>& lhs, const std::vector<T>& rhs)
{
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
    return lhs;
}

string toLower(const string& str);



string extractPackageImport(const string& import)
{
    size_t lastDotPos = import.find_last_of('.');
    string packageName = (lastDotPos == string::npos) ? import : import.substr(lastDotPos + 1);
    transform(packageName.begin(), packageName.end(), packageName.begin(), ::tolower);
    return (packageName);
}

string extractClassName(const string& import)
{
    size_t lastDot = import.rfind('.');
    if (lastDot == std::string::npos)
        return "";

    size_t secondLastDot = import.rfind('.', lastDot - 1);
    if (secondLastDot == std::string::npos)
        return "";

    string secondLastPackageName = import.substr(secondLastDot + 1, lastDot - secondLastDot - 1);

    transform(secondLastPackageName.begin(), secondLastPackageName.end(), secondLastPackageName.begin(), ::tolower);

    return (secondLastPackageName);
}

string replaceAllSubstrings(const string& str, const string& from, const string& to)
{
    string res = str;
    size_t start_pos = 0;
    while((start_pos = res.find(from, start_pos)) != string::npos)
    {
        res.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return (res);
}
