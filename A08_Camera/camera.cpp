#include "camera.h"

Camera* Camera::instance = nullptr;

Camera::Camera()
{
    //initialize view/rotation
    Initialize();
}

void Camera::Initialize()
{
    fov = 65.0f;
    aspectRatio = 4.0f / 3.0f;
    nearDist = 0.1f;
    farDist = 1000.0f;
    position = vector3(0.0f, 0.0f, -10.0f);
    up = vector3(0.0f, 1.0f, 0.0f);
    target = vector3(0.0f, 0.0f, 0.0f);
    rotation = glm::quat();
}

Camera::~Camera(){}

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
    if (bOrthographic)
        return glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
    else
        return glm::perspective(fov, aspectRatio, nearDist, farDist);
}

glm::mat4 Camera::GetView()
{
    return glm::lookAt(vector3(position.x, position.y, position.z),
        vector3(target.x, target.y, target.z),
        vector3(up.x, up.y, up.z));
}

void Camera::SetPosition(glm::vec3 pos)
{
    position = pos;
}

void Camera::SetTarget(glm::vec3 tar)
{
    vector3 forward = glm::normalize(tar - position);
    target = position + forward;
}

void Camera::SetUp(glm::vec3 u)
{
    up = glm::normalize(u);
}

void Camera::MoveForward(float fIncrement)
{
    vector3 forward = glm::normalize(target - position);
    position += fIncrement * forward;
    target += fIncrement * forward;
}
    
void Camera::MoveSideways(float fIncrement)
{
    vector3 forward = glm::normalize(target - position);
    glm::quat toRight = glm::angleAxis(-90.0f, vector3(up.x, up.y, up.z));
    vector3 right = forward * toRight;
    position += fIncrement * glm::normalize(right);
    target += fIncrement * glm::normalize(right);
}

void Camera::MoveVertical(float fIncrement)
{
    position += fIncrement * up;
    target += fIncrement * up;
}

void Camera::ChangePitch(float fIncrement) //x
{
    fIncrement *= 10;

    vector3 forward = vector3(target.x, target.y, target.z) - vector3(position.x, position.y, position.z);
    vector3 right = glm::cross(forward, vector3(up.x, up.y, up.z));

    rotation = glm::normalize(rotation * glm::angleAxis(fIncrement, right));

    up = vector3(0.0f, 1.0f, 0.0f) * rotation;
    target = vector3(0.0f, 0.0f, 1.0f) * rotation + position;
}

void Camera::ChangeRoll(float fIncrement) //z
{
    fIncrement *= 10;
    
    vector3 forward = vector3(target.x, target.y, target.z) - vector3(position.x, position.y, position.z);
    rotation = glm::normalize(rotation * glm::angleAxis(fIncrement, forward));

    up = vector3(0.0f, 1.0f, 0.0f) * rotation;
}

void Camera::ChangeYaw(float fIncrement) //y
{
    fIncrement *= 10;

    rotation = glm::normalize(rotation * glm::angleAxis(fIncrement, up));

    target = vector3(0.0f, 0.0f, 1.0f) * rotation + position;
}
