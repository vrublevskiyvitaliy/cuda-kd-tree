//
//  Quicksort.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/25/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "Quicksort.hpp"


void QuickSort::sort(std::vector<glm::vec2> & points, int dim)
{
    if (points.size() == 0) return;
    
    int low = 0;
    int high = points.size() - 1;
    
    if (dim == 0) quicksortX(points, low, high);
    if (dim == 1) quicksortY(points, low, high);
}

void QuickSort::quicksortX(std::vector<glm::vec2> & points, int low, int high)
{
    int i = low, j = high;
    float x = points[low + ((high-low)>>1)].x;
    
    while (i <= j) {
        while(points[i].x < x) i++;
        while(points[j].x > x) j--;
        
        if (i <= j) swapPoints(points, i++, j--);
    }
    
    if (low < j) quicksortX(points, low, j);
    if (i < high) quicksortX(points, i, high);
}

void QuickSort::quicksortY(std::vector<glm::vec2> & points, int low, int high)
{
    int i = low, j = high;
    float y = points[low + ((high-low)>>1)].y;
    
    while (i <= j) {
        while(points[i].y < y) i++;
        while(points[j].y > y) j--;
        
        if (i <= j) swapPoints(points, i++, j--);
    }
    
    if (low < j) quicksortY(points, low, j);
    if (i < high) quicksortY(points, i, high);
}


void QuickSort::swapPoints(std::vector<glm::vec2> & points, int i, int j)
{
    glm::vec2 tmp = points[i];
    points[i] = points[j];
    points[j] = tmp;
}

void QuickSort::testQuickSort(int n, int dim)
{
    Random random;
    
    std::vector<glm::vec2> test_data(n);
    
    for (int i = 0; i < n; i++) {
        test_data[i].x = random.getRandomPosition();
        test_data[i].y = random.getRandomPosition();
        printf("X = %.4f, Y = %.4f \n", test_data[i].x, test_data[i].y);
    }
    printf(" === === === === \n");
    
    sort(test_data, dim);
    
    for (int i = 0; i < n; i++) {
        printf("X = %.4f, Y = %.4f \n", test_data[i].x, test_data[i].y);
    }
    
}

