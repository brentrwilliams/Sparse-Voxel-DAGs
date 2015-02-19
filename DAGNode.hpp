/**
 * DAGNode.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef DAG_NODE_HPP
#define DAG_NODE_HPP

#include <iostream>
#include <stdint.h>

class DAGNode
{
   public:
      void* childPointers[8];

      DAGNode();
      ~DAGNode();
      bool operator< ( const DAGNode & val ) const;
      bool operator!= ( const DAGNode & other ) const;
      bool operator== ( const DAGNode & other ) const;
      void print();
      void printOneLine();

};

#endif
