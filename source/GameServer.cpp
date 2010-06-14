/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameServer.h"
#include "Config.h"

#include <iostream>
using namespace std;

GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}

void GameServer::run()
{
    Uint16 port = Config::get<Uint16>("server port", 9421);
    mSocket = SDLNet_UDP_Open(port);
    if (!mSocket)
    {
        cerr << "SDLNet_UDP_Open failed on port " << port << endl;
        return;
    }

    int packetSize = Config::get<int>("server packet size", 512);
    mPacket = SDLNet_AllocPacket(packetSize);

    while (!stopRequested())
    {
        if (SDLNet_UDP_Recv(mSocket, mPacket))
        {
            cout << "UDP packet: channel(" << mPacket->channel
                << ") data(" << (char*)mPacket->data
                << ") length(" << mPacket->len
                << ") max(" << mPacket->maxlen
                << ") status(" << mPacket->status
                << ") address(" << mPacket->address.host
                << ' ' << mPacket->address.port << ')' << endl;
        }
    }

    SDLNet_FreePacket(mPacket);
}
