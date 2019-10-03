#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Ball.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;
		bool CheckLimits(float x, float z);
		bool CheckColisionLeft(float x, float z);
		bool CheckColisionRight(float x, float z);
		bool CheckColisionTop(float x, float z);
		bool CheckColisionBottom(float x, float z);
		bool CheckBallCollision(Ball b1, Ball b2);
		bool CheckFalling(float x, float z);
		void ResetWhiteBall();
		void ChangePlayer();
		bool CheckWin();
		void IncrementFault();
		void ShowInfo();

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMeshModified(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix);
		void RenderMeshModifiedAnimation(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Laborator::CameraTema *camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
		
		//gameplay
		bool choosePositionWhiteBall = true;
		bool enterBallView = false;
		bool stayBallView = false;
		bool topView = true;
		bool ballMoving = false;
		bool setCamera = true;
		bool start = false;
		bool round = false;
		bool resetBallOnce = false;

		int player = 0;
		std::vector<int> activityPlayer1;
		std::vector<int> activityPlayer2;
		std::string targetPlayer1;
		std::string targetPlayer2;
		std::vector<int> reds;
		std::vector<int> yellows;

		int Player1Faults = 0;
		int Player2Faults = 0;

		bool colisions = false;

		//balls
		float WhiteBX = 0.82f;
		float WhiteBZ = 0;
		Ball WhiteB;
		std::vector<Ball> ballList;
		float ballRadius = 0.047f;
		
		float speedCoeficient = 0.17;
		std::vector<int> collidingPairs;
		float impactSpeed = 0.8;
		float holeRadius = 0.08f;

		std::vector<float> holes;
		float angle;

		float offset = 0.1f;
		float offsetX;
		float offsetZ;

		float clock;
		float clock_ref;
		bool startTiming = false;
};