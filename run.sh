mkdir build
cd build
exe=src/batadase
rm $exe
cmake ..
cmake --build . -j4
./$exe
