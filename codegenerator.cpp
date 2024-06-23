#include "codegenerator.h"
#include "util.h"
#define TAB "    "

void CodeGenerator::generate(const vector<string>& tokens, string& dest)
{
    int indentationLevel = 0;

    for (const auto& token : tokens)
    {
        if (token == "$_START")
            indentationLevel++; // Увеличить уровень вложенности

        else if (token == "$_END")
            indentationLevel = max(indentationLevel - 1, 0); // Уменьшить уровень вложенности

        else
        {
            // Если начало новой строки, добавить отступ
            if (dest.empty() || dest.back() == '\n')
                for (int i = 0; i < indentationLevel; ++i)
                    dest += TAB; // Добавить табы для отступа

            // Если не начало строки, добавить пробел перед очередным токеном
            else
                dest += ' ';
            dest += token;
        }
    }
}

