#ifndef MapGenerator_h
#define MapGenerator_h

#include <spellbook/grid/grid.h>

typedef au::grid<2, char> Map;

class MapGenerator
{
public:

    virtual ~MapGenerator() { }

    virtual void generate(Map& map) = 0;

private:
};

#endif

