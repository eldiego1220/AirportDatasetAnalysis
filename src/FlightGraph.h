#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include "PNG.h"

namespace cs225 {

    class FlightGraph {
        public:
            // Routes (edges) connect airports (vertices) on our map (graph)
            struct Route {
                unsigned long source_id;
                unsigned long dest_id;
                double distance;
            };

            // Airports represent vertices in our graph
            struct Airport {
                unsigned long id;
                std::string name;
                std::string city;
                std::string country;
                double latitude;
                double longitude;

                std::list<Route*> connected_airports; // vertex adjacency list
            };
            class compare_distances {
                std::unordered_map<unsigned long, double> *dists;
                public:
                    compare_distances(std::unordered_map<unsigned long, double> *distances) {
                        dists = distances;
                }
        
                    bool operator() (const unsigned long& lhs, const unsigned long &rhs) const {
                        return dists->at(lhs) > dists->at(rhs);
                    }
            };

            FlightGraph() = default;
            /* Paramaterized constructor to build a graph from csv files of airports and routes
             * @param the filename for the airports (vertices)
             * @param the filename for the routes (edges)
             */
            FlightGraph(std::string airports, std::string routes);

            // our flight graph "owns" the routes
            ~FlightGraph() {
                for (const auto &route : routes_) delete route;
            }
            
            // Gets pointers to all the airports on the map
            std::vector<Airport*> getAirports();
            Airport getAirport(unsigned long id) {
                return airports_.at(id);
            }

            // adds an airport to the flight graph
            void addAirport(const Airport newAirport) {
                airports_[newAirport.id] = newAirport;
            }

            // Gets pointers to all the routes on the map
            std::vector<Route*> getRoutes();

            /* Loads the airports into vertex map from a csv file such as 
             * https://raw.githubusercontent.com/jpatokal/openflights/master/data/airports.dat
             * !! Will not add duplicate vertices !!
             * @param the filename
             * @param the maximum # of airports to load onto the map
             * @return the # of airports loaded
             */
            size_t loadAirports(std::string filename, size_t max = std::numeric_limits<size_t>::max());
            
            /* Loads routes connecting airports from a csv file such as 
             * https://raw.githubusercontent.com/jpatokal/openflights/master/data/routes.dat
             * !! Will not add duplicate edges or edges referencing airports not currently on the map !!
             * @param the filename
             * @param the maximum # of routes to load onto the map
             * @return the # of routes loaded
             */
            size_t loadRoutes(std::string filename, size_t max = std::numeric_limits<size_t>::max());

            /* Utilize Dijkstra's algorithm to find the shorted path in terms of real distance between two airports
             * @param the airport used for initial take off 
             * @param the destination airport
             * @return a pointer to each of the edges in order from start to end
             */
            std::list<Route*> ShortestPath(Airport source, Airport destination);

            /* Depth First Search. Non-recursive DFS to traverse the entire graph and output onto a map
             */
            std::unordered_map<unsigned long, unsigned long> DFS();

            void DrawDFS(Airport* source, PNG* map, const HSLAPixel& path_color);

            int NumComponents();

            void reverse();
            std::vector<Airport> findSCC(FlightGraph graph);

            /* Draws routes between airports onto a Mercator Projected map
             * @param routes: vector of routes to be drawn
             * @param map: PNG pointer to the map being drawn
             * @param path_color: HSLAPixel specifying the color of the paths between airports
             * @param bold: boolean to specify whether to bold or not bold the airports on the map
             */
            void DrawPaths(const std::vector<Route*>& routes, cs225::PNG* map, const HSLAPixel& path_color, bool bold);


        private:
            /* Graph vertices.
             * Key: the airport id
             * Value: the complete RouteGraph::Airport
             */
            std::unordered_map<unsigned long, Airport> airports_;
            // Graph Edges
            std::vector<Route*> routes_;

            /* Approximate the distance between two pairs of coordinates using the Haversine formula
             * @param a pair describing the first point's latitude and longitude respectively
             * @param a pair describing the other point's latitude and longitude respectively
             * @return the approx. distance between the two points
             */
            double calcDistance(std::pair<double,double> airport1, std::pair<double,double> airport2) {
                double radius = 6371;
                double lat_dif = (airport2.first - airport1.first) * (M_PI/180);
                double long_dif = (airport2.second - airport1.second) * (M_PI/180);
                double param = (std::sin(lat_dif/2.0) * std::sin(lat_dif/2.0)) + std::cos(airport1.first) * std::cos(airport2.first) * (std::sin(long_dif/2.0) * std::sin(long_dif/2.0)); // parameter to arcsine from simplified wikipedia formula
                return 2 * radius * std::asin(std::sqrt(param));
            }

            bool isValidGPS(double latitude, double longitude) {
                return latitude >= -90 && latitude <= 90 && longitude >= -180 && longitude <= 180;
            }

            void DFS(Airport* source, std::unordered_map<unsigned long, unsigned long>& visited_airports);

            friend std::ostream& operator<<(std::ostream &os, FlightGraph::Airport a) {
                os << "Airport " << &a << std::endl;
                os << "ID: " << a.id << std::endl;
                os << "Name: " << a.name << std::endl;
                os << "City: " << a.city << std::endl;
                os << "Country: " << a.country << std::endl;
                os << "Latitiude: " << a.latitude << std::endl;
                os << "Longitude: " << a.longitude << std::endl;
                os << "Connections: " << a.connected_airports.size() << std::endl;
                return os;
            }

            friend std::ostream& operator<<(std::ostream &os, FlightGraph::Route r) {
                os << "Source: " << r.source_id << std::endl;
                os << "Destination: " << r.dest_id << std::endl;
                os << "Distance: " << r.distance;
                return os;
            }
    };
}
