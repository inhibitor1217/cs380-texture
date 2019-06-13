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
float g_window_width = 1440.f;
float g_window_height = 1080.f;
int g_framebuffer_width = 1440;
int g_framebuffer_height = 1080;

constexpr float PI = 3.14159265f;
constexpr float CAMERA_ROTATE_SPEED = 0.5f;

static float camera_direction = 0;

static Engine::Camera *camera;
static std::vector<Light> lights;

static bool mode_cam_left = false;
static bool mode_cam_right = false;
static bool mode_cam_forward = false;
static bool mode_cam_back = false;

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
			break;
		case GLFW_KEY_UP:
			mode_cam_forward = true;
			break;
		case GLFW_KEY_DOWN:
			mode_cam_back = true;
			break;
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
			break;
		case GLFW_KEY_UP:
			mode_cam_forward = false;
			break;
		case GLFW_KEY_DOWN:
			mode_cam_back = false;
			break;
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
	camera->SetProjection(g_window_width / g_window_height, 70.0, 0.1, 1200);

	lights.push_back({
		LightType::DirectionalLight,
		1,
		Engine::Transform(),
		glm::vec3(2, 3, 1),
		glm::vec3(1, 1, 1)
	});

	Engine::Mesh *roomMesh = new Engine::Mesh();
	roomMesh->AddAttribute(4);
	roomMesh->AddAttribute(4);
	roomMesh->AddAttribute(2);
	roomMesh->AddAttribute(4);

	for (int x = -10; x < 10; x++)
	{
		for (int y = -5; y < 5; y++)
		{
			int block_type = 0x06;
			int block_x = block_type & 0xF;
			int block_y = block_type >> 4;
			float offset = 0.025f / 16.0f;
			float atlas_size = 1.0f / 16.0f;

			roomMesh->AddVertexData(glm::vec4(x, y, -1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size + offset, (block_y + 1) * atlas_size - offset));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x+1, y, -1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size - offset, (block_y + 1) * atlas_size - offset));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x+1, y+1, -1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size - offset, block_y * atlas_size + offset));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x, y, -1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size + offset, (block_y + 1) * atlas_size - offset));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x+1, y+1, -1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size - offset, block_y * atlas_size + offset));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x, y+1, -1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size + offset, block_y * atlas_size + offset));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			block_type = (x == -10 || x == 9) || (y == -5 || y == 4) ? 0x15 : 0x10;
			block_x = block_type & 0xF;
			block_y = block_type >> 4;

			roomMesh->AddVertexData(glm::vec4(x, y, 3, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, -1, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, y + 1, 3, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, -1, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, y, 3, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, -1, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x, y, 3, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, -1, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x, y + 1, 3, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, -1, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, y + 1, 3, 1));
			roomMesh->AddVertexData(glm::vec4(0, 0, -1, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));

		}
	}

	for (int x = -10; x < 10; x++)
	{
		for (int z = -1; z < 3; z++)
		{
			int block_type = x == -10 || x == 9 ? 0x14 : 0x04;
			int block_x = block_type & 0xF;
			int block_y = block_type >> 4;
			float atlas_size = 1.0f / 16.0f;

			roomMesh->AddVertexData(glm::vec4(x, -5, z, 1));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, -5, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, -5, z, 1));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x, -5, z, 1));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x, -5, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, -5, z+1, 1));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			block_type = x == -10 || x == 9 ? 0x14 : (x > -7 && x < 5 && z == 1) ? 0x54 : 0x04;
			block_x = block_type & 0xF;
			block_y = block_type >> 4;

			roomMesh->AddVertexData(glm::vec4(x, 5, z, 1));
			roomMesh->AddVertexData(glm::vec4(0, -1, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, 5, z, 1));
			roomMesh->AddVertexData(glm::vec4(0, -1, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, 5, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(0, -1, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x, 5, z, 1));
			roomMesh->AddVertexData(glm::vec4(0, -1, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x + 1, 5, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(0, -1, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

			roomMesh->AddVertexData(glm::vec4(x, 5, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(0, -1, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 0, 1, 0));

		}
	}

	for (int y = -5; y < 5; y++)
	{
		for (int z = -1; z < 3; z++)
		{
			int block_type = y == -5 || y == 4 ? 0x14 : 0x04;
			int block_x = block_type & 0xF;
			int block_y = block_type >> 4;
			float atlas_size = 1.0f / 16.0f;

			roomMesh->AddVertexData(glm::vec4(-10, y, z, 1));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(-10, y + 1, z, 1));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(-10, y+1, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(-10, y, z, 1));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(-10, y + 1, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(-10, y, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));



			roomMesh->AddVertexData(glm::vec4(10, y, z, 1));
			roomMesh->AddVertexData(glm::vec4(-1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(10, y + 1, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(-1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(10, y + 1, z, 1));
			roomMesh->AddVertexData(glm::vec4(-1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(10, y, z, 1));
			roomMesh->AddVertexData(glm::vec4(-1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, (block_y + 1) * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(10, y, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(-1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2(block_x * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

			roomMesh->AddVertexData(glm::vec4(10, y + 1, z + 1, 1));
			roomMesh->AddVertexData(glm::vec4(-1, 0, 0, 0));
			roomMesh->AddVertexData(glm::vec2((block_x + 1) * atlas_size, block_y * atlas_size));
			roomMesh->AddVertexData(glm::vec4(0, 1, 0, 0));

		}
	}

	roomMesh->SetNumElements(3840);
	roomMesh->CreateMesh();

	Engine::TextureLoader *roomTextureLoader = new Engine::TextureLoader(0, "Resources\\Textures\\BlockTexture.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	DiffuseMaterial *diffuseMaterial = new DiffuseMaterial();
	diffuseMaterial->CreateMaterial(roomTextureLoader->GetUnit());

	Engine::RenderObject *roomObject = new Engine::RenderObject(roomMesh, diffuseMaterial);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

	glEnable(GL_TEXTURE_2D);
	// glEnable(GL_TEXTURE_CUBE_MAP);

	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(g_window, KeyboardCallback);

	float prev_time = (float)glfwGetTime();

	glClearColor(1, 0, 0, 1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		float total_time = (float)glfwGetTime();
		float elapsed_time = total_time - prev_time;
		prev_time = total_time;

		/* Clear buffers. */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera_direction += mode_cam_left ? elapsed_time * CAMERA_ROTATE_SPEED : 0;
		camera_direction += mode_cam_right ? -elapsed_time * CAMERA_ROTATE_SPEED : 0;
		camera->GetTransform()->SetPosition(glm::vec3(
			camera->GetTransform()->GetPosition()
			+ (mode_cam_forward ?  2 * elapsed_time * glm::vec3(cos(camera_direction + 0.5 * PI), sin(camera_direction + 0.5 * PI), 0.0f) : glm::vec3(0.0f))
			+ (mode_cam_back    ? -2 * elapsed_time * glm::vec3(cos(camera_direction + 0.5 * PI), sin(camera_direction + 0.5 * PI), 0.0f) : glm::vec3(0.0f))
		));
		glm::vec3 pos = camera->GetTransform()->GetPosition();
		if (pos.x > 9)
			camera->GetTransform()->SetPosition(glm::vec3(9, pos.y, pos.z));
		else if (pos.x < -9)
			camera->GetTransform()->SetPosition(glm::vec3(-9, pos.y, pos.z));
		if (pos.y > 4)
			camera->GetTransform()->SetPosition(glm::vec3(pos.x, 4, pos.z));
		else if (pos.y < -4)
			camera->GetTransform()->SetPosition(glm::vec3(pos.x, -4, pos.z));
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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		roomObject->Render(camera);

		glDisable(GL_BLEND);
			
		/* Swap front and back buffers */
		glfwSwapBuffers(g_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Delete resources
	glfwTerminate();
	return 0;

}