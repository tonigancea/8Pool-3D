#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

//#include <Core/Engine.h>

using namespace std;


Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	renderCameraTarget = false;

	camera = new Laborator::CameraTema();
	camera->Set(glm::vec3(3.5f, 2, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderTema2");
		shader->AddShader(RESOURCE_PATH::SHADERS + "VSTema2.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "Default.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	
	{
		Mesh* mesh = new Mesh("table");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "8Pool/table", "table.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("redBall");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "8Pool/ballRed", "redBall.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("yellowBall");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "8Pool/ballYellow", "yellowBall.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("whiteBall");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "8Pool/ballWhite", "whiteBall.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("blackBall");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "8Pool/ballBlack", "blackBall.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("tac");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "8Pool/tac", "tac.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	
	ballList.push_back(Ball("whiteBall", WhiteBX, WhiteBZ));

	ballList.push_back(Ball("redBall", -0.812f, 0));
	
	ballList.push_back(Ball("redBall", -0.909f, -0.047f));
	ballList.push_back(Ball("yellowBall", -0.909f, 0.047f));
	
	ballList.push_back(Ball("yellowBall", -1.006, -0.094f));
	ballList.push_back(Ball("blackBall", -1.006, 0));
	ballList.push_back(Ball("redBall", -1.006, 0.094f));
	
	ballList.push_back(Ball("yellowBall", -1.103f, -0.141f));
	ballList.push_back(Ball("redBall", -1.103f, -0.047f));
	ballList.push_back(Ball("redBall", -1.103f, 0.047f));
	ballList.push_back(Ball("yellowBall", -1.103f, 0.141f));

	ballList.push_back(Ball("redBall", -1.200f, -0.188f));
	ballList.push_back(Ball("redBall", -1.200f, -0.094f));
	ballList.push_back(Ball("yellowBall", -1.200f, 0));
	ballList.push_back(Ball("yellowBall", -1.200f, 0.094));
	ballList.push_back(Ball("redBall", -1.200f, 0.188f));

	// down-right
	holes.push_back(1.52f);
	holes.push_back(0.75f);

	//down
	holes.push_back(0);
	holes.push_back(0.75f);

	//down left
	holes.push_back(-1.52f);
	holes.push_back(0.75f);

	//top-right
	holes.push_back(1.52f);
	holes.push_back(-0.75f);

	//top
	holes.push_back(0);
	holes.push_back(-0.75f);

	//top-left
	holes.push_back(-1.52f);
	holes.push_back(-0.75f);

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	//printf("Este randul Player-ului%d \n", player);

	for (int i = 0; i < 16; i++)
	{
		Ball ball = ballList.at(i);
		if (ball.type == "redBall")
		{
			reds.push_back(i);
		}
		if (ball.type == "yellowBall")
		{
			yellows.push_back(i);
		}
	}

	targetPlayer1.assign("none");
	targetPlayer2.assign("none");
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
	
	//collidingPairs.clear();
}

void Tema2::Update(float deltaTimeSeconds)
{	
	std::sort(activityPlayer1.begin(), activityPlayer2.end());
	std::sort(activityPlayer2.begin(), activityPlayer2.end());

	if (startTiming)
	{
		clock = Engine::GetElapsedTime() - clock_ref;
	}
	
	
	
	if (enterBallView && setCamera)
	{
		camera->Set(glm::vec3(ballList.at(0).x + 0.5, 1.2, ballList.at(0).z), glm::vec3(ballList.at(0).x, 1, ballList.at(0).z), glm::vec3(0, 1, 0));
		enterBallView = false;
		stayBallView = true;
		setCamera = false;
		angle = 0;
		
		ShowInfo();
	}

	if (round)
	{
		if (!resetBallOnce)
		{
			resetBallOnce = true;
		}
	}

	if (topView)
	{
		camera->Set(glm::vec3(0, 3.4f, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0682f, -0.0635f, 0.11f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.0518f));
		RenderMeshModified(meshes["table"], shaders["Simple"], modelMatrix);
	}

	if (stayBallView)
	{
		//offsetX =  offset * 25;
		
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(ballList.at(0).x, 1, ballList.at(0).z));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-camera->forward.x * offset, 0, -camera->forward.z * offset));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.0518f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(angle*57.3f), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(10.0f), glm::vec3(0, 0, 1));
		RenderMeshModifiedAnimation(meshes["tac"], shaders["ShaderTema2"], modelMatrix);
	}

	ballMoving = false;

	for (int i = 0; i < 16; i++)
	{
		if (ballList.at(i).alive)
		{
			if (CheckFalling(ballList.at(i).x, ballList.at(i).z))
			{
				if (i == 0)
				{
					ResetWhiteBall();
					printf("Player%d a comis un fault.\n", player);
					printf("Acesta isi pierde randul. \n\n");
					ChangePlayer();
					IncrementFault();
				}
				else
				{
					//register targets
					if (targetPlayer1 == "none" || targetPlayer2 == "none")
					{
						if (ballList.at(i).type == "redBall")
						{
							if (player == 0)
							{
								targetPlayer1.assign("redBall");
								targetPlayer2.assign("yellowBall");
							}
							else
							{
								targetPlayer2.assign("redBall");
								targetPlayer1.assign("yellowBall");
							}
						}
						if (ballList.at(i).type == "yellowBall")
						{
							if (player == 0)
							{
								targetPlayer1.assign("yellowBall");
								targetPlayer2.assign("redBall");
							}
							else
							{
								targetPlayer2.assign("yellowBall");
								targetPlayer1.assign("redBall");
							}
						}
					}
					
					ballList.at(i).alive = false;
					if (player == 0)
					{
						activityPlayer1.push_back(i);
					}
					else
					{
						activityPlayer2.push_back(i);
					}

					if (CheckWin())
					{
						printf("Playerul %d a castigat.\n", player);
					}
				}
				
			}
			
			if (CheckColisionTop(ballList.at(i).x, ballList.at(i).z))
			{
				if (ballList.at(i).vz < 0) {
					ballList.at(i).vz = -ballList.at(i).vz;
				}
				ballList.at(i).z = -0.73f;
			}

			if (CheckColisionBottom(ballList.at(i).x, ballList.at(i).z))
			{
				if (ballList.at(i).vz > 0) {
					ballList.at(i).vz = -ballList.at(i).vz;
				}
				ballList.at(i).z = 0.73f;
			}

			if (CheckColisionLeft(ballList.at(i).x, ballList.at(i).z))
			{
				if (ballList.at(i).vx < 0) {
					ballList.at(i).vx = -ballList.at(i).vx;
				}
				ballList.at(i).x = -1.5f;
			}

			if (CheckColisionRight(ballList.at(i).x, ballList.at(i).z))
			{
				if (ballList.at(i).vx > 0) {
					ballList.at(i).vx = -ballList.at(i).vx;
				}
				ballList.at(i).x = 1.5f;
			}

			if (ballList.at(i).vx != 0 || ballList.at(i).vz != 0)
			{
				ballList.at(i).x += ballList.at(i).vx * deltaTimeSeconds;
				ballList.at(i).z += ballList.at(i).vz * deltaTimeSeconds;

				ballMoving = true;
				if (ballList.at(i).vx > 0)
				{
					ballList.at(i).vx -= deltaTimeSeconds * speedCoeficient;
				}
				else if (ballList.at(i).vx < 0) {
					ballList.at(i).vx += deltaTimeSeconds * speedCoeficient;
				}

				if (ballList.at(i).vz > 0)
				{
					ballList.at(i).vz -= deltaTimeSeconds * speedCoeficient;
				}
				else if (ballList.at(i).vz < 0) {
					ballList.at(i).vz += deltaTimeSeconds * speedCoeficient;
				}

				float mag = sqrtf((ballList.at(i).vx * ballList.at(i).vx) + (ballList.at(i).vx * ballList.at(i).vx));

				if (mag < 0.001f) {
					ballList.at(i).vx = 0;
					ballList.at(i).vz = 0;
				}
			}

			for (int j = 0; j < 16; j++)
			{
				if (i != j)
				{
					if (CheckBallCollision(ballList.at(i), ballList.at(j)))
					{
						colisions = true;
						
						float fDistance = sqrtf((ballList.at(i).x - ballList.at(j).x)*(ballList.at(i).x - ballList.at(j).x) +
							(ballList.at(i).z - ballList.at(j).z)*(ballList.at(i).z - ballList.at(j).z));

						//fix overlapping
						float fOverlap = 0.5f * (fDistance - ballRadius - ballRadius);

						ballList.at(i).x -= fOverlap * (ballList.at(i).x - ballList.at(j).x) / fDistance;
						ballList.at(i).z -= fOverlap * (ballList.at(i).z - ballList.at(j).z) / fDistance;

						ballList.at(j).x += fOverlap * (ballList.at(i).x - ballList.at(j).x) / fDistance;
						ballList.at(j).z += fOverlap * (ballList.at(i).z - ballList.at(j).z) / fDistance;

						// resolving colision
						float nx = (ballList.at(j).x - ballList.at(i).x) / fDistance;
						float nz = (ballList.at(j).z - ballList.at(i).z) / fDistance;

						float tx = -nz;
						float tz = nx;

						float dptan1 = ballList.at(i).vx * tx +
							ballList.at(i).vz * tz;

						float dptan2 = ballList.at(j).vx * tx +
							ballList.at(j).vz * tz;

						float dpnorm1 = ballList.at(i).vx * nx +
							ballList.at(i).vz * nz;

						float dpnorm2 = ballList.at(j).vx * nx +
							ballList.at(j).vz * nz;

						float m1 = dpnorm2;
						float m2 = dpnorm1;

						// ball i
						float vx1 = tx * dptan1 + nx * m1;
						float vz1 = tz * dptan1 + nz * m1;
						ballList.at(i).vx = vx1;
						ballList.at(i).vz = vz1;

						// ball j
						float vx2 = tx * dptan2 + nx * m2;
						float vz2 = tz * dptan2 + nz * m2;
						ballList.at(j).vx = vx2;
						ballList.at(j).vz = vz2;

						//version2
						/*float kx = (ballList.at(i).vx - ballList.at(j).vx);
						float kz = (ballList.at(i).vz - ballList.at(j).vz);
						float p = (nx * kx * nz * kz);

						ballList.at(i).vx = ballList.at(i).vx - p * nx;
						ballList.at(i).vz = ballList.at(i).vz - p * nz;

						ballList.at(j).vx = ballList.at(j).vx + p * nx;
						ballList.at(j).vz = ballList.at(j).vz + p * nz;*/
					}
				}
			}
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(ballList.at(i).x, 1, ballList.at(i).z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05));
			RenderMeshModified(meshes[ballList.at(i).type], shaders["Simple"], modelMatrix);
		}
	}

	if (ballMoving)
	{
		topView = true;
		enterBallView = false;
	}

	if (!ballMoving && start)
	{
		topView = false;
		enterBallView = true;
		round = false;
	}

	if (!ballMoving && !colisions && !round && resetBallOnce)
	{
		ResetWhiteBall();
		printf("Player%d a comis un fault. \n", player + 1);
		printf("Acesta isi pierde randul. \n\n");
		ChangePlayer();
		IncrementFault();
		resetBallOnce = false;
	}
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMeshModified(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderMeshModifiedAnimation(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int clock_loc = glGetUniformLocation(shader->program, "clock");
	glUniform1f(clock_loc, clock);

	mesh->Render();
}



// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	
	if (choosePositionWhiteBall)
	{
		float amount = 0.6;
		
		if (window->KeyHold(GLFW_KEY_SPACE))
		{
			choosePositionWhiteBall = false;
			topView = false;
			enterBallView = true;
			start = true;
		}

		if (window->KeyHold(GLFW_KEY_A))
		{
			float newX = ballList.at(0).x - amount * deltaTime;
			if (CheckLimits(newX, ballList.at(0).z)) {
				ballList.at(0).x = newX;
			}
		}

		if (window->KeyHold(GLFW_KEY_D))
		{
			float newX = ballList.at(0).x + amount * deltaTime;
			if (CheckLimits(newX, ballList.at(0).z)) {
				ballList.at(0).x = newX;
			}
		}

		if (window->KeyHold(GLFW_KEY_W))
		{
			float newZ = ballList.at(0).z - amount * deltaTime;
			if (CheckLimits(ballList.at(0).x, newZ)) {
				ballList.at(0).z = newZ;
			}
		}

		if (window->KeyHold(GLFW_KEY_S))
		{
			float newZ = ballList.at(0).z + amount * deltaTime;
			if (CheckLimits(ballList.at(0).x, newZ)) {
				ballList.at(0).z = newZ;
			}
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	float sensivityOX = 0.003f;
	float sensivityOY = 0.003f;

	if (stayBallView && window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		angle += deltaX * sensivityOY * (-2);
		camera->RotateThirdPerson_OY(deltaX  * sensivityOY * (-2));
	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	
	// add mouse button press event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && stayBallView)
	{
		clock_ref = Engine::GetElapsedTime();
		startTiming = true;
	}
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	if (startTiming)
	{
		startTiming = false;
		ballList.at(0).vx = camera->forward.x * impactSpeed * ((int)clock % 5);
		ballList.at(0).vz = camera->forward.z * impactSpeed * ((int)clock % 5);
		enterBallView = false;
		stayBallView = false;
		topView = true;
		setCamera = true;
		colisions = false;
		round = true;
	}
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

bool Tema2::CheckLimits(float x, float z)
{
	if (x > 1.5f || x < 0.5f ||
		z > 0.73f || z < -0.73f)
		return false;
	return true;
}

bool Tema2::CheckColisionLeft(float x, float z)
{
	if (x < -1.5f)
		return true;
	return false;
}

bool Tema2::CheckColisionRight(float x, float z)
{
	if (x > 1.5f)
		return true;
	return false;
}

bool Tema2::CheckColisionTop(float x, float z)
{
	if (z < -0.73f)
		return true;
	return false;
}

bool Tema2::CheckColisionBottom(float x, float z)
{
	if (z > 0.73f)
		return true;
	return false;
}

bool Tema2::CheckBallCollision(Ball b1, Ball b2)
{
	return fabs((b1.x - b2.x)*(b1.x - b2.x) +
		(b1.z - b2.z)*(b1.z - b2.z)) < (2 * ballRadius) * (2 * ballRadius);
}

bool Tema2::CheckFalling(float x, float z)
{
	for (int i = 0; i < 12; i = i + 2)
	{
		float centerX = holes.at(i);
		float centerZ = holes.at(i + 1);

		float distanceSquared = (centerX - x) * (centerX - x) + (centerZ - z) * (centerZ - z);

		if (distanceSquared < holeRadius * holeRadius)
		{
			return true;
		}
	}
	return false;
}

void Tema2::ResetWhiteBall()
{
	//printf("White Ball was cued\n");
	choosePositionWhiteBall = true;
	enterBallView = false;
	stayBallView = false;
	topView = true;
	ballMoving = false;
	setCamera = true;
	start = false;
	ballList.at(0).vx = 0;
	ballList.at(0).vz = 0;
	ballList.at(0).x = WhiteBX;
	ballList.at(0).z = WhiteBZ;
	round = false;
	ShowInfo();
}

void Tema2::ChangePlayer()
{
	if (player == 0)
	{
		player = 1;
	}
	else
	{
		player = 0;
	}
}

bool Tema2::CheckWin()
{
	//player1
	if (targetPlayer1 == "redBall")
	{
		if (std::equal(activityPlayer1.begin(), activityPlayer1.begin() + 7, reds.begin()) && activityPlayer1.size() == 8)
		{
			//check if black at the end
			if (activityPlayer1.at(activityPlayer1.size() - 1) == 5);
			return true;
		}
	}
	else if (targetPlayer1 == "yellowBall")
	{
		if (std::equal(activityPlayer1.begin(), activityPlayer1.begin() + 7, yellows.begin()) && activityPlayer1.size() == 8)
		{
			//check if black at the end
			if (activityPlayer1.at(activityPlayer1.size() - 1) == 5);
			return true;
		}
	}

	//player2
	else if (targetPlayer2 == "redBall")
	{
		if (std::equal(activityPlayer2.begin(), activityPlayer2.begin() + 7, reds.begin()) && activityPlayer2.size() == 8)
		{
			//check if black at the end
			if (activityPlayer2.at(activityPlayer2.size() - 1) == 5);
			return true;
		}
	}

	else if (targetPlayer2 == "yellowBall")
	{
		if (std::equal(activityPlayer2.begin(), activityPlayer2.begin() + 7, yellows.begin()) && activityPlayer2.size() == 8)
		{
			//check if black at the end
			if (activityPlayer2.at(activityPlayer2.size()-1) == 5);
			return true;
		}
	}
	
	return false;
}

void Tema2::IncrementFault()
{
	if (player == 0)
	{
		Player1Faults++;
	}
	else
	{
		Player2Faults++;
	}
}

void Tema2::ShowInfo()
{
	printf("Este randul Player-ului%d \n", player + 1);
	if (targetPlayer1 == "none")
	{
		printf("El inca nu este obligat sa bage o anumita culoare.\n");
	}
	else
	{
		if (player == 0)
		{
			printf("El trebuie sa bage %s.\n", targetPlayer1);
		}
		else
		{
			printf("El trebuie sa bage %s.\n", targetPlayer2);
		}
	}
	printf("Player1 a bagat %d bile. \n", activityPlayer1.size());
	printf("Player2 a bagat %d bile. \n", activityPlayer2.size());
	printf("Player1 a comis %d faulturi, iar Player2 a comis %d fault-uri. \n", Player1Faults, Player2Faults);
	printf("\n");
}