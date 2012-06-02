#include "Entity.h"
#include <CGE/Memory.h>
#include <cassert>

Entity::Entity(const CGE::LuaReference& inLuaTable) : mMass(1.0), mRadius(1.0),
    mMaxSpeed(0.0), mCurrentSpeed(0.0), mLuaTable(inLuaTable),
    mIsBeingDeleted(false), mModelViewProjection(mMatrix)
{
    assert(inLuaTable.isSet());
}

Entity::~Entity()
{
    for (size_t i = 0; i < mActors.size(); ++i)
    {
        ModelActor* a = mActors[i];
        delete a;
    }
}

bool Entity::isInRangeOf(Entity* inEntity)
{
    if (!inEntity) return false;

    double squareTotal = 0.0;
    for (size_t i = 0; i < 3; ++i)
    {
        double d = mPosition[i] - inEntity->mPosition[i];
        squareTotal += d * d;
    }

    double radiusTotal = mRadius + inEntity->mRadius;
    double distance = sqrt(squareTotal);

    return distance < radiusTotal;
}

void Entity::update()
{
    mPosition[0] += mVelocity[0];
    mPosition[1] += mVelocity[1];
    mPosition[2] += mVelocity[2];

    for (size_t i = 0; i < 3; ++i) mTranslation[i] = mPosition[i];

    calculateLocalOrientation();


    mMatrix.loadIdentity();

    mMatrix.translate(float(mPosition[0]), float(mPosition[1]),
        float(mPosition[2]));

    mMatrix.rotateY(mDefaultRotation[1]);
    mMatrix.rotateX(mDefaultRotation[0]);
    mMatrix.rotateZ(mDefaultRotation[2]);
}

/**
*   Moves the entity based on it's local orientation, taking into
*   account mass and friction.
**/
void Entity::addLocalMomentumVector(vec3d inMomentum, bool isSelfPropelled)
{
    calculateLocalOrientation(inMomentum);
    addGlobalMomentumVector(inMomentum);
}

/**
*   Moves the entity ignoring it's local orientation, taking into
*   account mass and friction
**/
void Entity::addGlobalMomentumVector(const vec3d& inMomentum, bool isSelfPropelled)
{
    const vec3d& vec = getMomentum();
    mVelocity[0] = (vec[0] + inMomentum[0]) / getMass();
    mVelocity[1] = (vec[1] + inMomentum[1]) / getMass();
    mVelocity[2] = (vec[2] + inMomentum[2]) / getMass();
}


/**
*   Moves the entity based on it's local orientation, ignoring
*   mass and friction
**/
void Entity::addLocalVelocityVector(vec3d inVelocity, bool isSelfPropelled)
{
    calculateLocalOrientation(inVelocity);
    addGlobalVelocityVector(inVelocity);
}

/**
*   Moves the entity ignoring on it's local orientation, ignoring
*   mass and friction
**/
void Entity::addGlobalVelocityVector(const vec3d& inVelocity, bool isSelfPropelled)
{
    const vec3d& vec = getMomentum();
    mVelocity[0] = (vec[0] + inVelocity[0]);
    mVelocity[1] = (vec[1] + inVelocity[1]);
    mVelocity[2] = (vec[2] + inVelocity[2]);
}

void Entity::impact(const vec3d& inMomentum)
{
    const vec3d& vec = getMomentum();
    mVelocity[0] = (vec[0] + inMomentum[0]) / getMass();
    mVelocity[1] = (vec[1] + inMomentum[1]) / getMass();
    mVelocity[2] = (vec[2] + inMomentum[2]) / getMass();
}

void Entity::setCollisionCallback(lua_State* inState)
{
    assert(inState != NULL);
    mLuaCallback.set(inState);
}

void Entity::onCollision(lua_State* inState, Entity* inEntity)
{
    assert(inState != NULL);
    assert(inEntity != NULL);

    if (mLuaCallback.isSet() && inEntity->mLuaTable.isSet())
    {
        mLuaCallback.get();
        inEntity->mLuaTable.get();
        lua_call(inState, 1, 0);
    }
}

size_t Entity::addActor(ModelActor* inActor)
{
    assert(inActor != NULL);
    mActors.push_back(inActor);
    mModelViewProjection.addChildNode(inActor->modelViewProjectionNode());
    return mActors.size() - 1;
}

size_t Entity::addActor(ModelActor* inActor, size_t inIndex)
{
    assert(inActor != NULL);
    mActors.push_back(inActor);

    CGE::MatrixNode<float>* node = &mModelViewProjection;

    if (inIndex < mActors.size())
    {
        node = &(mActors[inIndex]->modelViewProjectionNode());
    }

    node->addChildNode(inActor->modelViewProjectionNode());

    return mActors.size() - 1;
}

void Entity::rotateActor(size_t inIndex, double inXRotation,
    double inYRotation, double inZRotation)
{
    ModelActor* a = getActor(inIndex);

    if (a)
    {
        a->rotateY(inYRotation);
        a->rotateX(inXRotation);
        a->rotateZ(inZRotation);
    }
}

void Entity::calculateLocalOrientation()
{
    // For now, the first actor added is assumed to be the
    // base direction actor.

    vec3d rotation = mActors[0]->getRotation();

    CGE::Matrix4x4<double> transformation;
    transformation.rotateY(rotation[1]);
    transformation.rotateX(rotation[0]);
    transformation.rotateZ(rotation[2]);

    vec3d initialForward;
    initialForward[2] = -1.0;

    vec3d initialUp;
    initialUp[1] = 1.0; //not really sure this is correct, but testing it

    transformation.transform(initialForward, mForwardDirection);
    transformation.transform(initialUp, mUpDirection);
}

void Entity::calculateLocalOrientation(vec3d& inMomentum)
{
    vec3d rotation = mActors[0]->getRotation();

    CGE::Matrix4x4<double> transformation;
    transformation.rotateY(rotation[1]);
    transformation.rotateX(rotation[0]);
    transformation.rotateZ(rotation[2]);

    vec3d initial = inMomentum;
    transformation.transform(initial, inMomentum);
}

static CGE::Heap EntityHeap("Entity");

void* Entity::operator new(size_t inSize)
{
    return CGE::allocate(inSize, EntityHeap);
}

void Entity::operator delete(void* inData)
{
    CGE::release(inData);
}

ModelActor* Entity::popActor()
{
    ModelActor* outActor = NULL;

    if (mActors.size() > 0)
    {
        outActor = mActors.back();
        mActors.pop_back();
    }

    return outActor;
}
