#include "cppcodegenerator.h"

void CppCodeGenerator::generate(const JavaSourceDef &fileDef, string &dest)
{

}


void CppCodeGenerator::generate(const ClassDef &classDef, string &dest)
{

}

void CppCodeGenerator::generate(const MethodDef &method, string &dest)
{
    string  res;
    bool    flagOverride = false;

    /* Добавить аннотации */
    for (const auto &annotation : method.annotations)
    {
        if (annotation == "@Override") flagOverride = true;
        else res += "/* " + annotation + " */\n";
    }

    /* Добавить модификаторы */
    if (!(method.modifiers & STATIC)) res += "virtual ";
    else res += "static ";

    /* Добавить тип и имя */
    res += method.type + " " + method.name + "(";

    /* Добавить аргументы */
    for (auto arg : method.arguments)
    {
        generate(arg, res);
        res += ", ";
    }

    res.pop_back();
    res.pop_back();
    res += ")";

    /* Изменить поведение согласно установленным флагам по правилам преобразования */
    if (flagOverride) res += " override";
    if (method.modifiers & FINAL) res += " final";
    if (method.modifiers & ABSTRACT) res += " = 0";
    res += ";";

    dest += res;
}

void CppCodeGenerator::generate(const VarDef &field, string &dest)
{
    string res;

    /* Добавить аннотации */
    if (!field.annotations.empty())
    {
        res += "/* ";
        for (const auto &annotation : field.annotations)
            res += annotation + ", ";
        res.erase(res.end() - 2, res.end());
        res += " */\n";
    }

    /* Добавить модификаторы */
    if (field.modifiers & FINAL) res += "const ";
    if (field.modifiers & STATIC) res += "static ";

    /* Добавить тип и имя */
    res += field.type + " " + field.name;

    /* Добавить значение */
    if (!field.value.empty()) res += " = " + field.value;

    res += ";";
    dest += res;
}

void CppCodeGenerator::generateArg(const VarDef &arg, string &dest)
{

}
