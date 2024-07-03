# Development Log

## Mar 28 - Apr 3:

### Goals Outlined
- Download data
- Organize project file structure
- Clean the entries
- Add make targets
- Begin outlining adjacency list graph structure

### Tasks Completed
#### Richard
  - Created file structure along with boilerplate code 4/1
  - Created modified cs225.mk and Makefile for easy compilation 4/1

#### Ben
  - Began basic implementation of FlightGraph class with basic code to load vertices from csv 4/3

### Problems
  - None so far
  
### Next Week Goals
- Create the adjacency list structure for the graph
- Begin reading Dijkstra’s algorithm

## April 4 -10

### Tasks completed

#### Ben
Implemented basic FlightGraph constructor to build an adjacency list representation from routes and airports csv files.
Still needs to do validation on coordinates and to calculate distances for edges (idk do we do some fancy math cuz it's a globe?)

## April 11 - 17

### Tasks completed

#### Ben

Implemented Dijkstra's algorithm on our FlightGraph class.

#### Richard

Fixed Makefile compilation/linking errors. Implemented Depth-first-search on the FlightGraph.

## April 18 - 22

### Tasks completed

#### Ben

Added more test cases for Dijkstra's algorithm.

#### Richard

Added more test cases for DFS on the FlightGraph.

### Next Week Goals

- Finish Strongly Connected Components Algorithm and write test cases for it
- Implement Visualization through DFS

## April 25 - 29

### Tasks completed

#### Shreya

Started on Strongly Connected Components function as well as helper function reverse for the same.

### Next Week Goals

- Finish Strongly Connected Components Algorithm and write test cases for it
- Implement Visualization through DFS
