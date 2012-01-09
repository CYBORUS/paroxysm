#ifndef LUAINDEXEDRESOURCE_H
#define LUAINDEXEDRESOURCE_H

#include <CGE/Reference.h>
#include <vector>
#include <cassert>

template<class T>
class LuaIndexedResource
{
    public:
        LuaIndexedResource();
        ~LuaIndexedResource();

        CGE::Reference<T> get(size_t inIndex);
        size_t add(CGE::Reference<T> inResource);
        CGE::Reference<T> remove(size_t inIndex);
        CGE::Reference<T> first();
        CGE::Reference<T> next();

    protected:
    private:
        std::vector<size_t> mHoles;
        std::vector< CGE::Reference<T> > mResources;
        size_t mIterator;
};

template<class T>
LuaIndexedResource<T>::LuaIndexedResource() : mIterator(0)
{
}

template<class T>
LuaIndexedResource<T>::~LuaIndexedResource()
{
}

template<class T>
CGE::Reference<T> LuaIndexedResource<T>::get(size_t inIndex)
{
    return inIndex < mResources.size() ? mResources[inIndex] : NULL;
}

template<class T>
size_t LuaIndexedResource<T>::add(CGE::Reference<T> inResource)
{
    assert(!inResource.isNull());

    size_t outIndex = mResources.size();

    if (mHoles.size() > 0)
    {
        outIndex = mHoles.back();
        mHoles.pop_back();
        mResources[outIndex] = inResource;
    }
    else
    {
        mResources.push_back(inResource);
    }

    return outIndex;
}

template<class T>
CGE::Reference<T> LuaIndexedResource<T>::remove(size_t inIndex)
{
    CGE::Reference<T> outResource = get(inIndex);

    if (!outResource.isNull())
    {
        mResources[inIndex] = NULL;
        mHoles.push_back(inIndex);
    }

    return outResource;
}

template<class T>
CGE::Reference<T> LuaIndexedResource<T>::first()
{
    mIterator = 0;
    return next();
}

template<class T>
CGE::Reference<T> LuaIndexedResource<T>::next()
{
    CGE::Reference<T> outResource;

    for (size_t i = mIterator; i < mResources.size(); ++i)
    {
        outResource = mResources[i];

        if (!outResource.isNull())
        {
            mIterator = i + 1;
            break;
        }
    }

    return outResource;
}

#endif
