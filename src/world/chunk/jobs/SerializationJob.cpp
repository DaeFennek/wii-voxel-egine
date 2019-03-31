/***
 *
 * Copyright (C) 2018 DaeFennek
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

#include <sstream>
#include "SerializationJob.h"
#include "../../../utils/Filesystem.h"
#include "../../../utils/Debug.h"
#include "../../../event/eventmanager.h"
#include "../../../event/event.h"

void SerializationJob::Execute()
{   
    const CompressedChunkData& chunkData = m_queue.Pop();

    if (chunkData.m_CompressedData[0] != 0x78 ||
            chunkData.m_CompressedData[1] != 0x9c ||
            chunkData.m_CompressedData[2] != 0xed)
    {
        ERROR("SerializationJob: chunk %d %d wrong inflate signature", chunkData.m_X, chunkData.m_Z);
        delete [] chunkData.m_CompressedData;
        return;
    }

    std::ostringstream filename;
    filename << WORLD_PATH "/";
    filename << chunkData.m_X;
    filename << '_';
    filename << chunkData.m_Z;
    filename << ".data";

    std::ofstream stream(filename.str(), std::ios::out | std::ios::binary | std::ios::trunc);
    stream.write((const char*)&chunkData.m_X, sizeof(chunkData.m_X));
    stream.write((const char*)&chunkData.m_Z, sizeof(chunkData.m_Z));
    stream.write((const char*)&chunkData.m_bGroundUpCon, sizeof(chunkData.m_bGroundUpCon));
    stream.write((const char*)&chunkData.m_PrimaryBitMap, sizeof(chunkData.m_PrimaryBitMap));
    stream.write((const char*)&chunkData.m_AddBitMap, sizeof(chunkData.m_AddBitMap));
    stream.write((const char*)&chunkData.m_CompressedSize, sizeof(chunkData.m_CompressedSize));
    stream.write((const char*)chunkData.m_CompressedData, chunkData.m_CompressedSize);
    stream.close();

    delete [] chunkData.m_CompressedData;

    LOG("SerializationJob: Serialize Chunk %d %d", chunkData.m_X, chunkData.m_Z);

    if (m_queue.GetCount() == 0)
        EventManager::Dispatch(EVENT_SERIALIZED_ALL_CHUNKS);
}
