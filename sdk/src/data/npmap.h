/* -----------------------------------------------------------------------------
*
*  npmap.h
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2014 by Shane Saxon - saxon@openantz.com
*
*  Please see main.c for a complete list of additional code contributors.
*
*  To the extent possible under law, the author(s) have dedicated all copyright 
*  and related and neighboring rights to this software to the public domain
*  worldwide. This software is distributed without any warranty.
*
*  Released under the CC0 license, which is GPL compatible.
*
*  You should have received a copy of the CC0 Public Domain Dedication along
*  with this software (license file named COPYING.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

#ifndef NPMAP_H_
#define NPMAP_H_

#include "../npdata.h"


//--- Functions ---------------------------------------------------------------

void npInitMap (void* dataRef);
void npCloseMap (void* dataRef);

void* npMapNew (int mapType, void* dataRef);
void npMapDelete (pNPmapType map, void* dataRef);

void npGetMaps (void* dataRef);

void npGetMapIO (void* dataRef); //Ports, GL (Sort Visible, Sort-Z, screen 1/2/3 )
void npGetMapNPE (void* dataRef); // collision, rotation, translation...

int npGetRootIndex (pNPnode node, void* dataRef);
void npSelectNode (pNPnode node, void* dataRef);

void* npGetNodeByID (int id, void* dataRef);
void npSelectNodeByID (int id, void* dataRef);

void npTraverseTree (void (*nodeFunc)(pNPnode node, void* dataRef), 
					 pNPnode node, void* dataRef);
void npTraverseMap (void (*nodeFunc)(pNPnode node, void* dataRef),
				   void* dataRef);

//------------------------------------------------------------------------------
//zzhp
void npInitMalloc(void* dataRef);
void npCloseMalloc(void* dataRef);
void npUpdateMalloc(void* dataRef);
void* npMalloc(int type, int size, void* dataRef);
void* npCalloc(int type, int size, void* dataRef);
void npFree(void* memory, void* dataRef);

//moved from npmapfile, reconcile with methods above
void npMapSortInit (void** mapRef, void* dataRef);
void npMapSort(void* dataRef);
void* npMapNodeAdd (int id, int type, int branchLevel, int parentID, 
					 int format, void* dataRef);

void npSort (void* base, int count, int elementSize, 
			 int(*pCompareFunc)(const void* a, const void* b), void* dataRef );

pNPrecordTag npNewTag(void* dataRef);
void npTagSort (void* dataRef);
void npTagSortAdd (pNPrecordTag recordTag, void* dataRef);

//zz-q
void npSetMapItem( char* mapPath, char* item, char* element, char* typeTag,
				  char* value, void* dataRef );

//zz note that we use slightly different terminology for our tree/graph
//
// 'Internal nodes' we call 'nexus' nodes
//
// 'Edge' is called 'link' and is a type of node, (but perhaps should not be a node?)
// the 'link' (Edge) nodes allow for any graph type, acyclic, etc.. (non-tree types)
// links have both a parent and child that represent the A and B ends respectively.

/*

below excerpts from...
http://en.wikipedia.org/wiki/Tree_(data_structure)

Terminologies used in Trees[edit]

Root - the top most node in a tree.
Parent - node that has a child.
Siblings - nodes with the same parent.
Leaves - nodes with no children.
Internal nodes - nodes with at least one child.
Degree - number of sub trees of a node.
Edge - connection between one node to another.
Level - The level of a node is defined by 1 + the number of connections between the node and the root
Height - The height of a node is the length of the longest downward path between the node and a leaf
Forest - A forest is a set of n >= 0 disjoint trees.


Traversal methods

Common operations[edit]
Enumerating all the items
Enumerating a section of a tree
Searching for an item
Adding a new item at a certain position on the tree
Deleting an item
Pruning: Removing a whole section of a tree
Grafting: Adding a whole section to a tree
Finding the root for any node

http://en.wikipedia.org/wiki/List_of_data_structures

Linear data structures[edit]

Arrays[edit]

Array
Bidirectional map
Bit array
Bit field
Bitboard
Bitmap
Circular buffer
Control table
Image
Dynamic array
Gap buffer
Hashed array tree
Heightmap
Lookup table
Matrix
Parallel array
Sorted array
Sparse array
Sparse matrix
Iliffe vector
Variable-length array
Lists[edit]
Doubly linked list
Linked list
Self-organizing list
Skip list
Unrolled linked list
VList
Xor linked list
Zipper
Doubly connected edge list
Difference list

Trees[edit]

Main article: Tree (data structure)
http://en.wikipedia.org/wiki/Tree_(data_structure)

Binary trees[edit]

AA tree
AVL tree
Binary search tree
Binary tree
Cartesian tree
Pagoda
Randomized binary search tree
Red-black tree
Rope
Scapegoat tree
Self-balancing binary search tree
Splay tree
T-tree
Tango tree
Threaded binary tree
Top tree
Treap
Weight-balanced tree
Binary data structure

B-trees[edit]

B-tree
B+ tree
B*-tree
B sharp tree
Dancing tree
2-3 tree
2-3-4 tree
Queap
Fusion tree
Bx-tree

Heaps[edit]

Heap
Binary heap
Weak heap
Binomial heap
Fibonacci heap
AF-heap
2-3 heap
Soft heap
Pairing heap
Leftist heap
Treap
Beap
Skew heap
Ternary heap
D-ary heap
Brodal queue

Trees[edit]

In these data structures each tree node compares a bit slice of key values.

Tree
Radix tree
Suffix tree
Suffix array
Compressed suffix array
FM-index
Generalised suffix tree
B-tree
Judy array
X-fast tree
Y-fast tree
Ctree
Multiway trees[edit]

Ternary tree
K-ary tree
And–or tree
(a,b)-tree
Link/cut tree
SPQR-tree
Spaghetti stack
Disjoint-set data structure
Fusion tree
Enfilade
Exponential tree
Fenwick tree
Van Emde Boas tree
Space-partitioning trees[edit]

These are data structures used for space partitioning or binary space partitioning.

Segment tree
Interval tree
Range tree
Bin
Kd-tree
Implicit kd-tree
Min/max kd-tree
Adaptive k-d tree
Quadtree
Octree
Linear octree
Z-order
UB-tree
R-tree
R+ tree
R* tree
Hilbert R-tree
X-tree
Metric tree
Cover tree
M-tree
VP-tree
BK-tree
Bounding interval hierarchy
BSP tree
Rapidly exploring random tree

Application-specific trees[edit]

Abstract syntax tree
Parse tree
Decision tree
Alternating decision tree
Minimax tree
Expectiminimax tree
Finger tree
Expression tree

Hashes[edit]

Bloom filter
Count-Min sketch
Distributed hash table
Double Hashing
Dynamic perfect hash table
Hash array mapped trie
Hash list
Hash table
Hash tree
Hash trie
Koorde
Prefix hash tree
Rolling hash
MinHash
Quotient Filter
Ctrie

Graphs[edit]

Graph
Adjacency list
Adjacency matrix
Graph-structured stack
Scene graph
Binary decision diagram
Zero suppressed decision diagram
And-inverter graph
Directed graph
Directed acyclic graph
Propositional directed acyclic graph
Multigraph
Hypergraph

Other[edit]

Lightmap
Winged edge
Doubly connected edge list
Quad-edge
Routing table
Symbol table


*/

#endif

