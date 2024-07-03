# Proposal

## Leading Question
For our project, we will be using airport and route information for OpenFlight to analyze shortest paths between two airports, identify clusters of flights, and visualize the mentioned analysis. As an end goal, this project aims to create a search tool to efficiently process and consume flight data for common flight questions.


## Dataset Acquisition
The dataset we are using is the OpenFlight database of airport, airline and route data (https://openflights.org/data.html).


## Data format
The data is partitioned as a CSV file called airports.dat with the columns as Airport ID, Name, City, Country, and other positional information. It contains 14110 data points, and we will be using all of the data. In addition, we will be using the routes.dat file to include the connected routes between airports, which has 67663 data points. However, we will be using a subset of the data when projecting onto a map. The subset will be the origin airport, and a series of connected routes to the destination.


## Data correction
There is not much correcting to do other than if any row in the airports or routes file  has a NULL value then to discard it or if the coordinates are out of bounds of the Earth, then to discard it as well. If a route leads to an invalid destination, it will also be discarded.


## Data storage
To efficiently run the proposed algorithms, our airport data will need to be in a directed graph data structure. Since the dataset is very large, memory will be constrained. Therefore, an efficient directed graph structure will be the adjacency list implementation, which only requires O(n) memory. We will use geographic distance as the weights in our graph.


## Algorithm
To successfully complete this project we will need to implement a DFS traversal that we will use to visualize the data, a strongly connected components algorithm to find out the density of flights in certain regions, and a shortest path algorithm to find out how to minimize time/gas expenditure.


## Visualization (DFS)
We will utilize a depth first search to traverse the graph and output the paths. This will take O(n) time as we want to visit each node and avoid traversing vertices more than once.


## Shortest Path
We expect to use Dijkstra’s algorithm to find the shortest path between departure and destination airports. Dijkstra’s algorithm is known to run in O(ElogV) time.


## Strongly Connected Components
We will investigate the implementation of kusajaru’s algorithm utilizing our DFS implementation to find the most important airports in O(n).


## Timeline
With the airport and route dataset already discovered, the first week will be spent curating it for use by removing invalid entries. We plan to complete them by 04/01/22. Afterwards, we plan to create the directed graph data structure and implement the Shortest Path algorithm which should be completed by 04/15/22. The Strongly Connected Components algorithm will be implemented afterwards with a goal of 04/22/22. Finally, the Visualization (DFS) of the data should be completed by 04/29/22. This gives us a week to write the final report and make the presentation before the final deadline of 05/06/2022.

