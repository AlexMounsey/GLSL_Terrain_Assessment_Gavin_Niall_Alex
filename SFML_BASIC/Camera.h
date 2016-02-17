#pragma comment(lib,"assimp.lib")
#include <assimp\cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "GL/glu.h" 



class Camera{
	static aiVector3D zero, yaxis, zaxis;
public:
	aiVector3D position;
	aiVector3D velocity;
	aiVector3D originalForward;
	aiVector3D forward;
	aiVector3D up;
	float m_yAngle = 0;
	float m_xAngle = 0;

	sf::Vector2i prevMouse;

	float forwardSpeed;
	float rotationSpeed;

	Camera() :forwardSpeed(0.1f), rotationSpeed(0.1f){}

	void Init(sf::RenderWindow * window = nullptr, aiVector3D& p = zero, aiVector3D& f = zaxis, aiVector3D& u = yaxis){
		position = p;
		forward = f;
		originalForward = p;
		up = u;
		prevMouse = window->getPosition() + sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2);
	}

	void Update(sf::Event e, sf::RenderWindow * window){//respond to keyboard events
		if (e.type == sf::Event::KeyPressed) {
			if (e.key.code == sf::Keyboard::D){
				MoveLeftRight(+1);
			}

			if (e.key.code == sf::Keyboard::A){
				MoveLeftRight(-1);
			}

			if (e.key.code == sf::Keyboard::W){
				MoveForwardBack(1);
			}
			if (e.key.code == sf::Keyboard::S){
				MoveForwardBack(-1);
			}

			if (e.key.code == sf::Keyboard::Space){
				MoveUpDown(1);
			}
			if (e.key.code == sf::Keyboard::LShift){
				MoveUpDown(-1);
			}
		}
		if (e.type == sf::Event::MouseMoved)
		{
			sf::Vector2i mVec = sf::Mouse::getPosition() - prevMouse;
			sf::Mouse::setPosition(prevMouse);
			float pi = acos(-1);
			float yRotate = (mVec.x / 1200.0f) * 2 * pi;
			float xRotate = (mVec.y / 1200.0f) * 2 * pi;

			TurnRightLeft(yRotate);
			TurnUpDown(xRotate);
			window->pollEvent(e);
		}

	}

	void UpdatePosition(){
		position += velocity;
		velocity *= 0.99f;
	}

	void MoveLeftRight(int dir){ //Dir=+1=>Right, dir=-1=> Left
		aiVector3D right;
		right.x = -forward.z;
		right.z = forward.x;
		velocity += (right*(forwardSpeed*dir));
	}

	void MoveUpDown(int dir){ //Dir=+1=>Right, dir=-1=> Left
		velocity += (up*(forwardSpeed*dir));
	}

	void MoveForwardBack(int dir){ //Dir=+1=>Forward, dir=-1=> Back

		velocity += (forward*(forwardSpeed*dir));
	}

	void TurnRightLeft(float angle){

		aiQuaternion quatern = aiQuaternion(up, angle*rotationSpeed);
		forward = quatern.Rotate(forward);
		//up = quatern.Rotate(up);
	}

	void TurnUpDown(float angle){
		aiQuaternion quat = aiQuaternion(crossProduct(forward, up).Normalize(), angle*rotationSpeed);
		forward = quat.Rotate(forward);
		//up = quat.Rotate(up);
	}

	void ViewingTransform(){
		gluLookAt(position.x, position.y, position.z,// camera position
			position.x + forward.x, position.y + forward.y, position.z + forward.z, //look at this point //TODO: BUG here!! what is it ??
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
