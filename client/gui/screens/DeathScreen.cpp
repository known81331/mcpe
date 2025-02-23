#include "DeathScreen.hpp"

DeathScreen::DeathScreen() :
	m_btnRespawn(1, "Respawn!"),
	m_btnTitle  (2, "Main menu")
{
	m_tickCounter = 0;
}

void DeathScreen::init()
{
	m_btnRespawn.m_width = m_btnTitle.m_width = m_width / 4;
	m_btnRespawn.m_xPos  = m_width / 2 - 10 - m_btnRespawn.m_width;
	m_btnTitle.m_xPos    = m_width / 2 + 10;

	m_btnRespawn.m_yPos = m_btnTitle.m_yPos = m_height / 2;

	m_buttons.push_back(&m_btnRespawn);
	m_buttons.push_back(&m_btnTitle);
	m_buttonTabList.push_back(&m_btnRespawn);
	m_buttonTabList.push_back(&m_btnTitle);
}

void DeathScreen::buttonClicked(Button* pButton)
{
	if (pButton->m_buttonId == m_btnRespawn.m_buttonId)
	{
		m_pMinecraft->m_pLocalPlayer->respawn();
		m_pMinecraft->setScreen(nullptr);
	}
	if (pButton->m_buttonId == m_btnTitle.m_buttonId)
	{
		m_pMinecraft->leaveGame(false);
	}
}

void DeathScreen::tick()
{
	m_tickCounter++;
}

void DeathScreen::keyPressed(int key)
{
}

void DeathScreen::render(int x, int y, float f)
{
	fillGradient(0, 0, m_width, m_height, 0xA0303080, 0x60000050);

	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);
	drawCenteredString(m_pFont, "You died!", m_width / 4, m_height / 8, 0xFFFFFF);
	glPopMatrix();

	// render the buttons after 1.5 seconds
	if (m_tickCounter >= 30)
		Screen::render(x, y, f);
}
