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

#include "GameCamera.h"

GameCamera::GameCamera() : mFollow(NULL)
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::update()
{
    if (mFollow != NULL)
    {
        mPanning = mFollow->getPosition();
    }
}

void GameCamera::transform()
{
    glTranslatef(0.0f, 0.0f, -mTrackball[2]);
    glRotatef(mTrackball[0], 1.0f, 0.0f, 0.0f);
    glRotatef(mTrackball[1], 0.0f, 1.0f, 0.0f);
    glTranslatef(-mPanning[0], -mPanning[1], -mPanning[2]);
}

void GameCamera::follow(Entity* inEntity)
{
    mFollow = inEntity;
}

void GameCamera::setTrackball(const Vector3D<float>& inVector)
{
    mTrackball = inVector;
}

void GameCamera::setPanning(const Vector3D<float>& inVector)
{
    mFollow = NULL;
    mPanning = inVector;
}

void GameCamera::zoom(float inOffset)
{
    mTrackball[2] += inOffset;
    if (mTrackball[2] < 0.0f) mTrackball[2] = 0.0f;
}

void GameCamera::spin(float inOffset)
{
    mTrackball[1] += inOffset;
    if (mTrackball[1] > 180.0f)
        mTrackball[1] -= 360.0f;
    else if (mTrackball[1] < -180.0f)
        mTrackball[1] += 360.0f;
}

void GameCamera::rise(float inOffset)
{
    mTrackball[0] += inOffset;
    if (mTrackball[0] > 90.0f)
        mTrackball[0] = 90.0f;
    else if (mTrackball[0] < 0.0f)
        mTrackball[0] = 0.0f;
}
