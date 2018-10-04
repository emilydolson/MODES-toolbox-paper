// This is the main function for the NATIVE version of this project.

#include <iostream>

#include "base/vector.h"
#include "config/command_line.h"
#include "../nk_oee.h"

int main(int argc, char* argv[])
{
  NKConfig config;
  auto args = emp::cl::ArgManager(argc, argv);
  if (args.ProcessConfigOptions(config, std::cout, "NKConfig.cfg", "NK-macros.h") == false) exit(0);
  if (args.TestUnknown() == false) exit(0);  // If there are leftover args, throw an error.
  emp::Random rnd(config.SEED());
  
  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  config.Write(std::cout);
  std::cout << "==============================\n"
            << std::endl;

  NKWorld world(rnd);
  world.Setup(config);
  world.Run();
}
