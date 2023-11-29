# Tempus

## adj_list.h

### Class Definition

The `AdjList` class has the following members:

- **Public Member Functions:**
  - `addFromFile(const std::string& path)`: Reads commands and edge information from a file and updates the graph accordingly.
  - `printGraph() const`: Prints the edges of the graph to the console.

- **Private Member Variables:**
  - `edges`: Cuckoo hashmap with keys as uint64_t and value as another cuckoo hashmap. This one contains uint64_t as keys and a vector as values. The vector contains uint64_t.
            The first uint64_t corresponds to the temporal data of the edge, the second one corresponds the source of the edge, the last corresponds to the destination of the edge.
            Undirected graphs will have two entries with their source and destination swapped for the second one

- **Private Member Functions:**
  - `addSingleEdge(uint64_t source, uint64_t destination, uint64_t time, libcuckoo::cuckoohash_map<uint64_t,libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>> &map)`: Inserts a single edge into the map.
  - `addEdge(uint64_t source, uint64_t destination, uint64_t time)`: Adds an undirected edge to the graph. Also checks that no duplicates are inserted.
  - `deleteEdge(int source, int destination, int time) const`: Deletes an undirected edge from the graph.
  - `addBatchCuckoo(libcuckoo::cuckoohash_map<uint64_t, libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>>& groupedData)`: Adds a batch of edges into the map.
  - `sortBatch(bool sortBySource, const std::vector<uint64_t>& sourceAdds, const std::vector<uint64_t>& destinationAdds, const std::vector<uint64_t>& timeAdds, libcuckoo::cuckoohash_map<uint64_t,libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>>& groupedData)`: Sorts the input data to be used with addBatchCuckoo.
  - `printGroupedData(libcuckoo::cuckoohash_map<uint64_t,libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>>& groupedData)`: Testfunction to inspect the sorted data.


## adj_list.cpp

### `addSingleEdge`

This function inserts a single edge into the given map.

- Parameters: `source`, `destination`, `time` and `map`.

### `addEdge`

This function adds an undirected edge to the graph.

- Parameters: `source`, `destination`, and `time`.
- Checks if the edge already exists before adding it.

### `deleteEdge`

This function deletes an undirected edge from the graph.

- Parameters: `source`, `destination`, and `time`.
- Checks if the edge exists before deletion.

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
   - Determines the `sortBySource` based on the comparison result.
   - Creates vectors (`sourceAdds`, `destinationAdds`, `timeAdds`) to store edge data.

3. **Grouping with `sortBatch`:**
   - Calls `sortBatch` to group the edges based on the determined `sortFlag`.
   - Receives the grouped data in the `groupedData` hash map.

4. **Adding Grouped Edges to Adjacency List:**
   - Iterates through the `groupedData` hash map.
   - For each source vertex and its associated edges, adds the edges to the adjacency list using the `addEdge` method.

5. **Sorting:**
   - After processing the file, the adjacency list is sorted based on time using the `sortByTime` method.

## `sortBatch`

The `sortBatch` method is designed to efficiently group a batch of edges based on a specified sorting criterion (source or destination vertices). It takes the following parameters:

- `sortBySource`: A flag indicating whether to group by source vertices (true) or destination vertices (false).
- `sourceAdds`: A vector containing source vertices of the edges.
- `destinationAdds`: A vector containing destination vertices of the edges.
- `timeAdds`: A vector containing timestamps of the edges.
- `groupedData`: A reference to a `cuckoohash_map` that will store the grouped data.

This method creates a hash map (`groupedData`) where keys are source vertices, and values are vectors of pairs representing destination vertices and corresponding timestamps. It iterates through the input vectors, and for each edge, it inserts the data into the hash map under the appropriate source vertex.

### 'printGroupedData`

It involves printing the grouped data for a specific source vertex. It takes two parameters:

- `source`: The source vertex for which the data is grouped.
- `edges`: A vector of pairs representing destination vertices and corresponding timestamps for the given source.
