
#include <iostream>
#include <utility>
#include <cassert>
#include <vector>
#include <algorithm>

namespace util {
  template <class T>
  class vector2d {
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](std::size_t i) {
      return data[i];
    }
  };
}

namespace graphica {

  template <class Type = size_t, class Compare = std::less<Type>>
  struct edge {
    using size_type = std::size_t;
    using weight_type = Type;

    enum class type : uint8_t {
      none    = 0b00,
      u_to_v  = 0b01,
      v_to_u  = 0b10,
      bi_dir  = 0b11 // edges that are bidirectional in directed graphs are the same as edges in undirected graphs
    };

    size_type u;
    size_type v;
    weight_type w;
    type e;
    constexpr edge(size_type u_, size_type v_, weight_type w_ = 1, type e_ = type::none)
    : u{u_}, v{v_}, w{w_}, e{e_} {
      if (e > type::bi_dir) throw std::invalid_argument("unknown edge type.");
    }

    bool is_none() {
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

/* 
  template <class Vertex = vertex_empty>  
  class graph {
  public:
    using size_type = std::size_t;
    using vertex_type = Vertex;
  private:
    // Constrain nested vector size to be equal to that of the magnitude of vertex_list
    std::vector<std::vector<edge::type>> edge_table;
    std::vector<vertex_type> vertex_list;

   void reserve(size_type num_verticies) {
      if(num_verticies >= vertex_list.capacity()) {
        vertex_list.resize(num_verticies);
        for(auto it = edge_table.begin(); it != edge_table.end(); ++it) {
          if ((*it).capacity()
        }
        return;
      }
error:
        throw std::invalid_argument("remove verticies before shrinking.");
      
    }
  };*/
  

  /* Static graphs are better in terms of performance, and memory usage.
    However, they are limited by a maximum number of verticies. Meaning,
    changes can be made to the graph, such as adding or removing verticies
    and edges, as long as they don't involve creating verticies above V.
  */
  /* TODO compress storage size.
    this would be done by reducing the memory footprint of the classes, by using 2 bits per edge.
    and sticking 16 edges into 1 32 bit memory cell.

  */
  template <std::size_t VC, class V = vertex_empty, bool loops_allowed = false>
  class fixed_graph {
  public:
    using size_type = std::size_t;
    using vertex = V;

  private:
    vertex vertex_list[VC] = {};
    size_type vertex_count = 0;
    edge<>::type edge_table[VC][VC] = {{edge<>::type::none}}; // explicit specification for readability only
    size_type edge_count = 0;

    void add_edge(edge<> e) {
      if (e.u >= VC && e.v >= VC)
        throw std::invalid_argument("vertex outside graph's scope. u/v too large");

      if constexpr (!loops_allowed)
        if(e.u  == e.v)
          if (e.e != edge<>::type::none)
            // TODO debug flag that makes the exception more explanative
            throw std::invalid_argument("loop(s?) exist (edge drawn between vertex u == v).");

      this->edge_table[e.u][e.v] = e.e;
      ++this->edge_count;
    }

    void add_edge2(edge<> e) {
      if (e.u < VC && e.v < VC) {
        if(e.u  != e.v) {
          this->edge_table[e.u][e.v] = e.e;
          ++this->edge_count;
        } else {
          if (e.e != edge<>::type::none) {
            // TODO debug flag that makes the exception more explanative
            throw std::invalid_argument("loop(s?) exist (edge drawn between vertex u == v).");
            }
        }
      } else {
        // TODO debug flag that makes the arument more explanative
        throw std::invalid_argument("vertex outside graph's scope. u/v too large");
      }
    }

  public:
    fixed_graph() = default;

    /* This constructor builds a fixed maximum sized graph.
    preconditions:
    - se and sv are begining pointers and en and vn are the respective number of elements
    */
    fixed_graph(edge<>* edges, size_type en, vertex* verticies = nullptr, size_type vn = 0)
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

    fixed_graph(const char* t, vertex* verticies = nullptr, size_type vn = 0) {{
      // Create edge information from character input.
      std::size_t i = 0, j = 0;
      // Read characters, at most 
      for(; *t != '\0' && (i + j*VC) < VC*VC ; ++t)
      {
        if(i == VC) { i = 0; ++j; }
        if(*t == '\n') { i = 0; ++j; continue; } // skip newlines, that indicate early line endings.
        add_edge(edge<>(i++, j, 1, static_cast<edge<>::type>(*t - 48)));

      }}
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
      return edge_table[u][v] != edge<>::type::none;
    }
    // Returns the type of edge that exists between two different verticies
    edge<>::type get_edge(size_type u, size_type v) {
      return edge_table[u][v];
    }
  };
}


namespace graphica2 {
  
  enum class graph_type {
      undirected_simple_graph,
      directed_simple_graph,
      undirected_simple_graph_permitting_loops,
      directed_simple_graph_permitting_loops,
    };

  template <class Properties = graph_properties:: 
  class graph {
  public:
    

  private:
    uint32_t parse_properties() {
      return;
    }

    template <class X, class ... Xs>
    uint32_t parse_properties(X* const &&gp, Xs* const&&... gps) {
      return gp::value + parse_properties(gps...);
    }

  public:

    graph() = default;

    //graph_properties* (&&gps) ...
    template <class Arg, class ... Args>
    graph(Arg const&& gp, Args const&&... gps) {
      
    }
  };
}

int main() {
  const char* e = "00001"
                  "00010"
                  "00011"
                  "00100"
                  "00100";
  graphica::fixed_graph<5> grph{e};


  graphica2::graph g{};

  std::cout << grph.num_verticies() << " " << grph.num_edges() << " " << grph.max_verticies() << std::endl;
}
