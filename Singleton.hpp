

namespace ex6
{
    template<class T>
    class Singleton
    {
        protected:
            T obj;
            static Singleton<T> * instance;
            Singleton<T>(T const & value);
        public:
            Singleton(Singleton & single) = delete;
            void operator=(const Singleton & single) = delete;

            // getters and setters
            T &get_object();
            static Singleton<T> * Instance(T const & value);
            void Destroy();
    };
}






