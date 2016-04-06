#include "camera.h"

Camera* Camera::instance = nullptr;

Camera::Camera()
{
    //initialize view/rotation
}

Camera::~Camera()
{

}

Camera* Camera::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Camera();
    }
    return instance;
}

void Camera::ReleaseInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

glm::mat4 Camera::GetProjection(bool bOrthographic)
{

}

glm::mat4 Camera::GetView()
{

}

void Camera::SetPosition(glm::vec3 position)
{
    m4Pos = glm::translate(position);
    m4Camera = m4Pos * rotation.toMat4();
}

void Camera::SetTarget(glm::vec3 target)
{

}

void Camera::SetUp(glm::vec3 up)
{

}

void Camera::MoveForward(float fIncrement)
{

}

void Camera::MoveSideways(float fIncrement)
{

}