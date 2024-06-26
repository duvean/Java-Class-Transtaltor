#include "codegenerator.h"
#include "util.h"
#define TAB "    "

void CodeGenerator::generate(const vector<string> &tokens, string &dest)
{
    int indentationLevel = 0;
    bool lastWasSpecial = false;

    for (const auto &token : tokens)
    {
        if (token == "$_START")
        {
            indentationLevel++;
            lastWasSpecial = false;
        }
        else if (token == "$_END")
        {
            indentationLevel = max(indentationLevel - 1, 0);
            lastWasSpecial = false;
        }
        else
        {
            if (dest.empty() || dest.back() == '\n')
                for (int i = 0; i < indentationLevel; ++i)
                    dest += '\t';

            else if (!lastWasSpecial &&
                     token != ";" &&
                     token != "," &&
                     token != "." &&
                     token != "(" &&
                     token != ")" &&
                     token != "{" &&
                     token != "}" &&
                     token != "[" &&
                     token != "]" &&
                     token != ":")
                dest += ' ';

            dest += token;

            lastWasSpecial = (token == "#" ||
                              token == ";" ||
                              token == "," ||
                              token == "." ||
                              token == "(" ||
                              token == ")" ||
                              token == "{" ||
                              token == "}" ||
                              token == "[" ||
                              token == "]" ||
                              token == ":");
        }
    }
}

