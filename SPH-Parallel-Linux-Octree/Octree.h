#pragma once
#include "Particle.h"
#include <iostream>
#include <math.h>
#include <vector>
#include "pthread.h"

class Node
{
 public:
  vec3 center;
  Node* nodes[8];
  std::vector<Particle*> particles;
  Node()
    {
      for (int i = 0; i < 8; i++)
	nodes[i] = NULL;
    }
};
struct Assignment{
  int threadNum;
  int mortonCode;
};

class Octree
{
  
  pthread_t *threads;
  int tCount;
  class Pack *pack;
  vector<int>* mortonCodes;
  vector<int>* ZCurves;
  Node* root;
  void buildCurves(int numberOfThreads);
  void assignJobs(int numberOfThreads); 
  void buildOctree(Node* leaf, int depth, int threadCount);
  void buildOctreeParallel(Node* leaf);
 
  std::vector<Particle*> searchOctree(Node* leaf , Particle& particle);
  void Clean(Node* leaf);
 public:
  Octree();
  void buildOctreeAssign(vector<int>* morton);
  void init(int numberOfThreads);
  void buildOctree(vector<Particle> &arr, int threadCount);
  std::vector<Particle*> searchOctree(Particle& particle);
  void Clean(int threadCount);



};
