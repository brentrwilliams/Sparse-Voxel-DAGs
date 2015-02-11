/**
 * SVONode.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "SVONode.hpp"

SVONode::SVONode()
{
   for (int i = 0; i < 8; i++)
   {
      childPointers[i] = NULL;
   }
}

SVONode::~SVONode()
{
}

// Returns true if the current object is less than the other object
// Used for sorting
bool SVONode::operator< ( const SVONode & other ) const 
{ 
   for (int i = 0; i < 8; i++)
   {
      if (childPointers[i] < other.childPointers[i])
      {
         return true;
      }
      else if (childPointers[i] > other.childPointers[i])
      {
         return false;
      }
   }
   return false;
}

// Returns true if the current object is not equal to the other object
// Used in reduction
bool SVONode::operator!= ( const SVONode & other ) const 
{ 
   for (int i = 0; i < 8; i++)
   {
      if (childPointers[i] != other.childPointers[i])
      {
         return true;
      }
   }
   return false;
}

// Returns true if the current object is not equal to the other object
// Used in reduction
bool SVONode::operator== ( const SVONode & other ) const 
{ 
   for (int i = 0; i < 8; i++)
   {
      if (childPointers[i] != other.childPointers[i])
      {
         return false;
      }
   }
   return true;
}

void SVONode::print()
{
   std::cerr << "SVONode: " << std::endl;
   for (int i = 0; i < 8; ++i)
   {
      std::cerr << "\t" << childPointers[i] << std::endl;
   }
}

void SVONode::printOneLine()
{
   for (int i = 0; i < 8; ++i)
   {
      std::cout << childPointers[i] << " ";
   }
   std::cout << std::endl;
}