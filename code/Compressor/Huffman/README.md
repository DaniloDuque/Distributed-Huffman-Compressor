# Huffman Compressor - Priority Queue Component

This component is a crucial part of a Huffman compressor. It uses a **priority queue** to store nodes, which represent characters and their frequencies. These nodes are later used to build the Huffman tree, enabling efficient compression by assigning shorter binary codes to more frequent characters and longer codes to less frequent ones.

## Priority Queue Overview

A **priority queue** is a data structure that stores elements such that the element with the highest priority is always dequeued first. In the context of the Huffman compressor, the priority is determined by the frequency of characters, with lower frequency characters having higher priority (i.e., they are dequeued first).

### Usage in Huffman Compression

In Huffman compression, we first count the frequency of each character in the input data. Then, we create nodes for each character and insert them into the priority queue based on their frequency. These nodes are used to construct the Huffman tree, where:

- Characters with **higher frequencies** are placed **closer to the root** of the tree, resulting in shorter binary codes.
- Characters with **lower frequencies** are placed **deeper in the tree**, resulting in longer binary codes.

This structure allows for more efficient encoding, as frequently occurring characters get shorter codes, reducing the overall size of the compressed data.

### Code Functionality

1. **Priority Queue Initialization**: 
   The priority queue (`priority_queue pq`) is initialized using the function `initPriorityQueue()`.

2. **Input Parsing**: 
   The program takes input for the number of characters (`n`), followed by pairs of frequency (`f`) and character (`c`). It creates a node for each character using `createNode(f, c)` and inserts it into the priority queue using `push()`.

3. **Building the Huffman Tree**: 
   Once all nodes are inserted, the Huffman tree is constructed using the function `makeTree(&pq)`. The tree structure is based on the priority queue, where nodes with lower frequencies are combined first, ensuring they are deeper in the tree.

4. **Tree Display**: 
   The function `displayTree()` prints the structure of the Huffman tree for visualization.

5. **Route Creation**: 
   After building the tree, `makeRoutes(tree)` generates the binary codes (or routes) for each character. For instance, the route for the character 'a' might look like `{5, 10110}`, where `5` represents the length of the binary code and `10110` is the binary code itself.

6. **Route Display**: 
   Finally, the program displays the route for each character using `displayRoute()`.

### How to Use

1. **Compile the program**:
   You can compile the program using the following command:
   ```bash
   make
   ```

2. **Run the program**:
   After compiling, run the test program:
   ```bash
   ./test < test_input
   ```
   This will create a Huffman tree and display the binary codes (routes) for each character.

### Example Output

If you input the following data:
```
3
5 a
9 b
12 c
```
The program might produce an output like this:
```
12 c
└── (26)
    ├──c (12)
    └── (14)
        ├──a (5)
        └──b (9)
Character: a  Length: 2  Route: 10
Character: b  Length: 2  Route: 11
Character: c  Length: 1  Route: 0
```

In this case, the character 'b' appears more frequently, so it gets a shorter binary code (`00`), while 'a', which appears less frequently, gets a longer code (`101`).

### Files

- **test.c**: Test program that handles input, priority queue operations, and Huffman tree construction.
- **Node.h**: Defines the structure of a node (character and frequency) and includes functions to create nodes.
- **Tree.h**: Contains functions for building and displaying the Huffman tree and generating routes.

## Conclusion

The priority queue is essential for constructing an optimal Huffman tree, ensuring that characters with higher frequencies get shorter binary codes and characters with lower frequencies get longer codes. This results in efficient compression, minimizing the overall size of the encoded data.
