//
// Created by kest on 8/3/25.
//

#include "ScriptLoader.h"

#include "Bindings/NodeSq.h"
#include "Bindings/Vector3Sq.h"
#include "Bindings/SceneSq.h"

#include "ScriptUtils/script_binder.h"
#include "Urho3D/Scene/Node.h"

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>

namespace Urho3D
{

ScriptLoader::ScriptLoader(Context* context)
    : LogicComponent(context)
{
}

void ScriptLoader::RegisterObject(Context* context)
{
    context->AddFactoryReflection<ScriptLoader>(Category_Script);

    URHO3D_ACCESSOR_ATTRIBUTE("Script", GetScriptFileAttr, SetScriptFileAttr, ResourceRef, ResourceRef(ScriptFile::GetTypeStatic()), AM_DEFAULT);
}

ResourceRef ScriptLoader::GetScriptFileAttr() const
{
    return GetResourceRef(scriptFile_, ScriptFile::GetTypeStatic());
}

void ScriptLoader::SetScriptFileAttr(const ResourceRef& value)
{
    auto* cache = GetSubsystem<ResourceCache>();
    scriptFile_ = cache->GetResource<ScriptFile>(value.name_);
}

void ScriptLoader::Start()
{
    v_ = sq_open(1024);

    sqstd_register_iolib(v_);

    sqstd_seterrorhandlers(v_);

    sq_setprintfunc(v_, squirrel_print, squirrel_error);
    sq_pushroottable(v_);

    BEGIN_DEFINE_CLASS(v_, Scene)
        DEFINE_METHOD_NAMED(v_, "CreateChild", Scene_CreateChild)
        DEFINE_METHOD_NAMED(v_, "GetRootNode", Scene_GetRoot)
        DEFINE_METHOD_NAMED(v_, "SetName", Scene_SetName)
        DEFINE_METHOD_NAMED(v_, "GetName", Scene_GetName)
    END_DEFINE_CLASS(v_)

    DEFINE_INSTANCE(v_, "scene", Scene, GetScene())

    BEGIN_DEFINE_CLASS(v_, Node)
        DEFINE_METHOD_NAMED(v_, "CreateChild", Node_CreateChild)
        DEFINE_METHOD_NAMED(v_, "SetName", Node_SetName)
        DEFINE_METHOD_NAMED(v_, "GetName", Node_GetName)
        DEFINE_METHOD_NAMED(v_, "SetPosition", Node_SetPosition)
        DEFINE_METHOD_NAMED(v_, "GetPosition", Node_GetPosition)
    END_DEFINE_CLASS(v_)

    DEFINE_INSTANCE(v_, "node", Node, GetNode())

    BEGIN_DEFINE_CLASS_CONSTRUCTOR(v_, Vector3, Vector3_Constructor)
        DEFINE_MEMBER_VARIABLES(v_, "_get", Vector3_Get)
    END_DEFINE_CLASS(v_)
}


void ScriptLoader::DelayedStart()
{
    if (SQ_SUCCEEDED(sq_compilebuffer(v_, scriptFile_->GetText(), strlen(scriptFile_->GetText()), "", true)))
    {
        sq_pushroottable(v_);

        // 5. Call the compiled script
        if (SQ_FAILED(sq_call(v_, 1, SQFalse, SQTrue)))
        {
            URHO3D_LOGDEBUG("Failed to run script");
        }
    }
    else
    {
        URHO3D_LOGERROR("Failed to compile script");
    }
}

} // namespace Urho3D