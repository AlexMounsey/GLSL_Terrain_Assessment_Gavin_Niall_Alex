//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#include <iostream> 
#include "Terrain.h"
#include "Camera.h"




int main() 
{ 
    // Create the main window 
	sf::ContextSettings Settings;
	Settings.depthBits = 24;			// Request a 24 bits depth buffer
	Settings.stencilBits = 8;			// Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 16;	// Request 2 levels of antialiasing
	sf::RenderWindow App(sf::VideoMode(1280, 720, 32), "Terrain Assignment", sf::Style::Close, Settings);
    int width=600,height=600;
    // Create a clock for measuring time elapsed     
    sf::Clock Clock; 

	aiVector3D position(0,10,-30);
	Camera camera;
	camera.Init(&App, position);  //create a camera

      
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.0f, 0.0f, 0.0f, 0.f); //background colour
	glEnable(GL_DEPTH_TEST); // check for depth
	glEnable(GL_NORMALIZE); // automatically convert normals to unit normals
	glEnable(GL_LIGHTING); //This enables the possibility of lighting
	glEnable(GL_LIGHT0); //This enables a single light
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	//Giving the light positions
	GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 10.0, 10.0, -100.0, 0.0 };
	GLfloat light_amb[] = { 3.0, 3.0, 3.0, 1.0 };

	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create an array of RGBA values (White)
	GLfloat materialShininess[] = { 128.0f }; // select value between 0-128, 128=shiniest

   
    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
     
    //set up a 3D Perspective View volume
    gluPerspective(90.f, (float)width/height, 1.f, 300.0f);//fov, aspect, zNear, zFar 

	//load & bind the shader
	sf::Shader shader;
	//all the lighting & texture blending code should  be put in 'fragment.glsl'
	if(!shader.loadFromFile("vertex.glsl","fragment.glsl")){
        exit(1);
    }

#pragma region Textures
	// Textures
	sf::Texture waterTexture;
	if (!waterTexture.loadFromFile("water.png")) { std::cout << "Could not load water image"; }

	sf::Texture grassTexture;
	if (!grassTexture.loadFromFile("grass.png")){ std::cout << "Could not load grass image"; }

	sf::Texture snowTexture;
	if (!snowTexture.loadFromFile("snowy rock.png")) { std::cout << "Could not load rock image"; }
#pragma endregion
	shader.setParameter("waterTex", waterTexture);
	shader.setParameter("grassTex", grassTexture);
	shader.setParameter("snowTex", snowTexture);



	//Create our Terrain
	Terrain terrain;
	terrain.InitWithFileName("heightmap.bmp");

	shader.setParameter("tallestPoint", terrain.tallestPoint);

    // Start game loop 
    while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::I))
			{ 
				terrain.swapWireFrame();
			}
			//update the camera
			camera.Update(Event, &App);
        } 

		camera.UpdatePosition();
           

		// Clear color and depth buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Apply some transformations 
		//initialise the worldview matrix
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();


		//get the viewing transform from the camera
		camera.ViewingTransform();


		//make the world spin
		//TODO:probably should remove this in final
		static float ang = 0.0;
		ang += 0.01f;
		glRotatef(ang * 2, 0, 1, 0);//spin about y-axis

		sf::Shader::bind(&shader);


		glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb); // set color of diffuse component
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color); // set color of diffuse component
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_color); // set color of specular component

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // set position



		//make the world spin
		//TODO:probably should remove this in final
		//static float ang=0.0;
		//ang+=0.01f;
		//glRotatef(ang*2,0,1,0);//spin about y-axis
		

		
		//draw the world
		terrain.Draw();

		   
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
   
    return EXIT_SUCCESS; 
}

