# Tempus

## adj_list.h

### Class Definition

The `AdjList` class has the following members:

- **Public Member Functions:**
  - **Constructor (`explicit AdjList(int size)'):**
    - Constructs an instance of `AdjList` with a specified size.
    - Initializes the adjacency list (`graph`) as an array of vectors of pairs.

- **Public Member Functions:**
  - `addFromFile(const std::string& path)`: Reads commands and edge information from a file and updates the graph accordingly.
  - `printGraph() const`: Prints the edges of the graph to the console.

- **Private Member Variables:**
  - `graph`: An array of vectors of pairs representing the adjacency list.
  - `size`: An integer representing the size of the graph.

- **Private Member Functions:**
  - `findEdge(int source, int destination, int time) const`: Searches for an edge in the adjacency list.
  - `addEdge(int source, int destination, int time) const`: Adds an undirected edge to the graph.
  - `deleteEdge(int source, int destination, int time) const`: Deletes an undirected edge from the graph.
  - `resizeGraph(int newSize)`: Resizes the graph to accommodate a larger number of vertices.
  - `sortByTime() const`: Sorts the edges of each vertex in the graph based on the time attribute.
  - `static bool compareTime(std::pair<int, int>, std::pair<int, int>)`: A static function used as a custom comparison function for sorting edges based on their time attribute.


## adj_list.cpp

### `findEdge`

This function searches for an edge in the adjacency list.

- Parameters: `source`, `destination`, and `time`.
- Returns an iterator to the found edge or the end iterator if not found.

### `addEdge`

This function adds an undirected edge to the graph.

- Parameters: `source`, `destination`, and `time`.
- Checks if the edge already exists before adding it.

### `deleteEdge`

This function deletes an undirected edge from the graph.

- Parameters: `source`, `destination`, and `time`.
- Checks if the edge exists before deletion.

### `resizeGraph`

This function resizes the graph to accommodate a larger number of vertices.

- Parameter: `newSize`.
- Allocates a new array of vectors and copies the existing data.

### `printGraph`

This function prints the edges of the graph to the console.

- It uses a nested loop to iterate through the adjacency list and prints each edge.

### `addFromFile`

This function reads commands and edge information from a file and updates the graph accordingly.

- Parameter: `path` (path to the input file).
- The file is expected to contain commands ("add" or "delete") followed by source, destination, and time values.

### `sortByTime`

This function sorts the edges of each vertex in the graph based on the time attribute.

- It uses `std::sort` and a custom comparison function (`compareTime`).

### `compareTime`

A static function used as a custom comparison function for sorting edges based on their time attribute.
