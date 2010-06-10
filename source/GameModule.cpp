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

#include "GameModule.h"
#include "MathEngine.h"
#include "RobotControl.h"

GameCamera* GameModule::luaCamera = NULL;
vector<Tank*>* GameModule::luaTanks = NULL;
GameModule* GameModule::luaGM = NULL;
TerrainGrid* GameModule::luaTG = NULL;

GameModule::GameModule(const char* inMapFile) : mSun(4), mMoon(4)
{
    mNumTanks = 0;
    string inFile(Config::getUserFolder());
    inFile += "maps/";
    inFile += inMapFile;
    mSunRotation = 0;
    mMoonRotation = 0;

    //ModelStack* test = ModelStack::load("cube.3ds");

    ifstream input;
    input.clear();
    input.open(inFile.c_str());

    if (!input.fail())
    {
        input >> mTerrain;
        mTerrainSize.x = mTerrain.getMatrix().cols();
        mTerrainSize.y = mTerrain.getMatrix().rows();

        mTerrain.showWall();

        input.close();
    }
    else
    {
        if ((input.rdstate() & ifstream::failbit) != 0)
        {
            cerr << "error opening file: " << inFile << endl;
        }
        else
        {
            cerr << "badbit flipped." << endl;
        }
        exit(3);
    }

}

GameModule::~GameModule()
{
}

bool GameModule::onLoad()
{
    mTestModel = ModelStack::load("bradley.c3m");
    mProjection = Matrix<GLdouble>(4);
    mModelView = Matrix<GLdouble>(4);

    mTrackball[0] = 22.0f;
    mTrackball[2] = 20.0f;
    mPanning[0] = static_cast<GLfloat>(mTerrainSize.x) / -2.0f;
    mPanning[2] = static_cast<GLfloat>(mTerrainSize.y) / -2.0f;

    mDisplay.x = SDL_GetVideoSurface()->w;
    mCenter.x = mDisplay.x / 2;
    mDisplay.y = SDL_GetVideoSurface()->h;
    mCenter.y = mDisplay.y / 2;

    mHUD.setDisplay(mDisplay);

    mLuaConsole = new TextBox;
    mLuaConsole->setLocation(0.0f, -6.0f);
    mLuaConsole->setSize(12.0f, 1.0f);
    mLuaConsole->setTextColor(Vector3D<float>(0.0f, 0.5f, 1.0f));
    mLuaConsole->hideOnEnter(true);
    mLuaConsole->setVisible(false);
    mHUD.addWidget(mLuaConsole);

    mLua.addFunction("cameraPan", luaCameraPan);
    mLua.addFunction("addTank", luaAddTank);
    mLua.addFunction("getHeight", luaGetHeight);
    mLua.addFunction("setFriction", luaSetFriction);

    mFPSLabel = new Label("0", FPS);
    mFPSLabel->setFontColor(0.0f, 0.6f, 0.8f, 1.0f);
    mFPSLabel->setFontSize(24);
    mFPSLabel->setLocation(6.0f, -6.0f);
    mFPSLabel->setSize(3.0f, 1.0f);

    mHUD.addWidget(mFPSLabel);

    //mEntityLock = SDL_CreateMutex();


    CollisionEngine::onSetup();
    EntityGarbageCollector::onSetup();

    addTank(PLAYER_TANK);
    addTank(ROBOT_TANK);

    mCamera.setTrackball(mTrackball);
    mCamera.follow(mPlayerTank);

    Bullet::setSphere(&mBullet);

    return true;
}


void GameModule::onOpen()
{
    mRunning = true;
    mDead = true;
    mNextModule = NULL;
    mMouseMode = MM_DEFAULT;
    luaCamera = &mCamera;
    //luaTanks = &mTanks;
    luaTG = &mTerrain;
    luaGM = this;
    mSceneChanged = true;

    SoundEngine::loadBackgroundMusic("portal_still_alive.wav");

    glViewport(0, 0, (GLsizei)mDisplay.x, (GLsizei)mDisplay.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, (GLdouble)mDisplay.x / (GLdouble)mDisplay.y,
        NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);

    glGetDoublev(GL_PROJECTION_MATRIX, mProjection.array());
    glGetIntegerv(GL_VIEWPORT, mViewport.array());

    glEnable(GL_RESCALE_NORMAL_EXT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    mSunLight.diffuse.set(1.0f, 1.0f, 0.7f);
    //mSunLight.direction[1] = -1.0f;
    mSunLight.position[0] = mTerrainSize.x / 2.0f;
    mSunLight.position[1] = mTerrainSize.y / 2.0f + 20.0f;
    mSunLight.position[2] = mTerrainSize.y / 2.0f;
    mSunLight.position[3] = 1.0f; // distant light source

    mMoonLight.diffuse.set(0.5f, 0.5f, 0.7f);
    mMoonLight.position[0] = mTerrainSize.x / 2.0f;
    mMoonLight.position[1] = -mSunLight.position[1];
    mMoonLight.position[2] = mTerrainSize.y / 2.0f;
    mMoonLight.position[3] = 1.0f;

    //mSun.moveSphere(mSunLight.position[0], mSunLight.position[1], mSunLight.position[2]);
    mSun.setColor(1.0f, 1.0f, 0.0f);
    mSun.setScale(10.0f, 10.0f, 10.0f);
    mMoon.setColor(0.8f, 0.8f, 1.0f);
    mMoon.setScale(5.0f, 5.0f, 5.0f);

    mSunLight.ambient.set(0.1f);

    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    mLights[0] = false;
    mLights[1] = false;
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mSunLight.ambient.array());
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //start the collision engine and the entity garbage collector
    //CollisionEngine::checkCollisions();
    mCollisionThread = SDL_CreateThread(CollisionEngine::checkCollisions, NULL);
    mEntityGarbageCollectorThread = SDL_CreateThread(
        EntityGarbageCollector::runGarbageCollection, NULL);
    mTimes = 0;
}


void GameModule::onLoop()
{
    //cerr << "onLoop...";
    //CollisionEngine::checkCollisions();
    //mCollisionThread = SDL_CreateThread(CollisionEngine::checkCollisions, NULL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    mCamera.transform();

    if (mSceneChanged)
    {
        glGetDoublev(GL_MODELVIEW_MATRIX, mModelView.array());
        mSceneChanged = false;
    }

    glPushMatrix();
    {
        glTranslatef(mSunLight.position[0], 0.0f, mSunLight.position[2]);
        glRotatef(mSunRotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, mSunLight.position[1], 0.0f);

        if (mSunRotation <= 100.0f || mSunRotation >= 260.0f)
        {
            if (!mLights[0])
            {
                mLights[0] = true;
                glEnable(GL_LIGHT0);
            }
            //glLightfv(GL_LIGHT0, GL_AMBIENT, mSunLight.ambient.array());
            glLightfv(GL_LIGHT0, GL_DIFFUSE, mSunLight.diffuse.array());
            glLightfv(GL_LIGHT0, GL_SPECULAR, mSunLight.specular.array());
            glLightfv(GL_LIGHT0, GL_POSITION, Vector3D<float>().array());
        }
        else
        {
            if (mLights[0])
            {
                mLights[0] = false;
                glDisable(GL_LIGHT0);
            }
        }


        glPushAttrib(GL_LIGHTING_BIT);
        {
            glDisable(GL_LIGHTING);
            //glScalef(10.0f, 10.0f, 10.0f);
            mSun.display();

        }
        glPopAttrib();

    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(mMoonLight.position[0], 0.0f, mMoonLight.position[2]);
        glRotatef(mSunRotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, mMoonLight.position[1], 0.0f);

        if (mSunRotation > 80.0f && mSunRotation < 280.0f)
        {
            if (!mLights[1])
            {
                mLights[1] = true;
                glEnable(GL_LIGHT1);
            }
            glLightfv(GL_LIGHT1, GL_DIFFUSE, mMoonLight.diffuse.array());
            glLightfv(GL_LIGHT1, GL_SPECULAR, mMoonLight.specular.array());
            glLightfv(GL_LIGHT1, GL_POSITION, Vector3D<float>().array());
        }
        else
        {
            if (mLights[1])
            {
                mLights[1] = false;
                glDisable(GL_LIGHT1);
            }
        }

        glPushAttrib(GL_LIGHTING_BIT);
        {
            glDisable(GL_LIGHTING);
            //glScalef(10.0f, 10.0f, 10.0f);
            mMoon.display();
        }
        glPopAttrib();

    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(1, 1, 1);
        mTestModel->display();
    }
    glPopMatrix();

    mTerrain.display();

    list<Entity*>::iterator itEntities = mEntities.begin();
    while (itEntities != mEntities.end())
    {
        (*itEntities)->display();
        ++itEntities;
    }

    mHUD.display();
}

void GameModule::onFrame()
{
    //cerr << "onFrame...";
    //cerr << "updating camera...";
    mCamera.update();
    //cerr << "changing scene...";
    mSceneChanged = true;
    //cerr << "camera updated...";
    mSunRotation += 0.01f;
    mMoonRotation += 0.01f;
    if (mSunRotation > 360.0f)
    {
        mSunRotation -= 360.0f;
    }

    if (mMoonRotation > 360.0f)
    {
        mMoonRotation -= 360.0f;
    }
    //cerr << "rotations done...";
    map<Entity*, Control*>::iterator itControls = mControls.begin();
    for (; itControls != mControls.end(); ++itControls)
    {
        itControls->second->update();
    }

    list<Entity*>::iterator itEntities = mEntities.begin();

    while (itEntities != mEntities.end())
    {
        (*itEntities)->move();

        if (!(*itEntities)->isAlive())
        {
            if (mControls.find((*itEntities)) != mControls.end())
            {
                Control* thisControl = mControls[(*itEntities)];
                mControls.erase((*itEntities));
                //mControls[(*itEntities)]->setGameDead();
                thisControl->setGameDead();
            }
            Entity* thisEntity = *itEntities;
            itEntities = mEntities.erase(itEntities);
            //(*itEntities)->setRenderDead();
            thisEntity->setRenderDead();
        }
        else
        {
            ++itEntities;
        }

    }

    //cerr << "onframe lock...";
    /*
    SDL_mutexP(mEntityLock);
    ++mTimes;
    list<Entity*>::iterator itEntities = mEntities.begin();

    while (itEntities != mEntities.end())
    {
        (*itEntities)->move();

        if (!(*itEntities)->isAlive())
        {
            //CollisionEngine::removeEntity(*itEntities);

            if ((*itEntities)->getWhatIAm() == E_TANK)
            {
                Control* control = mControls[(Tank*)*itEntities];
                mControls.erase((Tank*)*itEntities);
                delete control;
                --mNumTanks;
            }

            delete *itEntities;
            itEntities = mEntities.erase(itEntities);
        }
        else
        {
            ++itEntities;
        }
    }
    //cerr << "onframe release" << endl;
    SDL_mutexV(mEntityLock);
    */

    if (mLuaConsole->isLockedIn())
    {
        mLua.runCommand(mLuaConsole->getText().c_str());
    }

    if (mFPS != DisplayEngine::mFPS)
    {
        mFPS = DisplayEngine::mFPS;
        stringstream s;
        s << mFPS << " FPS";
        mFPSLabel->setText(s.str().c_str());
    }
    //cerr << "done." << endl;
}

void GameModule::onUnload()
{
    cerr << "mTimes: " << mTimes << " collisionEngine: " << CollisionEngine::mTimes << endl;
    ModelStack::unloadAll();

/*
    map<Entity*, Control*>::iterator itControls = mControls.begin();

    for (; itControls != mControls.end(); ++itControls)
    {
        delete itControls->second;
    }

    list<Entity*>::iterator itEntities = mEntities.begin();
    for (; itEntities != mEntities.end(); ++itEntities)
    {
        delete *itEntities;
    }
*/

    mEntities.clear();
    mControls.clear();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_RESCALE_NORMAL_EXT);

    //SDL_DestroyMutex(mEntityLock);

    CollisionEngine::mCollisionsRunning = false;
    SDL_WaitThread(mCollisionThread, NULL);
    CollisionEngine::onUnload();
    EntityGarbageCollector::mGameRunning = false;
    SDL_WaitThread(mEntityGarbageCollectorThread, NULL);
    EntityGarbageCollector::onUnload();
}

void GameModule::onClose()
{
    //tell the collision engine to stop
    CollisionEngine::mCollisionsRunning = false;
    EntityGarbageCollector::mGameRunning = false;
    //EntityGarbageCollector::onUnload();
    //SDL_WaitThread(mEntityGarbageCollectorThread, NULL);

}

void GameModule::addTank(ControlType inControlType,
    const Vector3D<float>& inPosition)
{
    Tank* tank = new Tank(&mTerrain);
    tank->setPosition(inPosition);
    //CollisionEngine::addEntity(tank);

    Control* controls = NULL;

    //SDL_mutexP(mEntityLock);
    //mEntities.push_back(tank);
    //EntityGarbageCollector::addEntity(tank);
    //CollisionEngine::addEntity(tank);
    addEntity(tank);
    //SDL_mutexV(mEntityLock);

    if (inControlType != NO_CONTROLS)
    {

        switch (inControlType)
        {
            case PLAYER_TANK:
            {
                controls = new PlayerControl(tank);
                mPlayerTank = tank;
                mPlayerControls = controls;
                break;
            }

            case ROBOT_TANK:
            {
                controls = new RobotControl(tank);
                break;
            }

            default:
            {
            }
        }

        //mControls.push_back(controls);
        mControls[tank] = controls;
        EntityGarbageCollector::addControl(tank, controls);
    }
    ++mNumTanks;
}

void GameModule::addEntity(Entity* inEntity)
{
    mEntities.push_back(inEntity);
    EntityGarbageCollector::addEntity(inEntity);
    CollisionEngine::addEntity(inEntity);
}

Vector3D<float> GameModule::findMouseObjectPoint(int inX, int inY)
{

    GLdouble tempX = 0;
    GLdouble tempY = 0;
    GLdouble tempZ = 0;

    GLfloat depthZ = 0;

    //we have to invert the y axis because of opengl's viewport
    int newY = SDL_GetVideoSurface()->h - inY;

    //read the depth buffer to determine the z coordinate at the input
    //x,y coordinates
    glReadPixels(inX, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthZ);

    //now let the glu library do the math for us :)
    if (gluUnProject((GLdouble)inX, (GLdouble)newY, depthZ, mModelView.array(), mProjection.array(), mViewport.array(), &tempX, &tempY, &tempZ) == GL_FALSE)
    {
        cerr << "gluUnProject failed." << endl;
    }

    Vector3D<float> currentVertex((float)tempX, (float)tempY, (float)tempZ);
    return currentVertex;
}


void GameModule::onLButtonDown(int inX, int inY)
{
    Bullet* bullet = new Bullet(&mTerrain, mPlayerTank->getBulletStart(), mPlayerTank->getBulletDirection(), mPlayerTank->getBulletRotation());
    //mEntities.push_back(bullet);
    addEntity(bullet);
    //CollisionEngine::addEntity(bullet);
}


void GameModule::onLButtonUp(int inX, int inY)
{
}


void GameModule::onRButtonDown(int inX, int inY)
{
    if (mMouseMode == MM_DEFAULT)
    {
        SDL_ShowCursor(SDL_DISABLE);
        mOldMouse.x = inX;
        mOldMouse.y = inY;
        SDL_WarpMouse(mCenter.x, mCenter.y);
        mMouseMode = MM_ROTATING;
    }
}

void GameModule::onRButtonUp(int inX, int inY)
{
    if (mMouseMode == MM_ROTATING)
    {
        mMouseMode = MM_DEFAULT;
        SDL_WarpMouse(mOldMouse.x, mOldMouse.y);
        SDL_ShowCursor(SDL_ENABLE);
        mSceneChanged = true;
    }
}

void GameModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    bool lockMouse = false;

    switch (mMouseMode)
    {
        case MM_ROTATING:
        case MM_PANNING:
        {
            if (abs(inX - mCenter.x) > 100 || abs(inY - mCenter.y) > 100)
            {
                return;
            }
            break;
        }
    }

    switch (mMouseMode)
    {
        case MM_ROTATING:
        {
            lockMouse = true;

            mTrackball[1] += static_cast<GLfloat>(inX - mCenter.x) * TRACKBALL_STEP;
            if (mTrackball[1] < -180.0f)
                mTrackball[1] += 360.0f;
            else if (mTrackball[1] > 180.0f)
                mTrackball[1] -= 360.0f;

            mTrackball[0] += static_cast<GLfloat>(inY - mCenter.y) * TRACKBALL_STEP;
            if (mTrackball[0] < 0.0f)
                mTrackball[0] = 0.0f;
            else if (mTrackball[0] > 180.0f)
                mTrackball[0] = 180.0f;

            mCamera.setTrackball(mTrackball);
            break;
        }
        case MM_PANNING:
        {
            lockMouse = true;
            GLfloat dx = static_cast<GLfloat>(inX - mCenter.x) * PANNING_STEP;
            GLfloat dy = static_cast<GLfloat>(inY - mCenter.y) * PANNING_STEP;

            GLfloat theta = TO_RADIANS(mTrackball[1]);
            GLfloat dxp = cos(theta) * dx;
            GLfloat dyp = sin(theta) * dx;
            dxp -= sin(theta) * dy;
            dyp += cos(theta) * dy;

            mPanning[0] += dxp;
            mPanning[2] += dyp;
            break;
        }
        case MM_DEFAULT:
        {

            Vector3D<float> hoverPoint = findMouseObjectPoint(inX, inY);
            //cerr << "hoverPoint: " << hoverPoint << endl;
            //mSphere.moveSphere(hoverVertex[0], hoverVertex[1], hoverVertex[2]);
            Vector3D<float> relative;
            Vector3D<float> tankPos = mPlayerTank->getPosition();
            relative[0] = hoverPoint[0] - tankPos[0];
            relative[2] = hoverPoint[2] - tankPos[2];
            float angle = -atan2(relative[2], relative[0]) + PI_HALVES;

            ((Tank*)mPlayerTank)->setTurretDirection(TO_DEGREES(angle));

            mHUD.setStates(inX, inY, false);
            break;
        }
    }

    if (lockMouse && (inX != mCenter.x || inY != mCenter.y))
    {
        SDL_WarpMouse(mCenter.x, mCenter.y);
    }
}

void GameModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_RETURN:
        {
            mLuaConsole->setText("");
            mLuaConsole->setVisible(true);
            mHUD.setFocus(mLuaConsole);
            break;
        }

        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        case SDLK_a:
        {
            mPlayerControls->changeDirection(1.0f);
            break;
        }

        case SDLK_w:
        {
            mPlayerControls->changeSpeed(1.0f);
            break;
        }

        case SDLK_d:
        {
            mPlayerControls->changeDirection(-1.0f);
            break;
        }

        case SDLK_s:
        {
            mPlayerControls->changeSpeed(-1.0f);
            break;
        }

        case SDLK_n:
        {
            cerr << "Total number of tanks currently on map: " << mNumTanks << endl;
            break;
        }

        case SDLK_k:
        {

            break;
        }

        case SDLK_q:
        {
            mPlayerTank->modifyTurretRotation(true, 1.0f);
            break;
        }

        case SDLK_e:
        {
            mPlayerTank->modifyTurretRotation(true, -1.0f);
            break;
        }

        default:
        {
        }
    }
    mSceneChanged = true;
}

void GameModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch(inSym)
    {
        case SDLK_a:
        case SDLK_d:
        {
            mPlayerControls->changeDirection(0.0f);
            break;
        }
        case SDLK_w:
        case SDLK_s:
        {
            mPlayerControls->changeSpeed(0.0f);
            break;
        }

        case SDLK_q:
        {
            mPlayerTank->modifyTurretRotation(false, 0.0f);
            break;
        }

        case SDLK_e:
        {
            mPlayerTank->modifyTurretRotation(false, 0.0f);
            break;
        }

        default:
        {
        }
    }
}


void GameModule::onMouseWheel(bool inUp, bool inDown)
{
    if (inUp)
        mTrackball[2] -= TRACKBALL_STEP;
    else if (inDown)
        mTrackball[2] += TRACKBALL_STEP;

    if (mTrackball[2] < 0.0f) mTrackball[2] = 0.0f;

    mCamera.setTrackball(mTrackball);
}

void GameModule::getHeight(float inX, float inZ)
{
    cerr << "getHeight at " << inX << ", " << inZ << ": "
        << mTerrain.findHeight(inX, inZ) << endl;
}

int GameModule::luaCameraPan(lua_State* inState)
{
    int outSuccess = 1;
    int argc = lua_gettop(inState);

    if (argc == 0)
    {
        luaCamera->follow(NULL);
    }
    else if (argc == 1)
    {
        unsigned int index = (unsigned int)lua_tonumber(inState, 1);
        if (index >= luaTanks->size())
        {
            outSuccess = 0;
        }
        else
        {
            luaCamera->follow(luaTanks[0][index]);
        }
    }
    else if (argc > 1)
    {
        Vector3D<float> p(lua_tonumber(inState, 1), 0.0f,
            lua_tonumber(inState, 2));
        luaCamera->setPanning(p);
    }
    else
    {
        outSuccess = 0;
    }

    lua_pushnumber(inState, outSuccess);
    return 1;
}

int GameModule::luaAddTank(lua_State* inState)
{
    int outSuccess = 1;
    int argc = lua_gettop(inState);

    int num = argc > 0 ? (int)lua_tonumber(inState, 1) : 1;
    if (num < 1) num = 1;
    for (int i = 0; i < num; ++i)
    {
        Vector3D<float> pos;
        pos[0] = MathEngine::supremeRandom<float>(1.0f,
            luaTG->getMatrix().lastCol());
        pos[2] = MathEngine::supremeRandom<float>(1.0f,
            luaTG->getMatrix().lastRow());
        luaGM->addTank(ROBOT_TANK, pos);
    }

    lua_pushnumber(inState, outSuccess);
    return 1;
}

int GameModule::luaGetHeight(lua_State* inState)
{
    int outSuccess = 1;
    int argc = lua_gettop(inState);

    if (argc < 2)
    {
        outSuccess = 0;
    }
    else
    {
        float x = lua_tonumber(inState, 1);
        float z = lua_tonumber(inState, 2);
        luaGM->getHeight(x, z);
    }

    lua_pushnumber(inState, outSuccess);
    return 1;
}

int GameModule::luaSetFriction(lua_State* inState)
{
    int outSuccess = 1;
    int argc = lua_gettop(inState);

    if (argc < 1)
    {
        outSuccess = 0;
    }
    else
    {
        float f = lua_tonumber(inState, 1);
        luaTG->setFriction(f);
    }

    lua_pushnumber(inState, outSuccess);
    return 1;
}
