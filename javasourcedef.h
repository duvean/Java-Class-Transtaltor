#ifndef CLASSES_H
#define CLASSES_H

#pragma once
#include <string>
#include <vector>
#include <variant>
#include <map>
using namespace std;

enum Modifiers
{
     DEFAULT    = 1 << 0,  // 0000001
     PUBLIC     = 1 << 1,  // 0000010
     PROTECTED  = 1 << 2,  // 0000100
     PRIVATE    = 1 << 3,  // 0001000
     STATIC     = 1 << 4,  // 0010000
     FINAL      = 1 << 5,  // 0100000
     ABSTRACT   = 1 << 6,  // 1000000
};

class EntityTemplate
{
public:
     string          name;
     char            modifiers = 0;
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
     string          type;
     vector<VarDef>  arguments;
     bool isConstructor = false;
     bool isDestructor = false;
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
