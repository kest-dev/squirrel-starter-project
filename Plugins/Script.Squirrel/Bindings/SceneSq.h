#pragma once

#pragma once

#include <Urho3D/Scene/Scene.h>

#include <squirrel.h>

using namespace Urho3D;

inline SQInteger Scene_CreateChild(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Scene* parent = static_cast<Scene*>(ptr);

    Node* child = parent->CreateChild("Child");

    // Push Node as Squirrel object
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Node"), -1);
    if (SQ_SUCCEEDED(sq_get(v, -2)))
    {
        sq_createinstance(v, -1);
        sq_remove(v, -2); // class
        sq_remove(v, -2); // root
        sq_setinstanceup(v, -1, child);
        sq_setreleasehook(v, -1, nullptr);
    }
    return 1;
}

inline SQInteger Scene_GetRoot(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Scene* parent = static_cast<Scene*>(ptr);

    Node* child = parent->GetNode(1);

    // Push Node as Squirrel object
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Node"), -1);
    if (SQ_SUCCEEDED(sq_get(v, -2)))
    {
        sq_createinstance(v, -1);
        sq_remove(v, -2); // class
        sq_remove(v, -2); // root
        sq_setinstanceup(v, -1, child);
        sq_setreleasehook(v, -1, nullptr);
    }
    return 1;
}

inline SQInteger Scene_SetName(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Node* node = static_cast<Node*>(ptr);

    const SQChar* name;
    sq_getstring(v, 2, &name);
    node->SetName(name);
    return 0;
}

inline SQInteger Scene_GetName(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Node* node = static_cast<Node*>(ptr);

    sq_pushstring(v, node->GetName().c_str(), -1);
    return 1;
}