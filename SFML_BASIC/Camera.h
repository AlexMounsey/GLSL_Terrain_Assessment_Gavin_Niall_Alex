#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GL/glu.h" 



class Camera{
    static aiVector3D zero,yaxis,zaxis;
public:
    aiVector3D position;
	aiVector3D velocity;
    aiVector3D forward;
    aiVector3D up;

	sf::Vector2i prevMouse;
 
    float forwardSpeed;
    float rotationSpeed;
     
    Camera():forwardSpeed(0.1f),rotationSpeed(0.1f){}
 
	void Init(sf::RenderWindow * window = nullptr, aiVector3D& p = zero, aiVector3D& f = zaxis, aiVector3D& u = yaxis){
        position=p;
        forward=f;
        up=u;
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
			float pi = acos(-1);
			float yRotate = (mVec.x / 1200.0f) * 2 * pi;
			float xRotate = (mVec.y / 1200.0f) * 2 * pi;

			TurnRightLeft(yRotate);
			TurnUpDown(xRotate);
			sf::Mouse::setPosition(prevMouse);
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
        // x' = x cos θ − y sin θ
		// y' = x sin θ + y cos θ

		forward.x = forward.x * cos(angle) - forward.z * sin(angle);
		forward.z = forward.z * sin(angle) - forward.x * cos(angle);
    }
         
	void TurnUpDown(float amount){
        //TODO
    }
 
    void ViewingTransform(){
        gluLookAt(	position.x,position.y,position.z,// camera position
			position.x + forward.x, position.y + forward.y, position.z + forward.z, //look at this point //TODO: BUG here!! what is it ??
					0,1,0); //camera up
    }
 
};

//create some default vectors
aiVector3D Camera::zero(0.0f);
aiVector3D Camera::yaxis(0.0f,1.0f,0.0f);
aiVector3D Camera::zaxis(0.0f,0.0f,1.0f);
