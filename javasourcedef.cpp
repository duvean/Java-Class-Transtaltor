#include "javasourcedef.h"
#include "util.h"
#include <set>
#include <unordered_map>

#define ACCESS_MODIFIERS_MASK (Modifiers::DEFAULT | Modifiers::PUBLIC | Modifiers::PROTECTED | Modifiers::PRIVATE)

void validateMandatoryClassAttrs(const ClassDef &classDef, const string &filename, vector<Error> &dest)
{
    if (classDef.name.empty())
        dest.push_back(
            Error(ErrorCode::MissingMandatoryMember, {filename, classDef.fullPath, "<NONAME_CLASS>", "name"}));
    for (const MethodDef &method : classDef.methods)
    {
        if (method.name.empty())
            dest.push_back(
                Error(ErrorCode::MissingMandatoryMember,
                      {filename, classDef.fullPath, (classDef.name.empty() ? "<NONAME_CLASS>" : classDef.name),
                       "<NONAME_METHOD>", "name"}));
        if (method.type.empty() && !method.isConstructor)
            dest.emplace_back(
                Error(ErrorCode::MissingMandatoryMember,
                      {filename, classDef.fullPath, (classDef.name.empty() ? "<NONAME_CLASS>" : classDef.name),
                       (method.name.empty() ? "<NONAME_METHOD>" : method.name), "type"}));
        for (const VarDef &arg : method.arguments)
        {
            if (arg.name.empty())
                dest.emplace_back(
                    Error(ErrorCode::MissingMandatoryMember,
                          {filename, classDef.fullPath, (classDef.name.empty() ? "<NONAME_CLASS>" : classDef.name),
                           (method.name.empty() ? "<NONAME_METHOD>" : method.name), "<NONAME_ARG>", "name"}));
            if (arg.type.empty())
                dest.emplace_back(
                    Error(ErrorCode::MissingMandatoryMember,
                          {filename, classDef.fullPath, (classDef.name.empty() ? "<NONAME_CLASS>" : classDef.name),
                           (method.name.empty() ? "<NONAME_METHOD>" : method.name),
                           (arg.name.empty() ? "<NONAME_ARG>" : arg.name), "type"}));
        }
    }
    for (const VarDef &field : classDef.fields)
    {
        if (field.name.empty())
            dest.emplace_back(
                Error(ErrorCode::MissingMandatoryMember,
                      {filename, classDef.fullPath, (classDef.name.empty() ? "<NONAME_CLASS>" : classDef.name),
                       "<NONAME_FIELD>", "name"}));
        if (field.type.empty())
            dest.emplace_back(
                Error(ErrorCode::MissingMandatoryMember,
                      {filename, classDef.fullPath, (classDef.name.empty() ? "<NONAME_CLASS>" : classDef.name),
                       (field.name.empty() ? "<NONAME_FIELD>" : field.name), "type"}));
    }
    for (const ClassDef &nested : classDef.nestedClasses)
        validateMandatoryClassAttrs(nested, filename, dest);
}
void validateClassMembersProhibitedModifiers(const ClassDef &classDef, const string &filename, vector<Error> &dest)
{
    vector<string> constructorProh = {"abstract", "final", "static"};
    vector<string> classOuterProh = {"private", "protected", "static"};
    vector<string> interfaceOuterProh = {"private", "protected", "static", "final"};
    vector<string> interfaceNestedProh = {"final"};

    if (classDef.isInterface)
    {
        if (classDef.fullPath.empty())
        {
            for (auto &mod : classDef._modifiers)
                for (auto &modProh : interfaceOuterProh)
                    if (mod == modProh)
                        dest.push_back(
                            {ErrorCode::ProhibitedModifier, {filename, "Outer interface", classDef.fullPath, classDef.name, modProh, "[abstract, public]"}});
        }
        else
            dest.push_back(Error(ErrorCode::ProhibitedModifier, {filename, "Inner interface", classDef.fullPath, classDef.name, "final", "[abstract, private, protected, public, static]"}));
    }

    else if (classDef.fullPath.empty())
        for (auto &mod : classDef._modifiers)
            for (auto &modProh : classOuterProh)
                if (mod == modProh)
                    dest.emplace_back(
                        Error(ErrorCode::ProhibitedModifier, {filename, "Outer class", classDef.fullPath, classDef.name, modProh, "[abstract, final, public]"}));

    for (const VarDef &field : classDef.fields)
        for (auto &mod : field._modifiers)
            if (mod == "abstract")
                dest.emplace_back(Error(ErrorCode::ProhibitedModifier,
                                     {filename, "Field", classDef.fullPath, classDef.name, field.name, "abstract", "[final, private, protected, public, static]"}));

    for (auto &method : classDef.methods)
        if (method.isConstructor)
            for (auto &mod : method._modifiers)
                for (auto &modProh : constructorProh)
                    if (mod == modProh)
                        dest.emplace_back(Error(ErrorCode::ProhibitedModifier,
                                             {filename, "Constructor", classDef.fullPath, classDef.name, method.name, modProh, "[private, protected, public]"}));

    for (const ClassDef &nested : classDef.nestedClasses)
        validateClassMembersProhibitedModifiers(nested, filename, dest);
}
void findDuplicateClassNamesHelper(const ClassDef &classDef, unordered_map<string, int> &nameCount,
                                   set<string> &duplicates)
{
    nameCount[classDef.name]++;

    if (nameCount[classDef.name] == 2)
        duplicates.emplace(classDef.name);

    for (const auto &nestedClass : classDef.nestedClasses)
        findDuplicateClassNamesHelper(nestedClass, nameCount, duplicates);
}
set<string> findDuplicateClassNames(const vector<ClassDef> &classes)
{
    unordered_map<string, int> nameCount;
    set<string> duplicates;

    for (const auto &classDef : classes)
        findDuplicateClassNamesHelper(classDef, nameCount, duplicates);

    return (duplicates);
}

void collectClasses(const JavaSourceDef &file, unordered_set<string> &classSet)
{
    for (const auto &cls : file.classes)
    {
        classSet.emplace(cls.name);
        for (const auto &nestedCls : cls.nestedClasses)
            classSet.emplace(nestedCls.name);
    }
}
void findNonExistentParents(const ClassDef &cls, const unordered_set<string> &classSet,
                            const unordered_set<string> &importedClasses, const string &filename, vector<Error> &errors)
{
    if ((classSet.find(cls.extends) == classSet.end()) && !cls.extends.empty())
        errors.push_back({ErrorCode::InheritanceFromNonExistentParent, {filename, cls.name, cls.extends}});

    for (const auto &parent : cls.implements)
        if ((classSet.find(parent) == classSet.end()) && !cls.implements.empty())
            errors.push_back({ErrorCode::InheritanceFromNonExistentParent, {filename, cls.name, parent}});

    for (const auto &nestedCls : cls.nestedClasses)
        findNonExistentParents(nestedCls, classSet, importedClasses, filename, errors);
}

void findInvalidClassExtensions(const JavaSourceDef &file, const vector<JavaSourceDef> &hierarchy,
                                vector<Error> &errors)
{
    unordered_set<string> classSet;
    unordered_set<string> importedClasses(file.imports.begin(), file.imports.end());

    collectClasses(file, classSet);

    for (auto other : hierarchy)
        for (auto import : file.packageImports)
            if (import == other.package)
                collectClasses(other, classSet);

    for (const auto &cls : file.classes)
        findNonExistentParents(cls, classSet, importedClasses, file.name, errors);
}

void collectNestedPrivateClasses(const JavaSourceDef &file, unordered_set<string> &classSet)
{
    for (const auto &cls : file.classes)
        for (const auto &nestedCls : cls.nestedClasses)
            if (nestedCls.modifiers & PRIVATE)
                classSet.emplace(nestedCls.name);
}
void findNestedPrivateParents(const ClassDef &cls, const unordered_set<string> &classSet,
                              const unordered_set<string> &importedClasses, const string &filename,
                              vector<Error> &errors)
{
    if ((classSet.find(cls.extends) != classSet.end()) && !cls.extends.empty())
        errors.push_back({ErrorCode::InheritanceFromPrivateNestedParent, {filename, cls.name, cls.extends}});

    for (const auto &parent : cls.implements)
        if ((classSet.find(parent) != classSet.end()) && !cls.implements.empty())
            errors.push_back({ErrorCode::InheritanceFromPrivateNestedParent, {filename, cls.name, parent}});

    for (const auto &nestedCls : cls.nestedClasses)
        findNestedPrivateParents(nestedCls, classSet, importedClasses, filename, errors);
}

void findNestedPrivateExtensions(const JavaSourceDef &file, const vector<JavaSourceDef> &hierarchy,
                                 vector<Error> &errors)
{
    unordered_set<string> classSet;
    unordered_set<string> importedClasses(file.imports.begin(), file.imports.end());

    collectNestedPrivateClasses(file, classSet);

    for (auto other : hierarchy)
        for (auto import : file.packageImports)
            if (import == other.package)
                collectNestedPrivateClasses(other, classSet);

    for (const auto &cls : file.classes)
        findNestedPrivateParents(cls, classSet, importedClasses, file.name, errors);
}

void collectStaticOrFinalClasses(const JavaSourceDef &file, unordered_set<string> &classSet)
{
    for (const auto &cls : file.classes)
    {
        if ((cls.modifiers & STATIC) || (cls.modifiers & FINAL))
            classSet.emplace(cls.name);

        for (const auto &nestedCls : cls.nestedClasses)
            if ((nestedCls.modifiers & STATIC) || (nestedCls.modifiers & FINAL))
                classSet.emplace(nestedCls.name);
    }
}
void findStaticOrFinalParents(const ClassDef &cls, const unordered_set<string> &classSet,
                              const unordered_set<string> &importedClasses, const string &filename,
                              vector<Error> &errors)
{
    if ((classSet.find(cls.extends) != classSet.end()) && !cls.extends.empty())
        errors.push_back(
            {ErrorCode::InheritanceFromFinalOrStaticParent, {filename, cls.fullPath, cls.name, cls.extends}});

    for (const auto &parent : cls.implements)
        if ((classSet.find(parent) != classSet.end()) && !cls.implements.empty())
            errors.push_back(
                {ErrorCode::InheritanceFromFinalOrStaticParent, {filename, cls.fullPath, cls.name, parent}});

    for (const auto &nestedCls : cls.nestedClasses)
        findStaticOrFinalParents(nestedCls, classSet, importedClasses, filename, errors);
}

void findStaticOrFinalExtensions(const JavaSourceDef &file, const vector<JavaSourceDef> &hierarchy,
                                 vector<Error> &errors)
{
    unordered_set<string> classSet;
    unordered_set<string> importedClasses(file.imports.begin(), file.imports.end());

    collectStaticOrFinalClasses(file, classSet);

    for (auto other : hierarchy)
        for (auto import : file.packageImports)
            if (import == other.package)
                collectStaticOrFinalClasses(other, classSet);

    for (const auto &cls : file.classes)
        findStaticOrFinalParents(cls, classSet, importedClasses, file.name, errors);
}

void collectClassesOnly(const JavaSourceDef &file, unordered_set<string> &classSet)
{
    for (const auto &cls : file.classes)
    {
        if (!cls.isInterface)
            classSet.emplace(cls.name);
        for (const auto &nestedCls : cls.nestedClasses)
            if (!nestedCls.isInterface)
                classSet.emplace(nestedCls.name);
    }
}
void findClassParents(const ClassDef &cls, const unordered_set<string> &classSet,
                      const unordered_set<string> &importedClasses, const string &filename, vector<Error> &errors)
{
    if ((classSet.find(cls.extends) != classSet.end()) && !cls.extends.empty())
        errors.push_back({ErrorCode::InheritanceOfInterfaceFromClass, {filename, cls.name, cls.extends}});

    for (const auto &parent : cls.implements)
        if ((classSet.find(parent) != classSet.end()) && !cls.implements.empty())
            errors.push_back({ErrorCode::InheritanceOfInterfaceFromClass, {filename, cls.name, parent}});

    for (const auto &nestedCls : cls.nestedClasses)
        findClassParents(nestedCls, classSet, importedClasses, filename, errors);
}

void findInterfaceExtensionsFromClasses(const JavaSourceDef &file, const vector<JavaSourceDef> &hierarchy,
                                        vector<Error> &errors)
{
    unordered_set<string> classSet;
    unordered_set<string> importedClasses(file.imports.begin(), file.imports.end());

    collectClassesOnly(file, classSet);
    for (auto other : hierarchy)
        for (auto import : file.packageImports)
            if (import == other.package)
                collectClassesOnly(other, classSet);

    for (const auto &cls : file.classes)
        if (cls.isInterface)
            findClassParents(cls, classSet, importedClasses, file.name, errors);
}

void findMultiClassParents(const ClassDef &cls, const unordered_set<string> &classSet,
                           const unordered_set<string> &importedClasses, const string &filename, vector<Error> &errors)
{
    size_t count = 0;

    for (const auto &parent : cls.implements)
    {
        if ((classSet.find(parent) != classSet.end()) && !cls.implements.empty())
            count++;
        if (count > 1)
        {
            errors.push_back({ErrorCode::MultiInheritanceFromNonInterface, {filename, cls.fullPath, cls.name}});
            break;
        }
    }

    for (const auto &nestedCls : cls.nestedClasses)
        findMultiClassParents(nestedCls, classSet, importedClasses, filename, errors);
}

void findMultiExtensionsFromClasses(const JavaSourceDef &file, const vector<JavaSourceDef> &hierarchy,
                                    vector<Error> &errors)
{
    unordered_set<string> classSet;
    unordered_set<string> importedClasses(file.imports.begin(), file.imports.end());

    collectClassesOnly(file, classSet);
    for (auto other : hierarchy)
        for (auto import : file.packageImports)
            if (import == other.package)
                collectClassesOnly(other, classSet);

    for (const auto &cls : file.classes)
        findMultiClassParents(cls, classSet, importedClasses, file.name, errors);
}

void findFieldsMultiDeclarationInClass(const ClassDef &classDef, const string &filename, vector<Error> &errors)
{
    for (size_t i = 0; i < classDef.fields.size(); i++)
        for (size_t j = i + 1; j < classDef.fields.size(); j++)
            if (classDef.fields[i].name == classDef.fields[j].name)
                errors.push_back({ErrorCode::FieldMultiDeclaration,
                                  {filename, classDef.fullPath, classDef.name, classDef.fields[i].name}});

    for (auto &cls : classDef.nestedClasses)
        findFieldsMultiDeclarationInClass(cls, filename, errors);
}

void findFieldsMultiDeclarationInFile(const JavaSourceDef &file, vector<Error> &errors)
{
    for (auto &cls : file.classes)
        findFieldsMultiDeclarationInClass(cls, file.name, errors);
}

void findNonAbstractStaticFinalMethodsInClass(const ClassDef &classDef, const string &filename, vector<Error> &errors)
{
    for (auto method : classDef.methods)
    {
        if (method.modifiers & PUBLIC)
            errors.push_back({ErrorCode::NonAbstractDefaultStaticMethodsInInterface,
                              {filename, classDef.fullPath, classDef.name, method.name, "PUBLIC"}});
        if (method.modifiers & PROTECTED)
            errors.push_back({ErrorCode::NonAbstractDefaultStaticMethodsInInterface,
                              {filename, classDef.fullPath, classDef.name, method.name, "PROTECTED"}});
        if (method.modifiers & PRIVATE)
            errors.push_back({ErrorCode::NonAbstractDefaultStaticMethodsInInterface,
                              {filename, classDef.fullPath, classDef.name, method.name, "PRIVATE"}});
        if (method.modifiers & FINAL)
            errors.push_back({ErrorCode::NonAbstractDefaultStaticMethodsInInterface,
                              {filename, classDef.fullPath, classDef.name, method.name, "FINAL"}});
    }

    for (auto &nested : classDef.nestedClasses)
        findNonAbstractStaticFinalMethodsInClass(nested, filename, errors);
}

void findNonAbstractStaticFinalMethodsInFile(const JavaSourceDef &file, vector<Error> &errors)
{
    for (auto &cls : file.classes)
        if (cls.isInterface)
            findNonAbstractStaticFinalMethodsInClass(cls, file.name, errors);
}

void findWrongOverridingInClass(const ClassDef &classDef, const string &filename, vector<Error> &errors)
{
    for (size_t i = 0; i < classDef.methods.size(); i++)
        for (size_t j = i + 1; j < classDef.methods.size(); j++)
            if (classDef.methods[i].name == classDef.methods[j].name)
                if ((find(classDef.methods[i].annotations.begin(), classDef.methods[i].annotations.end(),
                          "@Override") == classDef.methods[i].annotations.end()) &&
                    (find(classDef.methods[j].annotations.begin(), classDef.methods[j].annotations.end(),
                          "@Override") == classDef.methods[j].annotations.end()))
                    errors.push_back({ErrorCode::MethodMultiDeclaration,
                                      {filename, classDef.fullPath, classDef.name, classDef.methods[i].name}});
}

void findWrongOverridingInFile(const JavaSourceDef &file, vector<Error> &errors)
{
    for (auto &cls : file.classes)
        findWrongOverridingInClass(cls, file.name, errors);
}

void findProhibitedAnnotationsInClass(const ClassDef &classDef, const string &filename, vector<Error> &errors)
{
    if (find(classDef.annotations.begin(), classDef.annotations.end(), "@Override") != classDef.annotations.end())
        errors.push_back({ErrorCode::ProhibitedAnnotation, {filename, classDef.fullPath, classDef.name, "@Override"}});

    for (auto &method : classDef.methods)
        if (find(method.annotations.begin(), method.annotations.end(), "@FunctionalInterface") !=
            method.annotations.end())
            errors.push_back({ErrorCode::ProhibitedAnnotation,
                              {filename, classDef.fullPath, classDef.name, method.name, "@FunctionalInterface"}});

    for (auto &field : classDef.fields)
        if (find(field.annotations.begin(), field.annotations.end(), "@Override") != field.annotations.end())
            errors.push_back({ErrorCode::ProhibitedAnnotation,
                              {filename, classDef.fullPath, classDef.name, field.name, "@Override"}});

    for (auto &field : classDef.fields)
        if (find(field.annotations.begin(), field.annotations.end(), "@FunctionalInterface") != field.annotations.end())
            errors.push_back({ErrorCode::ProhibitedAnnotation,
                              {filename, classDef.fullPath, classDef.name, field.name, "@FunctionalInterface"}});

    for (auto &nested : classDef.nestedClasses)
        findProhibitedAnnotationsInClass(nested, filename, errors);
}

void findProhibitedAnnotationsInFile(const JavaSourceDef &file, vector<Error> &errors)
{
    for (auto &cls : file.classes)
        findProhibitedAnnotationsInClass(cls, file.name, errors);
}

bool hasMultipleAccessModifiers(char modifiers)
{
    int accessModifiers = modifiers & ACCESS_MODIFIERS_MASK;
    int count = 0;

    while (accessModifiers)
    {
        accessModifiers &= (accessModifiers - 1);
        count++;
    }
    return (count > 1);
}

void findMoreThanOneAccessModInClass(const ClassDef &classDef, const string &filename, vector<Error> &errors)
{
    if (hasMultipleAccessModifiers(classDef.modifiers))
        errors.push_back({ErrorCode::MoreThanOneAccessModifier, {filename, classDef.fullPath, classDef.name}});

    for (auto &method : classDef.methods)
        if (hasMultipleAccessModifiers(method.modifiers))
            errors.push_back(
                {ErrorCode::MoreThanOneAccessModifier, {filename, classDef.fullPath, classDef.name, method.name}});

    for (auto &field : classDef.fields)
        if (hasMultipleAccessModifiers(field.modifiers))
            errors.push_back(
                {ErrorCode::MoreThanOneAccessModifier, {filename, classDef.fullPath, classDef.name, field.name}});

    for (auto &nested : classDef.nestedClasses)
        findMoreThanOneAccessModInClass(nested, filename, errors);
}

void findMoreThanOneAccessModInFile(const JavaSourceDef &file, vector<Error> &errors)
{
    for (auto &cls : file.classes)
        findMoreThanOneAccessModInClass(cls, file.name, errors);
}

void findNonFunctionalInterfacesInClass(const ClassDef &classDef, const string &filename, vector<Error> &errors)
{
    if ((find(classDef.annotations.begin(), classDef.annotations.end(), "@FunctionalInterface") !=
         classDef.annotations.end()) &&
        ((!classDef.isInterface) || (classDef.methods.size() > 1) || !(classDef.methods[0].modifiers & ABSTRACT)))
        errors.push_back({ErrorCode::NonFunctionalInterfaceAnnotated, {filename, classDef.fullPath, classDef.name}});
}

void findNonFunctionalInterfacesInFile(const JavaSourceDef &file, vector<Error> &errors)
{
    for (auto &cls : file.classes)
        findNonFunctionalInterfacesInClass(cls, file.name, errors);
}

void JavaSourceDef::validate(const vector<JavaSourceDef> &hierarchy, vector<Error> &errors)
{
    /* 4: Отсутствие заполнения обязательных полей структур */
    if (this->package.empty())
        errors.push_back(Error(ErrorCode::MissingMandatoryMember, {this->name, "package"}));
    for (auto &classDef : this->classes)
        validateMandatoryClassAttrs(classDef, this->name, errors);

    /* 5: Неизвестный импорт */
    if (!this->packageImports.empty())
        for (auto &imp : this->packageImports)
        {
            bool flagPresence = false;
            for (auto &file : hierarchy)
                if (file.package == imp)
                    flagPresence = true;
            if (!flagPresence)
                errors.push_back(Error(ErrorCode::UnidentifiedImport, {this->name, imp}));
        }

    if (!this->classImports.empty())
        for (auto &imp : this->classImports)
        {
            bool flagPresence = false;
            for (auto &file : hierarchy)
                if (file.package == extractPackage(imp))
                    for (auto &cls : file.classes)
                        if (cls.name == extractClassNamePython(imp))
                            flagPresence = true;

            if (!flagPresence)
                errors.push_back(Error(ErrorCode::UnidentifiedImport, {this->name, imp}));
        }

    /* 6: Наличие запрещённого модификатора у сущности */
    for (auto &classDef : this->classes)
        validateClassMembersProhibitedModifiers(classDef, this->name, errors);

    /* 7: Множественное определение одного класса */
    set<string> duplicateClasses = findDuplicateClassNames(this->classes);
    for (auto cls : duplicateClasses)
        errors.push_back(Error(ErrorCode::MultiDefOfOneClassInPackage, {this->name, cls}));

    /* 8: Наследование от несуществующего или недоступного родителя */
    findInvalidClassExtensions(*this, hierarchy, errors);

    /* 9: Наследование от вложенного private родителя */
    findNestedPrivateExtensions(*this, hierarchy, errors);

    /* 10: Наследование от Static или Final классса */
    findStaticOrFinalExtensions(*this, hierarchy, errors);

    /* 11: Наследование интерфейсом от класса */
    findInterfaceExtensionsFromClasses(*this, hierarchy, errors);

    /* 12: Множественное определение поля */
    findFieldsMultiDeclarationInFile(*this, errors);

    /* 13: Не Abstract, Default, Static методы в интерфейсе */
    findNonAbstractStaticFinalMethodsInFile(*this, errors);

    /* 14: Переопределение методов без пометки @Override */
    findWrongOverridingInFile(*this, errors);

    /* 15: Запрещённая аннотация */
    findProhibitedAnnotationsInFile(*this, errors);

    /* 21: Множественное наследование от классов */
    findMultiExtensionsFromClasses(*this, hierarchy, errors);

    /* 22: Сущность содержит больше одного модификатора доступа */
    findMoreThanOneAccessModInFile(*this, errors);

    /* 23: Не функциональный интерфейс аннотирован как он */
    findNonFunctionalInterfacesInFile(*this, errors);
}

template <typename T>
void compareVectors(const vector<T> &v1, const vector<T> &v2, const string &label, int depth,
                    vector<string> &differences)
{
    if (v1.size() != v2.size())
        differences.push_back(string(depth, '-') + label + " size mismatch: " + to_string(v1.size()) + " | " +
                              to_string(v2.size()));
    else
        for (size_t i = 0; i < v1.size(); ++i)
            if (v1[i] != v2[i])
                differences.push_back(string(depth, '-') + label + " mismatch at index " + to_string(i) + ": " + v1[i] +
                                      " | " + v2[i]);
}
void compareVarDef(const VarDef &v1, const VarDef &v2, int depth, vector<string> &differences);
void compareMethodDef(const MethodDef &m1, const MethodDef &m2, int depth, vector<string> &differences);
void compareClassDef(const ClassDef &c1, const ClassDef &c2, int depth, vector<string> &differences);
void compareEntityTemplate(const EntityTemplate &e1, const EntityTemplate &e2, int depth, vector<string> &differences)
{
    if (e1.name != e2.name)
        differences.push_back(string(depth, '-') + "name: " + e1.name + " | " + e2.name);

    if (e1.modifiers != e2.modifiers)
        differences.push_back(string(depth, '-') + "modifiers: " + to_string(e1.modifiers) + " | " +
                              to_string(e2.modifiers));

    compareVectors(e1._modifiers, e2._modifiers, "_modifiers", depth + 1, differences);
    compareVectors(e1.annotations, e2.annotations, "annotations", depth + 1, differences);
}
void compareVarDef(const VarDef &v1, const VarDef &v2, int depth, vector<string> &differences)
{
    compareEntityTemplate(v1, v2, depth, differences);

    if (v1.type != v2.type)
        differences.push_back(string(depth, '-') + "type: " + v1.type + " | " + v2.type);

    if (v1.value != v2.value)
        differences.push_back(string(depth, '-') + "value: " + v1.value + " | " + v2.value);
}
void compareMethodDef(const MethodDef &m1, const MethodDef &m2, int depth, vector<string> &differences)
{
    compareEntityTemplate(m1, m2, depth, differences);

    if (m1.type != m2.type)
        differences.push_back(string(depth, '-') + "type: " + m1.type + " | " + m2.type);

    if (m1.isConstructor != m2.isConstructor)
        differences.push_back(string(depth, '-') + "isConstructor: " + to_string(m1.isConstructor) + " | " +
                              to_string(m2.isConstructor));

    if (m1.isDestructor != m2.isDestructor)
        differences.push_back(string(depth, '-') + "isDestructor: " + to_string(m1.isDestructor) + " | " +
                              to_string(m2.isDestructor));

    if (m1.arguments.size() != m2.arguments.size())
        differences.push_back(string(depth, '-') + "arguments size mismatch: " + to_string(m1.arguments.size()) +
                              " | " + to_string(m2.arguments.size()));
    else
        for (size_t i = 0; i < m1.arguments.size(); ++i)
            compareVarDef(m1.arguments[i], m2.arguments[i], depth + 1, differences);
}
void compareClassDef(const ClassDef &c1, const ClassDef &c2, int depth, vector<string> &differences)
{
    compareEntityTemplate(c1, c2, depth, differences);
    if (c1.isInterface != c2.isInterface)
        differences.push_back(string(depth, '-') + "isInterface: " + to_string(c1.isInterface) + " | " +
                              to_string(c2.isInterface));

    if (c1.extends != c2.extends)
        differences.push_back(string(depth, '-') + "extends: " + c1.extends + " | " + c2.extends);

    compareVectors(c1.implements, c2.implements, "implements", depth + 1, differences);

    if (c1.fields.size() != c2.fields.size())
        differences.push_back(string(depth, '-') + "fields size mismatch: " + to_string(c1.fields.size()) + " | " +
                              to_string(c2.fields.size()));

    else
        for (size_t i = 0; i < c1.fields.size(); ++i)
            compareVarDef(c1.fields[i], c2.fields[i], depth + 1, differences);

    if (c1.methods.size() != c2.methods.size())
        differences.push_back(string(depth, '-') + "methods size mismatch: " + to_string(c1.methods.size()) + " | " +
                              to_string(c2.methods.size()));

    else
        for (size_t i = 0; i < c1.methods.size(); ++i)
            compareMethodDef(c1.methods[i], c2.methods[i], depth + 1, differences);

    if (c1.nestedClasses.size() != c2.nestedClasses.size())
        differences.push_back(string(depth, '-') + "nestedClasses size mismatch: " +
                              to_string(c1.nestedClasses.size()) + " | " + to_string(c2.nestedClasses.size()));

    else
        for (size_t i = 0; i < c1.nestedClasses.size(); ++i)
            compareClassDef(c1.nestedClasses[i], c2.nestedClasses[i], depth + 1, differences);

    if (c1.fullPath != c2.fullPath)
        differences.push_back(string(depth, '-') + "fullPath: " + c1.fullPath + " | " + c2.fullPath);
}

vector<string> compareJavaFilesContent(const JavaSourceDef &j1, const JavaSourceDef &j2, int depth)
{
    vector<string> differences;

    if (j1.name != j2.name)
        differences.push_back(string(depth, '-') + "name: " + j1.name + " | " + j2.name);

    if (j1.package != j2.package)
        differences.push_back(string(depth, '-') + "package: " + j1.package + " | " + j2.package);

    compareVectors(j1.imports, j2.imports, "imports", depth + 1, differences);
    compareVectors(j1.classImports, j2.classImports, "classImports", depth + 1, differences);
    compareVectors(j1.packageImports, j2.packageImports, "packageImports", depth + 1, differences);

    if (j1.classes.size() != j2.classes.size())
        differences.push_back(string(depth, '-') + "classes size mismatch: " + to_string(j1.classes.size()) + " | " +
                              to_string(j2.classes.size()));
    else
        for (size_t i = 0; i < j1.classes.size(); ++i)
            compareClassDef(j1.classes[i], j2.classes[i], depth + 1, differences);

    return (differences);
}
