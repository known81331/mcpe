/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "LevelChunk.hpp"
#include "ChunkCache.hpp"
#include "world/level/Level.hpp"
#include <stdexcept>
#include <iostream>

ChunkCache::ChunkCache(Level* pLevel, ChunkStorage* pStor, ChunkSource* pSrc)
{
	field_4 = true;
	m_pLevel = nullptr;
	m_pLastChunk = nullptr;
	m_LastChunkX = -999999999;
	m_LastChunkZ = -999999999;

	m_pChunkSource = pSrc;
	m_pChunkStorage = pStor;
	m_pLevel = pLevel;

	m_pEmptyChunk = new EmptyLevelChunk(pLevel, nullptr, 0, 0);
	m_pEmptyChunk->field_236 = true;

	memset(m_chunkMap, 0, sizeof(m_chunkMap));
}

LevelChunk* ChunkCache::create(int x, int z)
{
	return getChunk(x, z);
}

LevelChunk* ChunkCache::getChunk(int x, int z)
{
    
    if (x == m_LastChunkX && z == m_LastChunkZ && m_pLastChunk) {
        return m_pLastChunk;
    }

    if (!fits(x, z)) {
        return m_pEmptyChunk;
    }

    int chunkX = x & (C_MAX_CHUNKS_X - 1);
    int chunkZ = z & (C_MAX_CHUNKS_Z - 1);

    if (!hasChunk(x, z)) {
        if (m_chunkMap[chunkZ][chunkX]) {
            m_chunkMap[chunkZ][chunkX]->unload();
        //    save(m_chunkMap[chunkZ][chunkX]);

			//auto * pChk = m_chunkMap[chunkZ][chunkX];
			//m_chunkMap[chunkZ][chunkX] = nullptr;

			//pChk->deleteBlockData();
			//SAFE_DELETE(pChk);
        }

        LevelChunk* chunk = nullptr; //load(x, z);
        if (!chunk) {
            if (m_pChunkSource) {
                chunk = m_pChunkSource->getChunk(x, z);
            }
            else {
                chunk = m_pEmptyChunk;
            }
        }

        m_chunkMap[chunkZ][chunkX] = chunk;
        chunk->lightLava();

        if (chunk) {
           chunk->load();
        }

        if (!chunk->field_234 && hasChunk(x + 1, z + 1) && hasChunk(x, z + 1) && hasChunk(x + 1, z)) {
            postProcess(this, x, z);
        }

        if (hasChunk(x - 1, z) && !getChunk(x - 1, z)->field_234 && hasChunk(x - 1, z + 1) && hasChunk(x, z + 1) && hasChunk(x - 1, z)) {
            postProcess(this, x - 1, z);
        }

        if (hasChunk(x, z - 1) && !getChunk(x, z - 1)->field_234 && hasChunk(x + 1, z - 1) && hasChunk(x + 1, z) && hasChunk(x, z - 1)) {
            postProcess(this, x, z - 1);
        }

        if (hasChunk(x - 1, z - 1) && !getChunk(x - 1, z - 1)->field_234 && hasChunk(x - 1, z - 1) && hasChunk(x, z - 1) && hasChunk(x - 1, z)) {
            postProcess(this, x - 1, z - 1);
        }
    }

    m_LastChunkX = x;
    m_LastChunkZ = z;
    m_pLastChunk = m_chunkMap[chunkZ][chunkX];
    return m_chunkMap[chunkZ][chunkX];
    
    /*
	// get the last chunk quickly if needed
	if (m_LastChunkX == x && m_LastChunkZ == z)
	{
		if (m_pLastChunk)
			return m_pLastChunk;
	}

	if (x < 0 || z < 0 || x >= C_MAX_CHUNKS_Z || z >= C_MAX_CHUNKS_X)
		return m_pEmptyChunk;

	if (!hasChunk(x, z))
	{
		LevelChunk* pOldChunk = m_chunkMap[z][x];
		if (pOldChunk)
		{
			pOldChunk->unload();
			save(pOldChunk);
			if (m_pChunkStorage)
				m_pChunkStorage->saveEntities(m_pLevel, pOldChunk);
		}

		LevelChunk* pChunk = load(x, z);
		if (pChunk)
		{
			m_chunkMap[z][x] = pChunk;
			pChunk->lightLava();

			int globalX = x * 16, globalZ = z * 16;
			for (int i = globalX, m = 0; m < 16; i++, m++)
			{
				for (int j = globalZ, n = 0; n < 16; j++, n++)
				{
					int height = m_pLevel->getHeightmap(i, j);
					if (height >= 0)
					{
						for (int k = height; k > 0; k--)
						{
							m_pLevel->updateLight(LightLayer::Sky,   globalX,   k, globalZ,   globalX,   k, globalZ);
							m_pLevel->updateLight(LightLayer::Block, globalX-1, k, globalZ-1, globalX+1, k, globalZ+1);
						}
					}
				}
			}
		}
		else
		{
			pChunk = m_pEmptyChunk;
			if (m_pChunkSource)
				pChunk = m_pChunkSource->getChunk(x, z);

			m_chunkMap[z][x] = pChunk;
			pChunk->lightLava();
		}

		pChunk = m_chunkMap[z][x];
		if (pChunk)
			pChunk->load();

		if (!pChunk->field_234 && hasChunk(x + 1, z + 1) && hasChunk(x, z + 1) && hasChunk(x + 1, z))
			postProcess(this, x, z);

		//@OVERSIGHT: redundant call twice to hasChunk(x-1, z), hasChunk(x,z-1), and hasChunk(x-1,z-1)
		if (hasChunk(x - 1, z) && !getChunk(x - 1, z)->field_234 && hasChunk(x - 1, z + 1) && hasChunk(x, z + 1) && hasChunk(x - 1, z))
			postProcess(this, x - 1, z);

		if (hasChunk(x, z - 1) && !getChunk(x, z - 1)->field_234 && hasChunk(x + 1, z - 1) && hasChunk(x + 1, z) && hasChunk(x, z - 1))
			postProcess(this, x, z - 1);

		if (hasChunk(x - 1, z - 1) && !getChunk(x - 1, z - 1)->field_234 && hasChunk(x - 1, z - 1) && hasChunk(x, z - 1) && hasChunk(x - 1, z))
			postProcess(this, x - 1, z - 1);
	}

	m_LastChunkX = x;
	m_LastChunkZ = z;
	m_pLastChunk = m_chunkMap[z][x];
	return m_chunkMap[z][x];*/
}

std::vector<LevelChunk*> ChunkCache::getLoadedChunks() {
    std::vector<LevelChunk*> loadedChunks;

    for (int i = 0; i < C_MAX_CHUNKS_Z; ++i) {
        for (int j = 0; j < C_MAX_CHUNKS_X; ++j) {
            LevelChunk* chunk = m_chunkMap[i][j];
            if (chunk && chunk != m_pEmptyChunk) {
                loadedChunks.push_back(chunk);
            }
        }
    }

    return loadedChunks;
}

bool ChunkCache::fits(int x, int z) const {
    return true;
}

LevelChunk* ChunkCache::getChunkDontCreate(int x, int z)
{
	// get the last chunk quickly if needed
	if (m_LastChunkX == x && m_LastChunkZ == z)
	{
		if (m_pLastChunk)
			return m_pLastChunk;
	}

	//if (x < 0 || z < 0 || x >= C_MAX_CHUNKS_Z || z >= C_MAX_CHUNKS_X)
	//	return m_pEmptyChunk;

	if (!hasChunk(x, z))
	{
		LevelChunk* pOldChunk = m_chunkMap[z][x];
		if (pOldChunk)
		{
			pOldChunk->unload();
			save(pOldChunk);
			if (m_pChunkStorage)
				m_pChunkStorage->saveEntities(m_pLevel, pOldChunk);
		}

		// create an empty chunk
		LevelChunk* pChunk = m_pEmptyChunk;
		if (m_pChunkSource)
			pChunk = m_pChunkSource->getChunkDontCreate(x, z);

		m_chunkMap[z][x] = pChunk;
	}

	m_LastChunkX = x;
	m_LastChunkZ = z;
	m_pLastChunk = m_chunkMap[z][x];
	return m_chunkMap[z][x];
}

bool ChunkCache::hasChunk(int x, int z)
{
       if (!fits(x, z)) {
           return false;
       }
       if (x == m_LastChunkX && z == m_LastChunkZ) {
           return true;
       }

       int chunkX = x & (C_MAX_CHUNKS_X - 1);
       int chunkZ = z & (C_MAX_CHUNKS_Z - 1);
       LevelChunk* chunk = m_chunkMap[chunkZ][chunkX];
		if (chunk == nullptr) {
           return false;
       }

       if (chunk == m_pEmptyChunk) {
           return true;
       }

       return chunk->isAt(x, z);
    
    /*
	if (x < 0 || z < 0)
		return true;

	if (x >= C_MAX_CHUNKS_X || z >= C_MAX_CHUNKS_Z)
		return true;

	if (m_LastChunkX == x && m_LastChunkZ == z)
		return true;

	LevelChunk* pChunk = m_chunkMap[z][x];
	if (!pChunk)
		return false;

	if (pChunk == m_pEmptyChunk)
		return true;

	return pChunk->isAt(x, z)*/
}

int ChunkCache::tick()
{
	if (m_pChunkStorage)
		m_pChunkStorage->tick();

	return m_pChunkSource->tick();
}

void ChunkCache::postProcess(ChunkSource* pChkSrc, int x, int z)
{
	//if (x < 0 || z < 0 || x >= C_MAX_CHUNKS_X || z >= C_MAX_CHUNKS_Z)
	//	return;

	LevelChunk* pChunk = getChunk(x, z);
	if (!pChunk->field_234)
	{
		pChunk->field_234 = 1;
		if (m_pChunkSource)
		{
			m_pChunkSource->postProcess(m_pChunkSource, x, z);
			pChunk->clearUpdateMap();
		}
	}
}

void ChunkCache::save(LevelChunk* pChunk)
{
	if (m_pChunkStorage)
	{
		pChunk->field_23C = m_pLevel->getTime();
		m_pChunkStorage->save(m_pLevel, pChunk);
	}
}

void ChunkCache::saveAll()
{
	printf("The Game is now SAVING.... \n");
    //if (m_pChunkStorage) {
    //    int chunksToSave = 0;
    //    int savedChunks = 0;
//
    //    for (int i = 0; i < 32; ++i) {
    //        for (int j = 0; j < 32; ++j) {
    //            if (m_chunkMap[i][j] && m_chunkMap[i][j]->shouldSave(true)) {
    //                ++chunksToSave;
    //            }
    //        }
    //    }
//
    //    for (int i = 0; i < 32; ++i) {
    //        for (int j = 0; j < 32; ++j) {
    //            if (m_chunkMap[i][j]) {
    //                if (m_chunkMap[i][j]->shouldSave(true)) {
    //                    save(m_chunkMap[i][j]);
    //                    ++savedChunks;
    //                    if (savedChunks >= 2) {
    //                        return;
    //                    }
    //                }
    //            }
    //        }
    //    }
//
    //    if (m_pChunkStorage) {
    //        m_pChunkStorage->flush();
    //    }
    //}


    
	if (!m_pChunkStorage) return;

	std::vector<LevelChunk*> chunksToSave;

	for (int i = 0; i < C_MAX_CHUNKS_Z; i++)
	{
		for (int j = 0; j < C_MAX_CHUNKS_X; j++)
		{
			chunksToSave.push_back(m_pLevel->getChunk(j, i));
		}
	}

	m_pChunkStorage->saveAll(m_pLevel, chunksToSave);
}

#ifdef ENH_IMPROVED_SAVING

void ChunkCache::saveUnsaved()
{
	return;
	printf("The Game is now SAVING.... \n");
	saveAll();

	if (!m_pChunkStorage) return;

	std::vector<LevelChunk*> chunksToSave;
	for (int i = 0; i < C_MAX_CHUNKS_Z; i++)
	{
		for (int j = 0; j < C_MAX_CHUNKS_X; j++)
		{
			LevelChunk* pChunk = m_chunkMap[i][j];
			if (pChunk && pChunk != m_pEmptyChunk) // && pChunk->m_bUnsaved)
			{
				chunksToSave.push_back(pChunk);
			}
		}
	}
	/*
	for (int i = 0; i < C_MAX_CHUNKS_Z; i++)
	{
		for (int j = 0; j < C_MAX_CHUNKS_X; j++)
		{
			LevelChunk* pChunk = m_pLevel->getChunk(j, i);
			if (!pChunk->m_bUnsaved)
				continue;

			chunksToSave.push_back(pChunk);
		}
	}*/

	m_pChunkStorage->saveAll(m_pLevel, chunksToSave);
}

#endif

bool ChunkCache::shouldSave()
{
	return true;
}

std::string ChunkCache::gatherStats()
{
	return "ChunkCache: 1024";
}

ChunkCache::~ChunkCache()
{
	SAFE_DELETE(m_pChunkSource);
	SAFE_DELETE(m_pEmptyChunk);

	for (int i = 0; i < C_MAX_CHUNKS_Z; i++)
		for (int j = 0; j < C_MAX_CHUNKS_X; j++)
		{
			LevelChunk* pChk = m_chunkMap[i][j];
			if (pChk)
			{
				pChk->deleteBlockData();
				SAFE_DELETE(pChk);
			}
		}

	//delete m_pChunkSource;
	//delete m_pEmptyChunk;
	//for (int i = 0; i < 32; ++i) {
	//	for (int j = 0; j < 32; ++j) {
	//		delete m_chunkMap[i][j];
	//	}
	//}
}

LevelChunk* ChunkCache::load(int x, int z)
{
    if (!m_pChunkStorage) {
        return m_pEmptyChunk;
    }

    try {
        LevelChunk* chunk = m_pChunkStorage->load(m_pLevel, x, z);
        if (chunk) {
            chunk->field_23C = m_pLevel->getTime();
        }
        return chunk;
    }
    catch (const std::exception& e) {
        e.what();
        return m_pEmptyChunk;
    }
    /*
    
	if (!m_pChunkStorage)
		return m_pEmptyChunk;

	if (x < 0 || z < 0 || x >= C_MAX_CHUNKS_X || z >= C_MAX_CHUNKS_Z)
		return m_pEmptyChunk;

	LevelChunk* pChk = m_pChunkStorage->load(m_pLevel, x, z);
	if (pChk)
		pChk->field_23C = m_pLevel->getTime();
	
	return pChk;*/
}
