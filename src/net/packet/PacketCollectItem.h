#ifndef PACKETCOLLECTITEM_H
#define PACKETCOLLECTITEM_H

#include "Packet.h"
#include "PacketGlobals.h"

class PacketCollectItem : public Packet
{
public:
    PacketCollectItem() : Packet(PACKET_COLLECT_ITEM) {}

    void Read(const Socket &socket) override
    {
        m_CollectedID = socket.Read<int32_t>();
        m_CollectorID = socket.Read<int32_t>();
    }
    void Action() override
    {

    }
    Packet* CreateInstance() const override
    {
        return new PacketCollectItem();
    }

protected:
    void SendContent(const Socket &socket) const override
    {

    }

    int32_t m_CollectedID = 0, m_CollectorID = 0;
};

#endif // PACKETCOLLECTITEM_H
