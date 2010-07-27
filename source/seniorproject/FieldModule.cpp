#include "FieldModule.h"
#include "ASField.h"

#include <ctime>

FieldModule::FieldModule()
{
}

FieldModule::~FieldModule()
{
}

bool FieldModule::onLoad()
{
    srand(time(NULL));
    mField.createRandom();
    ASField asf(mField);
    asf.findPath(0, 0, mField.width() - 1, mField.height() - 1);
    mPath = asf.getPath();
    mPathSize = asf.getPathSize();
    mCurrentStep = 0;
    mTimer = 0;

    mBall.setScale(0.4f);
    mBall.moveSphere(0.5f, 0.5f, 0.5f);
    mBall.setColor(0.6f, 0.6f, 1.0f);
    mCamera.setTrackball(Vector3D<float>(20.0f, 0.0f, 10.0f));
    mSpin = 0.0f;
    mRise = 0.0f;
    mZoom = 0.0f;
    return true;
}

void FieldModule::onUnload()
{
}

void FieldModule::onOpen()
{
    mRunning = true;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLdouble w = SDL_GetVideoSurface()->w;
    GLdouble h = SDL_GetVideoSurface()->h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, w / h, NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);
}

void FieldModule::onClose()
{
}

void FieldModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    mCamera.transform();
    mField.display();
    mBall.display();
}

void FieldModule::onFrame()
{
    if (mCurrentStep < mPathSize)
    {
        ++mTimer;
        if (mTimer > 10)
        {
            mTimer = 0;

            switch(mPath[mCurrentStep])
            {
                case WallField::NORTH:
                {
                    --mPosition.y;
                    break;
                }

                case WallField::SOUTH:
                {
                    ++mPosition.y;
                    break;
                }

                case WallField::WEST:
                {
                    --mPosition.x;
                    break;
                }

                case WallField::EAST:
                {
                    ++mPosition.x;
                    break;
                }

                default:
                {

                }
            }

            ++mCurrentStep;
        }
    }
    mBall.moveSphere(0.5f + float(mPosition.x), 0.5f,
        0.5f + float(mPosition.y));
    mCamera.zoom(mZoom);
    mCamera.spin(mSpin);
    mCamera.rise(mRise);
    mCamera.setPanning(mBall.getTranslation());
}

void FieldModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        case SDLK_LEFT:
        {
            mSpin = 4.0f;
            break;
        }

        case SDLK_RIGHT:
        {
            mSpin = -4.0f;
            break;
        }

        case SDLK_UP:
        {
            mRise = 2.0f;
            break;
        }

        case SDLK_DOWN:
        {
            mRise = -2.0f;
            break;
        }

        case SDLK_PAGEUP:
        {
            mZoom = -1.0f;
            break;
        }

        case SDLK_PAGEDOWN:
        {
            mZoom = 1.0f;
            break;
        }

//        case SDLK_a:
//        {
//            if (mField.canMove(mPosition.x, mPosition.y, WallField::WEST))
//                mPosition.x -= 1;
//            break;
//        }
//
//        case SDLK_s:
//        {
//            if (mField.canMove(mPosition.x, mPosition.y, WallField::SOUTH))
//                mPosition.y += 1;
//            break;
//        }
//
//        case SDLK_d:
//        {
//            if (mField.canMove(mPosition.x, mPosition.y, WallField::EAST))
//                mPosition.x += 1;
//            break;
//        }
//
//        case SDLK_w:
//        {
//            if (mField.canMove(mPosition.x, mPosition.y, WallField::NORTH))
//                mPosition.y -= 1;
//            break;
//        }

        default:
        {
        }
    }
}

void FieldModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_LEFT:
        case SDLK_RIGHT:
        {
            mSpin = 0.0f;
            break;
        }

        case SDLK_UP:
        case SDLK_DOWN:
        {
            mRise = 0.0f;
            break;
        }

        case SDLK_PAGEUP:
        case SDLK_PAGEDOWN:
        {
            mZoom = 0.0f;
            break;
        }

        default:
        {

        }
    }
}
