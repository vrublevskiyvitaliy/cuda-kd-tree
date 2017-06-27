//
//  Quicksort.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/25/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef Quicksort_hpp
#define Quicksort_hpp

#include <stdio.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>

#include "RandomClass.hpp"

class QuickSort{
    static void quicksortX(std::vector<glm::vec2> & points, int low, int high);
    static void quicksortY(std::vector<glm::vec2> & points, int low, int high);
    static void swapPoints(std::vector<glm::vec2> & points, int i, int j);
public:
    static void sort(std::vector<glm::vec2> & points, int dim);
    static void testQuickSort(int n, int dim);
};



#endif /* Quicksort_hpp */
