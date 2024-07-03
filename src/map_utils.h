#include <utility>
#include "FlightGraph.h"
#include "PNG.h"

using namespace cs225;

constexpr double pi = 3.14159265358979323846;

/* Draws an airport onto a Mercator Projected map
 * @param airport: Airport to be drawn
 * @param map: PNG pointer to the map being drawn
 */
void draw_airport_on_map(const cs225::FlightGraph::Airport& airport, PNG* map);

/* Draws a pixel onto a map
 * @param x: x position of pixel to be drawn
 * @param y: y position of pixel to be drawn
 */
void draw_dot_on_map(int x, int y, PNG* map);

/* Draws a line onto a map
 * @param x1: x coordinate of initial point of line
 * @param x1: x coordinate of initial point of line
 * @param x2: x coordinate of end point of line
 * @param y2: y coordinate of end point of line
 * @param map: PNG pointer to map being drawn
 * @param path_color: HSLAPixel specifying the color of the paths between airports
 * @param bold: boolean to specify whether to bold or not bold the airports on the map
 */
void draw_line_on_map(int x1, int y1, int x2, int y2, PNG* map, const HSLAPixel& path_color, bool bold);

/* Converts longitude and latitude to x and y coordinates on a Mercator Projected map
 * @param longitude: longitude value in degrees
 * @param latitude: latitude value in degrees
 * @param width: width of map
 * @param height: height of map
 */
std::pair<int, int> GeoToCoord(double longitude, double latitude, int width, int height);