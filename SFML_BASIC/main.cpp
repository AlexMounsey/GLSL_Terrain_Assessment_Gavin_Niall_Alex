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
    camera.Init(&App, position); //create a camera
      
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.6f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
	

   
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
	sf::Shader::bind(&shader);

	//Create our Terrain
	Terrain terrain;
	terrain.InitWithFileName("heightmap.png");

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
             
			//update the camera
			camera.Update(Event, &App);
        } 

		camera.UpdatePosition();
           
        //Prepare for drawing 
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
		static float ang=0.0;
		ang+=0.01f;
		glRotatef(ang*2,0,1,0);//spin about y-axis
		

		
		//draw the world
		terrain.Draw();

		   
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
   
    return EXIT_SUCCESS; 
}
