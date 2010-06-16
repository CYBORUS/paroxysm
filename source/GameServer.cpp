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
#include "NetworkStream.h"

#include <SDL_net.h>
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
    NetworkStream net;
    net.listen(Config::get<Uint16>("server port", 9421));

    while (!stopRequested())
    {
        char* data = (char*)net.receiveData();
        if (data)
        {
            cout << "SERVER -- received message: " << data << endl;
            free(data);
        }

        SDL_Delay(1);
    }
}
