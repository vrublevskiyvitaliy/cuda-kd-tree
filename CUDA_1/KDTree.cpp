//
//  KDTree.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "KDTree.hpp"
#include <algorithm>

KDTree::KDTree(int n, int _build_algorithm, OpenGLHelper * _helper, bool _useMouse) {
    
    num_point = n;
    build_algorithm = _build_algorithm;
    helper = _helper;
    useMouse = _useMouse;
    
    p = new Points(n, helper);
    
    printf("Number of points = %d\n", num_point);
    
    if (build_algorithm == BUILD_ITERATIVE) {
        printf("BUILD_ITERATIVE\n");
    } else if (build_algorithm == BUILD_RECURSIVE) {
        printf("BUILD_RECURSIVE\n");
    } else {
        printf("BUILD_RECURSIVE_FAST\n");
    }

    initKdTree(num_point);
}

void KDTree::setData()
{
    p->applyMove(useMouse);
    std::vector<glm::vec2> points = p->getPointsPositions();
    
    if (useMouse) {
        glm::vec3 mousePosition = helper->getMousePosition();
        points[0].x = mousePosition.x;
        points[0].y = mousePosition.y;
    }
    
    build(points, false);
}

void KDTree::setData(std::vector<glm::vec2> points)
{
    build(points, false);
}

std::vector<float> KDTree::getData() {
    std::vector<float> data(num_nodes * 4);
    
    for (int i = 0; i < num_nodes; i++) {
        data[i*4] = kd_tree[i].r;
        data[i*4 + 1] = kd_tree[i].g;
        data[i*4 + 2] = kd_tree[i].b;
        data[i*4 + 3] = kd_tree[i].a;
    }
    
    return data;
}

int KDTree::get_p(int node_idx) {
    return node_idx >> 1;
}
int KDTree::get_l(int node_idx) {
    return node_idx << 1;
}
int KDTree::get_r(int node_idx) {
    return (node_idx << 1) + 1;
}

void KDTree::free_data() {
    
    if (kd_tree) {
        delete[] kd_tree;
    }
    if (points) {
        delete[] points;
    }

}

KDTree::~KDTree() {
    free_data();
 }

void KDTree::initKdTree(unsigned long num_points) {
    int tmp_max_depth = ceil( log(num_points * 1.) / log(2.) ); // log2
    int tmp_num_nodes = 1 << (tmp_max_depth+1);
    
    if( tmp_num_nodes != this->num_nodes || this->kd_tree == NULL){
        this->max_depth  = tmp_max_depth;
        this->num_nodes  = tmp_num_nodes;
        this->kd_tree    = new KDTreeNode[num_nodes ];
    }
    
    int max   = 2;
    int depth = 0;
    int dim   = depth % 2;
    for(int i = 0; i < num_nodes - 1; i++){
        if( i == max-1 ) {
            max <<= 1;
            depth++;
            dim = (depth % 2) << 15;
        }
        kd_tree[i+1].dim = dim ? 1 : 0; // setting  dim bits ... always the sam
    }
}


void KDTree::printTree() {
    for(int i = 0; i < num_nodes; i++) {
        if(kd_tree[i].isEmpty){
            printf("tree[%d]  null\n", i);
        } else {
            int leaf     = kd_tree[i].isLeaf;
            float x      = kd_tree[i].p.x;
            float y      = kd_tree[i].p.y;
            int dim      = kd_tree[i].dim;
            int P        = get_p(i);
            int L        = get_l(i);
            int R        = get_r(i);
            
            printf("tree[%d]  %s  dim=%d  [P,L,R]=[%d,%d,%d]  pnt=[%.2f,%.2f] \n",
                i, leaf ? "-> LEAF":"       ", dim, P,L,R, x, y);
        }
    }
}

void KDTree::build(std::vector<glm::vec2> & v_points, bool is_debug) {
    
    if (build_algorithm == BUILD_ITERATIVE) {
        buildIterative(v_points);
    } else if (build_algorithm == BUILD_RECURSIVE) {
        buildRecursive(1, v_points);
    } else {
        buildRecursiveFast(1, v_points, 0, v_points.size() - 1, 0);
    }
    
    if (is_debug) {
        printTree();
    }
}


bool sort_x (glm::vec2 i, glm::vec2 j) { return (i.x < j.x); }
bool sort_y (glm::vec2 i, glm::vec2 j) { return (i.y < j.y); }

void KDTree::buildIterative(std::vector<glm::vec2> & v_points) {
    int ptr_T = 0; // tree pointer for compressed tree-nodes (integer)
    int ptr_P = 1; // stack pointer for point-sets
    
    std::vector< std::vector<glm::vec2> > stack_P(v_points.size() * v_points.size()); // FIFO
    stack_P[ptr_P++] = v_points;
    
    while(ptr_T++ < num_nodes) {
        
        std::vector<glm::vec2> p_v = stack_P[ptr_T];
        
        if(!p_v.size()) continue;
        
        unsigned long e = p_v.size();
        unsigned long m = e >> 1;
        
        if( e > 1 ){ // not a leaf
            
            if (kd_tree[ptr_T].dim == 0) {
                //sort x
                std::sort (p_v.begin(), p_v.end(), sort_x);
            } else {
                //sort y
                std::sort (p_v.begin(), p_v.end(), sort_y);
            }
            
            stack_P[ptr_P++] = std::vector<glm::vec2> (p_v.begin(), p_v.begin() + m);
            stack_P[ptr_P++] = std::vector<glm::vec2> (p_v.begin() + m, p_v.end());
        } else { // leaf
            ptr_P += 2;
            kd_tree[ptr_T].isLeaf = 1;
            kd_tree[ptr_T].isEmpty = false;
        }
        
        kd_tree[ptr_T].p.x = p_v[m].x;
        kd_tree[ptr_T].p.y = p_v[m].y;
        kd_tree[ptr_T].isEmpty = false;
        kd_tree[ptr_T].setRGBA();
    }
    
}

int KDTree::getNumNodes() {
    return num_nodes;
}

void KDTree::buildRecursive(int idx, std::vector<glm::vec2> & points)
{
    
    unsigned long e = points.size();
    int m = e >> 1;
    if (e > 1) {
        QuickSort::sort(points, kd_tree[idx].dim);
        std::vector<glm::vec2> points_left = std::vector<glm::vec2> (points.begin(), points.begin() + m);
        std::vector<glm::vec2> points_right = std::vector<glm::vec2> (points.begin() + m, points.end());
        
        buildRecursive( (idx<<1), points_left );
        buildRecursive( (idx<<1)+1, points_right );
    } else {
        kd_tree[idx].isLeaf= true; // mark as leaf
    }
    
    kd_tree[idx].isEmpty = false;
    kd_tree[idx].p.x = points[m].x;
    kd_tree[idx].p.y = points[m].y;
    kd_tree[idx].setRGBA();
    
}

int nodes = 0;

void KDTree::buildRecursiveFast(int idx, std::vector<glm::vec2> & pnts, int left, int right, int level = 0)
{
	if (level == BUILD_MAX_KDTREE_LEVEL) {
		std::cout << "Prev Sort by " << (kd_tree[idx].dim + 1) % 2  << " == == == == == \n";
		for (int i = left; i <= right; ++i) {
			
			std::cout << pnts[i].x << " " << pnts[i].y << std::endl;
		}
		return;
	}
    int m = (left+right)>>1;
    QuickSelect q;

    if( (right-left) >= 2 ){
        
        q.sort(pnts, kd_tree[idx].dim, left, right-1, m);
        
        // it is necessary to make it here, not below
        // because buildRecursiveFast is modifing pnts also

		printf("Node: pnt=[%.0f ,%.0f] \n", pnts[m].x, pnts[m].y);

        kd_tree[idx].p.x = pnts[m].x;
        kd_tree[idx].p.y = pnts[m].y;
        kd_tree[idx].isEmpty = false;
        kd_tree[idx].setRGBA();

        buildRecursiveFast( (idx<<1),   pnts, left, m, level + 1);
		buildRecursiveFast((idx << 1) + 1, pnts, m, right, level + 1);
    } else {
        kd_tree[idx].isLeaf = true; // mark as leaf
        
		printf("Node: pnt=[%.0f ,%.0f] \n", pnts[m].x, pnts[m].y);

        kd_tree[idx].p.x = pnts[m].x;
        kd_tree[idx].p.y = pnts[m].y;
        kd_tree[idx].isEmpty = false;
        kd_tree[idx].setRGBA();
    }
    
}



