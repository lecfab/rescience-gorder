// MIT License Copyright (c) 2016, Hao Wei.

#ifndef _UNITHEAP_H
#define _UNITHEAP_H

#include "tools.h"
#include <climits>
#include <cstdlib>


class ListElement {
public:
  int key;
  ul prev;
  ul next;
};

class HeadEnd {
public:
  ul first;
  ul second;
};

class UnitHeap {
public:
  std::vector<int> update;
  std::vector<ListElement> LinkedList; // key=degree, prev=id of previous node
                                       // in degree DESC ordering, next=...
  std::vector<HeadEnd> Header; // first=id of first node with this degree,
                               // second=id of last node...
  size_t heapsize = 0;         // updated at each instant
  ul top;                      // id of node with highest degree
  ul huge;                     // ignore huge nodes (deg > sqrt of graph size)
  ul none;                     // integer that cannot be a node index
  const int infty = INT_MAX/2;

  // reserve memory
  UnitHeap(ul size);

  // prepare insertion of node by compensating the key with a negative update
  void InsertElement(const ul index, const int key);
  // oncle all elements are inserted, create headers and linked list
  void ReConstruct();

  // update headers when element is removed
  void erase_key_element(const ul index, const ul next, const ul prev);

  // find best element and output it
  ul ExtractMax();
  // update top and decrease it if necessary
  void DecreaseTop();
  // delete a node from linked list, update headers
  void DeleteElement(const ul index);

  // change element key: increase update if possible otherwise call IncrementKey
  void lazyIncrement(const ul index, const int up);
  // increment key, move element in linked list, update headers
  void IncrementKey(const ul index);


  // debug
  void print_status();
  void safety_check();

  // void DecrementKey(const ul index);
};

#endif
