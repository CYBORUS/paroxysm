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

#include "NetworkStream.h"

#include <iostream>
#include <cstring>
using namespace std;

NetworkStream::NetworkStream()
{
    mPacket = SDLNet_AllocPacket(1024);
}

NetworkStream::~NetworkStream()
{
    SDLNet_FreePacket(mPacket);
}

void NetworkStream::listen(Uint16 inPort)
{
    mSocketIn = SDLNet_UDP_Open(inPort);
    if (!mSocketIn)
    {
        cerr << "failed to listen on port " << inPort << endl;
    }
}

void NetworkStream::connect(const char* inAddress, Uint16 inPort)
{
    mSocketOut = SDLNet_UDP_Open(0);
    if (!mSocketOut)
    {
        cerr << "failed to open a socket out" << endl;
    }

    if (SDLNet_ResolveHost(&mAddress, inAddress, inPort) == -1)
    {
        cerr << "failed to resolve host -- " << inAddress << " on port "
            << inPort << endl;
    }
}

void NetworkStream::sendData(const void* inData, size_t inLength)
{
    memcpy(mPacket->data, inData, inLength);
    mPacket->len = inLength;
    mPacket->address.host = mAddress.host;
    mPacket->address.port = mAddress.port;
    SDLNet_UDP_Send(mSocketOut, -1, mPacket);
}

void NetworkStream::dump()
{
    if (SDLNet_UDP_Recv(mSocketIn, mPacket))
    {
        cout << "UDP packet: channel(" << mPacket->channel
            << ") data(" << (char*)mPacket->data
            << ") length(" << mPacket->len
            << ") max(" << mPacket->maxlen
            << ") \n    status(" << mPacket->status
            << ") address(" << mPacket->address.host
            << ' ' << mPacket->address.port << ')' << endl;
    }
}
