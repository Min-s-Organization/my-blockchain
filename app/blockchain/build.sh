mkdir build
cd build

# Configure and build
cmake ..
make

# Run tests
# ../bin/BlockTest
# ../bin/ProtobufTest
# ../bin/LevelDBTest
../bin/BlockchainTest
