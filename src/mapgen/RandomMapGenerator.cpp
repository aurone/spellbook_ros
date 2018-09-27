#include <spellbook/mapgen/RandomMapGenerator.h>

#include <stdio.h>
#include <random>

RandomMapGenerator::RandomMapGenerator()
{

}

RandomMapGenerator::~RandomMapGenerator()
{

}

void RandomMapGenerator::generate(Map& map)
{
    std::random_device rng;

    for (int x = 0; x < map.size(0); ++x) {
        for (int y = 0; y < map.size(1); ++y) {
            if (rng() % 2)
            {
                map(x, y) = 0;
            }
            else {
                map(x, y) = 1;
            }
        }
    }
}
