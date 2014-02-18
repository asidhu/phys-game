#pragma once
#ifndef OPENGL_HEADER
#define OPENGL_HEADER

#ifdef EMSCRIPTEN

#include <gl\glew.h>
#include "PhysGame\Source\PlatformDependent\GLFWWrapper.h"
#include <emscripten\emscripten.h>

#else

#include <gl\glew.h>
#include <gl\GL.h>
#include "PhysGame\Source\PlatformDependent\GLFWWrapper.h"

#endif

#endif