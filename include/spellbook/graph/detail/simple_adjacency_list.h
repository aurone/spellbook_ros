#ifndef au_detail_simple_adjacency_list_h
#define au_detail_simple_adjacency_list_h

#include "simple_adjacency_list.h"

#include <assert.h>

namespace au {

///////////////////////////////////
// simple_adjacency_list::vertex //
///////////////////////////////////

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex_data&
simple_adjacency_list<VD, ED>::vertex::data()
{
    return m_data;
}

template <class VD, class ED>
const typename simple_adjacency_list<VD, ED>::vertex_data&
simple_adjacency_list<VD, ED>::vertex::data() const
{
    return m_data;
}

template <class VD, class ED>
simple_adjacency_list<VD, ED>::vertex::vertex(const vertex_data& data) :
    m_edges(),
    m_data(data)
{
}

/////////////////////////////////
// simple_adjacency_list::edge //
/////////////////////////////////

template <class VD, class ED>
const typename simple_adjacency_list<VD, ED>::edge_data&
simple_adjacency_list<VD, ED>::edge::data() const
{
    return m_data;
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::edge_data&
simple_adjacency_list<VD, ED>::edge::data()
{
    return m_data;
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex::vertex_edge_iterator
simple_adjacency_list<VD, ED>::vertex::neighbors_begin()
{
    return m_edges.begin();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex::vertex_edge_iterator
simple_adjacency_list<VD, ED>::vertex::neighbors_end()
{
    return m_edges.end();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex::const_vertex_edge_iterator
simple_adjacency_list<VD, ED>::vertex::neighbors_begin() const
{
    return m_edges.begin();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex::const_vertex_edge_iterator
simple_adjacency_list<VD, ED>::vertex::neighbors_end() const
{
    return m_edges.end();
}

template <class VD, class ED>
bool
simple_adjacency_list<VD, ED>::vertex::remove_edge(edge_iterator e)
{
    for (auto veit = neighbors_begin(); veit != neighbors_end(); ++veit) {
        edge_iterator e2 = veit->first;

        if (e2 == e) { // point to the same edge
            m_edges.erase(veit);
            return true; // the graph is simple, so there should only be one
        }
    }

    return false;
}

template <class VD, class ED>
bool
simple_adjacency_list<VD, ED>::vertex::remove_edge(vertex_iterator v)
{
    for (auto veit = neighbors_begin(); veit != neighbors_end(); ++veit) {
        edge_iterator v2 = veit->second;

        if (v2 == v) { // point to the same edge
            m_edges.erase(veit);
            return true; // the graph is simple, so there should only be one
        }
    }

    return false;
}

template <class VD, class ED>
bool
simple_adjacency_list<VD, ED>::vertex::adjacent(vertex_iterator v) const
{
    for (const vertex_edge& ve : m_edges) {
        if (ve.second == v) {
            return true;
        }
    }
    return false;
}

template <class VD, class ED>
simple_adjacency_list<VD, ED>::edge::edge(
    vertex_iterator u,
    vertex_iterator v,
    const edge_data& data)
:
    m_u(u),
    m_v(v),
    m_data(data)
{
}

///////////////////////////
// simple_adjacency_list //
///////////////////////////

template <class VD, class ED>
simple_adjacency_list<VD, ED>::simple_adjacency_list() :
    m_verts(),
    m_edges()
{
}

template <class VD, class ED>
simple_adjacency_list<VD, ED>::~simple_adjacency_list()
{
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex_iterator
simple_adjacency_list<VD, ED>::insert_vertex(const vertex_data& data)
{
    vertex v(data);
    return m_verts.insert(m_verts.end(), v);
}

template <class VD, class ED>
void
simple_adjacency_list<VD, ED>::erase_vertex(vertex_iterator vit)
{
    // remove edges between this vertex and neighboring vertices
    for (auto veit = vit->neighbors_begin(); veit != vit->neighbors_end(); ++veit) {
        edge_iterator e = veit->first;
        vertex_iterator v = veit->second;

        // remove this edge from that neighbor's edge reference list
        bool res = v->remove_edge(e);
        assert(res);

        // remove the edge from the list of edges
        m_edges.erase(e);
    }

    // remove the vertex from the list of vertices
    m_verts.erase(vit);
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::edge_iterator
simple_adjacency_list<VD, ED>::insert_edge(
    vertex_iterator u,
    vertex_iterator v,
    const edge_data& data)
{
    if (u == v) {
        return edges_end();
    }

    // choose the vertex with the fewer amount of edges to search through
    vertex_iterator vless, vmore;
    if (u->m_edges.size() < v->m_edges.size()) {
        vless = u;
        vmore = v;
    }
    else {
        vless = v;
        vmore = u;
    }

    // check for an already existing edge
    for (auto veit = vless->neighbors_begin();
        veit != vless->neighbors_end(); ++veit)
    {
        edge_iterator e = veit->first;
        vertex_iterator vv = veit->second;

        // found an existing edge
        if (vv == vmore) {
            return e;
        }
    }

    // create an entry for this edge
    edge e(u, v, data);
    auto eit = m_edges.insert(m_edges.end(), e);

    // add reference to edge to both vertices
    u->m_edges.emplace_back(eit, v);
    v->m_edges.emplace_back(eit, u);

    // return edge iterator
    return eit;
}

template <class VD, class ED>
void
simple_adjacency_list<VD, ED>::erase_edge(edge_iterator e)
{
    e->m_u->remove_edge(e);
    e->m_v->remove_edge(e);
    m_edges.erase(e);
}

template <class VD, class ED>
bool
simple_adjacency_list<VD, ED>::adjacent(
    vertex_iterator u,
    vertex_iterator v) const
{
    return u->adjacent(v);
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex_iterator
simple_adjacency_list<VD, ED>::vertices_begin()
{
    return m_verts.begin();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex_iterator
simple_adjacency_list<VD, ED>::vertices_end()
{
    return m_verts.end();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::const_vertex_iterator
simple_adjacency_list<VD, ED>::vertices_begin() const
{
    return m_verts.begin();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::const_vertex_iterator
simple_adjacency_list<VD, ED>::vertices_end() const
{
    return m_verts.end();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::edge_iterator
simple_adjacency_list<VD, ED>::edges_begin()
{
    return m_edges.begin();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::edge_iterator
simple_adjacency_list<VD, ED>::edges_end()
{
    return m_edges.end();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::const_edge_iterator
simple_adjacency_list<VD, ED>::edges_begin() const
{
    return m_edges.begin();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::const_edge_iterator
simple_adjacency_list<VD, ED>::edges_end() const
{
    return m_edges.end();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex_edge_iterator
simple_adjacency_list<VD, ED>::neighbors_begin(vertex_iterator vit)
{
    return vit->neighbors_begin();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::vertex_edge_iterator
simple_adjacency_list<VD, ED>::neighbors_end(vertex_iterator vit)
{
    return vit->neighbors_end();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::const_vertex_edge_iterator
simple_adjacency_list<VD, ED>::neighbors_begin(const_vertex_iterator vit) const
{
    return vit->neighbors_begin();
}

template <class VD, class ED>
typename simple_adjacency_list<VD, ED>::const_vertex_edge_iterator
simple_adjacency_list<VD, ED>::neighbors_end(const_vertex_iterator vit) const
{
    return vit->neighbors_end();
}

template <class VD, class ED>
int simple_adjacency_list<VD, ED>::vertex_count() const
{
    return (int)m_verts.size();
}

template <class VD, class ED>
int simple_adjacency_list<VD, ED>::edge_count() const
{
    return m_edges.size();
}

} // namespace au

#endif
