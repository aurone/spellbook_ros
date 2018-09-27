#ifndef au_simple_adjacency_list_h
#define au_simple_adjacency_list_h

#include <list>
#include <utility>
#include <ostream>

namespace au {

/// \brief A simple, bidirectional, weighted graph
///
/// bidirectional => adjacent(u, v) = adjacent(v, u) for all (u, v)
///
/// simple        => undirected ^ no loops ^ no more than one edge between
///                  vertices
///
/// weighted      => value associated with each edge
template <class VD = int, class ED = int>
class simple_adjacency_list
{
public:

    typedef VD vertex_data;
    typedef ED edge_data;

    class vertex;
    class edge;

    typedef typename std::list<vertex>::iterator         vertex_iterator;
    typedef typename std::list<vertex>::const_iterator   const_vertex_iterator;

    typedef typename std::list<edge>::iterator           edge_iterator;
    typedef typename std::list<edge>::const_iterator     const_edge_iterator;

    class vertex
    {
    public:

        typedef std::pair<edge_iterator, vertex_iterator>       vertex_edge;
        typedef typename std::list<vertex_edge>::iterator       vertex_edge_iterator;
        typedef typename std::list<vertex_edge>::const_iterator const_vertex_edge_iterator;

        vertex_data& data();
        const vertex_data& data() const;

    private:

        std::list<vertex_edge> m_edges;
        vertex_data m_data;

        friend class simple_adjacency_list;

        vertex(const vertex_data& data);

        vertex_edge_iterator neighbors_begin();
        vertex_edge_iterator neighbors_end();

        const_vertex_edge_iterator neighbors_begin() const;
        const_vertex_edge_iterator neighbors_end() const;

        bool remove_edge(edge_iterator e);
        bool remove_edge(vertex_iterator v);

        bool adjacent(vertex_iterator v) const;
    };

    class edge
    {
    public:

        friend class simple_adjacency_list;

        const edge_data& data() const;
        edge_data& data();

    private:

        vertex_iterator m_u, m_v;
        edge_data m_data;

        edge(vertex_iterator u, vertex_iterator v, const edge_data& data);
    };

    typedef typename vertex::vertex_edge_iterator        vertex_edge_iterator;
    typedef typename vertex::const_vertex_edge_iterator  const_vertex_edge_iterator;

    simple_adjacency_list();
    ~simple_adjacency_list();

    vertex_iterator insert_vertex(const vertex_data& data);

    void erase_vertex(vertex_iterator v);

    edge_iterator insert_edge(
        vertex_iterator u,
        vertex_iterator v,
        const ED& data);

    void erase_edge(edge_iterator e);

    bool adjacent(vertex_iterator u, vertex_iterator v) const;

    vertex_iterator vertices_begin();
    vertex_iterator vertices_end();

    const_vertex_iterator vertices_begin() const;
    const_vertex_iterator vertices_end() const;

    edge_iterator edges_begin();
    edge_iterator edges_end();

    const_edge_iterator edges_begin() const;
    const_edge_iterator edges_end() const;

    vertex_edge_iterator neighbors_begin(vertex_iterator vit);
    vertex_edge_iterator neighbors_end(vertex_iterator vit);

    const_vertex_edge_iterator neighbors_begin(const_vertex_iterator vit) const;
    const_vertex_edge_iterator neighbors_end(const_vertex_iterator vit) const;

    int vertex_count() const;
    int edge_count() const;

private:

    std::list<vertex> m_verts;  ///< list of all vertices
    std::list<edge> m_edges;    ///< list of all edges
};

} // namespace au

#include "detail/simple_adjacency_list.h"

#endif
