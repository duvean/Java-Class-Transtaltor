#include "error.h"
#include "util.h"
#include <QString>
#include <unordered_set>
#include <vector>

Error::Error() = default;

Error::Error(ErrorCode code, const vector<string> &info)
{
    this->code = code;
    this->info = info;
}

bool Error::operator==(const Error &other) const
{
    return ((this->code == other.code) && (this->info == other.info));
}

bool Error::operator<(const Error &other) const
{
    if (this->code < other.code)
    {
        return true;
    }
    else if (this->code > other.code)
    {
        return false;
    }
    return this->info < other.info;
}

string Error::toText()
{
    string res;

    switch (this->code)
    {
    case ErrorCode::CoudntOpenFile:
        res = this->info[0] + ": Couldn't open file";
        break;
    case ErrorCode::CoudntWriteInOutputFile:
        res = this->info[0] + ": Couldn't write in output file " + this->info[0];
        break;
    case ErrorCode::MissingMandatoryMember:
        res = this->info[0] + ": Missing mandatory attribute [" + this->info.back() + "] at entity ";
        for (size_t i = 1; i < this->info.size() - 1; i++)
            res += this->info[i] + ".";
        break;
    case ErrorCode::UnidentifiedImport:
        res = this->info[0] + ": Undefined import " + this->info[1];
        break;
    case ErrorCode::ProhibitedModifier:
        res = this->info[0] + ": " + this->info[1] + " at path ";
        for (size_t i = 2; i < this->info.size() - 2; i++)
            res += this->info[i] + ".";
        res += " has prohibited modifier " + this->info[this->info.size() - 2] + ".\n";
        res += "    Allowed modifiers for this type of entity:\n";
        res += "    " + this->info.back();
        break;
    case ErrorCode::ProhibitedAnnotation:
        res = this->info[0] + ": Entity ";
        for (size_t i = 1; i < this->info.size() - 1; i++)
            res += this->info[i] + ".";
        res += " has prohibited annotation " + this->info.back();
        break;
    case ErrorCode::MultiDefOfOneClassInPackage:
        res = this->info[0] + ": Multiply declaration of class " + this->info[1];
        break;
    case ErrorCode::MethodMultiDeclaration:
        res = this->info[0] + ": Multiply declaration of method " + this->info[1] + "." + this->info[2] + "." +
              this->info[3];
        break;
    case ErrorCode::InheritanceFromNonExistentParent:
        res = this->info[0] + ": Class " + this->info[1] + " extended from non existent parent " + this->info[2];
        break;
    case ErrorCode::InheritanceFromFinalOrStaticParent:
        res = this->info[0] + ": Class " + this->info[1] + "." + this->info[2] +
              " extends from static or final parent " + this->info[3];
        break;
    case ErrorCode::InheritanceFromPrivateNestedParent:
        res = this->info[0] + ": Class " + this->info[1] + " extends from nested private parent " + this->info[2];
        break;
    case ErrorCode::InheritanceOfInterfaceFromClass:
        res = this->info[0] + ": Interface " + this->info[1] + " extends from class " + this->info[2];
        break;
    case ErrorCode::FieldMultiDeclaration:
        res = this->info[0] + ": Multi definition of field " + this->info[1] + this->info[2] + this->info[3];
        break;
    case ErrorCode::NonAbstractDefaultStaticMethodsInInterface:
        res = this->info[0] + ": Interface member " + this->info[1] + "." + this->info[2] + "." + this->info[3] +
              " has prohibited modifier " + this->info[4];
        break;
    case ErrorCode::MultiInheritanceFromNonInterface:
        res = this->info[0] + ": Multi inherence from non-interfaces in class " + this->info[1] + "." + this->info[2];
        break;
    case ErrorCode::MoreThanOneAccessModifier:
        res = this->info[0] + ": Entity at path " + this->info[1] + "." + this->info[2];
        if (this->info.size() == 4)
            res += "." + this->info[3];
        res += " has more than 1 access modifier";
        break;
    case ErrorCode::NonFunctionalInterfaceAnnotated:
        res = this->info[0] + ": Entity at path " + this->info[1] + "." + this->info[2] +
              " does not match the definition of a functional interface";
        break;
    }
    return (res);
}

