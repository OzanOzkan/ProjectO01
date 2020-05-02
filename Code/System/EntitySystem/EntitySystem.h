#pragma once

#include <System/EntitySystem/IEntitySystem.h>
#include "Entity.h"

#include <memory>
#include <set>
#include <vector>

class CEntitySystem : public IEntitySystem
{
public:
	CEntitySystem(ISystem* systemContext);

	// IEntitySystem
	IEntity* spawnEntity(const SEntitySpawnParams& spawnParams) override;
	void destroyEntity(const int& entityId) override;
	int getEntityCount() const override;
	void addEntityEventListener(IEntity* pEntity) override;
	void removeEntityEventListener(IEntity* pEntity) override;
	// ~IEntitySystem

	void onUpdate() override;

private:
	ISystem * GetSystem() { return m_pSystem; }

private:
	ISystem * m_pSystem;
	std::map<int, std::unique_ptr<CEntity>> m_entityList;
	std::set<IEntity*> m_eventListeners;
};