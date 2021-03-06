#include "TextRendererEntityComponent.h"

#include <System/EntitySystem/IEntity.h>
#include <Renderer/IRenderer.h>

/////////////////////////////////////////////////
void TextRendererEntityComponent::Init()
{

}

/////////////////////////////////////////////////
unsigned int TextRendererEntityComponent::getEventMask() const
{
	return EEntityEvent::ENTITY_EVENT_UPDATE | EEntityEvent::ENTITY_EVENT_DESTROY;
}

/////////////////////////////////////////////////
void TextRendererEntityComponent::onEvent(const SEntityEvent & event)
{
	switch (event.GetEvent())
	{
	case EEntityEvent::ENTITY_EVENT_UPDATE:
	{
		onEntityUpdateEvent();
	}
	break;
	case EEntityEvent::ENTITY_EVENT_DESTROY:
	{
		onEntityDestroyEvent();
	}
	break;
	}
}

/////////////////////////////////////////////////
void TextRendererEntityComponent::updateComponent()
{
	STextParams params;
	params.text = m_text;
	params.font = m_font;
	params.fontSize = m_fontSize;
	params.layerId = m_layerId;
	params.color = m_color;
	params.position = getEntity()->getPosition() + getPosition();

	if (m_pText)
		GetSystem()->GetRenderer()->RemoveRenderObject(m_pText);
	
	m_pText = static_cast<IText*>(GetSystem()->GetRenderer()->CreateRenderObject(params));
	m_pText->setRenderActive(true);
}

/////////////////////////////////////////////////
float TextRendererEntityComponent::getTextWidth()
{
	float textWidth = -1;

	if (m_pText) {
		textWidth = m_pText->getWidth();
	}

	return textWidth;
}

/////////////////////////////////////////////////
float TextRendererEntityComponent::getTextHeight()
{
	float textHeight = -1;

	if (m_pText) {
		textHeight = m_pText->getHeight();
	}

	return textHeight;
}

/////////////////////////////////////////////////
void TextRendererEntityComponent::onEntityUpdateEvent()
{
	m_pText->setPosition(getEntity()->getPosition() + getPosition());
}

/////////////////////////////////////////////////
void TextRendererEntityComponent::onEntityDestroyEvent()
{
	GetSystem()->GetRenderer()->RemoveRenderObject(m_pText);
}