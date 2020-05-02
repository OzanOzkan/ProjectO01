#pragma once

#include "../Core.h"
#include "../IModule.h"
#include "../System/EntitySystem/IEntity.h"

enum class EPhysics : int
{
	NONE = 0,
	SDL2
};

struct S2DPhysicalizeParams
{
	IEntity* pEntity;
	float mass;
};

struct SCollisionListenerParams
{
	IEntity* pEntity;
	int w, h = -1;
};

struct SPhysicsEventData
{
	IEntity* pEntity1 = nullptr;
	IEntity* pEntity2 = nullptr;
	Vector2 collisionPoint{ 0,0 };
};

class IPhysics : public IModule
{
public:
	virtual ~IPhysics() {}

	// 2D Basic Physics
	virtual void EnablePhysics2D(const S2DPhysicalizeParams& params) = 0;
	// ~2D Basic Physics

	// 2D Collision
	virtual bool AddCollisionListener(const SCollisionListenerParams& params) = 0;
	virtual void RemoveCollisionListener(IEntity* pEntity) = 0;
	// ~2D Collision
};