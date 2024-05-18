#include "cppcodegenerator.h"
#include "util.h"

void CppCodeGenerator::generate(const JavaSourceDef &fileDef, string &dest)
{

}

class Base {};
class Outer
{
    class Nested;
};
class Outer::Nested : public Base
{

};

void generateImplemHeader(const ClassDef &classDef, string &dest)
{
     /* Добавить имя */
     dest += "class " + classDef.fullPath + classDef.name;
     if (classDef.modifiers & FINAL) dest += " final";
     /* Добавить наследование */
     if (!classDef.extends.empty() || !classDef.implements.empty())
          dest += " : ";
     if (!classDef.extends.empty())
          dest += "public " + classDef.extends;
     if (!classDef.implements.empty())
     {
          for (const auto &iName : classDef.implements)
               dest += "public " + iName + ", ";
          dest.pop_back();
          dest.pop_back();
     }
}
void generateDeclHeader(const ClassDef &classDef, string &dest)
{
     /* Добавить имя */
     dest += "class " + classDef.name + ";";
}

void CppCodeGenerator::generate(const ClassDef &classDef, string &dest)
     {

          struct
          {
               std::vector<VarDef>   privateFields;
               std::vector<VarDef>   publicFields;
               std::vector<VarDef>   protectedFields;
               std::vector<VarDef>   defaultFields;
               std::vector<MethodDef>  privateMethods;
               std::vector<MethodDef>  publicMethods;
               std::vector<MethodDef>  protectedMethods;
               std::vector<MethodDef>  defaultMethods;
               std::vector<ClassDef>   privateClasses;
               std::vector<ClassDef>   publicClasses;
               std::vector<ClassDef>   protectedClasses;
               std::vector<ClassDef>   defaultClasses;
          } members;

          /* Отсортировать члены по группам модификаторов доступа */
          for (const auto field : classDef.fields)
          {
               if (field.modifiers & PRIVATE)
                    members.privateFields.push_back(field);
               if (field.modifiers & PROTECTED)
                    members.protectedFields.push_back(field);
               if (field.modifiers & PUBLIC)
                    members.publicFields.push_back(field);
               if (field.modifiers & DEFAULT)
                    members.defaultFields.push_back(field);
          }
          for (auto method : classDef.methods)
          {
//               if ((method.name == "finalize") && (method.type == "void"))
//                    method.name = "~" + classDef.name; method.type = "";
               if (method.modifiers & PRIVATE)
                    members.privateMethods.push_back(method);
               if (method.modifiers & PROTECTED)
                    members.protectedMethods.push_back(method);
               if (method.modifiers & PUBLIC)
                    members.publicMethods.push_back(method);
               if (method.modifiers & DEFAULT)
                    members.defaultMethods.push_back(method);
          }
          for (const auto nested : classDef.nestedClasses)
          {
               if (nested.modifiers & PRIVATE)
                    members.privateClasses.push_back(nested);
               if (nested.modifiers & PROTECTED)
                    members.protectedClasses.push_back(nested);
               if (nested.modifiers & PUBLIC)
                    members.publicClasses.push_back(nested);
               if (nested.modifiers & DEFAULT)
                    members.defaultClasses.push_back(nested);
          }

          for (const auto &annotation : classDef.annotations)
               dest += "/* " + annotation + " */\n";

          /* Добавить модификаторы */
          if (classDef.modifiers & STATIC)
          {
               for (auto method : classDef.methods)
                    method.modifiers |= STATIC;
               for (auto field : classDef.methods)
                    field.modifiers |= STATIC;
               for (auto nested : classDef.methods)
                    nested.modifiers |= STATIC;
          }

          generateImplemHeader(classDef, dest);
          dest += "\n{";

          /* Добавить private секцию */

          if (!members.privateFields.empty() || !members.privateMethods.empty() || !members.privateClasses.empty())
          {
               dest += "\nprivate:";
               for (const auto field : members.privateFields)
               {
                    dest += "\n    ";
                    generate(field, dest);
               }
               for (const auto method : members.privateMethods)
               {
                    dest += "\n    ";
                    generate(method, dest);
               }
               for (const auto nested : members.privateClasses)
               {
                    dest += "\n    ";
                    generateDeclHeader(nested, dest);
               }
          }

          /* Добавить public секцию */
          if (!members.publicFields.empty() || !members.publicMethods.empty() || !members.publicClasses.empty())
          {
               dest += "\npublic:";
               for (const auto field : members.publicFields)
               {
                    dest += "\n    ";
                    generate(field, dest);
               }
               for (const auto method : members.publicMethods)
               {
                    dest += "\n    ";
                    generate(method, dest);
               }
               for (const auto nested : members.publicClasses)
               {
                    dest += "\n    ";
                    generateDeclHeader(nested, dest);
               }
          }

          /* Добавить protected секцию */
          if (!members.protectedFields.empty() || !members.protectedMethods.empty() || !members.protectedClasses.empty())
          {
               if (members.publicFields.empty() && members.publicMethods.empty() && members.publicClasses.empty()) dest += "\npublic:";
               dest += "\n    class protectedMembers\n    {\n";
               dest += "    protected:";
               // dest += "        friend ...";
               for (const auto field : members.protectedFields)
               {
                    dest += "\n        ";
                    generate(field, dest);
               }
               for (const auto method : members.protectedMethods)
               {
                    dest += "\n        ";
                    generate(method, dest);
               }
               for (const auto nested : members.protectedClasses)
               {
                    dest += "\n        ";
                    generateDeclHeader(nested, dest);
               }
               dest += "\n    };";
          }

          /* Добавить default секцию */
          if (!members.defaultFields.empty() || !members.defaultMethods.empty() || !members.defaultClasses.empty())
          {
               if (members.publicFields.empty() && members.publicMethods.empty() && members.publicClasses.empty() &&
                   members.protectedFields.empty() && members.protectedMethods.empty() && members.protectedClasses.empty())
                    dest += "\npublic:";
               dest += "\n    class defaultMembers\n    {\n";
               dest += "    public:";
               // dest += "        friend ...";
               for (const auto field : members.defaultFields)
               {
                    dest += "\n        ";
                    generate(field, dest);
               }
               for (const auto method : members.defaultMethods)
               {
                    dest += "\n        ";
                    generate(method, dest);
               }
               for (const auto nested : members.defaultClasses)
               {
                    dest += "\n        ";
                    generateDeclHeader(nested, dest);
               }
               dest += "\n    };";
          }
          dest += "\n};\n\n";

          /* Добавить реализацию вложенных классов */
          vector<vector<ClassDef>> classes =
          {
              members.privateClasses,
              members.protectedClasses,
              members.publicClasses,
              members.defaultClasses
          };

          for (const auto vector : classes)
               for (const auto nested : vector)
                    generate(nested, dest);

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
