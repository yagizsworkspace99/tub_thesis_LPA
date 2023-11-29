# Tempus

## adj_list.h

This C++ header file defines a class named `AdjList` representing an adjacency list for a graph. The class uses the `libcuckoo` library for efficient hash map operations.

### Public Methods:

1. **addFromFile(const std::string& path):**
   - Reads graph data from a file specified by the provided path and adds it to the adjacency list.

2. **printGraph():**
   - Prints the contents of the graph, displaying edges and associated timestamps.

### Private Members:

- **edges:**
  - A cuckoo hash map storing graph data where:
    - Key: Timestamp
    - Value: Another cuckoo hash map where:
      - Key: Source vertex
      - Value: Vector of destination vertices.

### Private Methods:

1. **addEdge(uint64_t source, uint64_t destination, uint64_t time):**
   - Adds a single edge to the adjacency list.

2. **deleteEdge(uint64_t source, uint64_t destination, uint64_t time):**
   - Deletes a single edge from the adjacency list.

3. **batchOperationCuckoo(bool flag, ...):**
   - Performs batch operations (addition or deletion) on the graph using cuckoo hash map.

4. **batchOperationCuckooParlay(bool flag, ...):**
   - Similar to `batchOperationCuckoo` with an additional parameter for handling parlay operations.

5. **sortBatch(const std::vector<uint64_t>& sourceAdds, ...):**
   - Sorts a batch of data before performing batch operations.

6. **printGroupedData(...):**
   - Prints the grouped data within the cuckoo hash map.

7. **addBatch(int *source, int *destination, int *time, int numberElements):**
   - Adds a batch of edges to the graph.

8. **rangeQuery(uint64_t start, uint64_t end, void (*func)(uint64_t,uint64_t,uint64_t)):**
   - Performs a range query on the graph data within the specified timestamp range.

9. **memoryConsumption(...):**
   - Calculates and prints the memory consumption of the cuckoo hash map.


## adj_list.cpp

### Additions:

#### `addSingleEdge(uint64_t source, uint64_t destination, uint64_t time, libcuckoo::cuckoohash_map<uint64_t, Edge> &map):`
   - Adds a single edge to the specified cuckoo hash map (`map`) at the given timestamp (`time`).
   - Checks if the timestamp exists in the map. If it does, the source and destination are added to the corresponding vectors. If not, a new entry is created. This function is a helper used internally within the class.

#### `addEdge(uint64_t source, uint64_t destination, uint64_t time):`
   - Adds an edge to the adjacency list of the graph, checking for duplicate edges before insertion.
   - Checks if the specified edge (defined by source, destination, and timestamp) already exists in the graph. If it does, the edge is not added to avoid duplicates. Calls `addSingleEdge` twice to add the edge from both the source and destination perspectives.

### Deletions:

#### `deleteSingleEdge(uint64_t source, uint64_t destination, uint64_t time, libcuckoo::cuckoohash_map<uint64_t, Edge> &map):`
   - Deletes a single edge from the specified cuckoo hash map (`map`) at the given timestamp (`time`).
   - Checks if the timestamp exists in the map. If it does, removes the destination from the vector associated with the source. If the vector becomes empty, checks if the source node has no more edges and removes it as well. If the timestamp becomes empty, it is also removed from the map.

#### `deleteEdge(uint64_t source, uint64_t destination, uint64_t time):`
   - Deletes an edge from the graph, checking for the existence of the specified edge before deletion.
   - Checks if the specified edge exists in the graph. If it does, calls `deleteSingleEdge` twice to delete the edge from both the source and destination perspectives.


#### `printGraph():`
This function prints all edges in the graph along with their timestamps. It uses a lock table to iterate over the cuckoo hash map and its inner tables to display the graph's structure.

#### `addFromFile(const std::string &path):`
Reads graph data from a file specified by the provided path and performs batch operations on the graph. It parses the file content to identify "add" and "delete" commands, collects data into vectors, sorts and groups the data, and then applies batch operations on the graph using cuckoo hash maps.

#### `batchOperationCuckoo(bool flag, libcuckoo::cuckoohash_map<uint64_t, Edge> &groupedData):`
Performs batch operations (addition or deletion) on the graph using cuckoo hash maps. It locks the cuckoo hash map's table, iterates over the data, and adds or deletes edges based on the specified flag. It also measures the execution time and prints the duration.

#### `batchOperationCuckooParlay(bool flag, libcuckoo::cuckoohash_map<uint64_t, Edge> &groupedData, uint64_t maxTime):`
Parallelized version of `batchOperationCuckoo` with improved performance for large gaps between timestamps. Utilizes the `parlay::parallel_for` function to parallelize the edge processing.

#### `sortBatch(const std::vector<uint64_t> &sourceAdds, const std::vector<uint64_t> &destinationAdds, const std::vector<uint64_t> &timeAdds, libcuckoo::cuckoohash_map<uint64_t, Edge> &groupedData):`
Sorts a batch of data (source, destination, time) and adds it to the specified cuckoo hash map (`groupedData`). Measures the execution time and prints the duration.

#### `printGroupedData(libcuckoo::cuckoohash_map<uint64_t, Edge> &groupedData):`
Prints the grouped data within the cuckoo hash map. It uses a lock table to iterate over the cuckoo hash map and its inner tables, displaying the source, destination, and timestamp for each edge.

#### `addBatch(int *source, int *destination, int *time, int numberElements):`
Adds a batch of edges to the graph. It iterates over the provided arrays of source, destination, and time, adding each edge to the graph.

#### `rangeQuery(uint64_t start, uint64_t end, void (*func)(uint64_t, uint64_t, uint64_t)):`
Performs a range query on the graph data within the specified timestamp range. It iterates over the cuckoo hash map's lock table and executes the provided function for each edge within the specified range.

#### `memoryConsumption(libcuckoo::cuckoohash_map<uint64_t, libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>> &map):`
Calculates and prints the memory consumption of the cuckoo hash map. It uses a lock table to iterate over the cuckoo hash map and its inner tables, summing up the memory consumption of keys, source vertices, and edges.


