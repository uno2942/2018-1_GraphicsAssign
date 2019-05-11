#pragma once
#include<string>
#include<iostream>
#include<glew.h>
#include<freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyShader {
public:
	static GLuint myshader;
	static GLuint PhongShader;
	static GLuint GouraudShader;

	static void SetGouraudShader() {
		static bool isGouraudShaderGenerated = false;
		if (!isGouraudShaderGenerated)
		{
			const char* vertexShaderSource = "#version 460 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"layout (location = 1) in vec3 aNormal;\n"
				"layout (location = 2) in vec2 aTexCoord;\n"

				"uniform mat4 View;\n"
				"uniform mat4 Projection;\n"
				"uniform mat4 Model;\n"

				"uniform int isNormalTextureExists;\n"

				"uniform sampler2D normalTexture;\n"
				"uniform vec3 LightPosition;\n"
				"uniform vec3 CameraPos;\n"

				"uniform float ambientIaka;\n"
				"uniform float diffuseIpkd;\n"
				"uniform float specularIpks;\n"

				"uniform vec3 LightDirection;\n"
				"uniform float DambientIaka;\n"
				"uniform float DdiffuseIpkd;\n"
				"uniform float DspecularIpks;\n"

				"out vec2 TexCoord;\n"

				"out float ambientIntensity;\n"
				"out float diffuseIntensity;\n"
				"out float specularIntensity;\n"
				"void main()\n"
				"{\n"
				"	TexCoord = aTexCoord;\n"
				"   vec3 FragPos = vec3(Model * vec4(aPos, 1.0));\n"
				"	vec3 N;\n"
				"	if(isNormalTextureExists==1)"
				"		{N=normalize(vec3(texture(normalTexture, aTexCoord))-vec3(0.5, 0.5, 0.5));"
				"		 N = vec3(N.x, N.z, -N.y);}\n"
				"	else"
				"		N = aNormal;\n"


				"	float dL = length(LightPosition-FragPos);\n"
				"	float fatt = 1./(1+0.001*dL + (0.001*dL)*(0.001*dL));\n"

				"	N = normalize(mat3(transpose(inverse(Model))) * N);\n"
				"	vec3 E = normalize(CameraPos-FragPos);\n"
				"	vec3 L = normalize(LightPosition-FragPos);\n"
				"   vec3 DL = normalize(LightDirection);\n"
				"	vec3 H = normalize( L + E );\n"
				"	vec3 DH = normalize( DL + E );\n"

				"	ambientIntensity = ambientIaka + DambientIaka;\n"
				"	diffuseIntensity = (fatt * diffuseIpkd * max(dot(N, L), 0.0) + DdiffuseIpkd * max(dot(N, DL), 0.0));\n"
				"	specularIntensity = (fatt * specularIpks * pow(max(dot(H, N), 0.0), 16) + DspecularIpks * pow(max(dot(DH, N), 0.0), 16));\n"

				"   gl_Position = (Projection * View * vec4(FragPos, 1.0));\n"
				"}\0";

			const char* fragmentShaderSource = "#version 460 core\n"
				"in vec2 TexCoord;\n"

				"in float ambientIntensity;\n"
				"in float diffuseIntensity;\n"
				"in float specularIntensity;\n"
				"uniform vec4 myColor;\n"

				"uniform int numOfTexture;\n"
				"uniform sampler2D diffuseTexture;\n"
				"uniform sampler2D specularTexture;\n"
				"uniform int isText;\n"
				"out vec4 FragColor;\n"
				"void main()\n"
				"{\n"
				"	if(isText==1)"
				"		FragColor = myColor;\n"
				"	else"
				"	{\n"
				"	vec4 diffuseColor = vec4(0, 0, 0, 0);\n"
				"	vec4 specularColor = vec4(0, 0, 0, 0);\n"

				"   if(numOfTexture>0)\n"
				"		diffuseColor = texture(diffuseTexture, TexCoord);\n"
				"	else\n"
				"		diffuseColor = myColor;\n"
				"   if(numOfTexture>1)\n"
				"		specularColor = texture(specularTexture, TexCoord);\n"
				"	else\n"
				"		specularColor = myColor;\n"

				"   FragColor = (ambientIntensity + diffuseIntensity) * diffuseColor + specularIntensity * specularColor;\n"
				"	}\n"
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
				char infoLog[512];
				glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				assert(success == GL_TRUE);
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				assert(success == GL_TRUE);
			}
			//shader link
			GouraudShader = glCreateProgram();
			glAttachShader(GouraudShader, vertexShader);
			glAttachShader(GouraudShader, fragmentShader);
			glLinkProgram(GouraudShader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			isGouraudShaderGenerated = true;
		}
		myshader = GouraudShader;
		glUseProgram(myshader);
	}

	static void SetPhongShader() {
		static bool isPhongShaderGenerated = false;
		if (!isPhongShaderGenerated)
		{
			const char* vertexShaderSource = "#version 460 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"layout (location = 1) in vec3 aNormal;\n"
				"layout (location = 2) in vec2 aTexCoord;\n"

				"uniform mat4 View;\n"
				"uniform mat4 Projection;\n"
				"uniform mat4 Model;\n"

				"uniform int isNormalTextureExists;\n"
				"uniform vec3 LightPosition;\n"
				"uniform vec3 CameraPos;\n"

				"uniform sampler2D normalTexture;\n"

				"out vec2 TexCoord;\n"

				"out vec3 fN;\n"
				"out vec3 fE;\n"
				"out vec3 fL;\n"
				"void main()\n"
				"{\n"
				"	TexCoord = aTexCoord;\n"
				"   vec3 FragPos = vec3(Model * vec4(aPos, 1.0));\n"
				"	vec3 N;\n"
				"	if(isNormalTextureExists==1)"
				"		{N=normalize(vec3(texture(normalTexture, aTexCoord))-vec3(0.5, 0.5, 0.5));"
				"		 N = vec3(N.x, N.z, -N.y);}\n"
				"	else"
				"		N = aNormal;\n"
				"	fN = normalize(mat3(transpose(inverse(Model))) * N);\n"
				"	fE = CameraPos-FragPos;\n"
				"	fL = LightPosition-FragPos;\n"
				"   gl_Position = (Projection * View * vec4(FragPos, 1.0));\n"
				"}\0";

			const char* fragmentShaderSource = "#version 460 core\n"
				"in vec2 TexCoord;\n"
				"in vec3 fN;\n"
				"in vec3 fE;\n"
				"in vec3 fL;\n"
				"uniform vec4 myColor;\n"
				"uniform int numOfTexture;\n"
				"uniform int isText;\n"

				"uniform sampler2D diffuseTexture;\n"
				"uniform sampler2D specularTexture;\n"

				"uniform float ambientIaka;\n"
				"uniform float diffuseIpkd;\n"
				"uniform float specularIpks;\n"

				"uniform vec3 LightDirection;\n"
				"uniform float DambientIaka;\n"
				"uniform float DdiffuseIpkd;\n"
				"uniform float DspecularIpks;\n"


				"out vec4 FragColor;\n"
				"void main()\n"
				"{\n"
				"	if(isText==1)"
				"		FragColor = myColor;\n"
				"	else"
				"	{\n"
				"	float dL = length(fL);\n"
				"	float fatt = 1./(1+0.001*dL + (0.001*dL)*(0.001*dL));\n"
				
				"   vec3 N = normalize(fN);\n"
				"   vec3 E = normalize(fE);\n"
				"   vec3 L = normalize(fL);\n"
				"   vec3 DL = normalize(LightDirection);\n"
				"	vec3 H = normalize( L + E );\n"
				"	vec3 DH = normalize( DL + E );\n"

				"	vec4 diffuseColor = vec4(0, 0, 0, 0);\n"
				"	vec4 specularColor = vec4(0, 0, 0, 0);\n"

				"   if(numOfTexture>0)\n"
				"		diffuseColor = texture(diffuseTexture, TexCoord);\n"
				"	else\n"
				"		diffuseColor = myColor;\n"
				"   if(numOfTexture>1)\n"
				"		specularColor = texture(specularTexture, TexCoord);\n"
				"	else\n"
				"		specularColor = myColor;\n"

				"   FragColor = (ambientIaka * diffuseColor + fatt * (diffuseIpkd* max(dot(N, L), 0.0) * diffuseColor+ specularIpks * pow(max(dot(H, N), 0.0), 16) * specularColor));\n"
				"   FragColor = FragColor + (DambientIaka * diffuseColor + (DdiffuseIpkd* max(dot(N, DL), 0.0) * diffuseColor+ DspecularIpks * pow(max(dot(DH, N), 0.0), 16) * specularColor));\n"
				"	}\n"
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
				char infoLog[512];
				glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				assert(success == GL_TRUE);
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				assert(success == GL_TRUE);
			}
			//shader link
			PhongShader = glCreateProgram();
			glAttachShader(PhongShader, vertexShader);
			glAttachShader(PhongShader, fragmentShader);
			glLinkProgram(PhongShader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			isPhongShaderGenerated = true;
		}
		myshader = PhongShader;
		glUseProgram(myshader);
	}
	static void setFloat(const std::string& name, const GLfloat value) //https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
	{
		glUniform1f(glGetUniformLocation(myshader, name.c_str()), value);
	}
	static void setInt(const std::string& name, const GLuint value) //https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
	{
		glUniform1i(glGetUniformLocation(myshader, name.c_str()), value);
	}
	static void setVec3(const std::string& name, const glm::vec3& value) //https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
	{
		glUniform3fv(glGetUniformLocation(myshader, name.c_str()), 1, &value[0]);
	}
	static void setVec3(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(myshader, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	static void setVec4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(myshader, name.c_str()), 1, &value[0]);
	}
	static void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(myshader, name.c_str()), x, y, z, w);
	}
	static void setMat3(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(myshader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	static void setMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(myshader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	static void setTexture(const std::string& name, const GLuint n)
	{
		glUniform1i(glGetUniformLocation(myshader, name.c_str()), n);
	}
};