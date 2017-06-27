//
//  OpenGLHelper.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/18/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef OpenGLHelper_hpp
#define OpenGLHelper_hpp

#include <stdio.h>

#include <string>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>



class OpenGLHelper{
public:
    bool printFPS;

	glm::vec3 getMousePosition()
	{
		
		glm::vec3 mouse;

		
		mouse.z = 0;
		mouse.y = 0;
		mouse.x = 0;

		return mouse;
	}
};

#endif /* OpenGLHelper_hpp */
