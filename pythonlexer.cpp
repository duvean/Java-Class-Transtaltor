#include "pythonlexer.h"
#include "util.h"

PythonLexer::PythonLexer()
{

}

void PythonLexer::tokenize(const JavaSourceDef &fileDef, vector<string> &dest)
{

}

void PythonLexer::tokenize(const ClassDef &classDef, vector<string> &dest)
{
    string nameLoc = classDef.name;
    if (classDef.modifiers & PRIVATE)
        nameLoc = "__" + nameLoc;
    else if (classDef.modifiers & PROTECTED)
        nameLoc = "_" + nameLoc;

    for (auto &ann : classDef.annotations)
         dest.insert(dest.end(), { "#", ann, "#", "\n" });

    if (classDef.modifiers & FINAL)
        dest.insert(dest.end(), { "@", "final", "\n" });

    dest.insert(dest.end(), { "class", nameLoc });

    /* Добавить наследование */
    if (!classDef.extends.empty() || !classDef.implements.empty() || classDef.isInterface || (classDef.modifiers & ABSTRACT))
    {
        dest.insert(dest.end(), { "(" });
        if (classDef.isInterface || (classDef.modifiers & ABSTRACT))
        {
            dest.insert(dest.end(), { "ABC" });
            if (!classDef.extends.empty() || !classDef.implements.empty())
                dest.insert(dest.end(), { "," });
        }
        if (!classDef.extends.empty())
        {
            dest.insert(dest.end(), { classDef.extends });
            if (!classDef.implements.empty())
                dest.insert(dest.end(), { "," });
        }
        for (auto i : classDef.implements)
            dest.insert(dest.end(), { i, "," });
        if (!classDef.implements.empty())
            dest.pop_back();
        dest.insert(dest.end(), { ")" });
    }
    dest.insert(dest.end(), { ":", "$_START" });

    if (classDef.fields.empty() && classDef.methods.empty() && classDef.nestedClasses.empty())
        dest.insert(dest.end(), { "\n", "pass" });
    else
    {
        for (auto field : classDef.fields)
        {
            dest.insert(dest.end(), { "\n" });
            tokenize(field, dest);
        }
        for (auto method : classDef.methods)
        {
            dest.insert(dest.end(), { "\n" });
            if (classDef.modifiers & STATIC)
            {
                MethodDef methodNew = method;
                methodNew.modifiers |= STATIC;
                tokenize(methodNew, dest);
            }
            else tokenize(method, dest);

        }
        for (auto nested : classDef.nestedClasses)
        {
            dest.insert(dest.end(), { "\n" });
            if (classDef.modifiers & STATIC)
            {
                ClassDef classNew = nested;
                classNew.modifiers |= STATIC;
                tokenize(classNew, dest);
            }
            else tokenize(nested, dest);
        }
    }
   dest.insert(dest.end(), { "$_END" });


}
void PythonLexer::tokenize(const MethodDef &method, vector<string> &dest)
{
    string nameLoc = method.name;
    if (method.modifiers & PRIVATE)
        nameLoc = "__" + nameLoc;
    else if (method.modifiers & PROTECTED)
        nameLoc = "_" + nameLoc;
    if (method.isConstructor)
        nameLoc = "__init__";
    else if (method.isDestructor)
        nameLoc = "__del__";

    for (auto annotation : method.annotations)
         if (annotation != "@Override")
              dest.insert(dest.end(), { "#", annotation, "#", "\n" });

    if (method.modifiers & ABSTRACT) dest.insert(dest.end(), { "@", "abstractmethod", "\n" });
    if (method.modifiers & FINAL) dest.insert(dest.end(), { "@", "final", "\n" });
    if (method.modifiers & STATIC) dest.insert(dest.end(), { "@", "staticmethod", "\n" });

    dest.insert(dest.end(), { "def", nameLoc });

    /* Секция аргументов */
    dest.insert(dest.end(), { "(", "self", "," });
    for (auto arg : method.arguments)
    {
        dest.insert(dest.end(), { arg.name });
        dest.insert(dest.end(), { "," });
    }
    dest.pop_back();
    dest.insert(dest.end(), { ")", ":", "$_START", "\n", "pass", "$_END" });
}

void PythonLexer::tokenize(const VarDef &field, vector<string> &dest)
{
    string nameLoc = field.name;
    if (field.modifiers & PRIVATE) nameLoc = "__" + nameLoc;
    else if (field.modifiers & PROTECTED) nameLoc = "_" + nameLoc;

    for (auto annotation : field.annotations)
         dest.insert(dest.end(), { "#", annotation, "#", "\n" });

    if (field.modifiers & FINAL)
        dest.insert(dest.end(), { nameLoc, ":", "Final", "[", field.type, "]" });
    else
        dest.insert(dest.end(), { nameLoc, ":", field.type });
    if (!field.value.empty())
        dest.insert(dest.end(), { "=", field.value });
}
