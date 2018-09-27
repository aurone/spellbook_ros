#include <spellbook/mapgen/DFSMazeGenerator.h>

#include <assert.h>
#include <algorithm>

DFSMazeGenerator::DFSMazeGenerator() :
    m_rng(),
    m_hall_girth(),
    m_wall_girth()
{
    setHallGirth(1);
    setWallGirth(1);
}

DFSMazeGenerator::DFSMazeGenerator(int hall_girth, int wall_girth) :
    m_rng(),
    m_hall_girth(),
    m_wall_girth()
{
    setHallGirth(hall_girth);
    setWallGirth(wall_girth);
}

DFSMazeGenerator::~DFSMazeGenerator()
{

}

void DFSMazeGenerator::setHallGirth(int hall_girth)
{
    m_hall_girth = std::max(1, hall_girth);
}

void DFSMazeGenerator::setWallGirth(int wall_girth)
{
    m_wall_girth = std::max(1, wall_girth);
}

void DFSMazeGenerator::generate(Map& map)
{
    const int cell_girth = m_hall_girth + m_wall_girth;

    const int cx = map.size(0) / cell_girth;
    const int cy = map.size(1) / cell_girth;

    // initialize cell grid
    au::grid<2, Cell> cells(cx, cy);
    for (int x = 0; x < cells.size(0); ++x) {
        for (int y = 0; y < cells.size(1); ++y) {
            cells(x, y) = Cell(x, y);
        }
    }

    int mid_x = cells.size(0) >> 1;
    int mid_y = cells.size(1) >> 1;
    Cell* start = &cells(mid_x, mid_y);

    mazegen_dfs(cells, start);

    for (size_t x = 0; x < map.size(0); ++x) {
        for (size_t y = 0; y < map.size(1); ++y) {
            const int coff_x = x - cell_girth * (x / cell_girth);
            const int coff_y = y - cell_girth * (y / cell_girth);

            if (coff_x < m_hall_girth && coff_y < m_hall_girth) {
                map(x, y) = 0;
            }
            else {
                map(x, y) = 1;
            }
        }
    }

    for (size_t x = 0; x < cells.size(0); ++x) {
        for (size_t y = 0; y < cells.size(1); ++y) {
            const Cell& curr_cell = cells(x, y);

            assert(curr_cell.parent == nullptr ||
                    (
                        curr_cell.parent >= cells.data() &&
                        curr_cell.parent < cells.data() + sizeof(Cell) * cells.total_size()
                    ));

            const size_t curr_map_x = curr_cell.x * cell_girth;
            const size_t curr_map_y = curr_cell.y * cell_girth;

            if (curr_cell.parent) {
                const Cell& parent_cell = *curr_cell.parent;
                const size_t parent_map_x = parent_cell.x * cell_girth;
                const size_t parent_map_y = parent_cell.y * cell_girth;

                int clear_from_x, clear_from_y, clear_to_x, clear_to_y;
                if (parent_cell.x == curr_cell.x - 1) {
                    // parent to the left
                    clear_from_x    = parent_map_x + m_hall_girth;
                    clear_to_x      = curr_map_x;
                    clear_from_y    = curr_map_y;
                    clear_to_y      = curr_map_y + m_hall_girth;
                }
                else if (parent_cell.x == curr_cell.x + 1) {
                    // parent to the right
                    clear_from_x    = curr_map_x + m_hall_girth;
                    clear_to_x      = parent_map_x;
                    clear_from_y    = curr_map_y;
                    clear_to_y      = curr_map_y + m_hall_girth;
                }
                else if (parent_cell.y == curr_cell.y - 1) {
                    // parent up top
                    clear_from_x    = curr_map_x;
                    clear_to_x      = curr_map_x + m_hall_girth;
                    clear_from_y    = parent_map_y + m_hall_girth;
                    clear_to_y      = curr_map_y;
                }
                else {
                    assert(parent_cell.y == curr_cell.y + 1);
                    // parent down low
                    clear_from_x    = curr_map_x;
                    clear_to_x      = curr_map_x + m_hall_girth;
                    clear_from_y    = curr_map_y + m_hall_girth;
                    clear_to_y      = parent_map_y;
                }

                // clear wall
                for (int cx = clear_from_x; cx < clear_to_x; ++cx) {
                    for (int cy = clear_from_y; cy < clear_to_y; ++cy) {
                        map(cx, cy) = 0;
                    }
                }
            }
        }
    }
}

void DFSMazeGenerator::mazegen_dfs(au::grid<2, Cell>& grid, Cell* c)
{
    c->visited = true;

    const int width = grid.size(0);
    const int height = grid.size(1);

    Cell* neighbor_left     = c->x > 0 ? &grid(c->x - 1, c->y) : nullptr;
    Cell* neighbor_right    = c->x < width - 1 ? &grid(c->x + 1, c->y) : nullptr;
    Cell* neighbor_up       = c->y < height - 1 ? &grid(c->x, c->y + 1) : nullptr;
    Cell* neighbor_down     = c->y > 0 ? &grid(c->x, c->y - 1) : nullptr;

    assert(neighbor_left == nullptr ||
            neighbor_left >= grid.data() &&
            neighbor_left < grid.data() + sizeof(Cell) * grid.total_size());
    assert(neighbor_right == nullptr ||
            neighbor_right >= grid.data() &&
            neighbor_right < grid.data() + sizeof(Cell) * grid.total_size());
    assert(neighbor_up == nullptr ||
            neighbor_up >= grid.data() &&
            neighbor_up < grid.data() + sizeof(Cell) * grid.total_size());
    assert(neighbor_down == nullptr ||
            neighbor_down >= grid.data() &&
            neighbor_down < grid.data() + sizeof(Cell) * grid.total_size());

    std::vector<Cell*> neighbors;
    if (neighbor_left) {
        neighbors.push_back(neighbor_left);
    }

    if (neighbor_right) {
        neighbors.push_back(neighbor_right);
    }

    if (neighbor_up) {
        neighbors.push_back(neighbor_up);
    }

    if (neighbor_down) {
        neighbors.push_back(neighbor_down);
    }

    std::shuffle(neighbors.begin(), neighbors.end(), m_rng);

    for (Cell* neighbor : neighbors) {
        if (!neighbor->visited) {
            neighbor->parent = c;
            mazegen_dfs(grid, neighbor);
        }
    }
}

