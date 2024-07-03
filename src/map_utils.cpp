#include <cmath>
#include <utility>
#include "map_utils.h"

void draw_airport_on_map(const cs225::FlightGraph::Airport& airport, PNG* map) {
  std::pair<int, int> source_coords = GeoToCoord(airport.longitude, airport.latitude, map->width(), map->height());
  int x = source_coords.first;
  int y = source_coords.second;
  if (x < 3) x += 3;
  if (y < 3) y += 3;
  HSLAPixel yellow(60, 1, 0.5, 1);
  map->getPixel(x, y) = yellow;
  map->getPixel(x, y + 1) = yellow;
  map->getPixel(x, y - 1) = yellow;
  map->getPixel(x + 1, y) = yellow;
  map->getPixel(x - 1, y) = yellow;
  map->getPixel(x + 1, y + 1) = yellow;
  map->getPixel(x - 1, y - 1) = yellow;
  map->getPixel(x - 1, y + 1) = yellow;
  map->getPixel(x + 1, y - 1) = yellow;
  map->getPixel(x, y + 2) = yellow;
  map->getPixel(x, y - 2) = yellow;
  map->getPixel(x + 2, y) = yellow;
  map->getPixel(x - 2, y) = yellow;
  map->getPixel(x, y + 3) = yellow;
  map->getPixel(x, y - 3) = yellow;
  map->getPixel(x + 3, y) = yellow;
  map->getPixel(x - 3, y) = yellow;
}

void draw_dot_on_map(int x, int y, PNG* map, bool bold) {
  if (x < 3) x += 3;
  if (y < 3) y += 3;
  HSLAPixel red(0,1,0.5,1);
  HSLAPixel bolded(300, 1, 0.25, 1);
  map->getPixel(x, y) = red;
  map->getPixel(x, y + 1) = red;
  map->getPixel(x, y - 1) = red;
  map->getPixel(x + 1, y) = red;
  map->getPixel(x - 1, y) = red;
  if (bold) {
    map->getPixel(x, y) = bolded;
    map->getPixel(x, y + 1) = bolded;
    map->getPixel(x, y - 1) = bolded;
    map->getPixel(x + 1, y) = bolded;
    map->getPixel(x - 1, y) = bolded;
    map->getPixel(x + 1, y + 1) = bolded;
    map->getPixel(x - 1, y - 1) = bolded;
    map->getPixel(x - 1, y + 1) = bolded;
    map->getPixel(x + 1, y - 1) = bolded;
    map->getPixel(x, y + 2) = bolded;
    map->getPixel(x, y - 2) = bolded;
    map->getPixel(x + 2, y) = bolded;
    map->getPixel(x - 2, y) = bolded;
    map->getPixel(x, y + 3) = bolded;
    map->getPixel(x, y - 3) = bolded;
    map->getPixel(x + 3, y) = bolded;
    map->getPixel(x - 3, y) = bolded;
  }
}

void draw_line_on_map(int x1, int y1, int x2, int y2, PNG* map, const HSLAPixel& path_color, bool bold) {
  if (x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  double slope = (y2 - y1) * 1.0 / (x2 - x1);
  for (int x = x1; x <= x2; ++x) {
    int y = slope * (x - x1) + y1;
    map->getPixel(x, y) = path_color;
  }

  if (y1 > y2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  for (int y = y1; y <= y2; ++y) {
    int x = (y - y1) / slope + x1;
    map->getPixel(x, y) = path_color;
  }
  draw_dot_on_map(x1, y1, map, bold);
  draw_dot_on_map(x2, y2, map, bold);
}

std::pair<int, int> GeoToCoord(double longitude, double latitude, int width, int height) {
  int x = static_cast<int>(width * (180.0 + longitude) / 360.0) % width;
  double lat_rad = latitude * pi / 180.0;
  double y_merc = std::log(std::tan((lat_rad / 2.0) + (pi / 4.0)));
  int y = (height / 2.0) - (width * y_merc / (2.0 * pi));

  return { x, y };
}
