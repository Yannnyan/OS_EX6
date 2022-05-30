

namespace ex6
{
    template<class T>
    class Singleton
    {
        private:
            T obj;
            static Singleton<T> * instance;
            static bool created;
            Singleton<T>();
        public:
            // getters and setters
            T &get_object();
            void set_object(T & value);
            static Singleton<T> * Instance();
            void Destroy();
    };
}






