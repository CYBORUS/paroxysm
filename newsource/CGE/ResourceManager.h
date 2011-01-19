#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>

namespace CGE
{
    template<typename T>
    class ResourceManager
    {
        public:
            ResourceManager();
            virtual ~ResourceManager();

            T* load(const char* inFile);
            void unload(const char* inFile);

            void unloadAll();

        protected:
            std::map<std::string, T*> mResources;

        private:
    };

    template<typename T>
    ResourceManager<T>::ResourceManager()
    {
    }

    template<typename T>
    ResourceManager<T>::~ResourceManager()
    {
        unloadAll();
    }

    template<typename T>
    T* ResourceManager<T>::load(const char* inFile)
    {
        typename std::map<std::string, T*>::iterator i = mResources.find(inFile);

        if (i == mResources.end())
        {
            mResources[inFile] = new T(inFile);
        }

        return mResources[inFile];
    }

    template<typename T>
    void ResourceManager<T>::unload(const char* inFile)
    {

    }

    template<typename T>
    void ResourceManager<T>::unloadAll()
    {
        typename std::map<std::string, T*>::iterator i = mResources.begin();

        for (; i != mResources.end(); ++i)
        {
            delete i->second;
        }

        mResources.clear();
    }

}

#endif
