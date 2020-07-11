#pragma once
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace glm;
using namespace std;

/* IGNORE
static const vec3 grid_vertices[] =
{
	vec3(0.0f, 0.0f, 0.0f),
	vec3(1.0f, 0.0f, 0.0f),

	vec3(1.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 1.0f),


};

static const vec3 axis_vertices[] = {

	     //x 
	     // y axis of x and z is set to 0.5 because otherwise it blends into the grid
		 vec3(0.0f, 0.5f,  0.0f),
		 vec3(5.0f, 0.5f,  0.0f),

		 //y
		 vec3(0.0f, 0.0f,  0.0f),
         vec3(0.0f, 5.0f,  0.0f),

		 //z
		 vec3(0.0f, 0.5f,  0.0f),
		 vec3(0.0f, 0.5f,  5.0f),

};


static const vec3 axis_Colors[] =
{
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f)
};
*/

 //unit cube model to use for I and 9 model
static const vec3 model_vertices[] = {
	vec3(-0.5f,-0.5f,-0.5f),
	vec3(-0.5f,-0.5f, 0.5f),
	vec3(-0.5f, 0.5f, 0.5f),

	vec3(-0.5f,-0.5f,-0.5f),
	vec3(-0.5f, 0.5f, 0.5f),
	vec3(-0.5f, 0.5f,-0.5f),

	vec3(0.5f, 0.5f,-0.5f),
	vec3(-0.5f,-0.5f,-0.5f),
	vec3(-0.5f, 0.5f,-0.5f),

	vec3(0.5f, 0.5f,-0.5f),
	vec3(0.5f,-0.5f,-0.5f),
	vec3(-0.5f,-0.5f,-0.5f),

	vec3(0.5f,-0.5f, 0.5f),
	vec3(-0.5f,-0.5f,-0.5f),
	vec3(0.5f,-0.5f,-0.5f),

	vec3(0.5f,-0.5f, 0.5f),
	vec3(-0.5f,-0.5f, 0.5f),
	vec3(-0.5f,-0.5f,-0.5f),

	vec3(-0.5f, 0.5f, 0.5f),
	vec3(-0.5f,-0.5f, 0.5f),
	vec3(0.5f,-0.5f, 0.5f),

	vec3(0.5f, 0.5f, 0.5f),
	vec3(-0.5f, 0.5f, 0.5f),
	vec3(0.5f,-0.5f, 0.5f),

	vec3(0.5f, 0.5f, 0.5f),
	vec3(0.5f,-0.5f,-0.5f),
	vec3(0.5f, 0.5f,-0.5f),

	vec3(0.5f,-0.5f,-0.5f),
	vec3(0.5f, 0.5f, 0.5f),
	vec3(0.5f,-0.5f, 0.5f),

	vec3(0.5f, 0.5f, 0.5f),
	vec3(0.5f, 0.5f,-0.5f),
	vec3(-0.5f, 0.5f,-0.5f),

	vec3(0.5f, 0.5f, 0.5f),
	vec3(-0.5f, 0.5f,-0.5f),
	vec3(-0.5f, 0.5f, 0.5f)

};