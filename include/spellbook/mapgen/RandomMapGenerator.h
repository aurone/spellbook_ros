#ifndef RandomMapGenerator_h
#define RandomMapGenerator_h

#include "MapGenerator.h"

class RandomMapGenerator
{
public:

    RandomMapGenerator();

    virtual ~RandomMapGenerator();

    virtual void generate(Map& map);
};

#endif
