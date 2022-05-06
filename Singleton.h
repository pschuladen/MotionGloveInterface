#ifndef SINGLETON_H
#define SINGLETON_H

/*
 * Prototype definition of singleon.
 * Classes can use this by adding
 * typedef Singleton<Classname> name_to_call ;
 * behind the header. The singleton can then be called/created by using
 * name_to_call::Instance(); see end of mainbackend-header and main.cpp.
*/

#include <assert.h>

template <class T>
class Singleton
{
public:
    static T* Instance()
    {
        if(!m_Instance) m_Instance = new T;
        assert(m_Instance != NULL);
        return m_Instance;
    }

protected:
    Singleton();
    ~Singleton();
private:
    Singleton(Singleton const&);
    Singleton& operator=(Singleton const&);
    static T* m_Instance;
};
template <class T> T* Singleton<T>::m_Instance=NULL;

#endif // SINGLETON_H
