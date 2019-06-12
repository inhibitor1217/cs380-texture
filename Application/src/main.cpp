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
#include <RenderObject.hpp>
#include <ModelLoader.hpp>
#include <Texture.hpp>

#include <DiffuseMaterial.hpp>
#include <PickingMaterial.hpp>
#include <NormalMaterial.hpp>
#include <ShadowMaterial.hpp>
#include <Geometry.hpp>
#include <ModelLoader.hpp>

#include <picking.hpp>
#include <PickableObject.hpp>

#include <CubeMaterial.hpp>
#include <Animation.hpp>

#include <ShadowMap.hpp>
#

GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

Engine::Camera* main_camera;

Engine::Mesh* bunny_mesh;
Engine::Mesh* cube_mesh;

DiffuseMaterial* material;
CubeMaterial* cubemap_material;

Engine::RenderObject* bunny_object; 
Engine::RenderObject* cube_object;
Engine::RenderObject* cubemap_object;

std::vector<Light> lights;

Animation* bunny_animation;
Animation* camera_animation;
Animation* light_animation;

Engine::Transform* oriT;

static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
	if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(a_window, &xpos, &ypos);
		xpos = xpos / ((double)g_window_width) * ((double)g_framebuffer_width);
		ypos = ypos / ((double)g_window_height) * ((double)g_framebuffer_height);
		int target = pick((int)xpos, (int)ypos, g_framebuffer_width, g_framebuffer_height);
	}
}

static void CursorPosCallback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{

}

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
	if (a_action == GLFW_PRESS)
	{
		switch (a_key)
		{
		case GLFW_KEY_H:
			std::cout << "CS380 LAB 4" << std::endl;
			std::cout << "keymaps:" << std::endl;
			std::cout << "h\t\t Help command" << std::endl;
			std::cout << "r\t\t rotate the camera -20 degrees with respect to world's y axis" << std::endl;
			std::cout << "l\t\t rotate the camera +20 degrees with respect to world's y axis" << std::endl;
			break;
		case GLFW_KEY_L:
			oriT->SetOrientation(glm::rotate(oriT->GetOrientation(), glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			break;
		case GLFW_KEY_R:
			oriT->SetOrientation(glm::rotate(oriT->GetOrientation(), glm::radians(+20.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			break;
		default:
			break;
		}
	}
}

static void FramebufferResizeCallback(GLFWwindow* a_window, int width, int height)
{
	glViewport(0, 0, width, height);
	reallocatePickingTexture(width, height, 0);
	reallocateShadowTexture(width, height, 0);

	int window_width, window_height;
	glfwGetWindowSize(g_window, &window_width, &window_height);
	g_window_width = (float)window_width;
	g_window_height = (float)window_height;
	g_framebuffer_width = width;
	g_framebuffer_height = height;
	
	Engine::ProjectionData proj = main_camera->GetProjection();
	proj.aspectRatio = (float)width / (float)height;
	main_camera->SetProjection(proj.aspectRatio, proj.fov, proj.zNear, proj.zFar);
}

static void InitScene()
{
	main_camera = new Engine::Camera();
	main_camera->GetTransform()->SetPosition(glm::vec3(0.0f, 1.0f, 4.0f));
	oriT = new Engine::Transform;
	main_camera->GetTransform()->AddParent(oriT);

	Engine::ModelLoader *loader = new Engine::ModelLoader("Resources\\Models\\bunny.obj");

	bunny_mesh = new Engine::Mesh();
	cube_mesh = new Engine::Mesh();

	Geometry geometry = Geometry();
	geometry.GenerateCube(cube_mesh);
	
	loader->GenerateMesh(bunny_mesh, 0);
		
	Engine::TextureLoader* textureloader1 = new Engine::TextureLoader(2, "Resources\\Models\\bunny.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	std::string path_prefix = "Resources\\Textures\\skybox\\";
	Engine::TextureLoader* textureloader2 = new Engine::TextureLoader(3, path_prefix + "right.jpg", path_prefix + "left.jpg",
		path_prefix + "top.jpg", path_prefix + "bottom.jpg", path_prefix + "front.jpg", path_prefix + "back.jpg");
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	cubemap_material = new CubeMaterial();
	cubemap_material->CreateMaterial(textureloader2->GetUnit());
	cubemap_object = new Engine::RenderObject(cube_mesh, cubemap_material);


	material = new DiffuseMaterial();
	material->CreateMaterial(textureloader1->GetUnit());

	bunny_object = new Engine::RenderObject(bunny_mesh, material);
	cube_object = new Engine::RenderObject(cube_mesh, cubemap_material);

	Engine::Transform* bunny_T = bunny_object->GetTransform();
	bunny_T->SetPosition(glm::vec3(-1.5f, 0.8f, 0.0f));
	bunny_object->SetTransform(*bunny_T);

	Engine::Transform* cube_T = cube_object->GetTransform();
	cube_T->SetPosition(glm::vec3(1.5f, 0.8f, 0.0f));
	cube_T->SetScale(glm::vec3(0.5f));
	cube_object->SetTransform(*cube_T);

	Light sun = Light();
	sun.type = DirectionalLight; //directional light
	sun.transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	sun.transform.LookAt(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	sun.diffuse_illuminance = glm::vec3(1.0f, 1.0f, 1.0f);
	lights.push_back(sun);
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
	g_window = glfwCreateWindow(g_window_width, g_window_height, "HW4", NULL, NULL);
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


	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);

	glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
	glfwSetCursorPosCallback(g_window, CursorPosCallback);
	glfwSetKeyCallback(g_window, KeyboardCallback);
	glfwSetFramebufferSizeCallback(g_window, FramebufferResizeCallback);

	// Initialize framebuffer object and picking textures
	pickingInitialize(g_framebuffer_width, g_framebuffer_height, 0);
	shadowInitialize(g_framebuffer_width, g_framebuffer_height, 1);
	InitScene();

	float prev_time = (float)glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		float total_time = (float)glfwGetTime();
		float elapsed_time = total_time - prev_time;
		prev_time = total_time;

		// First Pass: Object Selection
		// this is for picking the object using mouse interaction
		// binding framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
		// Background: RGB = 0x000000 => objectID: 0
		glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* RenderPicking here */

		// TODO
		// Second Pass: Shadow Mapping (See the first pass and picking.hpp to understand how you implement render to texture)






		// Third Pass: Object Rendering
		// Drawing object again
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor((GLclampf)(128. / 255.), (GLclampf)(200. / 255.), (GLclampf)(255. / 255.), (GLclampf)0.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Render here */
		material->UpdateLight(lights);

		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		cubemap_material->IsSkybox(1);
		cubemap_object->Render(main_camera);
		glEnable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);

		material->UpdateDiffuseReflectance(glm::vec3(0.9f, 0.9f, 0.9f));
		bunny_object->Render(main_camera);

		cubemap_material->IsSkybox(0);
		cube_object->Render(main_camera);
			
		/* Swap front and back buffers */
		glfwSwapBuffers(g_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Delete resources
	delete main_camera;
	deletePickingResources();
	deleteShadowResources();
	glfwTerminate();
	return 0;

}