#pragma once

#include <Urho3D/Scene/LogicComponent.h>

#include <squirrel.h>
#include <sqstdaux.h>
#include <sqstdio.h>

#include "ScriptFile.h"

namespace Urho3D
{

URHO3D_GLOBAL_CONSTANT(ConstString Category_Script{"Component/Scripting"});

class ScriptLoader : public LogicComponent
{
    URHO3D_OBJECT(ScriptLoader, LogicComponent)

public:
    ScriptLoader(Context* context);

    static void RegisterObject(Context* context);

    ResourceRef GetScriptFileAttr() const;
    void SetScriptFileAttr(const ResourceRef& value);

    void DelayedStart() override;

    void Start() override;

private:
    SharedPtr<ScriptFile> scriptFile_;
    HSQUIRRELVM v_;

    static void squirrel_print(HSQUIRRELVM v, const SQChar* s, ...)
    {
        char buffer[1024]; // Adjust size as needed
        va_list args;
        va_start(args, s);
        vsnprintf(buffer, sizeof(buffer), s, args);
        va_end(args);

        URHO3D_LOGDEBUGF("[SQUIRREL] %s", buffer);
    }

    static void squirrel_error(HSQUIRRELVM v, const SQChar* s, ...)
    {
        char buffer[1024];
        va_list args;
        va_start(args, s);
        vsnprintf(buffer, sizeof(buffer), s, args);
        va_end(args);

        URHO3D_LOGERRORF("[SQUIRREL] %s", buffer);
    }
};

}