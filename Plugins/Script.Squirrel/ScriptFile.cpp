//
// Created by kest on 7/30/25.
//

#include "ScriptFile.h"

#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/IO/Deserializer.h>

namespace Urho3D
{

ScriptFile::ScriptFile(Context* context)
    : Resource(context)
{
}

void ScriptFile::RegisterObject(Context* context)
{
    context->AddFactoryReflection<ScriptFile>();
}

bool ScriptFile::BeginLoad(Deserializer& source)
{
    text_ = source.ReadString();
    return true;
}

}