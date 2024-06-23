#include "cpplexer.h"
#include "util.h"

CppLexer::CppLexer()
{

}

vector<string>& operator+=(vector<string>& vec, const string& str)
{
    vec.push_back(str);
    return vec;
}

void CppLexer::tokenize(const JavaSourceDef &fileDef, vector<string> &dest)
{
    for (auto &import : fileDef.packageImports)
         dest.insert(dest.end(), { "#",  "include", extractPackageImport(import), ".", "h", "\n" });
    for (auto &import : fileDef.classImports)
         dest.insert(dest.end(), { "#",  "include", extractClassName(import), ".", "h", "\n" });
    for (auto &import : fileDef.packageImports)
         dest.insert(dest.end(), { "using",  "namespace", replaceAllSubstrings(import, ".", "::"), ";", "\n"});
    for (auto &import : fileDef.classImports)
         dest.insert(dest.end(), { "using",  "namespace", replaceAllSubstrings(import, ".", "::"), ";", "\n"});
    if (!fileDef.packageImports.empty() || !fileDef.classImports.empty())
        dest += "\n";

    dest.insert(dest.end(), { "namespace", fileDef.package, "\n", "{", "\n"});

    for (auto &classDef : fileDef.classes)
    {
         tokenize(classDef, dest);
         dest += "\n";
    }
}

void tokenizeImplemHeader(const ClassDef &classDef, vector<string> &dest)
{
     /* Добавить имя */
     dest.insert(dest.end(), { "class", classDef.fullPath, classDef.name });
     if (classDef.modifiers & FINAL) dest += { "final" };
     /* Добавить наследование */
     if (!classDef.extends.empty() || !classDef.implements.empty())
          dest += {":"};
     if (!classDef.extends.empty())
          dest.insert(dest.end(), { "public", classDef.extends });
     if (!classDef.implements.empty())
     {
          for (const auto &iName : classDef.implements)
               dest.insert(dest.end(), { "public", iName, "," });
          dest.pop_back();
     }
}
void tokenizeDeclHeader(const ClassDef &classDef, vector<string> &dest)
{
     /* Добавить имя */
     dest.insert(dest.end(), { "class", classDef.name, ";" });
}

void CppLexer::tokenize(const ClassDef &classDef, vector<string> &dest)
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


          for (const auto &annotation : classDef.annotations)
               dest.insert(dest.end(), { "/*", annotation, "*/", "\n" });

          /* Добавить модификаторы */
          if (classDef.modifiers & STATIC)
          {
              for (auto field : classDef.fields)
              {
                   if (field.modifiers & PRIVATE)
                   {
                       VarDef fieldNew = field;
                       fieldNew.modifiers |= STATIC;
                       members.privateFields.push_back(fieldNew);
                   }

                   if (field.modifiers & PROTECTED)
                   {
                       VarDef fieldNew = field;
                       fieldNew.modifiers |= STATIC;
                       members.protectedFields.push_back(fieldNew);
                   }
                   if (field.modifiers & PUBLIC)
                   {
                       VarDef fieldNew = field;
                       fieldNew.modifiers |= STATIC;
                       members.publicFields.push_back(fieldNew);
                   }
                   if (field.modifiers & DEFAULT)
                   {
                       VarDef fieldNew = field;
                       fieldNew.modifiers |= STATIC;
                       members.defaultFields.push_back(fieldNew);
                   }
              }
              for (auto method : classDef.methods)
              {
                  if (method.modifiers & PRIVATE)
                  {
                      MethodDef methodNew = method;
                      methodNew.modifiers |= STATIC;
                      members.privateMethods.push_back(methodNew);
                  }

                  if (method.modifiers & PROTECTED)
                  {
                      MethodDef methodNew = method;
                      methodNew.modifiers |= STATIC;
                      members.protectedMethods.push_back(methodNew);
                  }
                  if (method.modifiers & PUBLIC)
                  {
                      MethodDef methodNew = method;
                      methodNew.modifiers |= STATIC;
                      members.publicMethods.push_back(methodNew);
                  }
                  if (method.modifiers & DEFAULT)
                  {
                      MethodDef methodNew = method;
                      methodNew.modifiers |= STATIC;
                      members.defaultMethods.push_back(methodNew);
                  }
              }
              for (auto nested : classDef.nestedClasses)
              {
                  if (nested.modifiers & PRIVATE)
                  {
                      ClassDef classNew = nested;
                      classNew.modifiers |= STATIC;
                      members.privateClasses.push_back(classNew);
                  }

                  if (nested.modifiers & PROTECTED)
                  {
                      ClassDef classNew = nested;
                      classNew.modifiers |= STATIC;
                      members.protectedClasses.push_back(classNew);
                  }
                  if (nested.modifiers & PUBLIC)
                  {
                      ClassDef classNew = nested;
                      classNew.modifiers |= STATIC;
                      members.publicClasses.push_back(classNew);
                  }
                  if (nested.modifiers & DEFAULT)
                  {
                      ClassDef classNew = nested;
                      classNew.modifiers |= STATIC;
                      members.defaultClasses.push_back(classNew);
                  }
              }
          }
          else
          {
              for (auto field : classDef.fields)
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
              for (auto nested : classDef.nestedClasses)
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
          }

          tokenizeImplemHeader(classDef, dest);
          dest.insert(dest.end(), { "\n", "{" });

          /* Добавить private секцию */

          if (!members.privateFields.empty() || !members.privateMethods.empty() || !members.privateClasses.empty())
          {
               dest.insert(dest.end(), { "\n", "private", ":", "$_START" });
               for (auto field : members.privateFields)
               {
                    dest += "\n";
                    tokenize(field, dest);
               }
               for (auto method : members.privateMethods)
               {
                    dest += "\n";
                    tokenize(method, dest);
               }
               for (auto nested : members.privateClasses)
               {
                    dest += "\n";
                    tokenizeDeclHeader(nested, dest);
               }

               dest += "$_END";
          }

          /* Добавить public секцию */
          if (!members.publicFields.empty() || !members.publicMethods.empty() || !members.publicClasses.empty())
          {
               dest.insert(dest.end(), { "\n", "public", ":", "$_START" });
               for (auto field : members.publicFields)
               {
                    dest += "\n";
                    tokenize(field, dest);
               }
               for (auto method : members.publicMethods)
               {
                    dest += "\n";
                    tokenize(method, dest);
               }
               for (auto nested : members.publicClasses)
               {
                    dest += "\n";
                    tokenizeDeclHeader(nested, dest);
               }

               if (members.protectedFields.empty() && members.protectedMethods.empty() && members.protectedClasses.empty()
                       && members.defaultFields.empty() && members.defaultMethods.empty() && members.defaultClasses.empty()) dest += "$_END";
          }

          /* Добавить protected секцию */
          if (!members.protectedFields.empty() || !members.protectedMethods.empty() || !members.protectedClasses.empty())
          {
               if (members.publicFields.empty() && members.publicMethods.empty() && members.publicClasses.empty()) dest.insert(dest.end(), { "\n", "public", ":", "$_START" });
               dest.insert(dest.end(), { "\n", "class", "protectedMembers", "\n",  "{", "\n" });
               dest.insert(dest.end(), { "protected", ":", "$_START" });
               // dest += "        friend ...";
               for (auto field : members.protectedFields)
               {
                    dest += "\n";
                    tokenize(field, dest);
               }
               for (auto method : members.protectedMethods)
               {
                    dest += "\n";
                    tokenize(method, dest);
               }
               for (auto nested : members.protectedClasses)
               {
                    dest += "\n";
                    tokenizeDeclHeader(nested, dest);
               }

               dest.insert(dest.end(), { "$_END", "\n", "}", ";" });
          }

          /* Добавить default секцию */
          if (!members.defaultFields.empty() || !members.defaultMethods.empty() || !members.defaultClasses.empty())
          {
               if (members.publicFields.empty() && members.publicMethods.empty() && members.publicClasses.empty() &&
                   members.protectedFields.empty() && members.protectedMethods.empty() && members.protectedClasses.empty())
                    dest.insert(dest.end(), { "\n", "public", ":", "$_START" });
               dest.insert(dest.end(), { "\n", "class", "defaultMembers", "\n", "{", "\n", "$_START" });
               dest.insert(dest.end(), { "public", ":" });
               // dest += "        friend ...";
               for (auto field : members.defaultFields)
               {
                    dest += "\n";
                    tokenize(field, dest);
               }
               for (auto method : members.defaultMethods)
               {
                    dest += "\n";
                    tokenize(method, dest);
               }
               for (auto nested : members.defaultClasses)
               {
                    dest += "\n";
                    tokenizeDeclHeader(nested, dest);
               }
               dest.insert(dest.end(), { "$_END", "\n", "}", ";" });
          }
          dest.insert(dest.end(), { "$_END", "\n", "}", ";", "\n", "\n" });

          /* Добавить реализацию вложенных классов */
          vector<vector<ClassDef>> classes =
          {
              members.privateClasses,
              members.protectedClasses,
              members.publicClasses,
              members.defaultClasses
          };

          for (auto vector : classes)
               for (auto nested : vector)
                    tokenize(nested, dest);

     }

void CppLexer::tokenize(const MethodDef &method, vector<string> &dest)
{
    bool flagOverride = false;

    /* Добавить аннотации */
    for (const auto &annotation : method.annotations)
    {
        if (annotation == "@Override") flagOverride = true;
        else dest.insert(dest.end(), { "/*", annotation, "*/", "\n" });
    }

    if (method.isDestructor) dest += "~";
    else
    {
        /* Добавить модификаторы */
        if (!(method.modifiers & STATIC) && !(method.isConstructor)) dest += "virtual";
        else if (!(method.isConstructor)) dest += "static";

        /* Добавить тип и имя */
        if (!method.type.empty()) dest += method.type;
    }

    dest.insert(dest.end(), { method.name, "(" });

    /* Добавить аргументы */
    for (auto arg : method.arguments)
        dest.insert(dest.end(), { arg.type, arg.name, "," });

    if (!method.arguments.empty())
        dest.pop_back();
    dest += ")";

    /* Изменить поведение согласно установленным флагам по правилам преобразования */
    if (flagOverride) dest += "override";
    if (method.modifiers & FINAL) dest += "final";
    if (method.modifiers & ABSTRACT) dest.insert(dest.end(), { "=",  "0" });
    dest += ";";
}

void CppLexer::tokenize(const VarDef &field, vector<string> &dest)
{
    /* Добавить аннотации */
    if (!field.annotations.empty())
    {
        dest += "/*";
        for (const auto &annotation : field.annotations)
            dest += annotation + ", ";
        dest.erase(dest.end() - 2, dest.end());
        dest += { "*/", "\n" };
    }

    /* Добавить модификаторы */
    if (field.modifiers & FINAL) dest += "const";
    if (field.modifiers & STATIC) dest += "static";

    /* Добавить тип и имя */
    dest.insert(dest.end(), { field.type, field.name });

    /* Добавить значение */
    if (!field.value.empty()) dest.insert(dest.end(), { "=", field.value });

    dest += ";";
}
