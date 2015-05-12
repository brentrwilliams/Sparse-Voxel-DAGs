/**
 *  BoundingVolumeHierarchy.cpp
 *  by Brent Williams
 **/

#include "BoundingVolumeHierarchy.hpp"

BVHNode::BVHNode()
{
}

BVHNode::BVHNode(BVHBoundingBox boundingBox, Triangle *triangle, BVHNode *left, BVHNode *right)
{
   this->boundingBox = boundingBox;
   this->triangle = triangle;
   this->left = left;
   this->right = right;
}

BVHNode* BoundingVolumeHierarchy::generateNode(vector<Triangle*> triangles, int axis)
{
   if (triangles.size() == 1)
   {
      BVHNode *bvhNode = new BVHNode(triangles[0]->getBVHBoundingBox(), triangles[0], NULL, NULL);
      //cout << "Leaf: ";
      //printType(triangle[0]->type);
      return bvhNode;
   }
   else
   {
      //printf("\t");
      if (axis == 0) //x-axis
         std::sort(triangles.begin(), triangles.end(), compareTrianglesX);
      else if (axis == 1) // y-axis
         std::sort(triangles.begin(), triangles.end(), compareTrianglesY);
      else
         std::sort(triangles.begin(), triangles.end(), compareTrianglesZ);
      
      int halfSize = triangles.size() / 2;
      vector<Triangle *> leftTriangles(triangles.begin(), triangles.begin() + halfSize);
      vector<Triangle *> rightTriangles(triangles.begin() + halfSize, triangles.end());
         
      BVHNode *left = generateNode(leftTriangles, ((axis+1)%3));
      BVHNode *right = generateNode(rightTriangles, ((axis+1)%3));
         
      BVHBoundingBox *boundingBox = new BVHBoundingBox(left->boundingBox, right->boundingBox);
      BVHNode *bvhNode = new BVHNode(*boundingBox, NULL, left, right);
      
      return bvhNode;
   }  
}

BoundingVolumeHierarchy::BoundingVolumeHierarchy(vector<Triangle*> triangles)
{
   tree = generateNode(triangles, 0);
}

bool BVHNode::intersect(Ray ray, float *t, Triangle& returnTriangle)
{
   bool leftBool, rightBool;
   float lt, rt;
   Triangle lGeo, rGeo;

   if (boundingBox.intersect(ray))
   {
      if (triangle == NULL) //It is NOT a leaf node
      {
         if (left == NULL)
            printf("NULL LEFT!\n");
         if (left == NULL)
            printf("NULL RIGHT!\n");
         leftBool = left->intersect(ray, &lt, lGeo);
         rightBool = right->intersect(ray, &rt, rGeo);
         
         if (leftBool && rightBool)
         {
            if (lt < rt)
            {
               *t = lt;
               returnTriangle = lGeo;
            }
            else
            {
               *t = rt;
               returnTriangle = rGeo;
            }
            //cout << "\t" << *returnTriangle << endl;
            return true;
         }
         else if (leftBool)
         {
            *t = lt;
            returnTriangle = lGeo;
            //cout << "\t" << *returnTriangle << endl;
            return true;
         }
         else if (rightBool)
         {
            *t = rt;
            returnTriangle = rGeo;
            //cout << "\t" << *returnTriangle << endl;
            return true;
         }
         else
            return false;
         
      }
      else //It is a leaf node
      {
         // cout << "At leaf!!!" << endl;
         Ray transformedRay = (ray);
         // transformedRay.toObjectSpace(triangle->invTrans);

         returnTriangle = *triangle;
         //cout << "End: " << *returnTriangle << endl;
         return triangle->intersect(transformedRay, *t);
      }
   }
   else
   {
      return false;
   }
   
   return false;
}

bool BoundingVolumeHierarchy::intersect(Ray ray, float *t, Triangle& returnTriangle)
{
   // cerr << "tree: " << tree << endl;
   return tree->intersect(ray, t, returnTriangle);
}

bool compareTrianglesX(Triangle *t1, Triangle *t2)
{
   if (t1->getCenterX() < t2->getCenterX())
      return true;
   return false;
}

bool compareTrianglesY(Triangle *t1, Triangle *t2)
{
   if (t1->getCenterY() < t2->getCenterY())
      return true;
   return false;
}

bool compareTrianglesZ(Triangle *t1, Triangle *t2)
{
   if (t1->getCenterZ() < t2->getCenterZ())
      return true;
   return false;
}
