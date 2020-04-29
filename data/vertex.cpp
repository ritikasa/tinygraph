//
// Created by ariel.simulevski on 29.04.20.
//

#include "types.h"
#include <utility>

Vertex::Vertex(std::string name, std::shared_ptr<Type> type) {
    this->name = std::move(name);
    this->type = std::move(type);
}

Vertex::~Vertex() = default;

void Vertex::add_prop(const char* key, std::any value) {
    this->properties[key] = std::move(value);
}