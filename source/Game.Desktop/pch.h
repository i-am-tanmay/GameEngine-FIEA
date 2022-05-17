#pragma once

#include <cstdint>
#include <stdexcept>
#include <functional>

#include<gsl/gsl>

#pragma warning(push)
#pragma warning(disable:4201)
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:26812)
#pragma warning(disable:4635)
#include <json/json.h>
#pragma warning(pop)