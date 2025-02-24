/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "../GuiComponent.hpp"
#include "client/app/Minecraft.hpp"
#include "Button.hpp"

class Screen;

struct ImageDef {
	int x,y,u,v, ox, oy;
	int ax, ay, au, av, aox, aoy;
	std::string sprite;
};

class ImageButton : public Button
{
private:
	void _init();
public:
	ImageButton();
	ImageButton(int, int x, int y, int width, int height, const std::string&);
	ImageButton(int, int x, int y, const std::string&);
	ImageButton(int, const std::string&);
	ImageButton(int, const std::string&, const ImageDef& other);
	ImageButton(int, const std::string&, const ImageDef& other, bool size);

	// I can't possibly explain why Minecraft is referenced here
	bool clicked(Minecraft*, int xPos, int yPos);
	int  getYImage(bool bHovered);
	void released(int xPos, int yPos);
	void renderBg(Minecraft*, int, int);
	void render(Minecraft*, int xPos, int yPos) override;
	void setImageDef(const ImageDef& other, bool param2);

public:
	ImageDef m_imageDef;

};

