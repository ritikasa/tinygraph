//
// Created by ariel.simulevski on 29.04.20.
//

#ifndef TINYGRAPH_GRAPH_H
#define TINYGRAPH_GRAPH_H

#include "types.h"
#include <vector>
#include <variant>

namespace tinygraph {
    class Graph {
    public:
        std::map<std::string, std::shared_ptr<Vertex>> vertices;

        Graph();
        ~Graph();

        std::shared_ptr<Vertex> add(const std::string& name, std::shared_ptr<Type> type);

        void add_vertex(std::shared_ptr<Vertex> vertex);

        std::shared_ptr<Vertex> get_vertex(const std::string& name);

        std::shared_ptr<std::map<std::string, std::any>> link(const std::string& from, const std::string& to, bool unidirectional);

        std::vector<std::vector<std::string>> connected_components();

        std::string str();

        using number = std::variant<int, float, double>; // more types can be added here
        
        std::map<std::string, number> distances;

        std::map<std::string, std::string> parent;

        std::string path_property;

        bool bellman_ford(const std::string& the_source_name, const std::string& sorting_property);
        
        bool vertex_exists(const std::string& vertex);

        void reset_paths();

        std::string source_name;

        std::vector<std::string> find_shortest_path(const std::string& destination);

        enum neg_cycle_state { unknown, negative, non_negative };

        neg_cycle_state negative_cycle = unknown;

        bool initialize_distances();

        number find_value(std::any& property);

    };
}

#endif //TINYGRAPH_GRAPH_H
