#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - Maximilian Wright"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
    //m_fDay = 0.05f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

    quaternion qStart = quaternion(vector3(0));
    quaternion qHalf = quaternion(vector3(0, PI, 0));
    quaternion qWhole = quaternion(vector3(0, (float)(PI * 2), 0));
    matrix4 m4Sun = glm::scale(IDENTITY_M4, vector3(5.936f));
    matrix4 m4Earth = IDENTITY_M4;
    matrix4 m4Moon = IDENTITY_M4;

    float* earthWhole = new float;
    float earthRot = modff((float)(fRunTime / fEarthHalfOrbTime), earthWhole);
    bool earthHalf = (int)(*earthWhole) % 2 == 0;
    float* moonWhole = new float;
    float moonRot = modff((float)(fRunTime / fMoonHalfOrbTime), moonWhole);
    bool moonHalf = (int)(*moonWhole) % 2 == 0;

    quaternion qEarth = glm::mix(earthHalf ? qHalf : qStart, earthHalf ? qWhole : qHalf, earthRot);
    m4Earth = m4Earth * glm::mat4_cast(qEarth);
    m4Earth = glm::translate(m4Earth, vector3(0, 0, 11));
    
    quaternion qMoon = glm::mix(moonHalf ? qHalf : qStart, moonHalf ? qWhole : qHalf, moonRot);
    m4Moon = glm::mat4_cast(qMoon);
    m4Moon = glm::translate(m4Moon, vector3(0, 0, 2));
    m4Moon = m4Earth * m4Moon;

    bool earthRevHalf = (int)(fRunTime / fEarthHalfRevTime) % 2 == 0;
    float earthRev = (fRunTime - (int)(fRunTime / fEarthHalfRevTime) * (float)fEarthHalfRevTime) / (float)fEarthHalfRevTime;
    m4Earth = m4Earth * glm::mat4_cast(glm::mix(earthRevHalf ? qHalf : qStart, earthRevHalf ? qWhole : qHalf, earthRev));
    bool moonRevHalf = (int)(fRunTime / fMoonHalfOrbTime) % 2 == 0;
    float moonRev = (fRunTime - (int)(fRunTime / fMoonHalfOrbTime) * (float)fMoonHalfOrbTime) / (float)fMoonHalfOrbTime;
    m4Moon = m4Moon * glm::mat4_cast(glm::mix(moonRevHalf ? qHalf : qStart, moonRevHalf ? qWhole : qHalf, -moonRev));

    m4Earth = glm::scale(m4Earth, vector3(0.524f));
    m4Moon = glm::scale(m4Moon, vector3(0.524f * 0.27f));

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}