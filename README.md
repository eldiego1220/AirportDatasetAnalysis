# ddelca3-raxiao2-evanoff3-shreyav3
Final Project for ddelca3-raxiao2-evanoff3-shreyav3

# File Structure

## /dataset
Contains airport and route data to be used.

## /make
Includes make targets for compilation.

## /src 
Contains source files for the project.

## /tests
Contains Catch2 test suite, test data, and unit tests.

# How to Run

## Test suite
`make test`

## Main project
`make`

or

`make project`

to run

`./project [airports_path] [routes_path] airport_source_id airport_destination_id draw_all_paths_flag [output_image_name]`

examples:

`./project 144 6 0`

`./project 144 6 1 example.png`

`./project dataset/airports.dat.txt dataset/routes.dat.txt 144 6 0 out.png`