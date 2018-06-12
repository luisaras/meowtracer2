#pragma once

#include "../Math/Util.h"
#include "../Math/Matrix4.h"
#include <json_spirit/json_spirit.h>

using json_spirit::read;
using json_spirit::Object;
using json_spirit::Value;
using json_spirit::Array;

Vec3 parseVec3(Value &value);
Matrix4 parseTransform(Value &value);