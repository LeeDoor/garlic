exe=src/garlic
cd ..
mkdir build
cd build
rm $exe
cmake .. 
cmake --build . -j$(nproc)
