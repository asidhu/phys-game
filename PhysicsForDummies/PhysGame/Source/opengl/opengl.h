#pragma once
#ifndef OPENGL_HEADER
#define OPENGL_HEADER

#ifdef EMSCRIPTEN

#include <gl\glew.h>
#include "PhysGame\Source\PlatformDependent\GLFWWrapper.h"
#include <emscripten\emscripten.h>
#define GLEW_OK 1
#define GlewInit() 1

#else

#include <gl\glew.h>
#include <gl\GL.h>
#include "PhysGame\Source\PlatformDependent\GLFWWrapper.h"
#define GlewInit glewInit

#endif

#endif