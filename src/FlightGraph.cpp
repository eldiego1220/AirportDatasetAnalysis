#include <fstream>
#include <stdexcept>
#include <queue>
#include <stack>
#include <algorithm>

#include "FlightGraph.h"
#include "map_utils.h"

using namespace cs225;

FlightGraph::FlightGraph(std::string airports, std::string routes) {
    loadAirports(airports);
    loadRoutes(routes);
}

std::vector<FlightGraph::Airport*> FlightGraph::getAirports() {
    std::vector<FlightGraph::Airport*> result;
    for (auto it = airports_.begin(); it != airports_.end(); ++it) {
        result.push_back(&(it->second));
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<FlightGraph::Route*> FlightGraph::getRoutes() {
    std::vector<FlightGraph::Route*> result;
    for (const auto &r : routes_) result.push_back(r);
    return result;
}

size_t FlightGraph::loadAirports(std::string filename, size_t max) {
    std::ifstream ifs(filename);
    if (!ifs.is_open())
        throw std::runtime_error("Could not open flight data from " + filename);
    
    std::string line;
    size_t counter = 0;
    size_t fail_count = 0;
    while (ifs.good() && counter < max && !ifs.eof()) {
        std::getline(ifs, line);
        if (line.empty()) continue;
        if (ifs.fail())
            throw std::runtime_error("Error reading data from file encountered at line " + std::to_string(counter+fail_count));
        
        FlightGraph::Airport airport_e;
        try {
            size_t comma = line.find(',');
            airport_e.id = std::stol( line.substr(0, comma) );
            line.erase(0, comma+1);

            comma = line.find(',');
            airport_e.name = line.substr(1, comma-2);
            line.erase(1, comma+1);

            comma = line.find(',');
            airport_e.city = line.substr(1, comma-2);
            line.erase(1, comma+1);

            comma = line.find(',');
            airport_e.country = line.substr(1, comma-2);
            line.erase(1, comma+1);

            // ignore the next two fields
            comma = line.find(',');
            line.erase(1, comma+1);
            comma = line.find(',');
            line.erase(0, comma+1); // start from 0 to get rid of trailing " before reading gps cords (doubles)

            comma = line.find(',');
            airport_e.latitude = stod( line.substr(0, comma) );
            line.erase(0, comma+1);
            comma = line.find(',');
            airport_e.longitude = stod( line.substr(0, comma) );

            if (!isValidGPS(airport_e.latitude, airport_e.longitude)) throw std::runtime_error("Invalid GPS value found");
        
            airports_[airport_e.id] = airport_e;
        } catch (const std::exception &e) {
            fail_count++;
            continue;
        }
        counter++;
    }
    return counter;
}

size_t FlightGraph::loadRoutes(std::string filename, size_t max) {
    std::ifstream ifs(filename);
    if (!ifs.is_open())
        throw std::runtime_error("Could not open route data from " + filename);
    
    std::string line;
    size_t counter = 0;
    while (ifs.good() && counter < max && !ifs.eof()) {
        std::getline(ifs, line);
        if (line.empty()) continue;
        if (ifs.fail())
            throw std::runtime_error("Error reading data from file");

        FlightGraph::Route route;
        const std::string line_orig = line;
        try {
            line.erase(0, line.find(',')+1);
            line.erase(0, line.find(',')+1);
            line.erase(0, line.find(',')+1);
            route.source_id = stol (line.substr(0, line.find(',')));
            line.erase(0, line.find(',')+1);
            line.erase(0, line.find(',')+1);
            route.dest_id = stol (line.substr(0, line.find(',')));
        } catch (const std::exception &e) {
            std::cerr << "Could not parse line " << line_orig << std::endl;
            continue;
        }
        
        if (airports_.find(route.source_id) == airports_.end() || airports_.find(route.dest_id) == airports_.end()) {
            std::cerr << "could not find airport " << route.source_id << " and/or " << route.dest_id << std::endl;
            continue;
        }

        Airport &source = airports_[route.source_id];
        Airport &dest = airports_[route.dest_id];

        double route_dist = calcDistance(std::make_pair(source.latitude, source.longitude), std::make_pair(dest.latitude, dest.longitude));

        FlightGraph::Route *heap_route = new FlightGraph::Route(route);
        routes_.push_back(heap_route);

        source.connected_airports.push_back(heap_route);
        dest.connected_airports.push_back(heap_route);

        counter++;
    }
    return counter;
}

typedef std::priority_queue<unsigned long,std::vector<unsigned long>, FlightGraph::compare_distances> dist_q;

std::list<FlightGraph::Route*> FlightGraph::ShortestPath(Airport source, Airport destination) {
    FlightGraph path;
    std::unordered_map<unsigned long, double> distances; // key=id, val=distance from source
    std::vector<unsigned long> all_airports;
    std::unordered_map<unsigned long, Route*> p_routes; // K: vertex id, V: the shortest route from the predecessor to the key vertex
    for (auto it = airports_.begin(); it != airports_.end(); it++) {
        distances[it->first] = std::numeric_limits<double>::max();
        all_airports.push_back(it->first); // to save time building priority queue, is there fancy syntax to optomize this out?
    }
    distances[source.id] = 0;

    auto q = dist_q(&distances, all_airports);
    while(!q.empty()) {
        Airport u = airports_[q.top()];
        addAirport(u);
        for(const auto &connected : u.connected_airports) {
            if (path.airports_.find(connected->dest_id) == path.airports_.end()) {
                if (distances.at(u.id) + connected->distance < distances.at(connected->dest_id)) {
                    distances[connected->dest_id] = distances.at(u.id) + connected->distance;
                    p_routes[connected->dest_id] = connected;
                }
            }
        }
        q.pop();
    }
    std::list<Route*> routes;
    unsigned long cur_id = destination.id;
    while (cur_id != source.id) {
        try {
            routes.push_front(p_routes.at(cur_id));
            cur_id = p_routes.at(cur_id)->source_id;
        } catch (const std::exception &e) {
            return std::list<Route*>(); // no path from destination to source
        }
    }

    return routes;
}

std::unordered_map<unsigned long, unsigned long> FlightGraph::DFS() {
    std::unordered_map<unsigned long, unsigned long> visited_airports;
    for (Airport* airport : getAirports()) {
        if (visited_airports.count(airport->id) == 0) {
            DFS(airport, visited_airports);
        }
    }
    return visited_airports;
}

int FlightGraph::NumComponents() {
    std::unordered_map<unsigned long, unsigned long> visited_airports;
    int num_components = 0;
    for (Airport* airport : getAirports()) {
        if (visited_airports.count(airport->id) == 0) {
            ++num_components;
            DFS(airport, visited_airports);
        }
    }
    return num_components;
}

void FlightGraph::DFS(Airport* source, std::unordered_map<unsigned long, unsigned long>& visited_airports) {
    visited_airports.insert({source->id, source->id});

    std::stack<Route*> routes_to_visit;
    for (Route* route : source->connected_airports) {
        routes_to_visit.push(route);
    }

    while (!routes_to_visit.empty()) {
        // Get the next route to process
        Route* next_route_to_visit = routes_to_visit.top();
        routes_to_visit.pop();
        // If destination of route is visited, continue
        // Else, visit the destination and 
        // add the neighboring edges of destination to be visited
        unsigned long dest_id = next_route_to_visit->dest_id;
        if (visited_airports.count(dest_id) > 0) {
            continue;
        }

        visited_airports.insert({dest_id, next_route_to_visit->source_id});

        Airport dest_airport = airports_[dest_id];
        for (Route* route : dest_airport.connected_airports) {
            routes_to_visit.push(route);
        }
    }
}

void FlightGraph::DrawDFS(Airport* source, PNG* map, const HSLAPixel& path_color) {
    std::unordered_map<unsigned long, unsigned long> visited_airports;
    visited_airports.insert({source->id, source->id});
    int width = map->width();
    int height = map->height();

    std::stack<Route*> routes_to_visit;
    for (Route* route : source->connected_airports) {
        routes_to_visit.push(route);
    }

    while (!routes_to_visit.empty()) {
        // Get the next route to process
        Route* next_route_to_visit = routes_to_visit.top();

        unsigned long source_id = next_route_to_visit->source_id;
        unsigned long dest_id = next_route_to_visit->dest_id;
        const Airport& source_airport = airports_.at(source_id);
        const Airport& dest_airport = airports_.at(dest_id);
        std::pair<int, int> source_coords = GeoToCoord(source_airport.longitude, source_airport.latitude, width, height);
        std::pair<int, int> dest_coords = GeoToCoord(dest_airport.longitude, dest_airport.latitude, width, height);
        draw_line_on_map(source_coords.first, source_coords.second, dest_coords.first, dest_coords.second, map, path_color, false);
        
        routes_to_visit.pop();
        // If destination of route is visited, continue
        // Else, visit the destination and 
        // add the neighboring edges of destination to be visited
        if (visited_airports.count(dest_id) > 0) {
            continue;
        }

        visited_airports.insert({dest_id, next_route_to_visit->source_id});

        for (Route* route : dest_airport.connected_airports) {
            routes_to_visit.push(route);
        }
    }
}

//helper function for findSCC, will reverse the original graph
void FlightGraph::reverse()
{
    //reverse source ids and destination ids
    for (Route* route: getRoutes()) 
    {
        unsigned long temp = route->source_id;
        route->source_id = route->dest_id;
        route->dest_id = temp;
    }
}

//find strongly connected components
std::vector<FlightGraph::Airport> FlightGraph::findSCC(FlightGraph graph)
{
    /*Step 1: perform DFS on the original graph*/
    //set a vector to keep track of all of the airports that have been visited
    std::unordered_map<unsigned long, unsigned long> visited;
    //populate map with visited airports and call DFS
    for (Airport* airport: graph.getAirports()) 
    {
        if (visited.find(airport->id) != visited.end())
        {
            continue;
        }
        DFS(airport, visited);
    }
    /*Step 2: reverse the graph*/
    //FlightGraph reversed = graph.reverse();
    graph.reverse();

    /*Step 3: perform DFS on the reversed graph to obtain SCCs*/
    for (Airport* airport: graph.getAirports()) 
    {
        if (visited.find(airport->id) != visited.end())
        {
            continue;
        }
        DFS(airport, visited);
    }
    
    //obtain the last airport that was visited, store in start
    Airport lastAirport = getAirport(visited.end()->first);
    Airport* start = &lastAirport;
    //clear the visited map
    visited.clear();

    //perform DFS starting from start
    DFS(start, visited);
    //store SCC components
    std::vector<Airport> strongConCom;
    for(auto i = visited.begin(); i != visited.end(); ++i){
        strongConCom.push_back(getAirport(i->first));
    }

    std::cout << "Strongly Connected Components:" << std::endl;
    for(unsigned i = 0; i < strongConCom.size(); i++){
        std::cout << strongConCom[i] << "\n";
    }
    
    //return value goes here
    return strongConCom;
     
}

void FlightGraph::DrawPaths(const std::vector<FlightGraph::Route*>& routes, PNG* map, const HSLAPixel& path_color, bool bold) {
    int width = map->width();
    int height = map->height();
    for (const FlightGraph::Route* route : routes) {
        unsigned long source_id = route->source_id;
        unsigned long dest_id = route->dest_id;
        const Airport& source_airport = airports_.at(source_id);
        const Airport& dest_airport = airports_.at(dest_id);
        std::pair<int, int> source_coords = GeoToCoord(source_airport.longitude, source_airport.latitude, width, height);
        std::pair<int, int> dest_coords = GeoToCoord(dest_airport.longitude, dest_airport.latitude, width, height);
        draw_line_on_map(source_coords.first, source_coords.second, dest_coords.first, dest_coords.second, map, path_color, bold);
    }
}
