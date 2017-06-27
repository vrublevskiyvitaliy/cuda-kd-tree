//
//  Points.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 2/24/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef Points_hpp
#define Points_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "OpenGLHelper.hpp"
#include "RandomClass.hpp"

class Points{

    const static int DEFAULT_POINTS_NUMBER = 100;
    const static int DEFAULT_CHANGE_MOVE_ITERATIONS = 1000;
    
    const float upperBoundArea = 0.9;
    const float lowerBoundArea = -0.9;
    
    int numberOfPoints;
    Random random;
    
    std::vector<glm::vec2> pointsPositions;
    std::vector<glm::vec2> pointsMoves;
    std::vector<glm::vec3> pointsColors;
    
    OpenGLHelper * openGLHelper;
    bool useMouse;
    
public:
    
    Points(int number, OpenGLHelper * _openGLHelper = NULL);
    void applyMove(bool useMouse);
    int getNumberOfPoints();
    
    std::vector<glm::vec2> getPointsPositions();
    std::vector<glm::vec2> getPointsMoves();
    std::vector<glm::vec3> getPointsColors();
    
private:
    void initPoints();
};

#endif /* Points_hpp */
