#include "SDLRenderer.h"

#include <ILog.h>

#include "RendererObject/SDLRendererObject.h"
#include "RendererObject/SDLSprite.h"
#include "RendererObject/SDLAnimatedSprite.h"
#include "RendererObject/SDLText.h"
#include "RendererObject/SDLRect.h"
#include "RendererObject/SDLPolygon.h"

/////////////////////////////////////////////////
CSDLRenderer::CSDLRenderer(ISystem * systemContext)
	: m_defaultRenderWidth(800)
	, m_defaultRenderHeight(600)
	, m_pSystem(systemContext)
	, m_pTextureManager(nullptr)
	, m_pSDLWindow(nullptr)
	, m_pSDLRenderer(nullptr)
	, m_renderObjectList()
{
}

/////////////////////////////////////////////////
void CSDLRenderer::InitializeModule()
{
	m_pSDLWindow = SDL_GetWindowFromID(GetSystem()->GetWindowManager()->GetWindowId());
	if(!m_pSDLWindow) GetSystem()->GetLogger()->Log("CSDLRenderer::InitializeModule(): Window Failure!");

	int windowWidth;
	int windowHeight;
	SDL_GetWindowSize(m_pSDLWindow, &windowWidth, &windowHeight);

	SDL_DestroyRenderer(SDL_GetRenderer(m_pSDLWindow));
	m_pSDLRenderer = SDL_CreateRenderer(m_pSDLWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);

    if(!m_pSDLRenderer) {
		GetSystem()->GetLogger()->Log("CSDLRenderer::InitializeModule(): SDL Renderer creation failed!");
    }

	if (TTF_Init() == -1)
		GetSystem()->GetLogger()->Log("Renderer [SDL2]: TTF initialization failed!");

	SDL_SetRenderDrawBlendMode(m_pSDLRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	//LoadTextures();

	GetSystem()->GetLogger()->Log("Renderer Initialized: SDL2");

	// Initialize Texture Manager
	m_pTextureManager = std::make_unique<CSDLTextureManager>(m_pSystem, m_pSDLRenderer);

	// Initialize Camera
	m_pCamera = std::make_unique<CSDLCamera>(m_pSystem);
	m_pCamera->SetSize(1440, 2960);
	//m_pCamera->SetPosition(Vector2(-100, 0));
}

/////////////////////////////////////////////////
void CSDLRenderer::onUpdate()
{
	doRender();
}

/////////////////////////////////////////////////
void CSDLRenderer::setResolution(const int& width, const int& height)
{
	if(m_pSDLRenderer)
		SDL_RenderSetLogicalSize(m_pSDLRenderer, width, height);
}

/////////////////////////////////////////////////
void CSDLRenderer::getResolution(int& width, int& height)
{
	if (m_pSDLRenderer)
		SDL_RenderGetLogicalSize(m_pSDLRenderer, &width, &height);
}

/////////////////////////////////////////////////
IRendererObject * CSDLRenderer::CreateRenderObject(const SRenderObjectParams & params)
{
	std::unique_ptr<IRendererObject> pRenderObject = nullptr;

	switch (params.type)
	{
	case ERendererObjectType::eRT_SPRITE:
	{
		pRenderObject = std::make_unique<CSDLSprite>(this, m_pSDLRenderer);
	}
	break;
	case ERendererObjectType::eRT_ANIMATED_SPRITE:
	{
		pRenderObject = std::make_unique<CSDLAnimatedSprite>(this, m_pSDLRenderer);
	}
	break;
	case ERendererObjectType::eRT_TEXT:
	{
		pRenderObject = std::make_unique<CSDLText>(this, m_pSDLRenderer);
	}
	break;
	case ERendererObjectType::eRT_RECT:
	{
		pRenderObject = std::make_unique<CSDLRect>(this, m_pSDLRenderer);
	}
	break;
	case ERendererObjectType::eRT_POLYGON:
	{
		pRenderObject = std::make_unique<CSDLPolygon>(this, m_pSDLRenderer);
	}
	break;
	}
		
	pRenderObject->setId(m_renderObjectList.size() > 0 ? m_renderObjectList.rbegin()->first + 1 : 0);
	pRenderObject->Load(params);
	
	m_renderLayerMap.emplace(params.layerId, pRenderObject->getId());
	
	return static_cast<IRendererObject*>(m_renderObjectList.insert(
		std::make_pair(pRenderObject->getId(),
			std::move(pRenderObject)
		)
	).first->second.get());
}

/////////////////////////////////////////////////
void CSDLRenderer::RemoveRenderObject(IRendererObject* pRenderObject)
{
	// Remove object from the layer map.
	for (auto layerItr = m_renderLayerMap.begin(); layerItr != m_renderLayerMap.end(); ++layerItr)
	{
		if (layerItr->second == pRenderObject->getId())
		{
			m_renderLayerMap.erase(layerItr);
			break;
		}
	}

	// And remove object from the list.
	m_renderObjectList.erase(pRenderObject->getId());
}

/////////////////////////////////////////////////
void CSDLRenderer::doRender()
{
	// Set background to black.
	SDL_SetRenderDrawColor(m_pSDLRenderer, 0, 0, 0, 0);

	// Clear SDL renderer.
	SDL_RenderClear(m_pSDLRenderer);

	//auto windowSize = GetSystem()->GetWindowManager()->GetWindowSize();
	//setResolution(windowSize.width, windowSize.height);

	for (auto& pRenderLayerEntry : m_renderLayerMap)
	{
		auto pRenderObject = m_renderObjectList.find(pRenderLayerEntry.second);
		IRendererObject* pRendererObject = pRenderObject->second.get();

		// Do not render if object render is not active.
		if (!pRendererObject->isRenderActive())
			continue;

		// Load object position, size and texture to SDL Renderer.
		pRendererObject->RenderCopy();
	}

	// Present prepared SDL renderer.
	SDL_RenderPresent(m_pSDLRenderer);
}