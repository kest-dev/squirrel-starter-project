#pragma once

#include <Urho3D/Math/Vector3.h>

#include <squirrel.h>

using namespace Urho3D;

inline SQInteger Vector3_Constructor(HSQUIRRELVM v)
{
    SQFloat x, y, z;
    if (SQ_FAILED(sq_getfloat(v, 2, &x)) ||
        SQ_FAILED(sq_getfloat(v, 3, &y)) ||
        SQ_FAILED(sq_getfloat(v, 4, &z)))
        return sq_throwerror(v, _SC("Expected 3 float arguments"));

    Vector3* obj = new Vector3(x, y, z);
    sq_setinstanceup(v, 1, obj);
    return 0;
}

inline SQInteger Vector3_Get(HSQUIRRELVM v)
{
    void* ptr;
    sq_getinstanceup(v, 1, &ptr, nullptr, true);
    auto* vec = static_cast<Vector3*>(ptr);

    const SQChar* key;
    sq_getstring(v, 2, &key);

    if (!strcmp(key, "x")) sq_pushfloat(v, vec->x_);
    else if (!strcmp(key, "y")) sq_pushfloat(v, vec->y_);
    else if (!strcmp(key, "z")) sq_pushfloat(v, vec->z_);
    else return sq_throwerror(v, _SC("Unknown property"));

    return 1;
}