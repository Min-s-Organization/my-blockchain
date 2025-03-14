mkdir build
cd build

# Configure and build
cmake ..
make

# Run tests
../bin/BlockTest
../bin/BlockchainTest