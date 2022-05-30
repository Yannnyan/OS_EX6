#include "Singleton.hpp"

using namespace ex6;

template<typename T>
ex6::Singleton<T> * ex6::Singleton<T>::Instance(T const & value)
{
    if(instance == nullptr)
    {
        instance = &Singleton<T>(value);
    }
    // if an instance already exists then dont care and return the singleton already exists.
    return instance;
}
template<typename T>
void ex6::Singleton<T>::Destroy()
{
    if(instance == nullptr)
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
ex6::Singleton<T>::Singleton(T const & value)
{
    this->obj = value;
    this->instance = this;
}




