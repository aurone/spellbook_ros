#include "MapGenerator.h"

#include <random>

class DFSMazeGenerator : public MapGenerator
{
public:

    DFSMazeGenerator();
    DFSMazeGenerator(int hall_girth, int wall_girth);

    virtual ~DFSMazeGenerator();

    void setHallGirth(int hall_girth);
    int hallGirth() const { return m_hall_girth; }

    void setWallGirth(int wall_girth);
    int wallGirth() const { return m_wall_girth; }

    void generate(Map& map);

private:

    struct Cell
    {
        int x;
        int y;
        bool visited;
        Cell* parent;

        Cell() : x(), y(), visited(false), parent(nullptr) { }
        Cell(int x, int y) : x(x), y(y), visited(false), parent(nullptr) { }
    };

    std::random_device m_rng;

    int m_hall_girth;
    int m_wall_girth;

    void mazegen_dfs(au::grid<2, Cell>& grid, Cell* c);
};
