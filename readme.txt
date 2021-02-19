1. Luke Julian, 10824615
2. C++
3. Linux
4. There are two methods I attempted at implementing. I believe the first violated some requirements for the project,
but it still works and was useful in getting my algorithm figured out for my actual method. This first method kept track
of the cummulative path in a vector as the second element in a pair; this worked but appeared to violate the requirement
of not keeping track of the explored cities.  For the second method - the true method - vectors were used rather than
maps to represent connections and cities. Paths were updated not by storing a complete path, but by linking each node to
its parent.  The depth first search algorithm was used in pre-processing to check if there was no path from the start to
the destination.  If there is indeed a path from start state to goal state, then the uninformed cost algorithm was used
to find to optimal path.
functions were defined before main if they were used in main.  All function code is found after main.  These are in the
order they are used in main.
The node class is defined in the node.h file.  This file also includes the struct used to overload the comparison used by
the priotity queue.
5. Firstly, input files should placed in the same directory as find_route.cpp. In linux, run the command "g++ -o
find_route find_route.cpp" to compile. Run the command "./find_route INPUT_FILE START_CITY END_CITY" to find the optimal
route from START_CITY to END_CITY from the input file INPUT_FILE.
