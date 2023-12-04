#include "includes/Utility.h"
#include "includes/shader.h"
#include "random.h"
#include "solve.h"
#include <cstdlib> // for std::rand and std::srand
#include <ctime>   // for std::time

/*
Integrantes: 
Ian Camero Gonzalez
Mauricio Carazas Segovia
*/


///// camera ////
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

///------------------------Variables------------------------///
string rutaImagen = "";

///------------------------Coordenadas------------------------///
GLfloat centerX = 0.0f,
        centerY = 0.0f,
        centerZ = 0.0f;
GLfloat arista = 0.5;	//tamaño de la arista de los cubitos
GLfloat offset = 0.0005;	//tamaño de la distancia entre cubos
GLuint shader;
///------------------------Objeto Cubo------------------------///
vector<Rubik*> rubikCubes;
///------------------------Matrices------------------------///
glm::mat4 projection;
glm::mat4 modelview;
glm::vec3 cameraCenter(0, 0, 0);
glm::vec3 cameraUp(0.0, 0.0, 1.0);
glm::vec3 cameraEye(1.0, -5.0, 1.0);
///------------------------Utilidad------------------------///
int cont = 7;
bool text = true;
///------------------------Shader------------------------///
GLuint projectionPos;
GLuint modelviewPos;
GLuint viewPosPos;
///------------------------Movimientos------------------------///
vector<string> cuboga;

///------------------------Animaciones------------------------///
bool Animation1 = false;
bool Animation2 = false;


void init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef APPLE
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
}

GLFWwindow* createWindow(const char* title) {
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    return window;
}

bool glad(){
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 1;
}
glm::mat4 model(1.0f);


int main(int agrc, char* argv[]) {
	
	std::srand(std::time(0));
    std::cout << "\n\nIntegrantes: " << std::endl; 
    std::cout << "Ian Gabriel Camero Gonzalez" << std::endl;
    std::cout << "Mauricio Carazas Segovia" << std::endl;

    std::cout << "\nInstrucciones para mover el cubo: " << std::endl;
    std::cout << "Presione la tecla 'Q' para el movmiento R en el sentido horario" << std::endl;
    std::cout << "Presione la tecla 'W' para el movmiento L en el sentido horario" << std::endl;
    std::cout << "Presione la tecla 'E' para el movmiento U en el sentido horario" << std::endl;
    std::cout << "Presione la tecla 'R' para el movmiento D en el sentido horario" << std::endl;
    std::cout << "Presione la tecla 'T' para el movmiento F en el sentido horario" << std::endl;
    std::cout << "Presione la tecla 'Y' para el movmiento B en el sentido horario" << std::endl;
    std::cout << "Presione la tecla 'Z' para resolver el cubo si se movio manualmente" << std::endl;
    std::cout << "Presione la tecla 'X' para desordenar el cubo aleatoriamente" << std::endl;
    std::cout << "Presione la tecla 'C' para ordenar el cubo que se deordeno aleatoriamente" << std::endl;

    std::cout << "\nInstrucciones para la animacion del cubo: " << std::endl;
    std::cout << "Con las teclas Up, Down, Left, Right, se puede mover la camara" << std::endl;
    std::cout << "Presione la tecla 'M' para realizar la accion expandir el cubo" << std::endl;
    std::cout << "Presione la tecla 'N' para realizar la accion contraer el cubo" << std::endl;
    std::cout << "Presione la tecla '2' para realizar la animacion de la explosion" << std::endl;    

    

    string route = argv[0];
    rutaImagen = "D:/LabCompiler_CG/glfw-master/OwnProjects/rubik_final/image/colors.jpg";
    init();
    GLFWwindow* window = createWindow("Cubo Rubik");
    if (window == NULL || !glad()) return -1;


    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);;
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    GLint linked;
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
    if (linked) {
        glUseProgram(shaderProgram);
        std::cout << "Success\n";
    }
    else {
        std::cout << "Error\n";
    }

    glUseProgram(shaderProgram);

    projectionPos = glGetUniformLocation(shaderProgram, "projection");
    modelviewPos = glGetUniformLocation(shaderProgram, "view");
    viewPosPos = glGetUniformLocation(shaderProgram, "viewPos");
    modelview = glm::lookAt(cameraEye, cameraCenter, cameraUp);
    projection = glm::mat4(1.0f);
    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);
    glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    glUniform3f(viewPosPos,cameraEye.x,cameraEye.y,cameraEye.z);
    
    ///------------------------Textura------------------------///
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(rutaImagen.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ///------------------------Cubo-Rubik------------------------///
    shader = shaderProgram;
    Rubik* tmp;
    for(int i = 0; i < 20; i++) {
        tmp = new Rubik(glm::vec3(centerX, centerY, centerZ), arista, offset, shader, texture);
        rubikCubes.push_back(tmp);
        tmp->Generador();
    }
    
    glEnable(GL_DEPTH_TEST);

    ///------------------------Velocidad-----------------------///
    float time;
    glfwSetTime(0);
    float frameRate = 0.4f / 60.0f;


    float incremento = 0.0f; 
    float incremento2 = 0.0f; 
	float incremento3 = 0.0f;
    float distance = 0.004f; 
	

    while (!glfwWindowShouldClose(window))
    {
        //cam.updateFrame();
        //glm::mat4 projection = glm::perspective(glm::radians(camera.ZOOM), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        //glm::mat4 view = glm::lookAt(cam.cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cam.cameraUp);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(projectionPos, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &view[0][0]);
        //glUniform3f(viewPosPos, camera.Position.x, camera.Position.y, camera.Position.z);
      

        time = glfwGetTime();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	
        if (time > frameRate) { // Movimiento 
			rubikCubes[0]->Movimientos(); // Movimiento 
			glfwSetTime(0); // Movimiento 
		} // Movimiento
		rubikCubes[0]->Dibujar(); // Dibujar cubos
		
		
		if (Animation1 == true) {
			for(auto it = rubikCubes.begin()+1; it != rubikCubes.end()-3; it++) {
				// Dibujar cubos
				if (time > frameRate) {
					// Movimiento
					(*it)->Movimientos();
					glfwSetTime(0);
				}
				// Dibujar cubos
				(*it)->Dibujar();
			}

			if (incremento < 13.0f) {
				// C:ccc
				rubikCubes[0]->move_north(distance);
				rubikCubes[1]->move_east(distance); // Different direction
				rubikCubes[5]->move_west(distance); // Different direction
				// ccc
				rubikCubes[3]->move_south(distance);
				rubikCubes[4]->move_south(distance);
				rubikCubes[6]->move_east(distance); // Different direction
				// S:ccc
				rubikCubes[8]->move_north(distance);
				rubikCubes[9]->move_west(distance); // Different direction
				rubikCubes[10]->move_south(distance);
				// ccc
				rubikCubes[11]->move_east(distance); // Different direction
				rubikCubes[12]->move_south(distance);
				rubikCubes[13]->move_north(distance); // Different direction
				//
				rubikCubes[15]->move_north(distance); // Different direction
				incremento += distance;
			if (incremento < 8.0f) {
				// C:c
				rubikCubes[2]->move_south(distance); // Different direction
				rubikCubes[7]->move_west(distance);
				rubikCubes[0]->move_north(distance); // Different direction
				rubikCubes[3]->move_east(distance); // Different direction
				// S:
				rubikCubes[8]->move_north(distance);
				rubikCubes[11]->move_south(distance);
				rubikCubes[14]->move_east(distance);
				rubikCubes[14]->move_north(distance); // Different direction
				rubikCubes[16]->move_west(distance); // Different direction
				incremento += distance;
			}

			if (incremento3 > -18.0f) {
				// C
				rubikCubes[1]->move_west(distance);
				rubikCubes[4]->move_north(distance); // Different direction
				// S
				rubikCubes[9]->move_east(distance);
				rubikCubes[12]->move_south(distance); // Different direction
				//rubikCubes[16]->move_east(0.001);
				incremento3 -= distance;
			}

			if (incremento3 > -23.0f) {
				// C
				rubikCubes[5]->move_east(distance); // Different direction
				rubikCubes[6]->move_west(distance); 
				rubikCubes[2]->move_north(distance); // Different direction
				rubikCubes[7]->move_south(distance); // Different direction
				// S
				rubikCubes[10]->move_east(distance);
				rubikCubes[13]->move_south(distance);
				rubikCubes[16]->move_west(distance); // Different direction
				incremento3 -= distance;
			}
		}
	}

         if (Animation2 == true){
            if ( incremento2 >= 0.0f and incremento2 < 1.0f) {
                for(auto it = rubikCubes.begin(); it != rubikCubes.end(); it++){
                    (*it)->Expandir(distance); //////////////////////////////////////////////
                }
                incremento2 += distance; 
            }
             else if ( incremento2 >= 1.0f and incremento2 < 2.0f) {
                 for(auto it = rubikCubes.begin(); it != rubikCubes.end(); it++)
                 {
                     (*it)->Expandir(-distance); //////////////////////////////////////////////
                 }incremento2 += distance; 
             }
             else {
                 incremento2 = 0.0f; 
             }
             //incremento += distance; 
        }
	

        glfwSwapBuffers(window); 
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

///------------------------Funciones------------------------///
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    glm::mat4 rotation = glm::mat4(1.0f);
    float cameraSpeed = 0.2; 
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        rubikCubes[0]->setAnimacion("R");
        cuboga.push_back("R");
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        rubikCubes[0]->setAnimacion("L");
        cuboga.push_back("L");
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        rubikCubes[0]->setAnimacion("U");
        cuboga.push_back("U");
    }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        rubikCubes[0]->setAnimacion("U1");
        cuboga.push_back("U1");
    }
	
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        rubikCubes[0]->setAnimacion("D");
        cuboga.push_back("D");
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        rubikCubes[0]->setAnimacion("F");
        cuboga.push_back("F");
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        rubikCubes[0]->setAnimacion("B");
        cuboga.push_back("B");
    }
	
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		if (cont < 13) {
			//rubikCubes[0]->Expandir(0.1f);
            for(auto it = rubikCubes.begin(); it != rubikCubes.end(); it++)
			    (*it)->Expandir(0.1f); //////////////////////////////////////////////
			cont++;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		if (cont > 1) {
			//rubikCubes[0]->Expandir(-0.1f);
            for(auto it = rubikCubes.begin(); it != rubikCubes.end(); it++)
			   (*it)->Expandir(-0.1f); //////////////////////////////////////////////
			cont--;
		}
	}
    ///////////////////////////////////////////////////////////////////////////////////
    
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		//rubikCubes[1]->move_south(0.1f);
		if(Animation2 == true) Animation2 = false;
		else Animation2 = true;

	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    Animation1 = true; // Activar la animación 1 (explosión)
}


    ///////////////////////////////////////////////////////////////////////////////////
    
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) { // esta funcion ordena el desorden manual 
		std::vector<std::string> solution = get_solution(to_cube_not(cuboga));
		rubikCubes[0]->Solve(solution);
		cuboga = vector<string>();
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) { // desordena de forma aleatoria 
		std::vector<std::string> movements_to_scramble = scramble(15);
		for (int i = 0; i < (int)movements_to_scramble.size(); ++i)std::cout << movements_to_scramble[i] << 'f'; std::cout << std::endl;
		rubikCubes[0]->Desordenador(movements_to_scramble);

	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) { //ordena lo que se desordeno de forma aleatoria 
		std::vector<std::string> solution = get_solution(to_cube_not(rubikCubes[0]->getSolucion()));
		rubikCubes[0]->Solve(solution);
	}

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (offset < 0.1) {
			offset = 0.2;
		}
		else { 
			offset = 0.05; 
		}
		//rubikCubes[0]->setOffset(offset);
		//rubikCubes[0]->Generador();
        for(auto it = rubikCubes.begin(); it != rubikCubes.end(); it++) {
            (*it)->setOffset(offset);
            (*it)->Generador();
        }

	} 
    
    

    cameraEye = glm::mat3(rotation) * cameraEye;
    cameraUp = glm::mat3(rotation) * cameraUp;
    modelview = glm::lookAt(cameraEye, cameraCenter, cameraUp);
    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
    glUniform3f(viewPosPos, cameraEye.x, cameraEye.y, cameraEye.z);


   if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.Position += cameraSpeed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.Position -= cameraSpeed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    


}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    
     float xpos = static_cast<float>(xposIn);
     float ypos = static_cast<float>(yposIn);

     if (firstMouse)
     {
         lastX = xpos;
         lastY = ypos;
         firstMouse = false;
     }

     float xoffset = xpos - lastX;
     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

     lastX = xpos;
	 lastY = ypos;

     camera.ProcessMouseMovement(xoffset, yoffset);
 }