/* Copyright (C) 2019 Ozan Ozkan
* All of the implementations are experimental and subject to change.
*/

#pragma once

#include <System/EntitySystem/IEntity.h>
#include <System/EntitySystem/IEntitySystem.h>
#include <System/EntitySystem/IEntityComponent.h>
#include <EntityComponents/SpriteRendererEntityComponent.h>

class CEnvironmentController : public IEntityComponent
{
public:
	CEnvironmentController();

	void Init() override;
	unsigned int getEventMask() const override;
	void onEvent(const SEntityEvent& event) override;
	void updateComponent() override;

private:
	std::vector<SpriteRendererEntityComponent*> m_tiles{};
};