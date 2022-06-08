

namespace ex6
{
    void * instance;
    template<typename T>
    class Singleton
    {
        protected:
            T obj;
            Singleton<T>(T const & value)
            {
                this->obj = value;
            }
        public:
            Singleton(Singleton & single) = delete;
            void operator=(const Singleton & single) = delete;

            // getters and setters
            T & get_object()
            {
                return this->obj;
            }
            static Singleton<T> * Instance(T const & value)
                {
                    if(instance == nullptr)
                    {
                        instance = (void *)new Singleton<T>(value);
                    }
                    // if an instance already exists then dont care and return the singleton already exists.
                    return (Singleton<T> *)instance;
                }
            void Destroy()
            {
                if(instance == nullptr)
                {
                    // throw exception
                    throw("cannot destroy singleton that wasn't created.");
                }
                delete((Singleton<T> *)instance);
            }
    };
}





