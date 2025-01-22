//
// Created by ariel.simulevski on 29.04.20.
//

#include <tinygraph.h>

#include <utility>
#include <iostream>
#include <sstream>
#include <functions/util.h>
#include <algorithm>
#include "graph.h"
#include <variant>

namespace tinygraph {
    Graph::Graph() = default;

    Graph::~Graph() = default;

    void Graph::add_vertex(std::shared_ptr<Vertex> vertex) {
        this->vertices[vertex->name] = std::move(vertex);
    }

    std::shared_ptr<Vertex> Graph::get_vertex(const std::string& name) {
        auto v = this->vertices.at(name);
        return v;
    }

    std::shared_ptr<std::map<std::string, std::any>> Graph::link(const std::string& from, const std::string& to, bool unidirectional) {
        return vertex_link(this->vertices[from], this->vertices[to], unidirectional);
    }

    std::shared_ptr<Vertex> Graph::add(const std::string &name, std::shared_ptr<Type> type) {
        auto v = vertex_create(name, std::move(type));
        this->add_vertex(v);
        return v;
    }

    std::string Graph::str() {
        std::string res;

        for (const auto& [key, vertex] : this->vertices) {
            res += key;

            for (const auto& [propKey, property] : vertex->properties) {
                res += " [" + propKey + " = " + any_to_str(&property) + "]";
            }

            res += "\n";

            for (const auto& c : vertex->connections) {
                res += "\t" + key + " -> " + c->to->name;

                for (const auto& [propKey, property] : *c->properties) {
                    res += " [" + propKey + " = " + any_to_str(&property) + "]";
                }

                res += "\n";
            }
        }

        return res;
    }

    std::vector<std::vector<std::string>> Graph::connected_components() {
        //TODO
        this->str();
        return std::vector<std::vector<std::string>>();
    }

    void Graph::reset_paths()
    {
        distances.clear();
        parent.clear();
    }

    bool Graph::vertex_exists(const std::string& vertex)
    {
        for(auto& [vertex_name, vertex_ptr] : vertices) 
        {
            if (vertex_name == vertex)
            {
                return true;
            }
        }
        return false;
    }


    bool Graph::initialize_distances() {
        number infinity;
        for (auto& [vertex_name, vertex_ptr] : vertices)
            {
            for (auto& edge : vertex_ptr->connections)
            {
                if (edge->properties && edge->properties->find(path_property) != edge->properties->end())
                {
                    std::any property = edge->properties->at(path_property);

                    if (property.type() == typeid(int))
                    {
                        infinity = std::numeric_limits<int>::max();
                    }
                    else if (property.type() == typeid(float))
                    {
                        infinity = std::numeric_limits<float>::max();
                    }
                    else if (property.type() == typeid(double))
                    {
                        infinity = std::numeric_limits<double>::max();
                    }
                    else return false;

                    for (auto& [vertex_name, vertex_ptr] : vertices)
                    {
                        distances[vertex_name] = infinity;
                        parent[vertex_name] = "";
                    }
                    distances[source_name] = 0;

                    return true;

                }
                else if (edge->properties && edge->properties->find(path_property) == edge->properties->end())
                {
                    return false;
                }
            }
        }
        return false;
    }

    Graph::number Graph::find_value(std::any& property) 
    {
        if (property.type() == typeid(int))
        {
            return std::any_cast<int>(property);
        }
        else if (property.type() == typeid(float))
        {
            return std::any_cast<float>(property);
        }
        else if (property.type() == typeid(double))
        {
            return std::any_cast<double>(property);
        }

        else return std::numeric_limits<int>::min();
    }

    bool Graph::bellman_ford(const std::string& the_source_name, const std::string& sorting_property)
    {
        if (sorting_property.empty() || the_source_name.empty()) return false;
        path_property = sorting_property;
        source_name = the_source_name;
        if (!vertex_exists(the_source_name))
        {
            reset_paths();
            return false;
        }

        if (!initialize_distances())
        {
            reset_paths();
            return false;
        } 
 
        for (int i=0; i<vertices.size()-1; i++) 
        {
            for (auto& [vertex_name, vertex_ptr] : vertices)
            {
                for (auto& edge : vertex_ptr->connections)
                {
                    if (edge->properties && edge->properties->find(sorting_property) != edge->properties->end())
                    {
                        std::any property = edge->properties->at(path_property);
                        
                        number weight = find_value(property);

                        if (typeid(weight) == typeid(int) && std::get<int>(weight) == std::numeric_limits<int>::min())
                        {
                            reset_paths();
                            return false;
                        }

                        auto& ref_vertex_name = vertex_name;

                        std::visit(
                        [this, &ref_vertex_name, &edge](auto& distance_from, auto& distance_to, auto& weight) {

                            using type_used = std::common_type_t<decltype(distance_from), decltype(distance_to), decltype(weight)>;
                            if (distance_from != std::numeric_limits<type_used>::max() && distance_to > distance_from + weight)
                            {
                                this->distances[edge->to->name] = distance_from + weight;
                                this->parent[edge->to->name] = ref_vertex_name;
                            }

                        },
                        distances[vertex_name],
                        distances[edge->to->name],
                        weight
                        );
                    }

                    
                    else if (edge->properties && edge->properties->find(sorting_property) == edge->properties->end())
                    {
                        reset_paths();
                        return false;
                    }
                }
            }
        }

        for (auto& [vertex_name, vertex_ptr] : vertices)
        {
            for (auto& edge : vertex_ptr->connections)
            {
                if (edge->properties && edge->properties->find(sorting_property) != edge->properties->end())
                {
                    std::any property = edge->properties->at(path_property);
                    
                    number weight = find_value(property);

                    if (typeid(weight) == typeid(int) && std::get<int>(weight) == std::numeric_limits<int>::min())
                    {
                        reset_paths();
                        return false;
                    }
                    
                    bool returning = true;

                    std::visit(
                        [this, &returning](auto& distance_from, auto& distance_to, auto& weight) {
                            using type_used = std::common_type_t<decltype(distance_from), decltype(distance_to), decltype(weight)>;
                            if (distance_from != std::numeric_limits<type_used>::max() && distance_to > distance_from + weight)
                            {
                                this->negative_cycle = negative;
                                returning = false;
                            }

                        },
                        distances[vertex_name],
                        distances[edge->to->name],
                        weight
                        );

                    if (!returning)
                    {
                        reset_paths();
                        return false;
                    }
                }
            }
        }
        negative_cycle = non_negative;
        return true;
    }


    std::vector<std::string> Graph::find_shortest_path(const std::string& destination)
    {
        std::vector<std::string> shortest_path;

        if (!vertex_exists(destination)) 
        {
            return shortest_path;
        }

        std::string current = destination;


        while (current != source_name)
        {
            if (current.empty())
            {
                shortest_path.clear();
                return shortest_path;
            }
            shortest_path.push_back(current);
            current = parent[current];
        }

        shortest_path.push_back(current);
        
        std::reverse(shortest_path.begin(), shortest_path.end());

        return shortest_path;
    }

    void Graph::dfs(const std::string& current, const std::string& destination, std::map<std::string, vector<std::string>>& adj, std::map<std::string, bool>& visited, std::vector<std::string>& path, bool& found) 
    {
        visited[current] = true;

        path.push_back(current);

        if (current == destination)
        {
            found = true;
            return;
        }

        for (std::string testing : adj[current])
        {
            if (!visited[testing] && !found) 
            {
                dfs(testing, destination, adj, visited, path, found);
            }
        }
        if (!found) path.pop_back();
    }

    bool Graph::dfsSetup(const std::string& source, const std::string& destination)
    {
        if (vertices.find(source) == vertices.end() || vertices.find(destination) == vertices.end()) return false;

        std::map<std::string, vector<std::string>> adj;
        std::map<std::string, bool> visited;

        std::vector<std::string> path;

        for (auto& [vertex_name, vertex_ptr] : vertices)
        {
            visited[vertex_name] = false;
        }
            
            

        for (auto& [vertex_name, vertex_ptr] : vertices)
        {
            for (auto& edge : vertex_ptr->connections)
            {
                if (undirected)
                {
                    adj[vertex_name].push_back(edge->to->name);
                    adj[edge->to->name].push_back(vertex_name);
                }
                else
                {
                    adj[vertex_name].push_back(edge->to->name);
                }
            }
        }
        
        bool found = false;

        dfs(source, destination, adj, visited, path, found);

        if (!found) path.clear();
        
        return found;


    }
}
