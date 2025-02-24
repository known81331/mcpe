/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "ImageButton.hpp"

void ImageButton::_init()
{
	m_width = 0;
	m_height = 0;
	m_xPos = 0;
	m_yPos = 0;
	m_text = "";
	m_bEnabled = true;
	m_bVisible = true;
	field_36 = false;

#ifndef ORIGINAL_CODE
	m_lastX = 0;
	m_lastY = 0;
#endif
}

ImageButton::ImageButton(int buttonId, int xPos, int yPos, int btnWidth, int btnHeight, const std::string& text):
Button(buttonId, xPos, yPos, btnWidth, btnHeight, text)
{
	_init();

	m_buttonId = buttonId;
	m_xPos = xPos;
	m_yPos = yPos;
	m_text = text;
	m_width  = btnWidth;
	m_height = btnHeight;
}

ImageButton::ImageButton():
Button(0, 0, 0, 0, 0, "")
{
	_init();
}


ImageButton::ImageButton(int buttonId, int xPos, int yPos, const std::string& text):
Button(buttonId, xPos, yPos, text)
{
	_init();

	m_buttonId = buttonId;
	m_xPos = xPos;
	m_yPos = yPos;
	m_text = text;
	m_width  = 200;
	m_height = 24;
}

ImageButton::ImageButton(int buttonId, const std::string& text) :
Button(buttonId, text)
{
	_init();

	m_buttonId = buttonId;
	m_text = text;
	m_width  = 200;
	m_height = 24;
}

ImageButton::ImageButton(int buttonId, const std::string& text, const ImageDef& other):
Button(buttonId, text)
{
	_init();

	m_buttonId = buttonId;
	m_text = text;
	m_width  = 200;
	m_height = 24;

	m_imageDef = other;
}


ImageButton::ImageButton(int buttonId, const std::string& text, const ImageDef& other, bool size):
Button(buttonId, text)
{
	_init();

	m_buttonId = buttonId;
	m_text = text;
	m_width  = other.u;
	m_height = other.v;

	m_imageDef = other;
}


bool ImageButton::clicked(Minecraft* pMinecraft, int xPos, int yPos)
{
	if (!m_bEnabled) return false;
	if (xPos < m_xPos) return false;
	if (yPos < m_yPos) return false;
	if (xPos >= m_xPos + m_width) return false;
	if (yPos >= m_yPos + m_height) return false;

	return true;
}

int ImageButton::getYImage(bool bHovered)
{
	if (!m_bEnabled) return 0;
	if (bHovered) return 2;
	return 1;
}

void ImageButton::released(int xPos, int yPos)
{

}

void ImageButton::renderBg(Minecraft*, int, int)
{

}

void ImageButton::setImageDef(const ImageDef& other, bool param2) {
	m_imageDef = other;
	// no idea tbh
	if (param2) { 
		m_width  = other.u;
		m_height = other.v;
		//*(int *)(this + 0x14) = (int)*(float *)(this + 0x5c);
		//*(int *)(this + 0x18) = (int)*(float *)(this + 0x60);
	}
}

void ImageButton::render(Minecraft* pMinecraft, int xPos, int yPos)
{

	Button::render(pMinecraft, xPos, yPos);
	if (!m_bVisible) return;

#ifdef ENH_HIGHLIGHT_BY_HOVER
if (m_bHoverable)
	field_36 = clicked(pMinecraft, xPos, yPos);
#endif

	Font* pFont = pMinecraft->m_pFont;
	Textures* pTexs = pMinecraft->m_pTextures;

	pTexs->loadAndBindTexture(m_imageDef.sprite);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    if (!field_36) {
        blit(m_xPos+m_imageDef.ox, m_yPos+m_imageDef.oy, m_imageDef.x, m_imageDef.y, m_width, m_height, m_imageDef.u,  m_imageDef.v);
    }
    else {
        blit(m_xPos+m_imageDef.ox, m_yPos+m_imageDef.oy, m_imageDef.ax, m_imageDef.ay, m_width, m_height, m_imageDef.au,  m_imageDef.av);
    }
    
	renderBg(pMinecraft, xPos, yPos);

	int textColor;
	if (!m_bEnabled)
		textColor = int(0xFFA0A0A0U);
	else if (field_36)
		textColor = int(0xFFFFA0U);
	else
		textColor = int(0xE0E0E0U);

	drawCenteredString(pFont, m_text, m_xPos + m_width / 2, m_yPos + (m_height - 8) / 2, textColor);
}
