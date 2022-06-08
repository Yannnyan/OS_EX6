cc = g++
flags = -pthread -Wall -g -std=c++17
headers = $(wildcard *.hpp)
sources = $(wildcard *.cpp)
objects = $(subst .cpp, .o, $(sources))

all: Server

selectServer: selectserver.o Reactor.o
	$(cc) $^ -o $@ $(flags)

selectClient: selectclient.o
	$(cc) $^ -o $@ $(flags)


Server: ActiveObject.o queue.o Server.o Guard.o
	$(cc) $^ -o $@ $(flags)

Client: selectclient.o Reactor.o
	$(cc) $^ -o $@ $(flags)

testSingleton: testSingleton.o
	$(cc) $^ -o $@ $(flags)

testQueue: testQueue.o queue.o TestRunner.o
	$(cc) $^ -o $@ $(flags)

testAO: testAO.o ActiveObject.o queue.o TestRunner.o 
	$(cc) $^ -o $@ $(flags)

testGuard: testGuard.o Guard.o TestRunner.o
	$(cc) $^ -o $@ $(flags)

%.o: %.cpp $(headers)
	$(cc) -c $< -o $@ $(flags)

.PHONY: clean

clean: 
	rm -f *.o Server testSingleton testQueue Client testAO testGuard selectClient selectServer








