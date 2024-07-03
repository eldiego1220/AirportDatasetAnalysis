#include <iostream>
#include <stdexcept>
#include <string>
#include "FlightGraph.h"
#include "map_utils.h"
#include "PNG.h"

using namespace cs225;

int main(int argc, char **argv) {
  int source_id;
  int dest_id;
  bool draw_all_routes;
  std::string airports_path = "dataset/airports.dat.txt";
  std::string routes_path = "dataset/routes.dat.txt";
  std::string output_image_name = "output.png";

  if (argc == 7) {
    airports_path = argv[1];
    routes_path = argv[2];
    source_id = std::stoi(argv[3]);
    dest_id = std::stoi(argv[4]);
    draw_all_routes = std::stoi(argv[5]);
    output_image_name = argv[6];
  }
  else if (argc == 6) {
    airports_path = argv[1];
    routes_path = argv[2];
    source_id = std::stoi(argv[3]);
    dest_id = std::stoi(argv[4]);
    draw_all_routes = std::stoi(argv[5]);
  } else if (argc == 5) {
    source_id = std::stoi(argv[1]);
    dest_id = std::stoi(argv[2]);
    draw_all_routes = std::stoi(argv[3]);
    output_image_name = argv[4];
  } else if (argc == 4) {
    source_id = std::stoi(argv[1]);
    dest_id = std::stoi(argv[2]);
    draw_all_routes = std::stoi(argv[3]);
  } else {
    throw std::runtime_error("Usage: [airports_path] [routes_path] source_id dest_id draw_all_routes [output_image_name]");
  }
  FlightGraph graph(airports_path, routes_path);

  FlightGraph::Airport source = graph.getAirport(source_id);
  FlightGraph::Airport dest = graph.getAirport(dest_id);

  std::list<FlightGraph::Route*> path = graph.ShortestPath(source, dest);
  std::vector<FlightGraph::Route*> shortest_path(std::begin(path), std::end(path));
  
  PNG map = PNG();
  map.readFromFile("dataset/mercatormap.png");

  if (draw_all_routes) {
    std::vector<FlightGraph::Route*> all_paths = graph.getRoutes();
    graph.DrawPaths(all_paths, &map, HSLAPixel(120, 1, 0.5, 1), false);
  }
  
  graph.DrawPaths(shortest_path, &map, HSLAPixel(0, 1, 0.5, 0.5), true);
  draw_airport_on_map(source, &map);
  draw_airport_on_map(dest, &map);

  std::cout << "Starting Airport: " << source.name << ", City: " << source.city << ", Country: " << source.country << std::endl;
  for (FlightGraph::Route* route : shortest_path) {
    FlightGraph::Airport dest_airport = graph.getAirport(route->dest_id);
    std::cout << "Airport: " << dest_airport.name << ", City: " << dest_airport.city << ", Country: " << dest_airport.country << std::endl;
  }
  if (shortest_path.empty()) {
    std::cout << "No path exists!" << std::endl;
  }

  map.writeToFile(output_image_name);

  return 0;
}