#include <Angel_commons/Angel.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

//irrKlang
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "filesystem.h"

#include <iostream>
#include <map>
#include <string>
#include <ctime>

#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

// start the sound engine with default parameters
ISoundEngine* engine = createIrrKlangDevice();

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

struct Point {
	unsigned int vbo;
	vector<GLuint> ebo;
} p1;


std::map<GLchar, Character> Characters;
GLuint textVAO, textVBO;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(vector<std::string> faces);
void renderScene(const Shader &shader, Model ball, Model lamp, GLFWwindow* window);
void renderCube();
void renderLight();
void renderDice();
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
struct Point Readobjfile(string filename, struct Point point);

// settings
const unsigned int SCR_WIDTH = 1366;
const unsigned int SCR_HEIGHT = 768;
bool shadows = true;
bool shadowsKeyPressed = false;
int reflectKey = 1;
bool reflectEnable = false;
int forBall = -1;
int finishGame = -1;
int collision = -1;
bool overr = false;
// camera
Camera camera(glm::vec3(0.0f, 5.0f, 25.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
float theta = 0.0f;
float rotatee = 0.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int woodTexture;
unsigned int glassTexture;
vector<unsigned int> venomTexture;
vector<unsigned int> diceFaces;

//Dice Rotate
bool  stop = false;
glm::vec3 aboutit = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 angles = glm::vec3(0.0f, 0.0f, 0.0f);
clock_t start;
int duration;
int loop = 0;
int pointindex = 0;
vector<int>  points;
bool  dalga = true;
float  added = 0.0f;

struct Point cyc;
struct Point flag;
float angle = 0.0f;

int a = -1, b = -1;

glm::vec3   cubecoor = glm::vec3(-9, 5, 0);
glm::vec3   flagcoor = glm::vec3(0, 0, 0);

//Objectpos
//-----------
float t = 0.0f;
float t2 = 0.0f;
float g = 0.0f;
//Game Mechanism
//-----------------------------------
int lives = 5;
int score = 0;
int refract = -1;
BOOLEAN ballFree = false;
BOOLEAN gameOver = false;
BOOLEAN collisionWait = false;
int collisionn = -1;
int waitTime = 20;
int movementID = 1;
//Ball movement variables
//--------------------------------------------------------------------------------------------

BOOLEAN state1 = false;
BOOLEAN state2 = false;
BOOLEAN state4 = false;
BOOLEAN ballAir = false;
BOOLEAN stateBack = false;
BOOLEAN stateAim = true;
BOOLEAN targetShooted = false;
int asd = 0;
float fallTime = 10.0f;
float spaceDelay = 0;
float count2 = 1.0f;
float count3 = -5.0f;
float countGoBack = 0.0f;
float countGoBackX = 0.0f;
float veloX = 0.0f;
float veloY = 0.8f;
//float veloZ = 0.2f;
float rotateForAim = 0.0f;
float rotateForAim2 = 0.0f;
BOOLEAN stateNoBack = false;
float timeToTouchF = 0.0f;
float touchVelocity = 0.0f;
BOOLEAN stateTouchFloor = false;
//float time = 0.0f;
float translateX = 0.0f;
float translateZ = 2.5f;
float translateX2 = 0.0f;
float translateZ2 = 0.0f;
float rotateForBall = 0.0f;
float positionOfBallZ = 0.0f;
float positionOfBallX = 0.0f;
float positionOfWallZ = -25.0f;
float crossTime = 0.0f;
float currentTime = 0.0f;
float countGoX = 0.0f;
//float currentTime2 = 0.0f;
glm::vec3 currentvelocity = glm::vec3(0.0f);
//---------------------------------------------------------------------------------------------

	// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(5.5f, 7.0f, -45.0f),
	glm::vec3(-5.5f, 7.0f, -45.0f),
	glm::vec3(5.5f, 7.0f, -65.0f),
	glm::vec3(-5.5f, 7.0f, -65.0f),
	glm::vec3(0.0f, 20.0f, -10.0f),
	glm::vec3(0.0f, 20.0f, -90.0f)
};
glm::vec3 spotLightPositions[] = {
	glm::vec3(-5.5f, 15.69f, -45.0f),
	glm::vec3(5.5f, 15.69f, -45.0f),
	glm::vec3(5.5f, 15.69f, -65.0f),
	glm::vec3(-5.5f, 15.69f, -65.0f)
};

glm::vec3 lightPos = pointLightPositions[4];
glm::vec3 lightDensity = glm::vec3(0.4f);


/////////////

bool  began = true;
float  goback = 25.0f;
bool over = false;
bool scaled = false;
float  scaleamonut = 0;


int main()
{
	

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ball To Wall", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global opengl state
	// -----------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// build and compile shaders
	// -------------------------
	Shader shader("pointShadowsV.glsl", "pointShadowsF.glsl");
	Shader simpleDepthShader("pointShadowsDepthV.glsl", "pointShadowsDepthF.glsl", "pointShadowsDepthG.glsl");
	Shader skyShader("skyBoxV.glsl", "skyBoxF.glsl");
	Shader textShader("textV.glsl", "textF.glsl");


	//Initialize the FreeText Library
	//---------------------------------

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//Fonts and Glyphs
	//----------------
	// Compile and setup the shader
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "NewBaskerville.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);




	// load textures
	// -------------
	woodTexture = loadTexture("wood.png");
	glassTexture = loadTexture("broken.jpg");

	vector<std::string> images
	{
		("venom/image_part_001.png"),
		("venom/image_part_002.png"),
		("venom/image_part_003.png"),
		("venom/image_part_004.png"),
		("venom/image_part_005.png"),
		("venom/image_part_006.png"),
		("venom/image_part_007.png"),
		("venom/image_part_008.png"),
		("venom/image_part_009.png"),
		("venom/image_part_010.png"),
		("venom/image_part_011.png"),
		("venom/image_part_012.png"),
		("venom/image_part_013.png"),
		("venom/image_part_014.png"),
		("venom/image_part_015.png"),
		("venom/image_part_016.png"),
		("venom/image_part_017.png"),
		("venom/image_part_018.png"),
		("venom/image_part_019.png"),
		("venom/image_part_020.png"),

	};

	for (int i = 0; i < images.size(); i++) {
		unsigned int venom = loadTexture(images[i].c_str());
		venomTexture.push_back(venom);
	}

	
	unsigned int one = loadTexture("dice/1.png");
	diceFaces.push_back(one);
	unsigned int two = loadTexture("dice/2.png");
	diceFaces.push_back(two);
	unsigned int three = loadTexture("dice/3.png");
	diceFaces.push_back(three);
	unsigned int four = loadTexture("dice/4.png");
	diceFaces.push_back(four);
	unsigned int five = loadTexture("dice/5.png");
	diceFaces.push_back(five);
	unsigned int six = loadTexture("dice/6.png");
	diceFaces.push_back(six);


	//SkyBox initialize
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------
	vector<std::string> faces
	{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------

	skyShader.use();
	skyShader.setInt("skybox", 0);
	shader.setInt("skybox", 0);

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Our Ball
	Model ball(FileSystem::getPath("Resources/sphere/sphere.obj"));
	Model lamp(FileSystem::getPath("Resources/lamp/street lamp.obj"));

	cyc = Readobjfile("../Resources/Flag/cyc.obj", cyc);
	flag = Readobjfile("../Resources/Flag/flag3.obj", flag);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// shader configuration
	// --------------------
	shader.use();
	shader.setInt("diffuseTexture", 0);

	shader.setInt("depthMap", 1);


	points.push_back(2);
	points.push_back(6);
	points.push_back(1);
	points.push_back(5);
	points.push_back(2);
	points.push_back(3);
	points.push_back(1);
	points.push_back(4);

	start = std::clock();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// move light position over time
		//lightPos.z = sin(glfwGetTime() * 0.5) * 3.0;

		if (a == -1 && b == -1) {
			a = std::rand() % 5;
			b = std::rand() % 4;
		}

		

		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set lighting uniforms
//------------------------------------------------------------------------------------------------------
		{
			shader.use();
			shader.setFloat("shininess", 64.0f);

			/*
			   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
			   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
			   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
			   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
			*/
			// directional light
			shader.setVec3("dirLight.position", pointLightPositions[4]);
			shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			shader.setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
			shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
			// point light 1
			shader.setVec3("pointLights[0].position", pointLightPositions[0]);
			shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("pointLights[0].constant", 1.0f);
			shader.setFloat("pointLights[0].linear", 0.09);
			shader.setFloat("pointLights[0].quadratic", 0.032);
			// point light 2
			shader.setVec3("pointLights[1].position", pointLightPositions[1]);
			shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("pointLights[1].constant", 1.0f);
			shader.setFloat("pointLights[1].linear", 0.09);
			shader.setFloat("pointLights[1].quadratic", 0.032);

			// point light 3
			shader.setVec3("pointLights[2].position", pointLightPositions[2]);
			shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
			shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
			shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("pointLights[2].constant", 1.0f);
			shader.setFloat("pointLights[2].linear", 0.09);
			shader.setFloat("pointLights[2].quadratic", 0.032);
			// point light 4
			shader.setVec3("pointLights[3].position", pointLightPositions[3]);
			shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
			shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
			shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("pointLights[3].constant", 1.0f);
			shader.setFloat("pointLights[3].linear", 0.09);
			shader.setFloat("pointLights[3].quadratic", 0.032);
			// point light 5
			shader.setVec3("pointLights[4].position", pointLightPositions[4]);
			shader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
			shader.setVec3("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
			shader.setVec3("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("pointLights[4].constant", 1.0f);
			shader.setFloat("pointLights[4].linear", 0.09);
			shader.setFloat("pointLights[4].quadratic", 0.032);
			// point light 6
			shader.setVec3("pointLights[5].position", pointLightPositions[5]);
			shader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
			shader.setVec3("pointLights[5].diffuse", 0.8f, 0.8f, 0.8f);
			shader.setVec3("pointLights[5].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("pointLights[5].constant", 1.0f);
			shader.setFloat("pointLights[5].linear", 0.09);
			shader.setFloat("pointLights[5].quadratic", 0.032);
		
			// spotLight 1
			shader.setVec3("spotLight[0].position", spotLightPositions[0]);
			shader.setVec3("spotLight[0].direction", 0.0, -1.0, 0.0);
			shader.setVec3("spotLight[0].ambient", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[0].diffuse", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("spotLight[0].constant", 1.0f);
			shader.setFloat("spotLight[0].linear", 0.09);
			shader.setFloat("spotLight[0].quadratic", 0.032);
			shader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(15.0f)));
			shader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(17.5f)));
			// spotLight 2
			shader.setVec3("spotLight[1].position", spotLightPositions[1]);
			shader.setVec3("spotLight[1].direction", 0.0, -1.0, 0.0);
			shader.setVec3("spotLight[1].ambient", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[1].diffuse", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[1].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("spotLight[1].constant", 1.0f);
			shader.setFloat("spotLight[1].linear", 0.09);
			shader.setFloat("spotLight[1].quadratic", 0.032);
			shader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(15.0f)));
			shader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(17.5f)));

			// spotLight 4
			shader.setVec3("spotLight[2].position", spotLightPositions[2]);
			shader.setVec3("spotLight[2].direction", 0.0, -1.0, 0.0);
			shader.setVec3("spotLight[2].ambient", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[2].diffuse", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[2].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("spotLight[2].constant", 1.0f);
			shader.setFloat("spotLight[2].linear", 0.09);
			shader.setFloat("spotLight[2].quadratic", 0.032);
			shader.setFloat("spotLight[2].cutOff", glm::cos(glm::radians(15.0f)));
			shader.setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(17.5f)));
			// spotLight 5
			shader.setVec3("spotLight[3].position", spotLightPositions[3]);
			shader.setVec3("spotLight[3].direction", 0.0, -1.0, 0.0);
			shader.setVec3("spotLight[3].ambient", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[3].diffuse", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[3].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("spotLight[3].constant", 1.0f);
			shader.setFloat("spotLight[3].linear", 0.09);
			shader.setFloat("spotLight[3].quadratic", 0.032);
			shader.setFloat("spotLight[3].cutOff", glm::cos(glm::radians(15.0f)));
			shader.setFloat("spotLight[3].outerCutOff", glm::cos(glm::radians(17.5f)));
			// spotLight 3
			shader.setVec3("spotLight[4].position", camera.Position);
			shader.setVec3("spotLight[4].direction", camera.Front);
			shader.setVec3("spotLight[4].ambient", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[4].diffuse", 1.0f, 1.0f, 1.0f);
			shader.setVec3("spotLight[4].specular", 1.0f, 1.0f, 1.0f);
			shader.setFloat("spotLight[4].constant", 1.0f);
			shader.setFloat("spotLight[4].linear", 0.09);
			shader.setFloat("spotLight[4].quadratic", 0.032);
			shader.setFloat("spotLight[4].cutOff", glm::cos(glm::radians(15.0f)));
			shader.setFloat("spotLight[4].outerCutOff", glm::cos(glm::radians(17.5f)));

		}

		shader.setInt("finishGame", finishGame);
		shader.setVec3("lightDensity", lightDensity);
		shader.setInt("collision", collision);
		//------------------------------------------------------------------------------------------------------

		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		float near_plane = 1.0f;
		float far_plane = 50.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		// 1. render scene to depth cubemap
		// --------------------------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		simpleDepthShader.use();
		for (unsigned int i = 0; i < 6; ++i)
			simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		simpleDepthShader.setFloat("far_plane", far_plane);
		simpleDepthShader.setVec3("lightPos", lightPos);
		
		renderScene(simpleDepthShader, ball, lamp, window);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. render scene as normal 
		// -------------------------
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.Position);
		shader.setInt("shadows", shadows); // enable/disable shadows by pressing 'O'
		shader.setFloat("far_plane", far_plane);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		renderScene(shader, ball, lamp, window);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyShader.setMat4("view", view);
		skyShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		RenderText(textShader, "Lives: " + std::to_string(lives), SCR_WIDTH / 25.0f, SCR_HEIGHT / 1.1f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		RenderText(textShader, "Score: " + std::to_string(score), SCR_WIDTH / 1.3f, SCR_HEIGHT / 1.1f, 1.0f, glm::vec3(1.0f, 0.00f, 0.00f));

		theta += 0.005;
		rotatee += 0.5f;
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// renders the 3D scene
// --------------------
void renderScene(const Shader &shader, Model ball, Model lamp, GLFWwindow* window)
{
	forBall = -1;
	shader.setInt("forBall", forBall);
	shader.setVec3("cameraPos", camera.Position);
	//First Level
	//-------------------------------------------------------------
	shader.setInt("reflectKey", refract);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	// room cube
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 0.5f, 5.0f));

	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();

	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0.0f, -10.0));
	model = glm::scale(model, glm::vec3(12.0f, 0.5f, 5.0f));

	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reverse_normals", 0); // and of course disable it
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0.0f, -20));
	model = glm::scale(model, glm::vec3(12.0f, 0.5f, 5.0f));

	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reverse_normals", 0); // and of course disable it
	//glEnable(GL_CULL_FACE);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0.0f, -30));
	model = glm::scale(model, glm::vec3(12.0f, 0.5f, 5.0f));

	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reverse_normals", 0); // and of course disable it
	//glEnable(GL_CULL_FACE);


	//Wall set
	//--------------------------------------------------
	//Left Side

	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-12.0f, 0.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 5.0f));

	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-12.0f, 0.5f, -10.0));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 5.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-12.0f, 0.5f, -20.0));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 5.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-12.0f, 0.5f, -30.0));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 5.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();

	//Right Side
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 0.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 5.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 0.5f, -10.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 5.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 0.5f, -20.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 5.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 0.5f, -30.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 5.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();

	//Back Side
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.5f, 5.0f));
	model = glm::scale(model, glm::vec3(12.0f, 0.5f, 0.5f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	//Front sides
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0, 0.5f, -35.0f));
	model = glm::scale(model, glm::vec3(3.0f, 0.5f, 0.5f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-9.0, 0.5f, -35.0f));
	model = glm::scale(model, glm::vec3(3.0f, 0.5f, 0.5f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	//Path Sides

	//--------------------------------------------------------------------


	// cubes
	shader.setInt("reflectKey", -1);
	int txcount = 0;

	for (int j = 0; j < 4; j++) {

		for (int i = 0; i < 5; i++) {

			model = glm::mat4(1.0f);
			if (!ballFree) {
				if (began)
				{


					if (i == 0)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 - goback / 2, 4 * (3 - j) + 2.5f + goback / 2, -25));
					}
					else if (i == 1)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 - goback * i, 4 * (3 - j) + 2.5f + goback * i, -25));
					}
					else if (i == 2)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 - goback * i, 4 * (3 - j) + 2.5f + goback * i, -25));
					}
					else if (i == 3)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 - goback * i, 4 * (3 - j) + 2.5f + goback * i, -25));
					}
					else if (i == 4)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 - goback * i, 4 * (3 - j) + 2.5f + goback * i, -25));
					}

					goback = goback - 0.005f;
					if (goback < 0)
					{


						began = false;
					}
				}
				else
				{
					model = glm::translate(model, glm::vec3(4 * i - 8, 4 * (3 - j) + 2.5f, -25));
				}

			}
			else
			{
				if (!scaled)
				{

					scaleamonut = scaleamonut + 0.002f;
					if (scaleamonut > 1.0f)
					{
						scaled = true;

					}
					model = glm::translate(model, glm::vec3(4 * i - 8, 4 * (3 - j) + 2.5f, -25));
				}
				else
				{
					if (i == 0)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 - goback, 4 * (3 - j) + 2.5f + goback, -25));
					}
					else if (i == 1)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 - goback / 2, 4 * (3 - j) + 2.5f + goback, -25));
					}
					else if (i == 2)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8, 4 * (3 - j) + 2.5f + goback, -25));
					}
					else if (i == 3)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 + goback / 2, 4 * (3 - j) + 2.5f + goback, -25));
					}
					else if (i == 4)
					{
						model = glm::translate(model, glm::vec3(4 * i - 8 + goback, 4 * (3 - j) + 2.5f + goback, -25));
					}

					goback = goback + 0.0005f;
				}



			}
			model = glm::scale(model, glm::vec3(2.0f - scaleamonut));

			shader.setMat4("model", model);
			if (i == a && j == b) {
				shader.setInt("reflectKey", 1);
				
				shader.setInt("collision", collisionn);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, glassTexture);

			}
			else {
				shader.setInt("reflectKey", -1);
				collision = -1;
				shader.setInt("collision", collision);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, venomTexture.at(txcount));
			}



			renderCube();
			txcount++;
		}
	}
	

	//Path Between Levels
	shader.setInt("reflectKey", -1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0.0f, -45.0f));
	model = glm::scale(model, glm::vec3(6.0f, 0.5f, 10.0f));

	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();

	shader.setInt("reflectKey", -1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0.0f, -67.5f));
	model = glm::scale(model, glm::vec3(6.0f, 0.5f, 12.5f));

	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();





	//Path Sides
	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-6.0, 0.5f, -45.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 10.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();

	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.0, 0.5f, -45.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 10.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();

	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-6.0, 0.5f, -67.5f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 12.5f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();

	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.0, 0.5f, -67.5f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 12.5f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();

	shader.setInt("reflectKey", -1);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.5f, -80.0f));
	model = glm::scale(model, glm::vec3(6.0f, 0.5f, 0.5f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();


	//Draw lamps on path

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.5f, 0.90f, -45.0f));
	model = glm::scale(model, glm::vec3(2.0f));
	shader.setMat4("model", model);
	lamp.Draw(shader);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-5.5f, 0.90f, -45.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(2.0f));
	shader.setMat4("model", model);
	lamp.Draw(shader);
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.5f, 0.90f, -65.0f));
	model = glm::scale(model, glm::vec3(2.0f));
	shader.setMat4("model", model);
	lamp.Draw(shader);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-5.5f, 0.90f, -65.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(2.0f));
	shader.setMat4("model", model);
	lamp.Draw(shader);



	collision = -1;
	shader.setInt("collision", collision);
	//Second Level
	//-----------------------------------------------------
	shader.setInt("reflectKey", 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	// room cube
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, -90));
	model = glm::scale(model, glm::vec3(12.0f, 15.0f, 20.0f));
	shader.setMat4("model", model);
	glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reverse_normals", 0); // and of course disable it
	glEnable(GL_CULL_FACE);
	// cubes
	shader.setInt("reflectKey", -1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	// room cube
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, -90));
	model = glm::scale(model, glm::vec3(11.99f, 14.99f, 19.99f));
	shader.setMat4("model", model);
	glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.setInt("reverse_normals", 0); // and of course disable it
	glEnable(GL_CULL_FACE);

	txcount = 0;

	for (int j = 0; j < 4; j++) {

		for (int i = 0; i < 5; i++) {

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(i - 2.5f, (3 - j) - 4, 0));
			model = glm::scale(model, glm::vec3(0.5f));
			shader.setMat4("model", model);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, venomTexture.at(txcount));

			//renderCube();
			txcount++;
		}
	}








	forBall = 1;
	shader.setInt("forBall", forBall);
	shader.setInt("reflectKey", -1);
	//Ball Draw
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(translateX, 2.1f, translateZ));
	//model = glm::scale(model, glm::vec3(0.25f));
	//shader.setMat4("model", model);

	
	//Ball movement keys
	//----------------------------------------------------------------
	int state3 = glfwGetKey(window, GLFW_KEY_SPACE);//input of user
	int stateleftRotateBall = glfwGetKey(window, GLFW_KEY_LEFT);
	int staterightRotateBall = glfwGetKey(window, GLFW_KEY_RIGHT);
	int stateforwardRotateBall = glfwGetKey(window, GLFW_KEY_UP);
	int statebackRotateBall = glfwGetKey(window, GLFW_KEY_DOWN);
	int stateAimLeft = glfwGetKey(window, GLFW_KEY_Q);
	int stateAimRight = glfwGetKey(window, GLFW_KEY_E);
	int stateAimDown = glfwGetKey(window, GLFW_KEY_T);
	int stateAimUp = glfwGetKey(window, GLFW_KEY_Y);


	//Ball Aim
	//-----------------------------------------------------------------

	//Ball movement
	//-----------------------------------------------------------------
	if (movementID == 1) {
		if (stateAimRight == GLFW_PRESS && rotateForAim >= -4.5f && ballAir == false && gameOver==false) {
			veloX += 0.05f;
			rotateForAim -= 0.05f;
			rotateForBall +=- 0.1f;
			model = glm::rotate(model, glm::radians(rotateForBall), glm::vec3(0.0, 1.0, 0.0));
		}
		else if (stateAimLeft == GLFW_PRESS && rotateForAim <= 4.5f && ballAir == false && gameOver == false) {
			veloX -= 0.05f;
			rotateForAim += 0.05f;
			rotateForBall += 0.1f;
			model = glm::rotate(model, glm::radians(rotateForBall), glm::vec3(0.0, 1.0, 0.0));
		}
		else if (stateAimUp == GLFW_PRESS && rotateForAim2 <= 4.5f && ballAir == false && gameOver == false) {
			veloY += 0.05f;
			rotateForAim2 += 0.07f;
			rotateForBall += 0.1f;
			model = glm::rotate(model, glm::radians(rotateForBall), glm::vec3(1.0, 0.0, 0.0));
		}
		else if (stateAimDown == GLFW_PRESS && rotateForAim2 >= -4.5f && ballAir == false && gameOver == false) {
			veloY -= 0.05f;
			rotateForAim2 -= 0.07f;
			rotateForBall +=- 0.1f;
			model = glm::rotate(model, glm::radians(rotateForBall), glm::vec3(1.0, 0.0, 0.0));
		}
		if (stateleftRotateBall == GLFW_PRESS && ballAir == false && gameOver == false) {
			translateX += -0.05f;
			translateX2 += -0.05f;
			if (translateZ > -33.19978f) {
				if (translateX < -10.35f) {
					translateX = -10.35f;
					translateX2 = -1.0f;
				}
				if (translateX > -10.35f) {
					rotateForBall +=0.8f;
				}
			}
			else {
				if (translateX < -4.35f) {
					translateX = -4.35f;
					translateX2 = -1.0f;
				}
				if (translateX > -4.35f) {
					rotateForBall += 0.8f;
				}

			}
			model = glm::rotate(model, glm::radians(rotateForBall), glm::vec3(0.0, 0.0, 1.0));
		}
		else if (staterightRotateBall == GLFW_PRESS && ballAir == false && gameOver == false) {
			translateX += 0.05f;
			translateX2 += 0.05f;
			if (translateZ > -33.19978f) {
				if (translateX > 10.35f) {
					translateX = 10.35f;
					translateX2 = 1.0f;
				}
				if (translateX < 10.35f) {

					rotateForBall += -0.8f;
				}
			}
			else {
				if (translateX > 4.35f) {
					translateX = 4.35f;
					translateX2 = 1.0f;
				}
				if (translateX < 4.35f) {
					rotateForBall += -0.8f;
				}

			}
			model = glm::rotate(model, glm::radians(rotateForBall), glm::vec3(0.0, 0.0, 1.0));
			//model = glm::translate(model, glm::vec3(translateX, 0.0f, translateZ));
		}
		else if (stateforwardRotateBall == GLFW_PRESS && ballAir == false && gameOver == false) {
			translateZ += -0.05f;
			translateZ2 += -0.05f;
			if (translateZ < -2.5f && ballFree == false) {
				translateZ = -2.5f;
				translateZ2 = -1.0f;
			}
			if (translateZ > -2.5f) {
				rotateForBall += -0.8f;
			}
			if (translateZ <= -33.0f && translateZ >= -33.1997f && (translateX<-4.35 || translateX>4.35)) {
				translateZ += 0.05f;
				rotateForBall += -0.8f;
			//	cout << "123";
			}
			if (ballFree == true && translateZ<-2.5f && translateZ >-78.329) {
				rotateForBall += -0.8f;
			}
			if (ballFree == true && translateZ < -78.32f) {
				translateZ = -78.32f;
				/*if (translateZ > -80.0f) {
					rotateForBall += -0.1f;
				}*/
			}
			model = glm::rotate(model, glm::radians(rotateForBall), glm::vec3(1.0, 0.0, 0.0));
			//model = glm::translate(model, glm::vec3(translateX, 0.0f, translateZ));
		}
		else if (statebackRotateBall == GLFW_PRESS && ballAir == false && gameOver == false) {
			translateZ += 0.05f;
			translateZ2 += 0.05f;
			if (translateZ > 3.3f) {
				translateZ = 3.3f;
				translateZ2 = 1.0f;
			}
			if (translateZ < 3.3f &&ballFree == false) {
				rotateForBall += 0.8f;
			}
			if (ballFree == true) {
				rotateForBall += 0.8f;
			}
			model = glm::rotate(model, glm::radians(rotateForBall), glm::vec3(1.0, 0.0, 0.0));
			//model = glm::translate(model, glm::vec3(translateX, 0.0f, translateZ));
		}
		


		positionOfBallZ = translateZ;
		positionOfBallX = translateX;

		if (gameOver == false) {
			//for throw
			if (state3 == GLFW_PRESS && ballAir == false && ballFree == false) {
				currentvelocity.x += veloX;
				currentvelocity.y += veloY;
				currentvelocity.z += -3.0f;
				count2 += 1.5f;
				state2 = true;
				g = -9.8f;
				shader.setFloat("g", -9.8f);
				//cout << "a";
			}
			if (state2 == true) {//delaytime
			//	cout << "b";
				t = 0.0f;
				t2 = 0.0f;
				shader.setFloat("t", 0.0f);
				shader.setFloat("t2", 0.0f);
				
				//else if(duvar a carparsa x ve z nin ortak noktasini bulup zamani bul. )
				count2 -= 1.0f;
				
				//else {
				count3 = ((positionOfBallZ - positionOfWallZ - 2.5f) * (100 / 8)) / currentvelocity.z;
				//}
			}
			if (count2 < 0.0f) {//wait for space
			
				if ((currentvelocity.y*(-count3) + -9.8f*count3*count3 / 2) * 8 / 100 +2.1f> 16.5f) {
					stateNoBack = true;
				}
				count2 = 0.0f;
				state1 = true;
				state2 = false;
				ballAir = true;
				stateAim = false;
			}
			if (state1 == true) {//ball going
				spaceDelay -= 0.01f;
				
				if ((currentvelocity.x*(-countGoX) * 8 / 100 + translateX) >= 10.5f || currentvelocity.x*(-countGoX) * 8 / 100 + positionOfBallX <= -10.50f) {
					stateNoBack = true;
					countGoX += -0.01f;
					
				}
				else {
					
					if ((currentvelocity.y*(-spaceDelay) + -9.8f*spaceDelay*spaceDelay / 2)*8/100 < 0.0f) {
						
						g = 0.0f;
						//shader.setFloat("g", 0.0f);
						currentvelocity.y = 0.0f;
						if ((currentvelocity.x*(-countGoX) * 8 / 100 + translateX) >= 10.35f || currentvelocity.x*(-countGoX) * 8 / 100 + positionOfBallX <= -10.35f) {//|| currentvelocity.x*(-count3) + positionOfBallX <= -50.50f
						//	currentTime2 += 0.01f;
							countGoX += 0.01f;
							//cout << "cembektas";
						}
						else {
							//cout << "bb";
							countGoX += -0.01f;
						}
						
						
					}
					else {
						//cout<<"aa";
						countGoX +=-0.01f;
					}
				}
				
				t = -spaceDelay;
				t2 = -countGoX;
				shader.setFloat("t", -spaceDelay);
				shader.setFloat("t2", -countGoX);
			}
			if (spaceDelay <= count3 && stateNoBack == false) {//when ball collision
				state4 = true;
				countGoBack = spaceDelay;
				countGoBackX = countGoX;
				spaceDelay = 0.0f;
				state1 = false;
				state2 = false;
				stateBack = true;
				
				if ((currentvelocity.x*(-countGoBack) * 8 / 100 + translateX) <= (4 * a - 8) + 2 && (currentvelocity.x*(-countGoBack) * 8 / 100 + translateX) >= (4 * a - 8) - 2) {
					//cout << "x";
					if (currentvelocity.y == 0) {
						if (4 * (3 - b) >= 0 && 4 * (3 - b) < 2) {
							//cout << "y";
							if (currentvelocity.z*(-countGoBack) * 8 / 100 + positionOfBallZ < -21.0f) {
								//cout << "z";
								
								engine->play2D("../Resources/audio/smashed.wav", GL_FALSE);
								collisionWait = true;
								collisionn = 1;
								targetShooted = true;
								score += points.at(pointindex);

							}
						}
					}
					else {
						if ((currentvelocity.y*(-countGoBack) + -9.8f*countGoBack*countGoBack / 2) * 8 / 100 >= 4 * (3 - b) - 2.15f && (currentvelocity.y*(-countGoBack) + -9.8f*countGoBack*countGoBack / 2) * 8 / 100 <= 4 * (3 - b) + 2.15f) {
						//	cout << "y";
							if (currentvelocity.z*(-countGoBack) * 8 / 100 + positionOfBallZ < -21.0f) {
							//	cout << "z";
								
								engine->play2D("../Resources/audio/smashed.wav", GL_FALSE);
								collisionWait = true;
								collisionn = 1;
								targetShooted = true;
								score += points.at(pointindex);
							}
						}
					}
				}

				//cout << "aa";
				if (targetShooted == false) {
					lives--;
					targetShooted = true;
				}
				//currentvelocity.z = -currentvelocity.z;
			}
			else if (spaceDelay <= -15.0f && stateNoBack == true) {
				//	cout << "asdasd";
				if (targetShooted == false) {
					lives--;
					//cout << "cc";
					targetShooted = true;
				}
				countGoBack = 1.0f;
			}
			if (state4 == true) {//ball is going back
				//cout << "e";

				countGoBack += 0.01f;
				if (stateBack == true) {
					currentvelocity.z = -currentvelocity.z;
					currentvelocity.y = -currentvelocity.y;
					//currentvelocity.x = -currentvelocity.x;
					stateBack = false;
				}
				t = countGoBack;
				shader.setFloat("t", countGoBack);
				currentTime = countGoBackX;
			

				if (((currentvelocity.x*(-countGoBackX) * 8 / 100 + positionOfBallX) >= 10.35f || (currentvelocity.x*(-countGoBackX) * 8 / 100 + positionOfBallX) <= -10.35f) && currentvelocity.y == 0.0f) {//
					currentTime += 0.01f;
					
				}
				else {
					countGoBackX += -0.01f;
				}
				t2 = -currentTime;
				shader.setFloat("t2", -currentTime);

			}
			shader.setVec3("velocity", currentvelocity);
			if (countGoBack > 0.0f) {//when ball went back and touch floor
				//cout << "f";
				count2 = 0.0f;
				count3 = -0.1f;
				countGoBack = 0.0f;
				countGoBackX = 0.0f;
				countGoX = 0.0f;
				spaceDelay = 0.0f;
				state1 = false;
				state2 = false;
				stateAim = true;
				state4 = false;
				ballAir = false;
				stateNoBack = false;
				targetShooted = false;
				//time++;
				t = 0.0f;
				t2 = 0.0f;
				shader.setFloat("t", 0.0f);
				shader.setFloat("t2", 0.0f);
				currentvelocity = glm::vec3(0.0f);
				shader.setVec3("velocity", currentvelocity);
				fallTime = 10.0f;
				if (lives == 0) {
					gameOver = true;
					began = true;
					goback = 25;
					scaled = false;
					scaleamonut = 0;
					engine->play2D("../Resources/audio/opening-walls.wav", GL_FALSE);
				}
				if (collisionWait) {
					a = -1;
					b = -1;
					collisionn = -1;
					collisionWait = false;
				}
			}
		}
		else
		{
			while (waitTime > 0) {
				waitTime--;

			}
			//cout << "Game Over";
			if (fallTime >= 0.0f) {
				asd++;
				refract = 1;
				shader.setFloat("g", -9.8f);
				g = -9.8f;
				t = fallTime;
				currentvelocity = glm::vec3(0.0f);
				shader.setVec3("velocity", currentvelocity);
				fallTime -= 0.01f;
				shader.setFloat("t", fallTime);
				if (asd == 1) {
					engine->play2D("../Resources/audio/water-drop.wav", GL_FALSE);
				}
			}
			else {
				
				waitTime = 20;
				g = 0.0f;
				t = 0.0f;
				shader.setFloat("g", 0.0f);
				shader.setFloat("t", 0.0f);
				score = 0;
				lives = 5;
				gameOver = false;
				refract = -1;
				asd = 0;

			}
		}
		model = glm::translate(model, glm::vec3(currentvelocity.x*t2 * 8 / 100, (currentvelocity.y*(t)+g * t*t / 2) * 8 / 100, (currentvelocity.z*t) * 8 / 100));
		if (score >= 15) {
			ballFree = true;

		}
	}
	
	model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", model);
	ball.Draw(shader);

	if (ballFree) {
		camera.Position.x = translateX;
		camera.Position.y = 5;
		camera.Position.z = translateZ+3;
		cout << translateZ;
		
	}
	if (camera.Position.z > -70.3 && camera.Position.z < -70) {
		engine->play2D("../Resources/audio/Frolic.mp3", GL_TRUE);
		shadows = false;
		shader.setBool("shadows", shadows);
	}
	

	//Draw Dice
	//---------------------------------------------------------------
	forBall = -1;
	shader.setInt("forBall", forBall);
	shader.setInt("reflectKey", -1);
	

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	model = glm::mat4(1.0f);
	model = glm::translate(model, cubecoor);


	if (aboutit.x == 1)
	{

		model = glm::rotate(model, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));


		if (!stop)
		{
			angles.x = angles.x + 1;


			int  value = (int)angles.x;
			if (value % 90 == 0)
			{


				stop = true;
				start = std::clock();
				loop++;
				pointindex++;

				if (pointindex == points.size())
				{
					pointindex = 0;
				}
				if (loop == 4)
				{

					loop = 0;
					aboutit.x = 0;
					aboutit.y = 1;
					aboutit.z = 0;
				}


			}
		}
		else
		{
			if (duration > 0)
			{
				stop = false;
			}

		}
	}
	else if (aboutit.y == 1)
	{
		model = glm::rotate(model, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));




		if (!stop)
		{
			angles.y = angles.y + 1.0f;



			int  value = (int)angles.y;
			if (value % 90 == 0)
			{


				stop = true;
				start = std::clock();
				loop++;
				pointindex++;
				if (pointindex == points.size())
				{
					pointindex = 0;
				}
				if (loop == 4)
				{

					loop = 0;
					aboutit.x = 1;
					aboutit.y = 0;
					aboutit.z = 0;
				}
			}
		}
		else
		{
			if (duration > 0)
			{
				stop = false;
			}
		}

	}
	
	model = glm::scale(model, glm::vec3(1.0f));
	shader.setMat4("model", model);
	//glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.setInt("reverse_normals", 0); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderDice();

	//Draw Flag 

	if (dalga) {
		added += 0.01f;
		if (added > 0.5){
			dalga = false;
		}
	}
	else{
		added -= 0.01f;
		if (added < -0.5){
			dalga = true;
		}
	}


	forBall = -1;
	shader.setInt("forBall", forBall);
	shader.setInt("reflectKey", -1);
	//Ball Draw
	model = glm::mat4(1.0f);
	model = glm::translate(model, flagcoor + glm::vec3(12.0, 0.0, 1.5f));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0, 4.0, 0.0));
	model = glm::scale(model, glm::vec3(0.06, 4, 0.06));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, &model[0][0]);
	////draw flag
	glBindVertexArray(cyc.vbo); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, venomTexture.at(0));

	glDrawElements(GL_TRIANGLES, cyc.ebo.size(), GL_UNSIGNED_INT, 0);


	////draw flag
	model = glm::mat4(1.0f);
	model = glm::translate(model, flagcoor + glm::vec3(-6.0, -3.0, 1.5));
	model = glm::rotate(model, glm::radians(angle + added), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(10.0, 10.0, 0.1));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, &model[0][0]);


	glBindVertexArray(flag.vbo); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, flag.ebo.size(), GL_UNSIGNED_INT, 0);

	



}



//render Text in Scene
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}





// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int lightVAO = 0;
unsigned int lightVBO = 0;
void renderLight() {
	// initialize (if necessary)
	if (lightVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &lightVAO);
		glGenBuffers(1, &lightVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(lightVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int diceVAO = 0;
unsigned int diceVBO = 0;
void renderDice()
{
	// initialize (if necessary)
	if (diceVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &diceVAO);
		glGenBuffers(1, &diceVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, diceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(diceVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(diceVAO);
	for (int i = 0; i < diceFaces.size(); i++){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diceFaces.at(i));
		glDrawArrays(GL_TRIANGLES, 0, 6 * (i + 1));
	}
	glBindVertexArray(0);
}

//Read Object File
struct Point Readobjfile(string filename, struct Point point) {

	vector<glm::vec4> modelvbo;
	vector<GLuint>;


	std::ifstream file(filename);
	std::string temp;
	while (std::getline(file, temp)) {
		stringstream ss(temp);
		vector<string> result;
		while (ss.good()) {
			string substr;
			getline(ss, substr, ' ');
			result.push_back(substr);

		}
		if (result.at(0) == "v") {
			glm::vec4 myvec;
			myvec.x = (float)std::stof(result.at(1));
			myvec.y = (float)std::stof(result.at(2));
			myvec.z = (float)std::stof(result.at(3));
			myvec.w = 1.0f;
			modelvbo.push_back(myvec);

		}
		else if (result.at(0) == "f")
		{
			point.ebo.push_back((GLuint)std::stoi(result.at(1)) - 1);
			point.ebo.push_back((GLuint)std::stoi(result.at(2)) - 1);
			point.ebo.push_back((GLuint)std::stoi(result.at(3)) - 1);
		}
		result.clear();

	}

	unsigned int VBO, EBO;
	glGenVertexArrays(1, &point.vbo);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(point.vbo);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, modelvbo.size() * sizeof(glm::vec4), &modelvbo[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, point.ebo.size() * sizeof(GLuint), &point.ebo[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	return  point;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !shadowsKeyPressed)
	{
		shadows = !shadows;
		shadowsKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
	{
		shadowsKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !reflectEnable)
	{
		reflectKey = -reflectKey;
		reflectEnable = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
	{
		reflectEnable = false;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		a = -1;
		b = -1;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		finishGame = -finishGame;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		lightDensity.x += 0.05;
		lightDensity.y += 0.05;
		lightDensity.z += 0.05;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		lightDensity.x -= 0.05;
		lightDensity.y -= 0.05;
		lightDensity.z -= 0.05;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		lightPos.y += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		lightPos.y -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		lightPos.x -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		lightPos.x += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		lightPos.z -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		lightPos.z += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && ballAir == false) {


		movementID = 2;


	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && ballAir == false) {


		movementID = 3;


	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && ballAir == false) {

		movementID = 1;


	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {


		switch (movementID)
		{

		case  1:

			break;
		case  2:
			cubecoor.z -= 0.1f;
			break;
		case  3:
			flagcoor.z -= 0.1f;
			break;
		default:
			break;
		}


	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {


		switch (movementID)
		{

		case  1:

			break;
		case  2:
			cubecoor.z += 0.1f;
			break;
		case  3:
			flagcoor.z += 0.1f;
			break;
		default:
			break;
		}


	}if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {


		switch (movementID)
		{

		case  1:

			break;
		case  2:
			cubecoor.x += 0.1f;
			break;
		case  3:
			flagcoor.x += 0.1f;
			break;
		default:
			break;
		}


	}if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {


		switch (movementID)
		{

		case  1:

			break;
		case  2:
			cubecoor.x -= 0.1f;
			break;
		case  3:
			flagcoor.x -= 0.1f;
			break;
		default:
			break;
		}


	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	stbi_set_flip_vertically_on_load(false);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}