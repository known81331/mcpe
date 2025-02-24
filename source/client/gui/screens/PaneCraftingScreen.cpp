/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "PaneCraftingScreen.hpp"
#include "OptionsScreen.hpp"
#include "network/ServerSideNetworkHandler.hpp"
#include "client/app/Minecraft.hpp"

ImageDef btndddef = {
    120, 0, 36, 36, 0, 0,
    156, 0, 36, 36, 0, 0,
    "gui/spritesheet.png"
};

PaneCraftingScreen::PaneCraftingScreen() :
	//field_3C(0),
	field_40(0),	
	m_btnBack(0, "", btndddef)
{

}

void PaneCraftingScreen::init()
{
	bool bAddVisibleButton = m_pMinecraft->m_pRakNetInstance && m_pMinecraft->m_pRakNetInstance->m_bIsHost;
	
	int nButtons = 2;

	int currY = 48, inc = 32;

	bool cramped = m_height < currY + inc * nButtons + 10; // also add some padding
	if (cramped)
		inc = 25;

	m_btnBack.m_width = m_btnBack.m_height = 19;

	m_btnBack.m_yPos = 2;
	m_btnBack.m_xPos = m_width-21;


	currY += inc;

	// add the buttons to the screen:
	m_buttons.push_back(&m_btnBack);

	m_btnCategory[0] = ImageButton(6, "", {
		0, 128, 64, 64, 0, 0,
		0, 128, 64, 64, 0, 0,
		"gui/spritesheet.png"
	});

	printf("%d \n", m_height);
	int btnsWidth = (54.f / 240) * m_height, btnsSpacing = (2.f / 240) * m_height;
	m_btnCategory[0].m_yPos = 9.f / 240 * m_height;
	m_btnCategory[0].field_36 = true;

	m_btnCategory[1] = ImageButton(7, "", {
		0, 192, 64, 64, 0, 0,
		0, 192, 64, 64, 0, 0,
		"gui/spritesheet.png"
	});


	m_btnCategory[2] = ImageButton(8, "", {
		64, 128, 64, 64, 0, 0,
		64, 128, 64, 64, 0, 0,
		"gui/spritesheet.png"
	});


	m_btnCategory[3] = ImageButton(9, "", {
		64, 192, 64, 64, 0, 0,
		64, 192, 64, 64, 0, 0,
		"gui/spritesheet.png"
	});

	m_btnCategory[0].m_xPos = 10.f / 240 * m_height;
	m_btnCategory[1].m_xPos = 10.f / 240 * m_height;
	m_btnCategory[2].m_xPos = 10.f / 240 * m_height;
	m_btnCategory[3].m_xPos = 10.f / 240 * m_height;

	m_btnCategory[0].m_width = m_btnCategory[0].m_height = btnsWidth;
	m_btnCategory[1].m_width = m_btnCategory[1].m_height = btnsWidth;
	m_btnCategory[2].m_width = m_btnCategory[2].m_height = btnsWidth;
	m_btnCategory[3].m_width = m_btnCategory[3].m_height = btnsWidth;

	m_btnCategory[1].m_yPos = m_btnCategory[0].m_yPos + m_btnCategory[0].m_height + btnsSpacing;
	m_btnCategory[2].m_yPos = m_btnCategory[1].m_yPos + m_btnCategory[0].m_height + btnsSpacing;
	m_btnCategory[3].m_yPos = m_btnCategory[2].m_yPos + m_btnCategory[0].m_height + btnsSpacing;

	m_btnCategory[0].m_bHoverable = false;
	m_btnCategory[1].m_bHoverable = false;
	m_btnCategory[2].m_bHoverable = false;
	m_btnCategory[3].m_bHoverable = false;

	m_buttons.push_back(&m_btnCategory[0]);
	m_buttons.push_back(&m_btnCategory[1]);
	m_buttons.push_back(&m_btnCategory[2]);
	m_buttons.push_back(&m_btnCategory[3]);
	
	for (int i = 0; i < int(m_buttons.size()); i++)
		m_buttonTabList.push_back(m_buttons[i]);

}

void PaneCraftingScreen::updateServerVisibilityText()
{
}

void PaneCraftingScreen::tick()
{
	field_40++;
}

void PaneCraftingScreen::render(int a, int b, float c)
{

	Textures* pTexs = m_pMinecraft->m_pTextures;

	pTexs->loadAndBindTexture("gui/spritesheet.png");

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
		int texSize = 12;
		
        blit(0, 0, 8, 8, m_width, m_height, 24-8,  24-8);

        blit(0, 0, 15, 0, m_width, texSize, 1,16);
        blit(0, m_height-texSize, 15,16, m_width, texSize, 1,16);
        blit(0, 0, 0, 8, texSize, m_height, 16,  16);
        blit(m_width-texSize, 0, 16, 8, texSize, m_height, 16,  16);


        blit(0, 0, 0, 0, texSize, texSize, 16,16);
        blit(m_width-texSize, 0, 16, 0, texSize, texSize, 16,  16);


        blit(0, m_height-texSize, 0, 16, texSize, texSize, 16,  16);
        blit(m_width-texSize, m_height-texSize, 16, 16, texSize, texSize, 16,  16);


		



	drawCenteredString(m_pFont, "Crafting TEST", m_width / 2, 24, 0xFFFFFF);



	Screen::render(a, b, c);
}

void PaneCraftingScreen::buttonClicked(Button* pButton)
{
	if (pButton->m_buttonId == m_btnBack.m_buttonId)
		m_pMinecraft->setScreen(nullptr);

	for (int i = 0; i < 4; i++) {
		m_btnCategory[i].field_36 = false;
		if (pButton->m_buttonId == m_btnCategory[i].m_buttonId) {
			m_btnCategory[i].field_36 = true;
		}
	}
}
