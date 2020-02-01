/* Copyright (C) 2019 Ozan Ozkan
* All of the implementations are experimental and subject to change.
*/

#pragma once

enum class EModule
{
	eM_SYSTEM = 0,
	eM_RENDERER,
	eM_INPUT,
	eM_PHYSICS,
	eM_GAME
};

class IModule
{
public:
	virtual ~IModule() {}

	virtual void InitializeModule() = 0;
	virtual void onUpdate() = 0;
};