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

#ifndef PACKETHANDSHAKE_H
#define PACKETHANDSHAKE_H

#include <string>
#include "Packet.h"
#include "PacketGlobals.h"
#include "PacketLogin.h"
#include "../utils/Debug.h"
#include "../utils/stringhelper.h"

class PacketHandshake : public Packet
{
public:
    PacketHandshake() : Packet(PACKET_HANDSHAKE) {}
    PacketHandshake(const std::string& playerName, const std::string& ip, uint16_t port ) : Packet(PACKET_HANDSHAKE)
    {
        m_String.clear();
        m_String.append(playerName);
        m_String+= ';';
        m_String.append(ip);
        m_String+= ':';
        m_String.append(ToString<uint16_t>(port));
    }

    void Read(const Session& session) override
    {
        m_String = session.ReadString();
        LOG("Got Server Handshake '%s'", m_String.c_str());
    }

     void Action() const override
     {
         // todo implement
         PacketLogin l("DaeFennek");
         l.Send();
     }

     Packet* CreateInstance() const override
     {
        return new PacketHandshake();
     }

protected:
    void SendContent(const Session& session) const override
    {
        session.Send<int16_t>((int16_t)m_String.length());
        session.SendString(m_String);
    }

private:
    std::string m_String;
};

#endif // PACKETHANDSHAKE_H
