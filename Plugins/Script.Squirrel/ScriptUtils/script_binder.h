#pragma once

#include <squirrel.h>
#include <functional>

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>

using namespace Urho3D;

inline void BeginClassDefinition(HSQUIRRELVM v, const char* className)
{
    sq_pushroottable(v);
    sq_pushstring(v, _SC(className), -1);
    sq_newclass(v, SQFalse); // No base class
}

inline void DefineConstructor(HSQUIRRELVM v, SQFUNCTION ctor)
{
    sq_pushstring(v, _SC("constructor"), -1);
    sq_newclosure(v, ctor, 0);
    sq_newslot(v, -3, false);
}

inline void DefineClassMethod(HSQUIRRELVM v, const char* methodName, SQFUNCTION method)
{
    sq_pushstring(v, _SC(methodName), -1);
    sq_newclosure(v, method, 0);
    sq_newslot(v, -3, false);
}

inline void DefineClassStaticMethod(HSQUIRRELVM v, const char* methodName, SQFUNCTION method)
{
    sq_pushstring(v, _SC(methodName), -1);
    sq_newclosure(v, method, 0);
    sq_newslot(v, -3, true);
}

inline void DefineClassVariables(HSQUIRRELVM v, const char* varType, SQFUNCTION func)
{
    sq_pushstring(v, _SC(varType), -1);
    sq_newclosure(v, func, 0);
    sq_newslot(v, -3, SQFalse);
}

inline void EndDefineClassVariable(HSQUIRRELVM v)
{
    sq_newslot(v, -3, SQFalse);
}

inline void EndClassDefinition(HSQUIRRELVM v)
{
    sq_newslot(v, -3, false);
    sq_pop(v, 1);
}

inline void RegisterGlobalFunction(HSQUIRRELVM v, const char* funcName, SQFUNCTION func)
{
    sq_pushroottable(v);
    sq_pushstring(v, _SC(funcName), -1);
    sq_newclosure(v, func, 0);
    sq_newslot(v, -3, false);
    sq_pop(v, 0);
}

template<class C> void RegisterClassInstance(HSQUIRRELVM v, const char* instanceName, const char* className, C* instance)
{
    sq_pushroottable(v);
    sq_pushstring(v, _SC(instanceName), -1);
    sq_pushroottable(v);
    sq_pushstring(v, _SC(className), -1);
    if (SQ_SUCCEEDED(sq_get(v, -2)))
    {
        sq_createinstance(v, -1);
        sq_remove(v, -2); // class
        sq_remove(v, -2); // roottable
        sq_setinstanceup(v, -1, instance);
        sq_setreleasehook(v, -1, nullptr); // Do not delete Scene!
        sq_newslot(v, -4, SQFalse); // set global `scene = Scene instance`
    }
    sq_pop(v, 1); // root
}

///This creates a class with a bare minimum constructor
#define BEGIN_DEFINE_CLASS(v, className) { \
    BeginClassDefinition(v, #className); \
}

///This creates a class with a pre-defined constructor built by the user
#define BEGIN_DEFINE_CLASS_CONSTRUCTOR(v, classname, ctor) { \
    BeginClassDefinition(v, #classname); \
    DefineConstructor(v, ctor); \
}

#define DEFINE_METHOD(v, method) DefineClassMethod(v, #method, method);
#define DEFINE_METHOD_NAMED(v, methodName, method) DefineClassMethod(v, methodName, method);

#define DEFINE_STAIC_METHOD(v, method) DefineClassStaticMethod(v, #method, method);
#define DEFINE_STATIC_METHOD_NAMED(v, methodName, method) DefineClassStaticMethod(v, methodName, method);

#define DEFINE_MEMBER_VARIABLES(v, varType, varFunc) DefineClassVariables(v, varType, varFunc);

#define END_DEFINE_CLASS(v) EndClassDefinition(v);

#define DEFINE_GLOBAL_FUNCTION(v, func) RegisterGlobalFunction(v, #func, func);
#define DEFINE_GLOBAL_FUNCTION_NAMED(v, funcName, func) RegisterGlobalFunction(funcName, func);

///This creates an active instance of an existing class, such as a node of a component. You must have already defined the class before instancing the class.
#define DEFINE_INSTANCE(v, instanceName, className, instance) RegisterClassInstance<className>(v, instanceName, #className, instance);