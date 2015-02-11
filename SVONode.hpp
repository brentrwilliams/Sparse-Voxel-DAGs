/**
 * SVONode.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef SVO_NODE_HPP
#define SVO_NODE_HPP

#include <iostream>
#include <stdint.h>

class SVONode
{
   public:
      void* childPointers[8];

      SVONode();
      ~SVONode();
      bool operator< ( const SVONode & val ) const;
      bool operator!= ( const SVONode & other ) const;
      bool operator== ( const SVONode & other ) const;
      void print();
      void printOneLine();

};

#endif
