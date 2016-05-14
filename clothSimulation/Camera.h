#pragma once
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
class Camera
{
public:
	Camera();
	~Camera();

	void getPosition(glm::vec3& _Position);
	float* getPositionF();
	glm::vec3* getDirection();
	glm::vec3* getUpDirection();

	float* getTransformF();
	glm::mat4* getTransformM();
	float* getPerspective();

	void translate(glm::vec3* _Translation);

	void setPosition(glm::vec3* _Position);
	void Camera::setPosition(float _x, float _y, float _z);

	void setTransform(glm::mat4* _Transform);
	void setPerspective(glm::mat4* _Perspective);

	void update();

	float pitch;
	float yaw;

private:
	
	glm::vec3 position;

	glm::vec3 direction;
	glm::vec3 upDirection;
	glm::vec3 rightDirection;

	glm::mat4 transform;
	glm::mat4 perspective;
};

