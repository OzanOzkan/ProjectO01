#include "OnSelectionListenerEntityComponent.h"

#include <System/EntitySystem/IEntity.h>
#include <IInput.h>
#include <Renderer/IRenderer.h>
#include <ILog.h>

/////////////////////////////////////////////////
OnSelectionListenerEntityComponent::OnSelectionListenerEntityComponent()
	: m_debugDraw(false)
	, m_pBBDebugRect(nullptr)
	, m_pInputDebugRect(nullptr)
{
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::Init()
{
	updateBoundingBox();
}

/////////////////////////////////////////////////
unsigned int OnSelectionListenerEntityComponent::getEventMask() const
{
	return EEntityEvent::ENTITY_EVENT_UPDATE;
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::onEvent(const SEntityEvent & event)
{
	processInputEvent();

	if (m_debugDraw)
		debugDraw();
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::processInputEvent()
{
	bool isEventInBoundingBox = false;
	Vector2 inputPosition(0, 0);

	updateBoundingBox();

	// Touch input
	STouchEventList touchEvents = GetSystem()->GetInput()->GetTouchEvents();
	if (!touchEvents.empty())
	{
		STouchEvent touchEvent = touchEvents.front();
		inputPosition = touchEvent.position;
	}
	else
	{
		// Mouse input
		if (GetSystem()->GetInput()->IsKeyPressed(EKey::eKID_MOUSE_LEFT))
			inputPosition = GetSystem()->GetInput()->GetMousePosition();
	}

	if (checkSelection(inputPosition))
	{
		for (auto fn : m_callbacks)
			fn(inputPosition);
	}
}

/////////////////////////////////////////////////
bool OnSelectionListenerEntityComponent::checkSelection(const Vector2& positionToCheck)
{
	Math::Point selectionPoint;
	selectionPoint.x = positionToCheck.x;
	selectionPoint.y = positionToCheck.y;

	return Math::isPointInRect(selectionPoint, m_boundingBox);
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::updateComponent()
{
	updateBoundingBox();
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::subscribeOnSelection(TSelectionEventCallbackFn fn)
{
	m_callbacks.emplace_back(fn);
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::unsubscribeOnSelection(TSelectionEventCallbackFn fn)
{
	//auto elem = std::find(m_callbacks.begin(), m_callbacks.end(), fn);
	//m_callbacks.erase(elem);
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::setSize(const float& w, const float& h)
{
	m_boundingBox.w = w;
	m_boundingBox.h = h;
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::setDebugDraw(bool isActive)
{
	m_debugDraw = isActive;
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::updateBoundingBox()
{
	Vector2 entityPosition = getEntity()->getPosition();
	m_boundingBox.x = entityPosition.x + m_componentRelativePosition.x;
	m_boundingBox.y = entityPosition.y + m_componentRelativePosition.y;
}

/////////////////////////////////////////////////
void OnSelectionListenerEntityComponent::debugDraw()
{
	int layerId = 98;

	Vector2 boundingBoxPos;
	boundingBoxPos.x = m_boundingBox.x;
	boundingBoxPos.y = m_boundingBox.y;

	if (!m_pBBDebugRect)
	{
		SRectParams params;
		params.layerId = layerId;
		params.position = boundingBoxPos;
		params.height = m_boundingBox.h;
		params.width = m_boundingBox.w;

		m_pBBDebugRect = (IRect*)GetSystem()->GetRenderer()->CreateRenderObject(params);
		m_pBBDebugRect->setRenderActive(true);
	}
	else
	{
		m_pBBDebugRect->setPosition(boundingBoxPos);
	}

	if (!m_pInputDebugRect)
	{
		SRectParams params;
		params.layerId = layerId;
		params.position = GetSystem()->GetInput()->GetMousePosition();
		params.height = 10;
		params.width = 10;

		m_pInputDebugRect = (IRect*)GetSystem()->GetRenderer()->CreateRenderObject(params);
		m_pInputDebugRect->setRenderActive(true);
	}
	else
	{
		m_pInputDebugRect->setPosition(GetSystem()->GetInput()->GetMousePosition());
	}
}