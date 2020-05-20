#include "Box2DTestLevelController.h"

#include <IInput.h>
#include <System/EntitySystem/IEntity.h>
#include <Renderer/IRenderer.h>
#include <EntityComponents/SpriteRendererEntityComponent.h>
#include <EntityComponents/OnSelectionListenerEntityComponent.h>
#include <EntityComponents/Physics/Rigidbody2D.h>

/////////////////////////////////////////////////
void CBox2DTestLevelController::Init()
{
}

/////////////////////////////////////////////////
unsigned int CBox2DTestLevelController::getEventMask() const
{
	return ENTITY_EVENT_UPDATE | ENTITY_LEVEL_LOAD_END;
}

/////////////////////////////////////////////////
void CBox2DTestLevelController::onEvent(const SEntityEvent& event)
{
	if (event.GetEvent() == ENTITY_LEVEL_LOAD_END) {
		configureLevel();
	}
	else if (event.GetEvent() == ENTITY_EVENT_UPDATE) {
		processUpdateEvent();
	}
}

/////////////////////////////////////////////////
void CBox2DTestLevelController::updateComponent()
{
}

/////////////////////////////////////////////////
void CBox2DTestLevelController::configureLevel()
{
	int resW, resH;
	GetSystem()->GetRenderer()->getResolution(resW, resH);

	if (IEntity* pGroundEntity = GetSystem()->GetEntitySystem()->findEntity("groundEntity")) {
		pGroundEntity->setPosition(Vector2(0, resH - 250));
	}

	SEntitySpawnParams leftWall;
	leftWall.entityName = "leftWall";
	leftWall.position = Vector2(0, 0);
	IEntity* pLeftWall = GetSystem()->GetEntitySystem()->spawnEntity(leftWall);
	Rigidbody2D* pLeftWallPhysicsComponent = pLeftWall->addEntityComponent<Rigidbody2D>();
	pLeftWallPhysicsComponent->setBoundingBox(Vector2(10, resH*2));
	pLeftWallPhysicsComponent->setDynamic(false);
	pLeftWallPhysicsComponent->updateComponent();

	SEntitySpawnParams rightWall;
	rightWall.entityName = "rightWall";
	rightWall.position = Vector2(resW, 0);
	IEntity* pRightWall = GetSystem()->GetEntitySystem()->spawnEntity(rightWall);
	Rigidbody2D* pRightWallPhysicsComponent = pRightWall->addEntityComponent<Rigidbody2D>();
	pRightWallPhysicsComponent->setBoundingBox(Vector2(10, resH*2));
	pRightWallPhysicsComponent->setDynamic(false);
	pRightWallPhysicsComponent->updateComponent();
}

/////////////////////////////////////////////////
void CBox2DTestLevelController::processUpdateEvent()
{
	if (GetSystem()->GetInput()->IsKeyPressed(EKey::eKID_MOUSE_RIGHT)) {
		spawnDynamicPhysicalEntity(GetSystem()->GetInput()->GetMousePosition());
	}
}

/////////////////////////////////////////////////
void CBox2DTestLevelController::spawnDynamicPhysicalEntity(const Vector2& position)
{
	SEntitySpawnParams params;
	params.entityName = "dynamicPhysicalEntity";
	params.position = position;

	IEntity* pEntity = GetSystem()->GetEntitySystem()->spawnEntity(params);

	SpriteRendererEntityComponent* pSpriteRenderer = pEntity->addEntityComponent<SpriteRendererEntityComponent>();
	pSpriteRenderer->setFile("Sprites/tntbox.png");
	pSpriteRenderer->setSize(100, 100);
	pSpriteRenderer->setLayerId(10);
	pSpriteRenderer->updateComponent();

	Rigidbody2D* physicsComponent = pEntity->addEntityComponent<Rigidbody2D>();
	physicsComponent->setBoundingBox(Vector2(100, 100));
	physicsComponent->setDensity(1.f);
	physicsComponent->updateComponent();

	OnSelectionListenerEntityComponent* pOnSelectionComponent = pEntity->addEntityComponent<OnSelectionListenerEntityComponent>();
	pOnSelectionComponent->subscribeOnSelection([physicsComponent](const bool& isSelected, const Vector2& selectionPos) {
		if (!isSelected) {
			//physicsComponent->applyForce(Vector2(0, -500));
			physicsComponent->applyLinearImpulse(Vector2(0.f, -80.f));
		}
		}
	);
}
