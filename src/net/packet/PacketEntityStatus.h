#ifndef PACKETENTITYSTATUS_H
#define PACKETENTITYSTATUS_H

#include "Packet.h"
#include "PacketGlobals.h"

class PacketEntityStatus : public Packet
{
public:
    PacketEntityStatus() : Packet(PACKET_ENTITY_STATUS) {}

    void Read(const Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_EntityStatus = socket.Read<char>();
    }

    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketEntityStatus();
    }

protected:
    void SendContent(const Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    char m_EntityStatus = 0;
};

#endif // PACKETENTITYSTATUS_H
