#include "../tinygraph.h"
#include <climits>
#include <iostream>
#include <memory>

static constexpr char DISTANCE[] = "distance";
static constexpr char NAME[] = "name";

void print_bellman_ford(std::unique_ptr<tinygraph::Graph> &g) {
  for (auto& [city, distance] : g->distances) {
    auto& city_ref = city;
    std::visit(
        [&city_ref](auto distance) {
          using distance_type = decltype(distance);
          std::cout << "\t" << city_ref << " : " << std::flush;
          if (distance == std::numeric_limits<distance_type>::max()) {
            std::cout << "infinity" << std::endl;
          } else
            std::cout << distance << std::endl;
        },
        distance);
  }
}

void handle_path(std::unique_ptr<tinygraph::Graph> &g,
                 std::string destination) {
  std::cout << "path: " << std::flush;
  std::vector<std::string> path = g->find_shortest_path(destination);

  for (std::string &parent : path) {
    std::cout << parent << std::flush;
  }
  std::cout << std::endl;
}

void handle_bellman_ford(std::unique_ptr<tinygraph::Graph> &g,
                         std::string vertex_name, std::string function_name,
                         bool print_path = false,
                         std::string destination = "") {
  bool value = g->bellman_ford(vertex_name, DISTANCE);
  std::cout << function_name << std::endl;
  if (value)
    print_bellman_ford(g);
  else
    std::cout << "\tdidn't work" << std::endl;

  if (print_path)
    handle_path(g, destination);
}

void play_around_example() {
  auto port = tinygraph::typestore_add("port");

  auto g = std::make_unique<tinygraph::Graph>();

  std::shared_ptr<tinygraph::Vertex> v;

  v = g->add("A", port);
  v->properties[NAME] = "dkfjdkfj";

  v = g->add("B", port);
  v->properties[NAME] = "lkja";

  v = g->add("C", port);
  v->properties[NAME] = "addjm";

  std::shared_ptr<std::map<std::string, std::any>> linkprops;

  bool unlinked = false;

  linkprops = g->link("A", "C", unlinked);
  linkprops->insert({DISTANCE, 4});

  linkprops = g->link("C", "B", unlinked);
  linkprops->insert({DISTANCE, -3.2});

  linkprops = g->link("A", "B", unlinked);
  linkprops->insert({DISTANCE, 2});

  handle_bellman_ford(g, "A", "more complex example", true, "C");
}

void more_complex_example() {
  auto port = tinygraph::typestore_add("port");

  auto g = std::make_unique<tinygraph::Graph>();

  std::shared_ptr<tinygraph::Vertex> v;

  v = g->add("A", port);
  v->properties[NAME] = "dkfjdkfj";

  v = g->add("B", port);
  v->properties[NAME] = "lkja";

  v = g->add("C", port);
  v->properties[NAME] = "addjm";

  v = g->add("D", port);
  v->properties[NAME] = "kjse";

  v = g->add("E", port);
  v->properties[NAME] = "aegf";

  std::shared_ptr<std::map<std::string, std::any>> linkprops;

  bool unlinked = false;

  linkprops = g->link("A", "B", unlinked);
  linkprops->insert({DISTANCE, -1});

  linkprops = g->link("B", "C", unlinked);
  linkprops->insert({DISTANCE, 3});

  linkprops = g->link("B", "D", unlinked);
  linkprops->insert({DISTANCE, 2});

  linkprops = g->link("D", "B", unlinked);
  linkprops->insert({DISTANCE, 1});

  linkprops = g->link("B", "E", unlinked);
  linkprops->insert({DISTANCE, 2});

  linkprops = g->link("E", "D", unlinked);
  linkprops->insert({DISTANCE, -3});

  linkprops = g->link("D", "C", unlinked);
  linkprops->insert({DISTANCE, 5});

  linkprops = g->link("A", "C", unlinked);
  linkprops->insert({DISTANCE, 4});

  handle_bellman_ford(g, "A", "more complex example");
}

void negative_cycle_example() {
  auto port = tinygraph::typestore_add("port");

  auto g = std::make_unique<tinygraph::Graph>();

  std::shared_ptr<tinygraph::Vertex> v;

  v = g->add("0", port);
  v->properties[NAME] = "dkfjdkfj";

  v = g->add("1", port);
  v->properties[NAME] = "lkja";

  v = g->add("2", port);
  v->properties[NAME] = "addjm";

  v = g->add("3", port);
  v->properties[NAME] = "kjse";

  std::shared_ptr<std::map<std::string, std::any>> linkprops;

  bool unlinked = false;

  linkprops = g->link("1", "0", unlinked);
  linkprops->insert({DISTANCE, 4});

  linkprops = g->link("1", "2", unlinked);
  linkprops->insert({DISTANCE, -6});

  linkprops = g->link("3", "1", unlinked);
  linkprops->insert({DISTANCE, -2});

  linkprops = g->link("2", "3", unlinked);
  linkprops->insert({DISTANCE, 5});

  handle_bellman_ford(g, "A", "negative cycles");
}

void double_example() {
  auto port = tinygraph::typestore_add("port");

  auto g = std::make_unique<tinygraph::Graph>();

  std::shared_ptr<tinygraph::Vertex> v;

  v = g->add("0", port);
  v->properties[NAME] = "dkfjdkfj";

  v = g->add("1", port);
  v->properties[NAME] = "lkja";

  v = g->add("2", port);
  v->properties[NAME] = "addjm";

  v = g->add("3", port);
  v->properties[NAME] = "kjse";

  v = g->add("4", port);
  v->properties[NAME] = "aegf";

  v = g->add("5", port);
  v->properties[NAME] = "llk";

  v = g->add("6", port);
  v->properties[NAME] = "luh";

  v = g->add("7", port);
  v->properties[NAME] = "knu";

  std::shared_ptr<std::map<std::string, std::any>> linkprops;

  bool unlinked = false;

  linkprops = g->link("5", "1", unlinked);
  linkprops->insert({DISTANCE, 0.32});

  linkprops = g->link("0", "2", unlinked);
  linkprops->insert({DISTANCE, 0.26});

  linkprops = g->link("7", "3", unlinked);
  linkprops->insert({DISTANCE, 0.39});

  linkprops = g->link("0", "4", unlinked);
  linkprops->insert({DISTANCE, 0.38});

  linkprops = g->link("4", "5", unlinked);
  linkprops->insert({DISTANCE, 0.35});

  linkprops = g->link("3", "6", unlinked);
  linkprops->insert({DISTANCE, 0.52});

  linkprops = g->link("2", "7", unlinked);
  linkprops->insert({DISTANCE, 0.34});

  handle_bellman_ford(g, "0", "doubles", true, "6");
}

void float_example() {
  auto port = tinygraph::typestore_add("port");

  auto g = std::make_unique<tinygraph::Graph>();

  std::shared_ptr<tinygraph::Vertex> v;

  v = g->add("0", port);
  v->properties[NAME] = "dkfjdkfj";

  v = g->add("1", port);
  v->properties[NAME] = "lkja";

  v = g->add("2", port);
  v->properties[NAME] = "addjm";

  v = g->add("3", port);
  v->properties[NAME] = "kjse";

  v = g->add("4", port);
  v->properties[NAME] = "aegf";

  v = g->add("5", port);
  v->properties[NAME] = "llk";

  v = g->add("6", port);
  v->properties[NAME] = "luh";

  v = g->add("7", port);
  v->properties[NAME] = "knu";

  std::shared_ptr<std::map<std::string, std::any>> linkprops;

  bool unlinked = false;

  linkprops = g->link("5", "1", unlinked);
  linkprops->insert({DISTANCE, 0.32f});

  linkprops = g->link("0", "2", unlinked);
  linkprops->insert({DISTANCE, 0.26f});

  linkprops = g->link("7", "3", unlinked);
  linkprops->insert({DISTANCE, 0.39f});

  linkprops = g->link("0", "4", unlinked);
  linkprops->insert({DISTANCE, 0.38f});

  linkprops = g->link("4", "5", unlinked);
  linkprops->insert({DISTANCE, 0.35f});

  linkprops = g->link("3", "6", unlinked);
  linkprops->insert({DISTANCE, 0.52f});

  linkprops = g->link("2", "7", unlinked);
  linkprops->insert({DISTANCE, 0.34f});

  handle_bellman_ford(g, "0", "floats");
}

int main() {
  tinygraph::typestore_init();
  more_complex_example();
  float_example();
  negative_cycle_example();
  double_example();
  play_around_example();
  return 0;
}
