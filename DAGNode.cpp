/**
 * DAGNode.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "DAGNode.hpp"

DAGNode::DAGNode()
{
   for (int i = 0; i < 8; i++)
   {
      childPointers[i] = NULL;
   }
}

DAGNode::~DAGNode()
{
}

void DAGNode::print()
{
   std::cerr << "DAGNode: " << std::endl;
   for (int i = 0; i < 8; ++i)
   {
      std::cerr << "\t" << childPointers[i] << std::endl;
   }
}

void DAGNode::printOneLine()
{
   for (int i = 0; i < 8; ++i)
   {
      std::cout << childPointers[i] << " ";
   }
   std::cout << std::endl;
}