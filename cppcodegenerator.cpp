#include "cppcodegenerator.h"
#include "util.h"

void CppCodeGenerator::generate(const JavaSourceDef &fileDef, string &dest)
{

}


void CppCodeGenerator::generate(const ClassDef &classDef, string &dest)
{

}

void CppCodeGenerator::generate(const MethodDef &method, string &dest)
{
    bool flagOverride = false;

    /* Добавить аннотации */
    for (const auto &annotation : method.annotations)
    {
        if (annotation == "@Override") flagOverride = true;
        else dest += "/* " + annotation + " */\n";
    }

    if (method.isDestructor) dest += "~";
    else
    {
        /* Добавить модификаторы */
        if (!(method.modifiers & STATIC) && !(method.isConstructor)) dest += "virtual ";
        else if (!(method.isConstructor)) dest += "static ";

        /* Добавить тип и имя */
        if (!method.type.empty()) dest += method.type + " ";
    }

    dest += method.name + "(";

    /* Добавить аргументы */
    for (auto arg : method.arguments)
        dest += arg.type + " " + arg.name + ", ";

    if (!method.arguments.empty()) { dest.pop_back(); dest.pop_back(); }
    dest += ")";

    /* Изменить поведение согласно установленным флагам по правилам преобразования */
    if (flagOverride) dest += " override";
    if (method.modifiers & FINAL) dest += " final";
    if (method.modifiers & ABSTRACT) dest += " = 0";
    dest += ";";
}

void CppCodeGenerator::generate(const VarDef &field, string &dest)
{
    /* Добавить аннотации */
    if (!field.annotations.empty())
    {
        dest += "/* ";
        for (const auto &annotation : field.annotations)
            dest += annotation + ", ";
        dest.erase(dest.end() - 2, dest.end());
        dest += " */\n";
    }

    /* Добавить модификаторы */
    if (field.modifiers & FINAL) dest += "const ";
    if (field.modifiers & STATIC) dest += "static ";

    /* Добавить тип и имя */
    dest += field.type + " " + field.name;

    /* Добавить значение */
    if (!field.value.empty()) dest += " = " + field.value;

    dest += ";";
}
