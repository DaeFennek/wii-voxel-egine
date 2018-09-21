#ifndef PACKETENTITYMETADATA_H
#define PACKETENTITYMETADATA_H

#include "Packet.h"
#include "PacketGlobals.h"

class PacketEntityMetadata : public Packet
{
public:
    PacketEntityMetadata() : Packet(PACKET_ENTITY_METADATA) {}

    void Read(const Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        // todo implement metadata
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketEntityMetadata();
    }

protected:
    void SendContent(const Socket &socket) const override
    {
    }

    int32_t m_EID = 0;

};

#endif // PACKETENTITYMETADATA_H
