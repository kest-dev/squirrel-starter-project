#pragma once

#include <Urho3D/Scene/Node.h>

#include <squirrel.h>

using namespace Urho3D;

inline SQInteger Node_CreateChild(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Node* parent = static_cast<Node*>(ptr);

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

inline SQInteger Node_SetName(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Node* node = static_cast<Node*>(ptr);

    const SQChar* name;
    sq_getstring(v, 2, &name);
    node->SetName(name);
    return 0;
}

inline SQInteger Node_GetName(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Node* node = static_cast<Node*>(ptr);

    sq_pushstring(v, node->GetName().c_str(), -1);
    return 1;
}

inline SQInteger Node_SetPosition(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Node* node = static_cast<Node*>(ptr);

    void* vecPtr;
    if (SQ_FAILED(sq_getinstanceup(v, 2, &vecPtr, nullptr, true))) {
        return sq_throwerror(v, _SC("Expected Vector3 as argument"));
    }

    Vector3* pos = static_cast<Vector3*>(vecPtr);

    node->SetPosition(*pos);
    return 0;
}

inline SQInteger Node_GetPosition(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    Node* node = static_cast<Node*>(ptr);

    Vector3 tmp = node->GetPosition();
    Vector3* vec = new Vector3(tmp.x_, tmp.y_, tmp.z_);

    sq_pushroottable(v);
    sq_pushstring(v, _SC("Vector3"), -1);
    if (SQ_SUCCEEDED(sq_get(v, -2)))
    {
        sq_createinstance(v, -1);
        sq_remove(v, -2); // class
        sq_remove(v, -2); // root
        sq_setinstanceup(v, -1, vec);
        sq_setreleasehook(v, -1, nullptr);
    }
    return 1;
}