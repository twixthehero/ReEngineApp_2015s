#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Sandbox"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	m_selection = std::pair<int, int>(-1, -1);
	//Set the camera at a position other than the default
    m_pCameraMngr->SetPosition(vector3(0, 0, 35));

	m_pLightMngr->SetColor(REWHITE, 0);
	m_pLightMngr->SetIntensity(0.1f, 0);
	m_pLightMngr->SetColor(REWHITE, 1);
	m_pLightMngr->SetIntensity(0.5f, 1);
	m_pLightMngr->SetPosition(vector3(0.0f, 1.0f,-1.0f), 1);

    srand(time(NULL));
    m_nObjects = rand() % 23 + 5;

    vector3 start(-m_nObjects, 0, 0);
    vector3 end(m_nObjects, 0, 0);

    m_pSphere = new PrimitiveClass[m_nObjects];
    m_pMatrix = new matrix4[m_nObjects];

    for (int i = 0; i < m_nObjects; i++)
    {
        float fPercent = MapValue(static_cast<float>(i), 0.0f, static_cast<float>(m_nObjects), 0.0f, 1.0f);

        m_pSphere[i].GenerateSphere(1.0f, 5, vector3(fPercent, 0.0f, 0.0f));
        m_pMatrix[i] = glm::translate(IDENTITY_M4, glm::lerp(start, end, fPercent));
    }
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

	//Call the arcball method
	ArcBall();
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3Position) * ToMatrix4(m_qArcBall), 0);
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
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
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}

    matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
    matrix4 m4View = m_pCameraMngr->GetViewMatrix();

    for (int i = 0; i < m_nObjects; i++)
        m_pSphere[i].Render(m4Projection, m4View, m_pMatrix[i]);
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields

    delete[] m_pSphere;
    delete[] m_pMatrix;
}