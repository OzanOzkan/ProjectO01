/* Copyright (C) 2020 Ozan Ozkan
* All of the implementations are experimental and subject to change.
*/

#include <Physics/IPhysics.h>
#include "SDLPhysics/SDLPhysics.h"

extern "C"
{
	API_EXPORT IPhysics* CreateModuleInterface(ISystem* systemContext, SCreateModuleParams<EPhysics> createParams)
	{
		IPhysics* pPhysics = nullptr;

		switch(createParams.implType)
		{
		case EPhysics::SDL2:
		{
			pPhysics = new CSDLPhysics(systemContext);
		}
		break;
		}

		pPhysics->InitializeModule();

		return pPhysics;
	}
}