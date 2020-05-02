#pragma once

#include <System/ISystem.h>
#include "../ILevel.h"

class CMainMenu : public ILevel
{
public:
	CMainMenu(ISystem* systemContext);

	// ILevel
	void loadLevel() override;
	void onUpdate() override;
	// ~ILevel

private:
	ISystem * GetSystem() noexcept { return m_pSystem; }

private:
	ISystem * m_pSystem = nullptr;
};