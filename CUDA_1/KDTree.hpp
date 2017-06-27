//
//  KDTree.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef KDTree_hpp
#define KDTree_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

#include "Points.hpp"
#include "KDTreeNode.hpp"
#include "QuickSort.hpp"
#include "QuickSelect.hpp"
#include "OpenGLHelper.hpp"

#include <glm/glm.hpp>


class KDTree {
public:
    
    static const int BUILD_ITERATIVE = 0;
    static const int BUILD_RECURSIVE = 1;
    static const int BUILD_RECURSIVE_FAST = 2;

	static const int BUILD_MAX_KDTREE_LEVEL = 1;


private:
    
    int num_nodes = 0;
    int num_point = 0;
    
    int max_depth = 0;
    
    glm::vec2 * points = NULL;
    
    void free_data();
public:
    Points * p = NULL;
private:
    int build_algorithm = BUILD_ITERATIVE;
    
    OpenGLHelper * helper;
    bool useMouse;
public:
    
    KDTreeNode * kd_tree = NULL;
    
    KDTree(int n = 4, int _build_algorithm = BUILD_ITERATIVE, OpenGLHelper * _helper = NULL, bool _useMouse = false);
    ~KDTree();
    void setData();
    void setData(std::vector<glm::vec2> points);
    
    std::vector<float> getData();
    int getNumNodes();
    void setUseMouse(bool _useMouse) {
        useMouse = _useMouse;
    }
    
    // get node indices for: Parent, Left-child, Right-child
    int get_p(int node_idx);
    int get_l(int node_idx);
    int get_r(int node_idx);
    
    void initKdTree(unsigned long num_points);
    void printTree();
    
    void build(std::vector<glm::vec2> & v_points, bool is_debug = true);
    
    void buildIterative(std::vector<glm::vec2> & v_points);
    void buildRecursive(int idx, std::vector<glm::vec2> & points);
    void buildRecursiveFast(int idx, std::vector<glm::vec2> & pnts, int left, int right, int level);
    
};


#endif /* KDTree_hpp */
