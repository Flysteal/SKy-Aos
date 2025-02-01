#include "Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

int main()
{

    if (!glfwInit()) { std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;  //check for error
     }
	 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //the version
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     // Tell GLFW we are using the CORE profile
     // So that means we only have the modern functions
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
     GLFWwindow* window = glfwCreateWindow(width, height, "Sky Lands ", NULL, NULL);

    if (!window) { std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); //here too
        return -2;
     }
    glfwMakeContextCurrent(window);
     // Initialize GLAD
	 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -3; // special one:)
    }
    gladLoadGL();
    glViewport(0, 0, width, height);



	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("/media/fly/o-o/GitHub/Flysteal/SKY-t5/SKY_LANDS/Resourcefiles/Shader/default.vert",  "/media/fly/o-o/GitHub/Flysteal/SKY-t5/SKY_LANDS/Resourcefiles/Shader/default.frag");
    //for outline
    Shader outliningProgram("/media/fly/o-o/GitHub/Flysteal/SKY-t5/SKY_LANDS/Resourcefiles/Shader/outlining.vert", "/media/fly/o-o/GitHub/Flysteal/SKY-t5/SKY_LANDS/Resourcefiles/Shader/outlining.frag");


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);



	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    

    glfwSwapInterval(false); //vsync on 
    glEnable(GL_DEPTH_TEST);// what is closer then others
    //do not use!! .. 
    //glDepthFunc(GL_LESS);


    glEnable(GL_CULL_FACE); // for renedering and sonic faster
    glEnable(GL_FRONT); // the front for an obkect
    glEnable(GL_CCW); //counter clock wise


    // Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;



    // Enables the Stencil Buffer
	glEnable(GL_STENCIL_TEST);
	// Sets rules for outcomes of stecil tests
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);



    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	
	// Load in a model
	Model model("/SKY_LANDS/Resourcefiles/models/map/scene.gltf");

    while (!glfwWindowShouldClose(window))
    {
        // Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

        if (timeDiff >= 1.0 / 20.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000 );
			std::string newTitle = "Sky Lands " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;

			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}
    
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clears buffer

    	camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);



        // Make it so the stencil test always passes
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		// Enable modifying of the stencil buffer
		glStencilMask(0xFF);
        // ||||||
	    //  Draw a model
		model.Draw(shaderProgram, camera);


        // Make it so only the pixels without the value 1 pass the test
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// Disable modifying of the stencil buffer
		glStencilMask(0x00);
		// Disable the depth buffer
		glDisable(GL_DEPTH_TEST);
        outliningProgram.Activate();
		glUniform1f(glGetUniformLocation(outliningProgram.ID, "outlining"), 0.08f);
		model.Draw(outliningProgram, camera);


        // Enable modifying of the stencil buffer
		glStencilMask(0xFF);
		// Clear stencil buffer
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		// Enable the depth buffer
		glEnable(GL_DEPTH_TEST);


	    glfwSwapBuffers(window);
	    glfwPollEvents();
    }
    

    // Delete all the objects we've created
   
    shaderProgram.Delete();
    outliningProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}