//
//  QuickSelect.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/25/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "QuickSelect.hpp"



void QuickSelect::sort(std::vector<glm::vec2> & points, int _dim, int left, int right, int  m) {
    if (points.size() == 0) return;
    dim = _dim;
    quickfindFirstK(points, left, right, m);
}


void QuickSelect::quickfindFirstK(std::vector<glm::vec2> & points, int left, int right, int k) {
    if (right > left) {
        int pivot_idx = (left + right) >> 1;
        
        switch(dim) {
            case 0: pivot_idx = partitionX( points, left, right, pivot_idx); break;
            case 1: pivot_idx = partitionY( points, left, right, pivot_idx); break;
        }
        
        if (pivot_idx > k)  quickfindFirstK( points, left, pivot_idx-1, k);
        if (pivot_idx < k)  quickfindFirstK( points, pivot_idx+1, right, k);
    }
}


int QuickSelect::partitionX(std::vector<glm::vec2> & points, int left, int right, int pivot_idx) {
    float pivot = points[pivot_idx].x;
    swapPoints(points, right, pivot_idx); // Move pivot to end
    
    for (int i = pivot_idx = left; i < right; i++) {
        if (points[i].x < pivot) {
            swapPoints(points, pivot_idx++, i);
        }
    }
    swapPoints(points, right, pivot_idx); // Move pivot to its final place
    return pivot_idx;
}

int QuickSelect::partitionY(std::vector<glm::vec2> & points, int left, int right, int pivot_idx) {
    float pivot = points[pivot_idx].y;
    swapPoints(points, right, pivot_idx); // Move pivot to end
    for (int i = pivot_idx = left; i < right; i++) {
        if (points[i].y < pivot) {
            swapPoints(points, pivot_idx++, i);
        }
    }
    swapPoints(points, right, pivot_idx); // Move pivot to its final place
    return pivot_idx;
}

void QuickSelect::swapPoints(std::vector<glm::vec2> & points, int i, int j) {
    glm::vec2 tmp = points[i];
    points[i] = points[j];
    points[j] = tmp;
}

void QuickSelect::testSort(int n, int dim)
{
    Random random;
    
    std::vector<glm::vec2> test_data(n);
    
    for (int i = 0; i < n; i++) {
        test_data[i].x = random.getRandomPosition();
        test_data[i].y = random.getRandomPosition();
        printf("X = %.4f, Y = %.4f \n", test_data[i].x, test_data[i].y);
    }
    printf(" === === === === \n");
    
    sort(test_data, dim, 0, n-1, n >> 1);
    
    for (int i = 0; i < n; i++) {
        printf("X = %.4f, Y = %.4f \n", test_data[i].x, test_data[i].y);
    }
    
}
