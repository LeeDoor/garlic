exe=src/batadase
cd ..
mkdir build
cd build
rm $exe
cmake .. 
cmake --build . -j$(nproc)
