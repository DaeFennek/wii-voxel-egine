#ifndef PACKETATTACHENTITY_H
#define PACKETATTACHENTITY_H

#include "Packet.h"
#include "PacketGlobals.h"

class PacketAttachEntity : public Packet
{
public:
    PacketAttachEntity() : Packet(PACKET_ATTACH_ENTITY) {}

    void Read(const Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_VehicleID = socket.Read<int32_t>();
    }

    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketAttachEntity();
    }

protected:
    void SendContent(const Socket &socket) const override
    {
    }

    int32_t m_EID = 0, m_VehicleID = 0;
};

#endif // PACKETATTACHENTITY_H
