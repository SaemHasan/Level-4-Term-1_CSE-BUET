#compile
echo "Compiling..."
g++ -o offline Offline.cpp
#run upto stage 3
echo "Running upto stage 3"
./offline "input/scene.txt"
#compile stage 4 using z buffer algo
echo "Compiling stage 4"
g++ -o zbuffer zBufferAlgo.cpp
#run stage 4
echo "Running stage 4"
./zbuffer "output/stage3.txt" "input/config.txt"