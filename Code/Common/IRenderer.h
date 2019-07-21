#pragma once

#include "IModule.h"

#include <Windows.h>

#define EXTERN_LIBRARY_EXPORT __declspec(dllexport)
#define EXTERN_LIBRARY_IMPORT __declspec(dllimport)
#define LoadExternalLibrary(libname) :: LoadLibrary(libname)

class IRenderer : IModule
{
public:
	virtual ~IRenderer() {}

	// IModule
	virtual void InitializeModule() = 0;
	virtual void Update() = 0;
	// ~IModule

	virtual bool TestRendererModule() = 0;
};

extern "C"
{
	EXTERN_LIBRARY_EXPORT IRenderer* CreateModuleInterface(SEnvironment *env);
}