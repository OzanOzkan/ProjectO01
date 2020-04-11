/* Copyright (C) 2019 Ozan Ozkan
* All of the implementations are experimental and subject to change.
*/

#pragma once

#include <memory>
#include <set>
#include <functional>

#include <System/ISystem.h>
#include "Window.h"

//#include "GLFWWindow.h"

enum class EWindowType
{
	eWT_NONE = 0,
	//eWT_GLFW,
	eWT_SDL2
};

class CWindowManager : public IWindowManager
{
public:
	CWindowManager(ISystem* systemContext);

	// IWindowManager
	const int& GetWindowId() override;
	const SWindowSize& GetWindowSize() override;
	// ~IWindowManager

	void initWindow(const EWindowType& windowType);
	void setWindowSize(const int& height, const int& width);
	void onUpdate();

	void registerWindowEvents(IWindowEventListener* listener);
	void unregisterWindowEvents(IWindowEventListener* listener);
	void onWindowEvent(const SWindowEvent& event);

	int getTicks() { return m_activeWindow->getTicks(); }

private:
	ISystem * GetSystem() { return m_pSystem; }
	void notifyListeners(const SWindowEvent& event);

private:
	ISystem * m_pSystem;

	int m_height = 1000;
	int m_width = 480;

	std::unique_ptr<CWindow> m_activeWindow = nullptr;
	std::set<IWindowEventListener*> m_eventListeners{};
};