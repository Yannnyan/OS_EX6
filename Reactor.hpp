


namespace ex6
{
    class Reactor
    {
        private:
            
        public:
            void * newReactor();
            void InstallHandler(void (*func)(), int fd);
            void RemoveHandler(void * reactor);

    };
}







