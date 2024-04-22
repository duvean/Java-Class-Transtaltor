#ifndef CLASSES_H
#define CLASSES_H

#pragma once
#include <string>
#include <vector>
#include <variant>
#include <map>
using namespace std;

class EntityTemplate
{
public:
     string          name;
     vector<string>  modifiers;
     char            _modifiers = 0;
     vector<string>  annotations;
};

class VarDef : public EntityTemplate
{
public:
     string  type;
     string  value;
};

class MethodDef : public EntityTemplate
{
public:
     string            type;
     vector<VarDef>  arguments;
};

class ClassDef : public EntityTemplate
{
public:
     bool               isInterface;
     string             extends;
     vector<string>     implements;
     vector<VarDef>     fields;
     vector<MethodDef>  methods;
     vector<ClassDef>   nestedClasses;
     string             fullPath;
};

class JavaSourceDef
{
public:
     string            name;
     string            package;
     vector<string>    imports;
     vector<string>    classImports;
     vector<string>    methodImports;
     vector<ClassDef>  classes;
};

#endif // CLASSES_H
