#include <iostream>
#include <utility>
#include <cassert>

namespace graphica {
  

  struct edge {
    using size_type = std::size_t;
    enum class type : uint8_t {
      none    = 0b00,
      u_to_v  = 0b01,
      v_to_u  = 0b10,
      bi_dir  = 0b11 // edges that are bidirectional in directed graphs are the same as edges in undirected graphs
    };

    size_type u;
    size_type v;
    type e;
    constexpr edge(size_type u_, size_type v_, type e_ = type::none)
    : u{u_}, v{v_}, e{e_} {}

    inline constexpr bool is_none() const {
      return e == type::none;
    }
  };

  // Zero-sized array makes struct zero-sized.
  struct vertex_empty { int x[0]; };

  template <class T>
  struct vertex {
    T data;
    constexpr vertex() = default;
    constexpr vertex(T data_)
    : data{data_} {}
  };

  template <class V = vertex_empty>  
  class graph {

  };
  

  /* Static graphs are better in terms of performance, and memory usage.
    However, they are limited by a maximum number of verticies. Meaning,
    changes can be made to the graph, such as adding or removing verticies
    and edges, as long as they don't involve creating verticies above V.
  */
  /* TODO compress storage size.
    this would be done by reducing the memory footprint of the classes, by using 2 bits per edge.
    and sticking 16 edges into 1 32 bit memory cell.

  */
  template <std::size_t VC, class V = vertex_empty>
  class fixed_graph : public graph<V> {
  public:
    using size_type = std::size_t;
    using vertex = V;

  private:
    vertex vertex_list[VC] = {};
    size_type vertex_count = 0;
    edge::type edge_table[VC][VC] = {{edge::type::none}}; // explicit specification for readability only
    size_type edge_count = 0;

    void add_edge(edge& e) {
      if (e.u < VC && e.v < VC) {
        if(e.u != e.v)
          this->edge_table[e.u][e.v] = e.e;
        else
          if (!e.is_none())
            // TODO debug flag that makes the exception more explanative
            throw std::invalid_argument("loop(s?) exist (edge drawn between vertex u == v).");
      } else
        // TODO debug flag that makes the arument more explanative
        throw std::invalid_argument("vertex outside graph's scope. u/v too large");
    }

  public:
    fixed_graph() = default;

    /* This constructor builds a fixed maximum sized graph.
    preconditions:
    - se and sv are begining pointers and en and vn are the respective number of elements
    */
    fixed_graph(edge* edges, size_type en, vertex* verticies, size_type vn)
    {
      // advanced copy required for edge definitions
      this->edge_count = en;
      for (int i = 0; i < en; ++i)
        add_edge(edges[i]);
      
      // simple copy for vertex information
      this->vertex_count = vn;
      for (int i = 0; i < vn; ++i)
        this->vertex_list[i] = verticies[i];
    }

    inline size_type num_edges() const { return this->edge_count; }
    inline size_type num_verticies() const { return this->vertex_count; }
    inline constexpr size_type max_verticies() const { return VC; }
    inline constexpr size_type max_edges() const { return VC * (VC - 1); }

    // Tests to see if there exists an edge between two given verticies
    bool test_edge(size_type u, size_type v) {
      return edge_table[u][v] != edge::type::none;
    }
    // Returns the type of edge that exists between two different verticies
    edge::type get_edge(size_type u, size_type v) {
      return edge_table[u][v];
    }
  };
}

int main() {
  graphica::edge e[] = { graphica::edge{0,1,graphica::edge::type::bi_dir}, graphica::edge{1,4,graphica::edge::type::bi_dir} };
  graphica::vertex<const char*> v[] = { "H" };
  graphica::fixed_graph<5, graphica::vertex<const char *>> grph{e, 2, v, 1};

  std::cout << grph.num_verticies() << " " << grph.num_edges() << " " << grph.max_verticies() << std::endl;
}
