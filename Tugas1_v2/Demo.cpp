#include "Demo.h"



Demo::Demo() {
	
}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();

	bezierInit();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}



}

void Demo::Update(double deltaTime) {
	angle1 += (float) ((deltaTime * 1.5f) / 70);
	angle2 += (float) ((deltaTime * 1.5f) / 70);
	//ss1 += (float)((deltaTime * 1.5f) / 1000);
	//std::cout << ss1;

	////buat maju mundur
	//if (ss1 >= 10 || ss1 < -10) {
	//	speed *= -1;
	//}

	////update posisi x
	//ss1 += (float)((deltaTime * speed) / 100);

	//waktu(t) sama dengan speed
	//ss1 sama dengan posisi si x

	//coba posisi-in helinya

	if (p != 749) {
		tempX = posisiX[p] - ss1;
		tempZ = posisiZ[p] - ss3;

		ss1 += tempX;
		ss3 += tempZ;

		//std::cout << sudutCari(posisiX[p], posisiZ[p], posisiX[p + 1], posisiZ[p + 1]) << " ";
		//std::cout << posisiX[1];
		//std::cout << posisiZ[p];


		pN = p + 1;

		p++;
	}
	else if (p == 749) {
		p = 0;
		pN = 0;
		angle = 0;
	}

	if (p == 2) {
		//std::cout << tempPrev - sudutCari(posisiX[p], posisiZ[p], posisiX[p + 1], posisiZ[p + 1]);
		//std::cout << posisiZ[p];
	}

	/*if (tempPrev - sudutCari(posisiX[p], posisiZ[p], posisiX[p + 1], posisiZ[p + 1]) < 0 || sudutCari(posisiX[p], posisiZ[p], posisiX[p + 1], posisiZ[p + 1]) - tempPrev < 0) {
		angle += sudutCari(posisiX[p], posisiZ[p], posisiX[p + 1], posisiZ[p + 1]);
	}
	else {
		angle -= sudutCari(posisiX[p], posisiZ[p], posisiX[p + 1], posisiZ[p + 1]);
	}

	tempPrev = sudutCari(posisiX[p], posisiZ[p], posisiX[p + 1], posisiZ[p + 1]);*/

	
	//std::cout << sudutCari(posisiX[p], posisiZ[p], posisiX[p + 1], posisiZ[p + 1]) << " ";

	//std::cout << tempPrev << " ";

	pN = p + 1;


	if (p > 20) {
		angle -= sudutCari(posisiX[p], posisiZ[p], posisiX[pN], posisiZ[pN]);
	}
	else {
		angle += sudutCari(posisiX[p], posisiZ[p], posisiX[pN], posisiZ[pN]);
	}

	std::cout << angle << " ";
	
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(25 + ss1, 20, 1 + ss3), glm::vec3(-45, -35, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//balingbaling
	DrawColoredCube(1, 0,6,0, 10, 0.8, 0.8);
	//tangkai
	DrawColoredCube(0, 0,5,0, 0.8,0.8,0.8);
	//badan
	DrawColoredCube(0, 0,3,0, 5,3,3);
	//ekor
	DrawColoredCube(0, 4,3,0, 7,1.25,1.25);
	//ekormuter
	DrawColoredCube(2, 6.9,3,1.5, 0.7,2,0.7);
	
	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 0.5, 0, 0,  // 0
		0.5, -0.5, 0.5, 1, 0,   // 1
		0.5,  0.5, 0.5, 1, 1,   // 2
		-0.5,  0.5, 0.5, 0, 1,  // 3

		// right
		0.5,  0.5,  0.5, 0, 0,  // 4
		0.5,  0.5, -0.5, 1, 0,  // 5
		0.5, -0.5, -0.5, 1, 1,  // 6
		0.5, -0.5,  0.5, 0, 1,  // 7

		// back
		-0.5, -0.5, -0.5, 0, 0, // 8 
		0.5,  -0.5, -0.5, 1, 0, // 9
		0.5,   0.5, -0.5, 1, 1, // 10
		-0.5,  0.5, -0.5, 0, 1, // 11

		// left
		-0.5, -0.5, -0.5, 0, 0, // 12
		-0.5, -0.5,  0.5, 1, 0, // 13
		-0.5,  0.5,  0.5, 1, 1, // 14
		-0.5,  0.5, -0.5, 0, 1, // 15

		// upper
		0.5, 0.5,  0.5, 0, 0,   // 16
		-0.5, 0.5,  0.5, 1, 0,  // 17
		-0.5, 0.5, -0.5, 1, 1,  // 18
		0.5, 0.5, -0.5, 0, 1,   // 19

		// bottom
		-0.5, -0.5, -0.5, 0, 0, // 20
		0.5, -0.5, -0.5, 1, 0,  // 21
		0.5, -0.5,  0.5, 1, 1,  // 22
		-0.5, -0.5,  0.5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube(int type, float s1, float s2, float s3, float sc1, float sc2, float sc3)
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	//karena s1 itu posisi x helikopter yang statis, jadi harus ditambah terus sama ss1 yang selalu berubah di deltatime
	s1 += ss1;
	s3 += ss3;

	//ss1 = s1;
	//ss3 = s3;

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(s1, s2, s3));

	if (type == 0) {
		model = glm::rotate(model, angle, glm::vec3(0, -1, 0));
	}
	else if (type == 1) {
		model = glm::rotate(model, angle1, glm::vec3(0, -1, 0));
	}
	else {
		model = glm::rotate(model, angle2, glm::vec3(0, 0, 1));
	}

	model = glm::scale(model, glm::vec3(sc1, sc2, sc3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-100.0, -0.5, -100.0,  0,  0,
		 100.0, -0.5, -100.0, 100,  0,
		 100.0, -0.5,  100.0, 100, 100,
		-100.0, -0.5,  100.0,  0, 100,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	

	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();

	app.Start("Transformation: Transform Cube", 800, 600, true, false);
}

void Demo::bezierInit() {

	int segmentCount = 750;

	//float posisiX[100];
	//float posisiZ[100];

	//rumus bezier curve jika empat waypoint

	for (int i = 0; i < segmentCount; i++) {
		t = (float)i / segmentCount;
		x = ((1 - t) * (1 - t) * (1 - t) * p0x) + (3 * (1 - t) * (1 - t) * t * p1x) + (3 * (1 - t) * (1 - t) * t * t * p2x) + (t * t * t * p3x);
		z = ((1 - t) * (1 - t) * (1 - t) * p0z) + (3 * (1 - t) * (1 - t) * t * p1z) + (3 * (1 - t) * (1 - t) * t * t * p2z) + (t * t * t * p3z);

		posisiX[i] = x;
		posisiZ[i] = z;

		//std::cout << posisiX[i];
		//std::cout << " ";
		//std::cout << posisiZ[i];
	}
}

float Demo::sudutCari(float x0, float z0, float x1, float z1) {
	//float sudut = ((x0 * x1) + (z0 * z1)) / (sqrt(x0 * x0 + z0 * z0) * sqrt(x1 * x1 + z1 * z1));
	//float sudut = atan2(z1 - z0, x1 - x0);

	glm::vec3 p0(x0, 0, z0);
	glm::vec3 p1(x1, 0, z1);
	glm::vec3 orign(40, 0, 0);

	//normalize
	glm::vec3 da = glm::normalize(p0);
	glm::vec3 db = glm::normalize(p1);
	//besar sudut dalam radian
	float sudut = glm::acos(glm::dot(da, db));

	return sudut;
}