#include "WindowManager.h"
#include "GLFWWindow.h"
#include <ILog.h>

#include <iostream>

/////////////////////////////////////////////////
void CWindowManager::setWindowSize(const int& height, const int& width)
{
	m_height = height;
	m_width = width;
}

CWindowManager::CWindowManager(SEnvironment * env)
	: m_pEnv(env)
{
}

/////////////////////////////////////////////////
void CWindowManager::initWindow(const EWindowType& windowType)
{
	switch (windowType)
	{
	case EWindowType::eWT_GLWF:
	{
		m_activeWindow = std::make_unique<CGLFWWindow>(m_pEnv);
		m_pEnv->pLog->Log("CWindowManager:initWindow: Initializing GLFW Window");
	}
	break;
	default:
	{
		// TODO
	}
	break;
	}
	
	if (m_activeWindow)
	{
		m_activeWindow.get()->openWindow(
			m_height,
			m_width,
			[this](const SWindowEvent& event) { onWindowEvent(event); }
		);
	}
}

/////////////////////////////////////////////////
WindowProcAddr CWindowManager::getWindowProcAddress()
{
	return m_activeWindow->getWindowProcAddress();
}

/////////////////////////////////////////////////
void CWindowManager::registerWindowEvents(IWindowEventListener* listener)
{
	m_eventListeners.insert(listener);
}

/////////////////////////////////////////////////
void CWindowManager::unregisterWindowEvents(IWindowEventListener* listener)
{
	m_eventListeners.erase(listener);
}

/////////////////////////////////////////////////
void CWindowManager::onUpdate()
{
	if (m_activeWindow)
		m_activeWindow->onUpdate();
}

/////////////////////////////////////////////////
void CWindowManager::onWindowEvent(const SWindowEvent& event)
{
	notifyListeners(event);
}

/////////////////////////////////////////////////
void CWindowManager::notifyListeners(const SWindowEvent& event)
{
	for (IWindowEventListener* listener : m_eventListeners)
	{
		listener->onWindowEvent(event);
	}
}