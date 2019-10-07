/***
 *
 * Copyright (C) 2016 DaeFennek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
***/

#include <stdint.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <inttypes.h>
#include "GameWorld.h"
#include "Frustrum.h"

#include "../renderer/MasterRenderer.h"
#include "../utils/Debug.h"
#include "../utils/Filesystem.h"
#include "../utils/clock.h"
#include "chunk/Chunk.h"
#include "Camera.h"



GameWorld::GameWorld()
{    
    m_blockManager = new BlockManager();
    m_blockManager->LoadBlocks();    

    //ReadSeed();
}

GameWorld::~GameWorld()
{
	m_blockManager->UnloadBlocks();
	delete m_blockManager;	
}

void GameWorld::GenerateWorld()
{
    auto& playerPosition = static_cast<Basic3DScene*>(Engine::Get().GetSceneHandler().GetCurrentScene())->GetEntityHandler().GetPlayer()->GetPosition();
    m_chunkLoader.Init(playerPosition, this);
}

void GameWorld::Draw()
{
    //Clock clockRender, clockUpdate;
    //clockRender.Start();
    auto& playerPosition = static_cast<Basic3DScene*>(Engine::Get().GetSceneHandler().GetCurrentScene())->GetEntityHandler().GetPlayer()->GetPosition();
    auto& loadedChunks = m_chunkLoader.GetLoadedChunks();
    //Frustrum::Instance().CalculateFrustum();
    for (auto& chunk : loadedChunks)
    {        
        if (chunk->IsDirty())
        {             
            chunk->RebuildDisplayList();            
        }
        chunk->Render();
    }
    //clockRender.Stop();
    //clockUpdate.Start();
    m_chunkLoader.UpdateChunksBy(playerPosition);
    DrawFocusOnSelectedCube();
    //clockUpdate.Stop();
    //if (clockUpdate.GetSecs() > 0.0f || clockRender.GetSecs() > 0.0f)
        //LOG("World Chunk Update: %f s, Render %f s", clockUpdate.GetSecs(), clockRender.GetSecs());
}

BlockManager& GameWorld::GetBlockManager()
{
	return *m_blockManager;
}

Chunk* GameWorld::GetCashedChunkAt(const Vec2i& centerPosition)
{
    return m_chunkLoader.GetChunkFromCash(centerPosition);
}

Chunk* GameWorld::GetCashedChunkByWorldPosition(const Vector3& worldPosition)
{
    return m_chunkLoader.GetCashedChunkByWorldPosition(worldPosition);
}

void GameWorld::RemoveBlockByWorldPosition(const Vector3& blockPosition)
{
    auto pChunk = GetCashedChunkByWorldPosition(blockPosition);
	if ( pChunk )
	{
		pChunk->RemoveBlockByWorldPosition( blockPosition );
	}
}
void GameWorld::AddBlockAtWorldPosition(const Vector3& blockPosition, BlockType type)
{
    auto pChunk = GetCashedChunkByWorldPosition(blockPosition);
	if ( pChunk )
	{
		pChunk->AddBlockByWorldPosition(blockPosition, type );
	}
}

void GameWorld::UpdateFocusedBlockByWorldPosition( const Vector3& blockPosition )
{
    auto pChunk = GetCashedChunkByWorldPosition(blockPosition);
	if ( pChunk )
	{
		m_SelectedBlockPosition = pChunk->GetBlockPositionByWorldPosition( blockPosition );
		m_bHasSelectedBlock = true;
	}
	else
	{
		m_bHasSelectedBlock = false;
	}
}

Vector3 GameWorld::GetBlockPositionByWorldPosition(const Vector3& worldPosition)
{
    auto pChunk = GetCashedChunkByWorldPosition(worldPosition);
	if ( pChunk )
	{
		return pChunk->GetBlockPositionByWorldPosition(worldPosition);
	}

    return Vector3();
}

BlockType GameWorld::GetBlockByWorldPosition(const Vector3& worldPosition)
{
    auto pChunk = GetCashedChunkByWorldPosition(worldPosition);
	if ( pChunk )
	{
		return pChunk->GetBlockTypeByWorldPosition(worldPosition);
	}

	return BlockType::AIR;
}


void GameWorld::DrawFocusOnSelectedCube()
{    
    if (m_bHasSelectedBlock )
	{
        MasterRenderer::SetGraphicsMode(false, false);
        BlockRenderer::DrawFocusOnSelectedCube( m_SelectedBlockPosition, BLOCK_SIZE_HALF );
        MasterRenderer::SetGraphicsMode(true, true);
    }
}

void GameWorld::ReadSeed()
{
    srand(time(nullptr));
    int seed = rand();

    if (FileSystem::FileExist(SEED_FILE))
    {
        std::ifstream file;
        file.open(SEED_FILE);
        if (file.is_open())
        {
            std::string line;
            std::getline(file, line);
            seed = std::atoi(line.c_str());
            file.close();
        }
    }
    else
    {
        std::ofstream stream(SEED_FILE);
        stream << seed << '\n';
        stream.flush();
        stream.close();
    }

    m_noise.Set(.10, .1, .5, 6.0, seed);
}

void GameWorld::Serialize(const CompressedChunkData& data)
{
    m_chunkLoader.Serialize(data);
}

uint32_t GameWorld::GetSerializationQueueCount()
{
    return m_chunkLoader.m_serializationJob.GetQueueCount();
}
