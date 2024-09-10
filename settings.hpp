
#pragma once
#include <array>
#include "glm.hpp"
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>


#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include "gtc/noise.hpp"

const sf::VideoMode WIN_RES(1600, 900); // Define your window resolution here
const sf::Color BG_COLOR(110, 110, 110, 255); // Background color

constexpr float V_FOV = glm::radians(100.0f); // Vertical field of view
constexpr float H_FOV = glm::radians(100.0f); // Horizontal field of view
constexpr float ASPECT_RATIO = 16.0f / 9.0f; // Aspect ratio
constexpr float NEAR = 0.1f;                 // Near plane distance
constexpr float FAR = 1000.0f;               // Far plane distance
constexpr float CHUNK_SPHERE_RADIUS = 1.0f;  // Radius of the bounding sphere of a chunk
constexpr float PITCH_MAX = glm::radians(89.0f); // Maximum pitch angle

// chunk
constexpr int CHUNK_SIZE = 32;
constexpr int H_CHUNK_SIZE = CHUNK_SIZE / 2;
constexpr int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
constexpr int CHUNK_VOL = CHUNK_AREA * CHUNK_SIZE;

//world
constexpr int WORLD_W = 10;
constexpr int WORLD_H = 3;
constexpr int WORLD_D = 10;
constexpr int WORLD_AREA = WORLD_W * WORLD_D;
constexpr int WORLD_VOL = WORLD_AREA * WORLD_H;

constexpr float PLAYER_SPEED = 21.0f;
constexpr float MOUSE_SENSITIVITY = 0.002f;
constexpr float PlAYER_ROT_SPEED = 0.003f;
constexpr glm::vec3 PLAYER_POS = glm::vec3(1.0f, 0.0f, 1.0f);


#define ASSERT(x) if (!(x)) __debugbreak()

#define GLCall(x) do { \
GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
} while (0)

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);




