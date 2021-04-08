/**
 * Main Entry point for all tests (defines catch main, and only one 
 * test file can do it). This file is there to include utility 
 * functions used in all other test files.
 */


 // as far as i can tell this needs to be defined
#define COPASI_MAIN
#include <copasi/copasi.h>
#include <copasi/core/CRootContainer.h>


// now include catch
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <string.h>
#include <cstdlib>

/**
 * Tries to find the test file in the srcdir environment variable.
 *
 * @param fileName the filename relative to this tests srcdir
 *        environment variable.
 *
 * If the filename cannot be found, the test from which this function
 * is called will fail.
 *
 * @return the full path to the test file
 */
std::string getTestFile(const std::string& fileName)
{
  std::stringstream str;
  char* srcDir = getenv("srcdir");

  if (srcDir != NULL) str << srcDir;
  else str << ".";

  str << "/" << fileName;
  std::string fullName = str.str();

  return fullName;
}


int main(int argc, char * argv[])
{
  // setup copasi 
  CRootContainer::init(0, NULL);
 
  // global setup...

  int result = Catch::Session().run(argc, argv);
 
  // global clean-up...

  return result;
}