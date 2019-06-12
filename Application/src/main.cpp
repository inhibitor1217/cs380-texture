// 2019 Spring CS380 Homework 4
// You have to implement
// 1. Normal mapping
//	NormalMaterial.cpp, NormalMaterial.hpp
//	Vertex has new attribute vec4 tangent (see ModelLoader.cpp or Geometry.cpp)
// 2. Shadow Mapping
//	ShadowMap.hpp, ShadowMaterial.cpp, ShadowMaterial.hpp
//
// You can create/modify any functions or files in APP
// Use the OpenGL documentation
// You could use glm::ortho(), glm::lookAt() for shadow mapping

// STL
#include <iostream>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <Texture.hpp>
#include <RenderObject.hpp>

#include <Geometry.hpp>
#include <DiffuseMaterial.hpp>

GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

constexpr float PI = 3.14159265f;
constexpr float CAMERA_ROTATE_SPEED = 0.5f;

static Engine::Camera *camera;
static std::vector<Light> lights;

static bool mode_cam_left = false;
static bool mode_cam_right = false;

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
	if (a_action == GLFW_PRESS)
	{
		switch (a_key)
		{
		case GLFW_KEY_LEFT:
			mode_cam_left = true;
			break;
		case GLFW_KEY_RIGHT:
			mode_cam_right = true;
		}
	}
	else if (a_action == GLFW_RELEASE)
	{
		switch (a_key)
		{
		case GLFW_KEY_LEFT:
			mode_cam_left = false;
			break;
		case GLFW_KEY_RIGHT:
			mode_cam_right = false;
		}
	}
}

int main(int argc, char** argv)
{
	// Initialize GLFW library
	if (!glfwInit())
	{
		return -1;
	}

	// Create window and OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/* Create a windowed mode window and its OpenGL context */
	g_window = glfwCreateWindow(g_window_width, g_window_height, "HW4 - 20170737", NULL, NULL);
	if (!g_window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(g_window);

	// Initialize GLEW library
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
		exit(1);
	}

	// Initialization
	// Screw OOP and just write everything here

	camera = new Engine::Camera(
		glm::vec3(0),
		glm::rotate(
			glm::mat4(1.0f), 0.5f * PI, glm::vec3(1, 0, 0)
		)
	);
	camera->SetProjection(g_window_width / g_window_height, 70.0, 0.1, 120);

	lights.push_back({
		LightType::DirectionalLight,
		1,
		Engine::Transform(),
		glm::vec3(-0.7, -0.3, 1.0),
		glm::vec3(1, 1, 1)
	});

	Geometry geometry = Geometry();

	Engine::Mesh *roomMesh = new Engine::Mesh();
	geometry.GenerateCube(roomMesh, true);

	Engine::Mesh *cubeMesh = new Engine::Mesh();
	geometry.GenerateCube(cubeMesh);

	DiffuseMaterial *diffuseMaterial = new DiffuseMaterial();
	diffuseMaterial->CreateMaterial(0);

	Engine::RenderObject *roomObject = new Engine::RenderObject(roomMesh, diffuseMaterial);
	roomObject->GetTransform()->SetScale(glm::vec3(20.0f, 20.0f, 5.0f));

	Engine::RenderObject *cube = new Engine::RenderObject(cubeMesh, diffuseMaterial);
	cube->GetTransform()->SetPosition(glm::vec3(2, 5, -1.5f));

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

	// glEnable(GL_TEXTURE_2D);
	// glEnable(GL_TEXTURE_CUBE_MAP);

	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(g_window, KeyboardCallback);

	float prev_time = (float)glfwGetTime();

	glClearColor(0, 0, 0, 1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		float total_time = (float)glfwGetTime();
		float elapsed_time = total_time - prev_time;
		prev_time = total_time;

		/* Clear buffers. */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera->GetTransform()->SetOrientation(
			glm::rotate(
				camera->GetTransform()->GetOrientation(),
				mode_cam_left ? elapsed_time * CAMERA_ROTATE_SPEED :
				mode_cam_right ? -elapsed_time * CAMERA_ROTATE_SPEED : 0,
				glm::vec3(0, 1, 0)
			)
		);
		
		diffuseMaterial->UpdateDiffuseReflectance(glm::vec3(1, 1, 1));
		diffuseMaterial->UpdateLight(lights);

		roomObject->Render(camera);
		cube->Render(camera);
			
		/* Swap front and back buffers */
		glfwSwapBuffers(g_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Delete resources
	glfwTerminate();
	return 0;

}