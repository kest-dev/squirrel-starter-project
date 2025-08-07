#pragma once

#include <Urho3D/Resource/Resource.h>

namespace Urho3D
{

class ScriptFile : public Resource
{
    URHO3D_OBJECT(ScriptFile, Resource);

public:
    static void RegisterObject(Context* context);

    explicit ScriptFile(Context* context);

    bool BeginLoad(Deserializer& source) override;

    const char* GetText() const { return text_.c_str(); }

private:
    ea::string text_;
};

}