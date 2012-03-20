#include <cstdint>
#include "Individual.h"

/*
 *
 */
Individual::Individual(int size)
{
  assigned = new uint8_t[size];
}

/*
 *
 */
Individual::~Individual()
{
  delete [] assigned;
}
