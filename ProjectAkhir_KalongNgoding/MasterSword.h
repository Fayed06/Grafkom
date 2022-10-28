#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

class MasterSword {
public:
	MasterSword();
	float anglep = 0;
	void BuildCube(GLuint shaderProgram3);
	void DrawAll();
	void DrawColoredCube(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize, float rColor, float gColor, float bColor);

private:
	GLuint shaderProgram1, VBO3, VAO3, EBO3, texture3;
	
};