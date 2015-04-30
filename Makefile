CC=icpc
OPTS= -Wall -Wextra -m64 -g -pg -O3 -openmp -ltbb

all: Main

test: Main

Main: Main.o Vec2.o Vec3.o Triangle.o Face.o OBJFile.o Intersect.o BoundingBox.o SparseVoxelOctree.o DAG.o Node.o Voxels.o MortonCode.o SVONode.o DAGNode.o Image.o Raytracer.o Ray.o PhongMaterial.o AABB.o Camera.o Makefile
	$(CC) -o main Main.o Vec2.o Vec3.o Triangle.o Face.o OBJFile.o Intersect.o BoundingBox.o SparseVoxelOctree.o DAG.o Node.o Voxels.o MortonCode.o SVONode.o DAGNode.o Image.o Raytracer.o Ray.o PhongMaterial.o AABB.o Camera.o $(OPTS)

Camera.o: Camera.cpp Camera.hpp
	$(CC) -c Camera.cpp $(OPTS) 

AABB.o: AABB.cpp AABB.hpp
	$(CC) -c AABB.cpp $(OPTS) 

Ray.o: Ray.cpp Ray.hpp
	$(CC) -c Ray.cpp $(OPTS) 

PhongMaterial.o: PhongMaterial.cpp PhongMaterial.hpp
	$(CC) -c PhongMaterial.cpp $(OPTS) 

Raytracer.o: Raytracer.cpp Raytracer.hpp
	$(CC) -c Raytracer.cpp $(OPTS) 

SVONode.o: SVONode.cpp SVONode.hpp
	$(CC) -c SVONode.cpp $(OPTS) 

DAGNode.o: DAGNode.cpp DAGNode.hpp
	$(CC) -c DAGNode.cpp $(OPTS) 

MortonCode.o: MortonCode.cpp MortonCode.hpp
	$(CC) -c MortonCode.cpp $(OPTS) 

Vec2.o: Vec2.cpp Vec2.hpp
	$(CC) -c Vec2.cpp $(OPTS) 

Vec3.o: Vec3.cpp Vec3.hpp
	$(CC) -c Vec3.cpp $(OPTS) 

Triangle.o: Triangle.cpp Triangle.hpp Vec3.hpp
	$(CC) -c Triangle.cpp $(OPTS) 

Face.o: Face.cpp Face.hpp
	$(CC) -c Face.cpp $(OPTS) 

OBJFile.o: OBJFile.cpp OBJFile.hpp Vec3.hpp Face.hpp Triangle.hpp BoundingBox.hpp
	$(CC) -c OBJFile.cpp $(OPTS) 

Intersect.o: Intersect.cpp Intersect.hpp Vec3.hpp Triangle.hpp Vec2.hpp
	$(CC) -c Intersect.cpp $(OPTS) 

BoundingBox.o: BoundingBox.cpp BoundingBox.hpp Vec3.hpp
	$(CC) -c BoundingBox.cpp $(OPTS) 

SparseVoxelOctree.o: SparseVoxelOctree.cpp Intersect.hpp Vec3.hpp Triangle.hpp Vec2.hpp Voxels.hpp SVONode.hpp
	$(CC) -c SparseVoxelOctree.cpp $(OPTS) 

DAG.o: DAG.cpp SparseVoxelOctree.hpp Intersect.hpp Vec3.hpp Triangle.hpp Vec2.hpp Voxels.hpp
	$(CC) -c DAG.cpp $(OPTS) 

Node.o: Node.cpp Node.hpp
	$(CC) -c Node.cpp $(OPTS) 

Voxels.o: Voxels.cpp Voxels.hpp 
	$(CC) -c Voxels.cpp $(OPTS) 

Image.o: Image.cpp Image.hpp 
	$(CC) -c Image.cpp $(OPTS) 

Main.o: Main.cpp Intersect.hpp
	$(CC) -c Main.cpp $(OPTS)

cleanOs:
	rm -f *.o

clean:
	rm -f *.o main
