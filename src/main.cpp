#include "main.h"
#include "math.h"
#include "timer.h"
#include "ball.h"
#include "rectangle.h"
#include "semicircle.h"
#include "leg.h"
#include "slope.h"
#include "magnet.h"
#include "spikes.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

float randomPos(float min, float max)
{
  return ((float(rand()) / float(RAND_MAX)) * (max-min)) + min;
}

bool detect_collision_tramp(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (b.height));
}

bool detect_collision_mag(bounding_box_t a, bounding_box_t b) {
   return (abs(a.x - b.x) * 2 < (a.width - 1.5 + b.width)) &&
         (abs(a.y - b.y) * 2 < (a.height - 3.05 + b.height));
}

Rectangle mudFloor;
Ball playerBall, balls[25];
Rectangle grassFloor;
Semicircle trampolineFloor, pond;
Leg trampolineLeg1;
Leg trampolineLeg2;
Magnet magnet;
Spikes spike1, spike2;
Slope slopes[7];
int slopePos[7], angle[7];

int up_check = 0;
int in_pond = 0;


int points = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

double floor_y = -2;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render

    mudFloor.draw(VP);

    grassFloor.draw(VP);

    trampolineFloor.draw(VP);

    trampolineLeg1.draw(VP);
    trampolineLeg2.draw(VP);
    magnet.draw(VP);

    spike1.draw(VP);
    spike2.draw(VP);

    pond.draw(VP);

    playerBall.draw(VP);

    for (int i = 0; i < 25; i++) {
      balls[i].draw(VP);
    }

    for (int i = 0; i < 7; i++) {
      slopes[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {

    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);

    if (left && playerBall.position.y >= floor_y) {
        playerBall.position.x -= 0.05;
    } //Move Left Until no collision

    if (left && playerBall.position.y < floor_y) {
      playerBall.position.x -= 0.025;

      if (playerBall.position.x >= 1 && playerBall.position.x <= 2) {
        playerBall.position.y = - 2.35 + 0.25 - sqrt(1-pow(2-playerBall.position.x,2));
      }
      else if(playerBall.position.x > 2 && playerBall.position.x <= 3){
        playerBall.position.y = - 2.35 + 0.25 - sqrt(1-pow(2-playerBall.position.x,2));
      }
    }

    if (up && !up_check) {

      if (playerBall.position.y == floor_y) {
        playerBall.speedy = -0.1;
      }

      else if(playerBall.position.y < floor_y){
        playerBall.speedy = -0.05;
      }

      up_check = 1;
    }//Vertical Jump until no collision

    if(right && playerBall.position.y >= floor_y) {
      playerBall.position.x += 0.05;
    }//Move right until no collision

    if(right && playerBall.position.y < floor_y){

      playerBall.position.x += 0.025;

      if (playerBall.position.x >= 1 && playerBall.position.x <= 2) {
        playerBall.position.y = - 2.35 + 0.25 - sqrt(1-pow(2-playerBall.position.x,2));
      }
      else if(playerBall.position.x > 2 && playerBall.position.x <= 3){
        playerBall.position.y = - 2.35 + 0.25 - sqrt(1-pow(2-playerBall.position.x,2));
      }

    }
    playerBall.tick();
}

void tick_elements() {

    float tempPos;

    playerBall.tick();

    if (spike1.position.x >= 0) {
      spike1.speedx = 0.02;
    }

    if (spike1.position.x <= -5) {
      spike1.speedx = -0.02;
    }

    spike1.tick();

    if (spike2.position.x <= 3) {
      spike2.speedx = -0.02;
    }

    if (spike2.position.x >= 5) {
      spike2.speedx = 0.02;
    }

    spike2.tick();

    //Spike 1 Penalty
    if (detect_collision(spike1.bounding_box(), playerBall.bounding_box())) {
      points -= 1;

      if (playerBall.speedy > 0) {
        playerBall.speedy = -0.1;
      }

      else if (playerBall.position.x > spike1.position.x) {
        playerBall.speedx = -0.025;
      }

      else if (playerBall.position.x < spike1.position.x) {
        playerBall.speedx = 0.025;
      }

      if (playerBall.position.x == spike1.position.x) {
        playerBall.speedy = -0.1;
      }
    }

    //Spike 2 Penalty
    if (detect_collision(spike2.bounding_box(), playerBall.bounding_box())) {
      points -= 1;

      if (playerBall.speedy > 0) {
        playerBall.speedy = -0.1;
      }

      else if (playerBall.position.x > spike2.position.x) {
        playerBall.speedx = -0.025;
      }

      else if (playerBall.position.x < spike2.position.x) {
        playerBall.speedx = 0.025;
      }

      if (playerBall.position.x == spike2.position.x) {
        playerBall.speedy = -0.1;
      }
    }

    //Come in the magnetic field of the magnet
    if (detect_collision(magnet.bounding_box(), playerBall.bounding_box())) {

      if (playerBall.position.y >= magnet.position.y - 0.25 && playerBall.position.y <= magnet.position.y + 0.25 && playerBall.position.x <= magnet.position.x - 1) {
        playerBall.speedx = -0.075;
      }

      else if (playerBall.position.y >= magnet.position.y - 0.25 && playerBall.position.y <= magnet.position.y + 0.25 && playerBall.position.x >= magnet.position.x + 1) {
        playerBall.speedx = +0.075;
      }

    }

    //Stick to Magnet
    if (detect_collision_mag(magnet.bounding_box(), playerBall.bounding_box())) {
      playerBall.speedx = 0;
      playerBall.speedy = 0;
    }

    //Dip in Pond
    if (detect_collision(pond.bounding_box(),playerBall.bounding_box()) && playerBall.position.x - 0.25 >= 1 && playerBall.position.x - 0.25 <= 3) {
      in_pond = 1;
      playerBall.speedy += 0.00015;
    }

    else {
      in_pond = 0;
    }


    //To reduce Horizontal speed after bouncing off the slopes on the balls
    if (playerBall.speedx < 0) {
      playerBall.speedx += 0.0015;
    }

    else if (playerBall.speedx > 0) {
      playerBall.speedx -= 0.0015;
    }


    //For bouncing off the Balls without slopes on top
    for (int i = 0; i < 25; i++) {

      int exists = 0;

      for (int j = 0; j < 7; j++) {
        if (i == slopePos[j]) {
          exists = 1;
          break;
        }
      }

      if (!exists) {

        if(detect_collision(balls[i].bounding_box(),playerBall.bounding_box()) && playerBall.speedy > 0){
          balls[i].position.x = randomPos(-15.0,-4.0);
          playerBall.speedy = -0.101;
          points += 10;
        }

        if (balls[i].position.x >= 4.5) {
          balls[i].position.x = randomPos(-15.0,-4.0);
        }

        balls[i].tick();
      }
    }


    //For Bouncing off balls and off the slopes on top of them
    for(int i = 0; i < 7; i++) {

      if(detect_collision(balls[slopePos[i]].bounding_box(),playerBall.bounding_box()) && playerBall.speedy > 0){

        tempPos = randomPos(-15.0,-4.0);
        balls[slopePos[i]].position.x = tempPos;

        if (angle[i] <= 60) {

          slopes[i].position.x = balls[slopePos[i]].position.x - 0.175 * cos((angle[i] * 3.147)/180);

        }

        else {

          slopes[i].position.x = balls[slopePos[i]].position.x + 0.175 * cos(((angle[i]-90) * 3.147)/180);

        }

        playerBall.speedy = -0.101;
        points += 20;

      }

      if (detect_collision(slopes[i].bounding_box(),playerBall.bounding_box()) && playerBall.speedy > 0) {

        if (angle[i] <= 60) {
          playerBall.speedx -= sqrt(pow(playerBall.speedx,2) + pow(playerBall.speedy,2))*cos(((90+angle[i])*3.147)/180);
          playerBall.speedy = -sqrt(pow(playerBall.speedx,2) + pow(playerBall.speedy,2))*sin(((90+angle[i])*3.147)/180);
        }

        else{
          playerBall.speedx += sqrt(pow(playerBall.speedx,2) + pow(playerBall.speedy,2))*cos(((angle[i])*3.147)/180);
          playerBall.speedy = -sqrt(pow(playerBall.speedx,2) + pow(playerBall.speedy,2))*sin(((angle[i])*3.147)/180);
        }

      }

      if (balls[slopePos[i]].position.x >= 4.5) {

        tempPos = randomPos(-15.0,-4.0);
        balls[slopePos[i]].position.x = tempPos;

        if (angle[i] <= 60) {

          slopes[i].position.x = balls[slopePos[i]].position.x - 0.175 * cos((angle[i] * 3.147)/180);

        }

        else {

          slopes[i].position.x = balls[slopePos[i]].position.x + 0.175 * cos(((angle[i]-90) * 3.147)/180);

        }

      }

      balls[slopePos[i]].tick();
      slopes[i].tick();
    }


    //For Bouncing off the floor of the Trampoline
    if(detect_collision_tramp(trampolineFloor.bounding_box(), playerBall.bounding_box()) && playerBall.speedy > 0){
      playerBall.speedy = -0.135;
    }


    //To avoid going below the floor provided no collision unless pond detected
    if(!(playerBall.position.x - 0.25 >= 1 && playerBall.position.x + 0.25 <= 3) && playerBall.position.y < floor_y){

      playerBall.position.y = floor_y;
      playerBall.speedy = 0.0;
      playerBall.speedx = 0.0;
      up_check = 0;
      playerBall.tick();

    }


    //To make the ball fall down after jumping until collision.
    if (up_check && !detect_collision(pond.bounding_box(), playerBall.bounding_box()) && !detect_collision_mag(magnet.bounding_box(), playerBall.bounding_box())) {
      playerBall.speedy += 0.004;
    }

    //To make the ball fall down into the pool with deceleration at pond surface
    if (up_check && detect_collision(pond.bounding_box(), playerBall.bounding_box()) && playerBall.position.x - 0.25 >= 1 && playerBall.position.x + 0.25 <= 3) {
      playerBall.speedy = 0;
      playerBall.speedy += 0.00015;
      up_check = 0;
    }

    //To stop the ball at the floor of the pond

    if (playerBall.position.x >= 1 && playerBall.position.x <= 2 && playerBall.position.y  <= - 2.35 + 0.25 - sqrt(1-pow(2-playerBall.position.x,2))) {
      playerBall.speedy = 0;
    }

    else if (playerBall.position.x >= 2 && playerBall.position.x <= 3 && playerBall.position.y  <= - 2.35 + 0.25 - sqrt(1-pow(playerBall.position.x-2,2))) {
      playerBall.speedy = 0;
    }

    char s[1000];
    sprintf(s, "Score: %d", points);
    glfwSetWindowTitle(window, s);

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models


    grassFloor = Rectangle(0, -1.225, 0.125,0.125, COLOR_GREEN);
    mudFloor = Rectangle(0, -1.4, 0.25, 2, COLOR_BROWN);

    trampolineFloor = Semicircle(-2,-1.5,0.5, COLOR_PURPLE);
    trampolineFloor.rotation = 181;
    trampolineLeg1 = Leg(-2.5, -1.5, COLOR_PURPLE);
    trampolineLeg2 = Leg(-1.5, -1.5, COLOR_PURPLE);

    spike1 = Spikes(-4, -2.325, COLOR_SILVER);
    spike2 = Spikes(6,-2.325,COLOR_SILVER);

    spike1.speedx = -0.02;
    spike2.speedx = 0.01;

    magnet = Magnet(1,2,COLOR_BRED);
    magnet.rotation = 180;

    pond = Semicircle(2, -2.325, 1, COLOR_LBLUE);
    pond.rotation = 181;


    playerBall = Ball(0, -2, 0.3, COLOR_RED);

    playerBall.speedx = 0;
    playerBall.speedy = 0;

    int slope_count = 0;
    int placeSlope;
    float slopeAngle;
    int randomizeAngle;

    for (int i = 0; i < 25; i++) {
      balls[i] = Ball(randomPos(-15.0,-4.0), randomPos(-1,3), 0.175, COLOR_YELLOW);
      placeSlope = rand() % 2;
      if (placeSlope && slope_count<7) {
        slopePos[slope_count] = i;
        randomizeAngle = rand() % 2;
        slopeAngle = (randomPos(5,60)*randomizeAngle) + (randomPos(120,175) * (1-randomizeAngle));


        //Making the slopes tangential to the balls

        if(randomizeAngle)
          slopes[slope_count] = Slope(balls[i].position.x - 0.175 * cos((slopeAngle * 3.147)/180), balls[i].position.y + 0.175, COLOR_CHOC);

        else
          slopes[slope_count] = Slope(balls[i].position.x + 0.175 * cos(((slopeAngle-90) * 3.147)/180), balls[i].position.y + 0.175, COLOR_CHOC);

        slopes[slope_count].rotation = slopeAngle;
        angle[slope_count] = slopeAngle;

        slopes[slope_count].speedx = -0.01;
        slope_count++;
      }
      balls[i].speedy = 0;
      balls[i].speedx = -0.01;
    }


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 800;
    int height = 800;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}



void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
