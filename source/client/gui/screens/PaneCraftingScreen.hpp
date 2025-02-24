/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "../Screen.hpp"
#include "../components/ImageButton.hpp"

class PaneCraftingScreen : public Screen
{
public:
	PaneCraftingScreen();
	virtual void init() override;
	virtual void tick() override;
	virtual void render(int a, int b, float c) override;
	virtual void buttonClicked(Button*) override;
	
	void updateServerVisibilityText();

private:
	//int field_3C;
	int field_40;
	ImageButton m_btnBack;
	ImageButton m_btnCategory[4];
};

