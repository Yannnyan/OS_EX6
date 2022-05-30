#include "Singleton.hpp"

using namespace ex6;

template<typename T>
ex6::Singleton<T> * ex6::Singleton<T>::Instance()
{
    if(!created)
    {
        created = true;
        instance = &Singleton<T>();
    }
    return instance;
}
template<typename T>
void ex6::Singleton<T>::Destroy()
{
    if(!created)
    {
        // throw exception
        throw("cannot destroy singleton that wasn't created.");
    }
    this->created = false;
}
template<typename T>
T & ex6::Singleton<T>::get_object()
{
    return this->obj;
}
template<typename T>
void ex6::Singleton<T>::set_object(T & value)
{
    this->obj = value;
}




