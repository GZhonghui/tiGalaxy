all: Loader

Loader: Loader.cpp; -@ g++ -fPIC -shared Loader.cpp -o Loader.so --std=c++17

clean: ; -@ rm Loader.so
