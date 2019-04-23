
#include<vector>
#include "GameManager.h"
#include "Objects.h"
#include"CommonBetweenGameAndDraw.h"
#include "ObjData.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyShader {
	
	static GLuint GetShader() {
		static bool isShaderGenerated = false;
		static GLuint myshader = 0;
		if (!isShaderGenerated)
		{
			const char* vertexShaderSource = "#version 460 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"uniform mat4 View;\n"
				"uniform mat4 Projection;\n"
				"uniform mat4 Model;\n"
				"void main()\n"
				"{\n"
				"   gl_Position = (Projection * View * Model) * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
				"}\0";

			const char* fragmentShaderSource = "#version 460 core\n"
				"out vec4 FragColor;\n"
				"void main()\n"
				"{\n"
				"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
				"}\n\0";
			//code copy from here

	/* make shaders */
	//compile vertex shader
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);

			//compile fragment shader
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);

			//debug compile completed
			{
				int success;
				glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
				assert(success == 0);
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
				assert(success == 0);
			}
			//shader link
			myshader = glCreateProgram();
			glAttachShader(myshader, vertexShader);
			glAttachShader(myshader, fragmentShader);
			glLinkProgram(myshader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}
		return myshader;
	}
};