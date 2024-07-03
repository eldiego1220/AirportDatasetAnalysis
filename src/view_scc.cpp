#include "FLightGraph.h"

using namespace cs225;

int main(int argc, char **argv) {
  std::string airports_path = "dataset/airports.dat.txt";
  std::string routes_path = "dataset/routes.dat.txt";
  std::string output_image_name = "view_scc.png";
  int airport_id = 3830;
  FlightGraph graph(airports_path, routes_path);

  // std::vector<FlightGraph::Airport> scc = graph.findSCC(graph);

  PNG map = PNG();
  map.readFromFile("dataset/mercatormap.png");

  FlightGraph::Airport airport = graph.getAirport(airport_id);

  graph.DrawDFS(&airport, &map, HSLAPixel(120, 1, 0.5, 1));

  map.writeToFile(output_image_name);

  return 0;
}