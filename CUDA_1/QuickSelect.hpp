//
//  QuickSelect.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/25/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef QuickSelect_hpp
#define QuickSelect_hpp

#include <stdio.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>

#include "RandomClass.hpp"

class QuickSelect {
    int dim = 0;
public:
    
    // the idea is not to sort, but to have
    // for all i < m points[i] < points[m]
    // for all i > m points[i] > points[m]
    void sort(std::vector<glm::vec2> & points, int _dim, int left, int right, int  m);
    
    void quickfindFirstK(std::vector<glm::vec2> & points, int left, int right, int k);
    
    int partitionX(std::vector<glm::vec2> & points, int left, int right, int pivot_idx);
    int partitionY(std::vector<glm::vec2> & points, int left, int right, int pivot_idx);
    
    void swapPoints(std::vector<glm::vec2> & points, int i, int j);
    
    void testSort(int n, int dim);
};



#endif /* QuickSelect_hpp */
