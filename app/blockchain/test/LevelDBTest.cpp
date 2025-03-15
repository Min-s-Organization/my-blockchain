#include "leveldb/db.h"
#include <iostream>

int main()
{
  std::cout << "\nLevelDBTest running..." << std::endl;

  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "../test.db", &db);
  if (!status.ok())
  {
    std::cerr << "Failed to open database: " << status.ToString() << std::endl;
  }
  else
  {
    std::cout << "Database opened successfully" << std::endl;
  }

  delete db;
}