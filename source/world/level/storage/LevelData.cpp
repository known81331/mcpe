/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "LevelData.hpp"

void LevelData::_init(TLong seed, int x)
{
	m_seed = seed;
	field_10 = 0;
	field_14 = 0;
	field_18 = 0;
	field_1C = 0;
	field_20 = x;
	m_nPlayers = -1;
}

void LevelData::_init(TLong seed, int x, const std::string& name)
{
	_init(seed, x);
	field_78 = name;
}

void LevelData::read(RakNet::BitStream& bs, int version)
{
	field_20 = version;
	bs.Read(m_seed);
	bs.Read(m_spawnPos.x);
	bs.Read(m_spawnPos.y);
	bs.Read(m_spawnPos.z);
	bs.Read(field_10);
	bs.Read(field_18);
	bs.Read(field_14);

	RakNet::RakString rs;
	bs.Read(rs);
	field_78 = std::string(rs.C_String());
}

void LevelData::write(RakNet::BitStream& bs)
{
	bs.Write(m_seed);
	bs.Write(m_spawnPos.x);
	bs.Write(m_spawnPos.y);
	bs.Write(m_spawnPos.z);
	bs.Write(field_10);
	bs.Write(field_18);
	bs.Write(int(getEpochTimeS()));

	RakNet::RakString rs(field_78.c_str());
	bs.Write(rs);
}

void PlayerData::loadPlayer(Player* player)
{
	player->setPos(0.0f, 0.0f, 0.0f);

	player->m_pos    = m_pos;
	player->field_3C = m_pos;
	player->field_98 = m_pos;
	player->m_vel.x = Mth::abs(m_vel.x) > 10.0f ? 0.0f : m_vel.x;
	player->m_vel.y = Mth::abs(m_vel.y) > 10.0f ? 0.0f : m_vel.y;
	player->m_vel.z = Mth::abs(m_vel.z) > 10.0f ? 0.0f : m_vel.z;

	player->m_pitch = player->field_60 = m_pitch;
	player->m_yaw   = player->field_5C = m_yaw;
	player->m_distanceFallen = m_distanceFallen;
	player->field_C0 = field_24;
	player->field_BC = field_26;
	player->field_7C = field_28;

	// @NOTE: Why are we updating m_pos, field_3C and field_98 above if we do this?
	player->setPos(m_pos.x, m_pos.y, m_pos.z);

	if (player->m_pInventory->m_bIsSurvival) {
		for (int i = 0; i < C_NUM_SURVIVAL_SLOTS; i++) {
			printf("Loading %s %d \n",  m_items[i].getItem() ? m_items[i].getItem()->getName().data() : "Nothing", m_items[i].m_amount);
		//	player->m_pInventory->m_items.push_back(m_items[i]);
			if (m_items[i].getItem())
				player->m_pInventory->m_items[i] = (m_items[i]); //player->m_pInventory->addItem(&m_items[i]);
		}
	}

	// TODO: survival mode stuff
	for (int i = 0; i < C_MAX_HOTBAR_ITEMS; i++) {
		player->m_pInventory->m_hotbar[i] = m_hotbar[i];
		auto* item = player->m_pInventory->getQuickSlotItem(i);
		printf("slot %d %d %d %d\n", i, m_hotbar[i], player->m_pInventory->getQuickSlotItemId(m_hotbar[i]), item ? item->m_amount : 0 );
	}
}
	

void PlayerData::savePlayer(Player* player)
{
	m_pos = player->m_pos;
	m_vel = player->m_vel;
	m_pitch = player->m_pitch;
	m_yaw   = player->m_yaw;
	m_distanceFallen = player->m_distanceFallen;
	field_24 = player->field_C0;
	field_26 = player->field_BC;
	field_28 = player->field_7C;

	if (player->m_pInventory->m_bIsSurvival) {
		for (int i = 0; i < C_NUM_SURVIVAL_SLOTS; i++) {
			if (i < player->m_pInventory->m_items.size()) {
				printf("Saving %s %d \n", m_items[i].getItem() ? m_items[i].getItem()->getName().data() : "Nothing", m_items[i].m_amount);
				m_items[i] = player->m_pInventory->m_items[i];
			}
		}
	}
	
	// TODO: survival mode stuff
	for (int i = 0; i < C_MAX_HOTBAR_ITEMS; i++) {
		m_hotbar[i] = player->m_pInventory->m_hotbar[i];
		auto* item = player->m_pInventory->getQuickSlotItem(i);
		printf("slot %d %d %d %d\n", i, m_hotbar[i], player->m_pInventory->getQuickSlotItemId(m_hotbar[i]), item ? item->m_amount : 0 );
	}
		// player->m_pInventory->m_hotbar[i]; 

}
