make clean
TARGET=demo.exe CFLAGS="-static-libgcc -static-libstdc++" CC=i686-w64-mingw32-c++ LIBS="-lmingw32 -lglew32s -lopengl32 -lSDL2main -lSDL2 -l3dapp" LFLAGS="-L/cygdrive/f/windev/lib/" INCLUDES="-I/cygdrive/f/windev/include" make -j 6
cp demo.exe .. 
cd ..
./demo.exe
