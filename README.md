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

The `addFromFile` method reads edge data from a specified file and populates the adjacency list (`edges`) based on the grouped data obtained from `sortBatch`. It has the following key steps:

1. **Reading and Preprocessing:**
   - Reads edge data from the file, extracting commands, source, destination, and time information.
   - Identifies the maximum vertex value encountered (`maxEdge`).

2. **Sorting and Grouping:**
   - Compares the number of unique source and destination vertices.
   - Determines the `sortFlag` based on the comparison result.
   - Creates vectors (`sourceAdds`, `destinationAdds`, `timeAdds`) to store edge data.

3. **Grouping with `sortBatch`:**
   - Calls `sortBatch` to group the edges based on the determined `sortFlag`.
   - Receives the grouped data in the `groupedData` hash map.

4. **Adding Grouped Edges to Adjacency List:**
   - Iterates through the `groupedData` hash map.
   - For each source vertex and its associated edges, adds the edges to the adjacency list using the `addEdge` method.

5. **Sorting:**
   - After processing the file, the adjacency list is sorted based on time using the `sortByTime` method.

### `sortByTime`

This function sorts the edges of each vertex in the graph based on the time attribute.

- It uses `std::sort` and a custom comparison function (`compareTime`).

### `compareTime`

A static function used as a custom comparison function for sorting edges based on their time attribute.

## `sortBatch`

The `sortBatch` method is designed to efficiently group a batch of edges based on a specified sorting criterion (source or destination vertices). It takes the following parameters:

- `sortFlag`: A flag indicating whether to group by source vertices (0) or destination vertices (1).
- `sourceAdds`: A vector containing source vertices of the edges.
- `destinationAdds`: A vector containing destination vertices of the edges.
- `timeAdds`: A vector containing timestamps of the edges.
- `groupedData`: A reference to a `cuckoohash_map` that will store the grouped data.

This method creates a hash map (`groupedData`) where keys are source vertices, and values are vectors of pairs representing destination vertices and corresponding timestamps. It iterates through the input vectors, and for each edge, it inserts the data into the hash map under the appropriate source vertex.

### 'printGroupedData`

It involves printing the grouped data for a specific source vertex. It takes two parameters:

- `source`: The source vertex for which the data is grouped.
- `edges`: A vector of pairs representing destination vertices and corresponding timestamps for the given source.
