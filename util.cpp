#include "util.h"

util::util()
{

}

string util::normalize(const string& input)
{
    std::string res;
    for (char ch : input)
        if (!std::isspace(ch))
            res.push_back(ch);
    return res;
}
