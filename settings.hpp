
#pragma once
#include <array>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
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
#include "glm/gtc/noise.hpp"

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <glm/glm.hpp>
#include <cmath>

// OpenGL settings
const int MAJOR_VER = 3;
const int MINOR_VER = 3;
const int DEPTH_SIZE = 24;
const int NUM_SAMPLES = 1;  // Antialiasing

// Resolution
const glm::vec2 WIN_RES(1600, 900);

// World generation
const int SEED = 16;

// Ray casting
const int MAX_RAY_DIST = 6;

// Chunk settings
const int CHUNK_SIZE = 48;
const int H_CHUNK_SIZE = CHUNK_SIZE / 2;
const int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
const int CHUNK_VOL = CHUNK_AREA * CHUNK_SIZE;
const float CHUNK_SPHERE_RADIUS = H_CHUNK_SIZE * std::sqrt(3);

// World dimensions
const int WORLD_W = 10;
const int WORLD_H = 2;
const int WORLD_D = WORLD_W;
const int WORLD_AREA = WORLD_W * WORLD_D;
const int WORLD_VOL = WORLD_AREA * WORLD_H;

// World center
const int CENTER_XZ = WORLD_W * H_CHUNK_SIZE;
const int CENTER_Y = WORLD_H * H_CHUNK_SIZE;

// Camera settings
const float ASPECT_RATIO = WIN_RES.x / WIN_RES.y;
const float FOV_DEG = 90.0f;
const float V_FOV = glm::radians(FOV_DEG);
const float H_FOV = 2.0f * std::atan(std::tan(V_FOV * 0.5f) * ASPECT_RATIO);
const float NEAR = 0.1f;
const float FAR = 2000.0f;
const float PITCH_MAX = glm::radians(89.0f);

// Player settings
const float PLAYER_SPEED = 50.0f;
const float PLAYER_ROT_SPEED = 0.003f;
const glm::vec3 PLAYER_POS(CENTER_XZ, CHUNK_SIZE, CENTER_XZ);
const float MOUSE_SENSITIVITY = 0.002f;

// Colors
const glm::vec3 BG_COLOR(0.58f, 0.83f, 0.99f);

// Texture IDs
const int SAND = 1;
const int GRASS = 2;
const int DIRT = 3;
const int STONE = 4;
const int SNOW = 5;
const int LEAVES = 6;
const int WOOD = 7;

// Terrain levels
const int SNOW_LVL = 54;
const int STONE_LVL = 49;
const int DIRT_LVL = 40;
const int GRASS_LVL = 8;
const int SAND_LVL = 7;

// Tree settings
const float TREE_PROBABILITY = 0.02f;
const int TREE_WIDTH = 4;
const int TREE_HEIGHT = 8;
const int TREE_H_WIDTH = TREE_WIDTH / 2;
const int TREE_H_HEIGHT = TREE_HEIGHT / 2;

// Water settings
const float WATER_LINE = 5.6f;
const int WATER_AREA = 5 * CHUNK_SIZE * WORLD_W;

// Cloud settings
const float CLOUD_SCALE = 25.0f;
const float CLOUD_HEIGHT = WORLD_H * CHUNK_SIZE * 2;

#endif // SETTINGS_HPP



#define ASSERT(x) if (!(x)) __debugbreak()

#define GLCall(x) do { \
GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
} while (0)

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);




