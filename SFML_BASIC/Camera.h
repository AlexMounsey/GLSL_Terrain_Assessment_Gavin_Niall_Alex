#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GL/glu.h" 

class Camera
{
	static aiVector3D zero, yaxis, zaxis, xaxis;

public:
	aiVector3D position;
	aiVector3D forward;
	aiVector3D up;
	aiVector3D right;

	int w, h;

	float forwardSpeed;
	float rotationSpeed;

	bool perspect;
	bool ortho;

	Camera() :forwardSpeed(0.5f), rotationSpeed(0.1f){}

	void Init(aiVector3D& p = zero, aiVector3D& f = zaxis, aiVector3D& u = yaxis){
		position = p;
		forward = f;
		up = u;
		w = 1280, h = 720;
		perspect = true;
	}

	void Update(sf::Event e)
	{
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::D)){ MoveLeftRight(1); }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::A)){ MoveLeftRight(-1); }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::W)){ MoveForwardBack(1); }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::S)){ MoveForwardBack(-1); }

		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Q)){ MoveUpDown(1); }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::E)){ MoveUpDown(-1); }

		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Right)){ TurnRightLeft(1); }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Left)){ TurnRightLeft(-1); }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Up)){ TurnUpDown(1); }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Down)){ TurnUpDown(-1); }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::P)){ perspect = true; }
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::O)){ perspect = false; }
	}

	void MoveLeftRight(int dir)
	{
		aiVector3D crossp = crossProduct(forward, up);
		position += crossp.Normalize() * (forwardSpeed*dir);
	}
	void MoveUpDown(int dir) { position += (up*(forwardSpeed*dir)); }
	void MoveForwardBack(int dir) { position += (forward*(forwardSpeed*dir)); }

	void TurnRightLeft(int dir)
	{
		aiQuaternion quatern = aiQuaternion(up, dir*rotationSpeed);
		forward = quatern.Rotate(forward);
		up = quatern.Rotate(up);
	}

	void TurnUpDown(int dir)
	{
		aiQuaternion quat = aiQuaternion(crossProduct(forward, up).Normalize(), -dir*rotationSpeed);
		forward = quat.Rotate(forward);
		up = quat.Rotate(up);
	}

	void changeProjction()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (perspect)
		{
			gluPerspective(90.f, (float)w / h, 0.001f, 300.0f);//fov, aspect, zNear, zFar
		}
		else if (!perspect)
		{
			glOrtho(-30, 30, -30, 30, 0.001f, 100000);
		}
	}

	void ViewingTransform()
	{
		gluLookAt(position.x, position.y, position.z,
			position.x + forward.x, position.y + forward.y, position.z + forward.z,
			0, 1, 0); //camera up
	}

	aiVector3D crossProduct(aiVector3D a, aiVector3D b)
	{
		aiVector3D crossPro = aiVector3D((a.y*b.z) - (a.z*b.y), (a.z*b.x) - (a.x*b.z), (a.x*b.y) - (a.y*b.x));
		return crossPro;
	}
};

//create some default vectors
aiVector3D Camera::zero(0.0f);
aiVector3D Camera::yaxis(0.0f, 1.0f, 0.0f);
aiVector3D Camera::zaxis(0.0f, 0.0f, 1.0f);

