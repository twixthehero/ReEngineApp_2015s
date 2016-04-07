#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("Instance Rendering");
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pMesh = new MyMesh();
    m_fMatrixArray = new float[m_nObjects * 16];

    for (int i = 0; i < m_nObjects; i++)
    {
        int row = CalcRow(i);
        vector3 trans = vector3(0, row, 0);

        matrix4 transMat = glm::translate(trans);
        float* array = glm::value_ptr(transMat);
        
        for (int k = 0; k < 16; k++)
        {
            *(m_fMatrixArray + (i * 16) + k) = array[k];
        }

        std::cout << m_fMatrixArray[i] << std::endl;


        //CreateTriangle(trans);
    }

	//Compiling the mesh
	m_pMesh->CompileOpenGL3X();
}

int AppClass::CalcRow(int elements)
{
    int row = 1;

    while (elements - row > 0)
    {
        elements -= row;
        row++;
    }

    return row;
}

void AppClass::CreateTriangle(vector3 pos)
{
    m_pMesh->AddVertexPosition(pos + vector3(-1.0f, -1.0f, 0.0f));
    m_pMesh->AddVertexColor(REGREEN);
    m_pMesh->AddVertexPosition(pos + vector3(1.0f, -1.0f, 0.0f));
    m_pMesh->AddVertexColor(RERED);
    m_pMesh->AddVertexPosition(pos + vector3(0.0f, 1.0f, 0.0f));
    m_pMesh->AddVertexColor(REBLUE);
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//m_pMesh->Render(m4Projection, m4View, IDENTITY_M4);//Rendering nObjects
    m_pMesh->RenderList(m4Projection, m4View, m_fMatrixArray, m_nObjects);

	m_pMeshMngr->Render();

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}