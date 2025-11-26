# BTree

This project provides a complete implementation of a B-Tree data structure in C, along with an alternative templated implementation in C++. A B-Tree is a balanced multi-way search tree optimized for systems that read and write large blocks of data. It is widely used in databases, filesystems, and indexing structures.

The project supports insertion, search, deletion, traversal, and finding the minimum element for arbitrary minimum degree t.

Features
Core Functionality

Creation of an empty B-Tree for any minimum degree t ≥ 2

Insertion of keys using the standard B-Tree insertion algorithm
(node splitting, promotion of median keys)

Search operation with logarithmic height complexity

Deletion operation fully implementing all B-Tree cases:

Deleting from leaf nodes

Deleting from internal nodes using predecessor or successor

Borrowing keys from siblings when a child underflows

Merging nodes when necessary

Shrinking the root when it becomes empty

Inorder traversal (prints keys in sorted order)

Minimum key retrieval

C Implementation (main.c)

The C version contains modular functions for each part of the B-Tree algorithm:

insert and insertNonFull

splitChild

search

deleteKey, deleteFromLeaf, deleteFromNonLeaf

borrowFromPrevious, borrowFromNext

mergeNodes and fillChild

Interactive menu to:

Insert multiple keys

Search for a key

Delete a key

Print inorder traversal

Find the minimum element

C++ Implementation (Btree.cpp)

The C++ file contains a templated B-Tree class that supports:

Generic key types through templates

Insert, search, delete, inorder, preorder, and minimum operations

Randomized testing in the provided main function

How to Compile
C Version
gcc main.c -o btree
./btree

C++ Version
g++ Btree.cpp -o btree_cpp
./btree_cpp

Usage

Run the program.

Enter the minimum degree t when prompted.

Select one of the options from the interactive menu:

Insert keys

Search for a key

Delete a key

Print the B-Tree in sorted order

Find the minimum key
