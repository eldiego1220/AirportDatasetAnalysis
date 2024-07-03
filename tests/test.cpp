#include "catch/catch.hpp"
#include <iostream>
#include "../src/FlightGraph.h"
#include "../src/map_utils.h"

using Airport = cs225::FlightGraph::Airport;
using Route = cs225::FlightGraph::Route;

bool RoutesAreEqual(std::list<Route*> test_routes, std::list<Route*> routes) {
  if (test_routes.size() == 0 && routes.size() == 0)
    return true;
  if (test_routes.size() != routes.size())
    return false;
  auto t_route_it = test_routes.begin();
  auto route_it = routes.begin();
  while (t_route_it != test_routes.end()) {
    Route* t_route = *t_route_it;
    Route* route = *route_it;
    if (t_route->source_id != route->source_id || t_route->dest_id != route->dest_id) {
      return false;
    }
    ++t_route_it;
    ++route_it;
  }
  return true;
}

bool AirportsAreEqual(Airport test_airport, Airport airport) {
  return test_airport.id == airport.id &&
     test_airport.name == airport.name &&
     test_airport.city == airport.city &&
     test_airport.country == airport.country &&
     test_airport.latitude == airport.latitude &&
     test_airport.longitude == airport.longitude &&
     RoutesAreEqual(test_airport.connected_airports, airport.connected_airports);
}

TEST_CASE("loadAirports basic") {
  cs225::FlightGraph graph;
  REQUIRE(graph.loadAirports("tests/datasets/goroka_single.dat") == 1);
}

TEST_CASE("loadAirports small") {
  cs225::FlightGraph graph;
  REQUIRE(graph.loadAirports("tests/datasets/airports_small.dat") == 3);
  auto airports = graph.getAirports();
  Airport airport1{1, "Goroka Airport", "Goroka", "Papua New Guinea", -6.081689834590001, 145.391998291, {}};
  REQUIRE(AirportsAreEqual(airport1, *airports[0]));
  Airport airport2{2, "Madang Airport", "Madang", "Papua New Guinea", -5.20707988739, 145.789001465, {}};
  REQUIRE(AirportsAreEqual(airport2, *airports[1]));
  Airport airport3{3, "Mount Hagen Kagamuga Airport", "Mount Hagen", "Papua New Guinea", -5.826789855957031, 144.29600524902344, {}};
  REQUIRE(AirportsAreEqual(airport3, *airports[2]));
}

TEST_CASE("Graph Constructor small") {
  cs225::FlightGraph graph("tests/datasets/airports_small.dat", "tests/datasets/airports_small_routes1.dat");
  auto airports = graph.getAirports();
  Route airport1_route1{1, 2, 0};
  Route airport1_route2{1, 3, 0};
  Airport airport1{
    1, "Goroka Airport", "Goroka", "Papua New Guinea", -6.081689834590001, 145.391998291,
    {&airport1_route1, &airport1_route2}
  };
  REQUIRE(AirportsAreEqual(airport1, *airports[0]));
  Airport airport2{
    2, "Madang Airport", "Madang", "Papua New Guinea", -5.20707988739, 145.789001465,
    {&airport1_route1}
  };
  REQUIRE(AirportsAreEqual(airport2, *airports[1]));
  Airport airport3{
    3, "Mount Hagen Kagamuga Airport", "Mount Hagen", "Papua New Guinea", -5.826789855957031, 144.29600524902344,
    {&airport1_route2}
  };
  REQUIRE(AirportsAreEqual(airport3, *airports[2]));
}

TEST_CASE("Dijkstra_pair") {
  cs225::FlightGraph graph("tests/datasets/pair.dat", "tests/datasets/pair_route.dat");
  auto seattle = graph.getAirport(3577);
  auto kansas_city = graph.getAirport(3458);
  auto path = graph.ShortestPath(kansas_city, seattle);
  for (const auto r : path) { // should only run once
    REQUIRE(r->source_id == 3458);
    REQUIRE(r->dest_id == 3577);
  }
}

TEST_CASE("Dijkstra_Disney1") {
  cs225::FlightGraph graph("tests/datasets/usa/airports.dat", "tests/datasets/usa/routes1.dat");
  auto ohare = graph.getAirport(3830);
  auto orlando = graph.getAirport(3878);
  auto path = graph.ShortestPath(ohare, orlando);
  size_t i = 0;
  for (const auto r : path) {
    if (i == 0) {
      REQUIRE(r->source_id == 3830);
      REQUIRE(r->dest_id == 3458);
    } else if (i == 1) {
      REQUIRE(r->source_id == 3458);
      REQUIRE(r->dest_id == 3577);
    } else if (i == 2) {
      REQUIRE(r->source_id == 3577);
      REQUIRE(r->dest_id == 3878);
    }
    i++;
  }
}

TEST_CASE("Dijkstra_Disney2") {
  cs225::FlightGraph graph("tests/datasets/usa/airports.dat", "tests/datasets/usa/routes2.dat");
  auto ohare = graph.getAirport(3830);
  auto orlando = graph.getAirport(3878);
  auto path = graph.ShortestPath(ohare, orlando);
  size_t i = 0;
  for (const auto r : path) {
    if (i == 0) {
      REQUIRE(r->source_id == 3830);
      REQUIRE(r->dest_id == 3458);
    } else if (i == 1) {
      REQUIRE(r->source_id == 3458);
      REQUIRE(r->dest_id == 3878);
    }
    i++;
  }
}

TEST_CASE("Dijkstra_Disney_Detour") {
  cs225::FlightGraph graph("tests/datasets/usa/airports.dat", "tests/datasets/usa/routes3.dat"); // a direct path exists but we will make it a really long path due to say weather detour
  auto ohare = graph.getAirport(3830);
  auto orlando = graph.getAirport(3878);

  for (Route*& r : orlando.connected_airports) {
    if (r->source_id == 3830 && r->dest_id == 3878) r->distance = r->distance * 3;
  }

  auto path = graph.ShortestPath(ohare, orlando);
  size_t i = 0;
  for (const auto r : path) {
    if (i == 0) {
      REQUIRE(r->source_id == 3830);
      REQUIRE(r->dest_id == 3458); // OHARE to Kansas
    } else if (i == 1) {
      REQUIRE(r->source_id == 3458);
      REQUIRE(r->dest_id == 3878); // Kansas to Orlando
    }
    i++;
  }
}

TEST_CASE("Dijkstra_NoPath") {
  cs225::FlightGraph graph("tests/datasets/usa/airports.dat", "tests/datasets/usa/routes2.dat");
  auto ohare = graph.getAirport(3830);
  auto hunter_army = graph.getAirport(3441);
  auto path = graph.ShortestPath(ohare, hunter_army);
  REQUIRE(path.size() == 0);
}


TEST_CASE("Depth-First-Search No Routes") {
  cs225::FlightGraph graph;
  graph.loadAirports("tests/datasets/airports_small.dat");
  auto airports = graph.getAirports();
  REQUIRE(graph.NumComponents() == 3);
  std::unordered_map<unsigned long, unsigned long> visited_airports = graph.DFS();
  REQUIRE(visited_airports[1] == 1);
  REQUIRE(visited_airports[2] == 2);
  REQUIRE(visited_airports[3] == 3);
}

TEST_CASE("Depth-First-Search 2 Routes 1 Source") {
  cs225::FlightGraph graph("tests/datasets/airports_small.dat", "tests/datasets/airports_small_routes1.dat");
  auto airports = graph.getAirports();
  REQUIRE(graph.NumComponents() == 1);
  std::unordered_map<unsigned long, unsigned long> visited_airports = graph.DFS();
  REQUIRE(visited_airports[1] == 1);
  REQUIRE(visited_airports[2] == 1);
  REQUIRE(visited_airports[3] == 1);
}


TEST_CASE("Depth-First-Search 2 Routes 2 Sources") {
  cs225::FlightGraph graph("tests/datasets/airports_small.dat", "tests/datasets/airports_small_routes2.dat");
  auto airports = graph.getAirports();
  REQUIRE(graph.NumComponents() == 1);
  std::unordered_map<unsigned long, unsigned long> visited_airports = graph.DFS();
  REQUIRE(visited_airports[1] == 1);
  REQUIRE(visited_airports[2] == 1);
  REQUIRE(visited_airports[3] == 2);
}

TEST_CASE("Longitude Latitude Conversion Easy") {
  std::pair<int, int> coords = GeoToCoord(50, 50, 1000, 1000);
  int x = coords.first;
  int y = coords.second;
  REQUIRE(x == 638);
  REQUIRE(y == 339);

  coords = GeoToCoord(8.8951, -77.0364, 1000, 1000);
  x = coords.first;
  y = coords.second;
  REQUIRE(x == 524);
  REQUIRE(y == 846);

  coords = GeoToCoord(139.839478, 35.652832, 1000, 1000);
  x = coords.first;
  y = coords.second;
  REQUIRE(x == 888);
  REQUIRE(y == 393);
}

TEST_CASE("Longitude Latitude Conversion Edges") {
  std::pair<int, int> coords = GeoToCoord(180, 0, 2000, 2000);
  int x = coords.first;
  int y = coords.second;
  REQUIRE(x == 0);
  REQUIRE(y == 1000);

  coords = GeoToCoord(-180, 0, 2000, 2000);
  x = coords.first;
  y = coords.second;
  REQUIRE(x == 0);
  REQUIRE(y == 1000);
}