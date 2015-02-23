NVCC=nvcc

CUDA_INCLUDEPATH=/usr/local/cuda-5.0/include

NVCC_OPTS=-O3 -arch=sm_20 -Xcompiler -Wall -Xcompiler -Wextra -m64

GCC_OPTS= -Wall -Wextra -m64 -g -pg

all: Main

test: Main

Main: Main.o Vec2.o Vec3.o Triangle.o Face.o OBJFile.o Intersect.o BoundingBox.o SparseVoxelOctree.o DAG.o Node.o Voxels.o MortonCode.o SVONode.o DAGNode.o Image.o Raytracer.o Ray.o PhongMaterial.o AABB.o Makefile
	g++ -o main Main.o Vec2.o Vec3.o Triangle.o Face.o OBJFile.o Intersect.o BoundingBox.o SparseVoxelOctree.o DAG.o Node.o Voxels.o MortonCode.o SVONode.o DAGNode.o Image.o Raytracer.o Ray.o PhongMaterial.o AABB.o $(GCC_OPTS)

AABB.o: AABB.cpp AABB.hpp
	g++ -c AABB.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Ray.o: Ray.cpp Ray.hpp
	g++ -c Ray.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

PhongMaterial.o: PhongMaterial.cpp PhongMaterial.hpp
	g++ -c PhongMaterial.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Raytracer.o: Raytracer.cpp Raytracer.hpp
	g++ -c Raytracer.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

SVONode.o: SVONode.cpp SVONode.hpp
	g++ -c SVONode.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

DAGNode.o: DAGNode.cpp DAGNode.hpp
	g++ -c DAGNode.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

MortonCode.o: MortonCode.cpp MortonCode.hpp
	g++ -c MortonCode.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Vec2.o: Vec2.cpp Vec2.hpp
	g++ -c Vec2.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Vec3.o: Vec3.cpp Vec3.hpp
	g++ -c Vec3.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Triangle.o: Triangle.cpp Triangle.hpp Vec3.hpp
	g++ -c Triangle.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Face.o: Face.cpp Face.hpp
	g++ -c Face.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

OBJFile.o: OBJFile.cpp OBJFile.hpp Vec3.hpp Face.hpp Triangle.hpp BoundingBox.hpp
	g++ -c OBJFile.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Intersect.o: Intersect.cpp Intersect.hpp Vec3.hpp Triangle.hpp Vec2.hpp
	g++ -c Intersect.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

BoundingBox.o: BoundingBox.cpp BoundingBox.hpp Vec3.hpp
	g++ -c BoundingBox.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

SparseVoxelOctree.o: SparseVoxelOctree.cpp Intersect.hpp Vec3.hpp Triangle.hpp Vec2.hpp Voxels.hpp SVONode.hpp
	g++ -c SparseVoxelOctree.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

DAG.o: DAG.cpp SparseVoxelOctree.hpp Intersect.hpp Vec3.hpp Triangle.hpp Vec2.hpp Voxels.hpp
	g++ -c DAG.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Node.o: Node.cpp Node.hpp
	g++ -c Node.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Voxels.o: Voxels.cpp Voxels.hpp 
	g++ -c Voxels.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Image.o: Image.cpp Image.hpp 
	g++ -c Image.cpp $(GCC_OPTS) -I $(CUDA_INCLUDEPATH)

Main.o: Main.cpp Intersect.hpp
	g++ -c Main.cpp $(GCC_OPTS)

cleanOs:
	rm -f *.o

clean:
	rm -f *.o main
