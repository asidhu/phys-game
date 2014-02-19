

PHYSENGINE=$(wildcard PhysicsEngine/Source/*.cpp)
ENGINE=$(wildcard PhysGame/Source/*.cpp)
ENGINE2=$(wildcard PhysGame/Source/PlatformDependent/*.cpp)
ENGINE3=$(wildcard PhysGame/Source/opengl/*.cpp)
GAME=$(wildcard NerdFestGame/*.cpp)


all: tool

tool:
	emcc -I. -IPhysGame/Source $(PHYSENGINE) $(ENGINE) $(ENGINE2) $(ENGINE3) $(GAME) -O2 -o emscripten/test.html