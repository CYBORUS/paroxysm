#ifndef TESTMODULE_H
#define TESTMODULE_H

#include <CGE/ManagedModule.h>
#include <CGE/Program.h>
#include <CGE/ClusterVBO.h>
#include <CGE/Matrix4x4.h>
#include <CGE/CubeMap.h>
#include <CGE/Sound.h>
#include <CGE/Music.h>
#include <CGE/LuaMachine.h>

class TestModule : public CGE::ManagedModule
{
    public:
        TestModule();
        virtual ~TestModule();

        virtual void onLoad(CGE::PropertyList& inList);
        virtual void onUnload();
        virtual void onOpen();
        virtual void onClose();
        virtual void onLoop();
        virtual void onPulse();

        virtual void onResize(int inWidth, int inHeight);
        virtual void onMouseWheel(bool inUp, bool inDown);

    private:
        CGE::Program<2> mProgram;
        CGE::Shader mVS;
        CGE::Shader mFS;
        CGE::ClusterVBO<3> mVBO;
        CGE::IndexVBO mIVBO;
        CGE::LuaMachine mLua;

        CGE::CubeMap mCubeMap;
        GLint mUniMVPM;
        GLint mUniMVM;
        GLint mUniNM;
        mat4f mProjection;
        mat4f mModelView;
        mat4f mNormalView;
        float mRotate;

        CGE::Sound mTestSound;
        CGE::Music mMusic;
};

#endif
