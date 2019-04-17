/***
 Assignment-2: Geometric Modeling of a Scene
 Name: Prochnow, Sophia
 Collaborators: Helped by looking at https://github.com/itsEmShoji/CMSI371-2018/blob/master/Generate_Scene/assignment2.cpp
 Project Summary: I started by copying over my assignment2 cube, camera, and multiplication matrices and changed the color matrices to different shades of grey. I then rotated, translated, and scaled the cube multiple times to different rectangles to create the sectional, ottoman, coffee table, and TV stand.
 ***/
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts degrees to radians for rotation
float deg2rad(float d) {
    return (d * M_PI) / 180.0;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < cartesian_coords.size(); i++) {
        result.push_back(cartesian_coords[i]);
        if ((i + 1) % 3 == 0) {
            result.push_back(1.0);
        }
    }
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogeneous_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < homogeneous_coords.size(); i++) {
        if ((i + 1) % 4 == 0) {
            continue;
        } else {
            result.push_back(homogeneous_coords[i]);
        }
    }
    return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate_mat = {
        1.0, 0.0, 0.0, dx,
        0.0, 1.0, 0.0, dy,
        0.0, 0.0, 1.0, dz,
        0.0, 0.0, 0.0, 1.0
    };
    
    return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat = {
        sx, 0.0, 0.0, 0.0,
        0.0, sy, 0.0, 0.0,
        0.0, 0.0, sz, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return scale_mat;
}

// Definition of a rotation matrix along the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    float theta_degrees = theta * (M_PI/180);
    vector<GLfloat> rotate_mat_x = {
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(theta_degrees), -sin(theta_degrees), 0.0,
        0.0, sin(theta_degrees), cos(theta_degrees), 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    return rotate_mat_x;
}


// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    float theta_degrees = theta * (M_PI/180);
    vector<GLfloat> rotate_mat_y = {
        cos(theta_degrees), 0.0, sin(theta_degrees), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(theta_degrees), 0.0, cos(theta_degrees),
        0.0, 0.0, 0.0, 0.0, 1.0
    };
    
    return rotate_mat_y;
}


// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    float theta_degrees = theta * (M_PI/180);
    vector<GLfloat> rotate_mat_z = {
        cos(theta_degrees), -sin(theta_degrees), 0.0, 0.0,
        sin(theta_degrees), cos(theta_degrees), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    return rotate_mat_z;
}


// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    
    B = to_homogenous_coord(B);
    
    for (int i = 0; i < B.size(); i += 4) {
        for (int j = 0; j < 16; j += 4) {
            result.push_back((A[j + 0] * B[i + 0]) + (A[j + 1] * B[i + 1]) + (A[j + 2] * B[i + 2]) + (A[j + 3] * B[i + 3]));
        }
    }
    result = to_cartesian_coord(result);
    return result;
}

vector<GLfloat> vector_concat(vector<vector<GLfloat> > v) {
    vector<GLfloat> result;
    for (int i = 0; i < v.size(); i++) {
        result.insert(result.end(), v[i].begin(), v[i].end());
    }
    return result;
}

// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> front = mat_mult(translation_matrix(0, 0, 0.5), init_plane());
    vector<GLfloat> back = mat_mult(translation_matrix(0, 0, -0.5), init_plane());
    vector<GLfloat> left = mat_mult(translation_matrix(0.5, 0, 0), mat_mult( rotation_matrix_y(-90.0), init_plane()));
    vector<GLfloat> right = mat_mult(translation_matrix(-0.5, 0, 0), mat_mult(rotation_matrix_y(90.0), init_plane()));
    vector<GLfloat> top = mat_mult(translation_matrix(0, -0.5, 0), mat_mult(rotation_matrix_x(-90.0), init_plane()));;
    vector<GLfloat> bottom = mat_mult(translation_matrix(0, 0.5, 0), mat_mult(rotation_matrix_x(90.0), init_plane()));;
    
    vector<vector<GLfloat>> result = {front, back, left, right, top, bottom};
    return vector_concat(result);
}

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by using setting *
 *  the projection viewing matrices               *
 *                                                *
 *************************************************/

float theta = 0.0;
int planes = 0;

void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    // Set up brown background (changed to make furniture clear)
    glClearColor(0.6, 0.4, 0.0, 0.0);
}

void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, 1.0, 2.0, 10.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(0.1, 1.5, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

vector<GLfloat> create_tvStand() {
    vector<GLfloat> top = mat_mult(translation_matrix(0.8, 0.8, 6.45), mat_mult(scaling_matrix(1.2, 0.2, 1), build_cube()));
    vector<GLfloat> legFL = mat_mult(translation_matrix(0.4, 0.2, 6.85), mat_mult(scaling_matrix(0.05, 1, 0.05), build_cube()));
    vector<GLfloat> legBL = mat_mult(translation_matrix(0.9, 0, 0), legFL);
    vector<GLfloat> legBR = mat_mult(translation_matrix(0, 0, -0.85), legFL);
    vector<GLfloat> legFR = mat_mult(translation_matrix(0, 0, -0.85), legBL);
    vector<GLfloat> legconnectR = mat_mult(translation_matrix(0, -0.52, -0.45), mat_mult(scaling_matrix(1, 1, 0.85), mat_mult(rotation_matrix_x(-90.0),legFR)));
    vector<GLfloat> legconnectL = mat_mult(translation_matrix(-0.9, 0, 0), legconnectR);
    vector<vector<GLfloat>> tvStand_parts = {top, legFL, legBL, legBR, legFR, legconnectR, legconnectL};
    
    vector<GLfloat> tvStand = vector_concat(tvStand_parts);
    tvStand = mat_mult(translation_matrix(-1, -0.2, -1), mat_mult(scaling_matrix(0.5, 0.5, 0.5), tvStand));
    planes = (int)tvStand_parts.size() * 6;
    return tvStand;
}

vector<GLfloat> create_coffeeTable() {
    vector<GLfloat> top = mat_mult(translation_matrix(0.3, 0.30, 0.55), mat_mult(scaling_matrix(1.75, 0.12, 1), build_cube()));
    
    vector<GLfloat> legFL = mat_mult(translation_matrix(-0.45, -0.25, 1), mat_mult(scaling_matrix(0.05, 1, 0.05), build_cube()));
    
    vector<GLfloat> legBL = mat_mult(translation_matrix(1.5, .1, 0), legFL);
    
    vector<GLfloat> legBR = mat_mult(translation_matrix(0, 0, -0.85), legFL);
    
    vector<GLfloat> legFR = mat_mult(translation_matrix(0, 0, -0.85), legBL);
    
    vector<GLfloat> legconnectR = mat_mult(translation_matrix(0, -0.65, 0.5), mat_mult(scaling_matrix(1, 0.55, 0.85), mat_mult(rotation_matrix_x(-90.0),legFR)));
    
    vector<GLfloat> legconnectL = mat_mult(translation_matrix(-1.5, 0, 0), legconnectR);
    
    vector<GLfloat> legconnectF = mat_mult(translation_matrix(-0.8, 0, 0.56), mat_mult(scaling_matrix(1.75, 1, 1), mat_mult(rotation_matrix_y(90.0), legconnectL)));
    
    vector<GLfloat> legconnectB = mat_mult(translation_matrix(0, 0, -0.85),legconnectF);
    
    vector<vector<GLfloat>> coffeeTable_parts = {top, legFL, legBL, legBR, legFR, legconnectR, legconnectL, legconnectB ,legconnectF};
    
    vector<GLfloat> coffeeTable = vector_concat(coffeeTable_parts);
    coffeeTable = mat_mult(translation_matrix(0, 0, 0.5), mat_mult(scaling_matrix(0.5, 0.5, 0.5), coffeeTable));
    
    planes = planes + (int)coffeeTable_parts.size() * 6;
    return coffeeTable;
}

vector<GLfloat> create_carpet() {
    vector<GLfloat> carpet = mat_mult(translation_matrix(0.2, -0.29, 0.8), mat_mult(scaling_matrix(3.5, 0.01, 3), build_cube()));
    planes = planes + 6;
    return carpet;
}

vector<GLfloat> create_couch() {
    vector<GLfloat> base = mat_mult(translation_matrix(0.3, -0.2, -0.8), mat_mult(scaling_matrix(2.6, 0.25, 0.75), build_cube()));
    vector<GLfloat> base2 = mat_mult(translation_matrix(2.0, 0, 0), mat_mult(scaling_matrix(1.0, 1, 0.6), mat_mult(rotation_matrix_y(90), base)));
    vector<GLfloat> back = mat_mult(translation_matrix(0, 0.5, -1.2), mat_mult(scaling_matrix(1, 0.5, 0.5), mat_mult(rotation_matrix_x(-90.0), base)));
    vector<GLfloat> back2 = mat_mult(translation_matrix(2.7, 0, -0.1), mat_mult(scaling_matrix(1.0, 1, 0.65), mat_mult(rotation_matrix_y(90), back)));
    vector<GLfloat> cushionR = mat_mult(translation_matrix(0.45, 0.1, -0.09), mat_mult(scaling_matrix(0.65, 0.5, 0.8), base));
    vector<GLfloat> cushionL = mat_mult(translation_matrix(-0.65, 0, 0), cushionR);
    vector<GLfloat> armrestR = mat_mult(translation_matrix(-0.9, -0.15, 0), mat_mult(rotation_matrix_z(90.0), mat_mult(scaling_matrix(0.25, 0.65, 1), cushionR)));
    vector<GLfloat> armrestL = mat_mult(translation_matrix(2, 0, -0.3), mat_mult(rotation_matrix_y(90), armrestR)  );
    vector<GLfloat> pillowR = mat_mult(translation_matrix(0.25, 0.75, -1.06), mat_mult(scaling_matrix(0.88, 0.65, 0.5), mat_mult(rotation_matrix_x(-100.0), cushionR)));
    vector<GLfloat> pillowM = mat_mult(translation_matrix(-0.95, 0, 0), pillowR);
    vector<GLfloat> backCushion2 = mat_mult(rotation_matrix_y(90), mat_mult( translation_matrix(-0.1, 0.1, 2), mat_mult(scaling_matrix(0.4, 0.5, 0.8), base)));
    vector<GLfloat> topCushion2 = mat_mult(translation_matrix(0.5, 0, -1), mat_mult(rotation_matrix_y(-90), pillowR));
    
    vector<vector<GLfloat>> couch_parts = {base, back, cushionR, cushionL, armrestR, armrestL, pillowR, pillowM, base2, back2, backCushion2, topCushion2};
    vector<GLfloat> couch = vector_concat(couch_parts);
    planes = planes + (int)couch_parts.size() * 6;
    return couch;
}

vector<GLfloat> create_ottoman() {
    vector<GLfloat> base = mat_mult(translation_matrix(-0.5, 0, 0.5), mat_mult(scaling_matrix(0.5, 0.3, 0.5), build_cube()));
    vector<GLfloat> top = mat_mult(translation_matrix(-0.5, 0.24, 0.5), mat_mult(scaling_matrix(0.5, 0.15, 0.5), build_cube()));
    
    vector<vector<GLfloat>> ottoman_parts = {base, top};
    vector<GLfloat> ottoman = vector_concat(ottoman_parts);
    ottoman = mat_mult(translation_matrix(0.8, -0.2, -0.5), ottoman);
    planes = planes + (int)ottoman_parts.size() * 6;
    return ottoman;
}

// Construct the scene using objects built from cubes/prisms
GLfloat* init_scene() {
    vector<vector<GLfloat>> scene = {create_ottoman(), create_tvStand(), create_coffeeTable(), create_carpet(), create_couch()};
    return vector2array(vector_concat(scene));
}

// Construct the color mapping of the scene
GLfloat* init_color() {
    
    vector<GLfloat> lightGrey = {
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,

        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,

        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,

        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,

        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,

        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
        0.55, 0.55, 0.55,
    };
    
    vector<GLfloat> darkGrey = {
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
        0.4, 0.4, 0.4,
    };
    
    vector<GLfloat> white = {
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        // Back plane
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        // Right
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        // Left
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        // Top
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        // Bottom
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
    };
    
    vector<GLfloat> greyOmbre = {
        // Front plane
        0.5,    0.5,    0.5,
        0.3,    0.4,    0.5,
        0.5,    0.5,    0.5,
        0.8,    0.8,    0.8,
        // Back plane
        0.5,    0.5,    0.5,
        0.3,    0.4,    0.5,
        0.5,    0.5,    0.5,
        0.8,    0.8,    0.8,
        // Right
        0.5,    0.5,    0.5,
        0.3,    0.4,    0.5,
        0.5,    0.5,    0.5,
        0.8,    0.8,    0.8,
        // Left
        0.5,    0.5,    0.5,
        0.3,    0.4,    0.5,
        0.5,    0.5,    0.5,
        0.8,    0.8,    0.8,
        // Top
        0.5,    0.5,    0.5,
        0.3,    0.4,    0.5,
        0.5,    0.5,    0.5,
        0.8,    0.8,    0.8,
        // Bottom
        0.5,    0.5,    0.5,
        0.3,    0.4,    0.5,
        0.5,    0.5,    0.5,
        0.8,    0.8,    0.8,
    };
    
    vector<GLfloat> grey = {
        // Front plane
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        // Back plane
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        // Right
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        // Left
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        // Top
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        // Bottom
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
        0.5,    0.5,    0.5,
    };
    

    
    vector<vector<GLfloat>> scene_colors = {
        //ottoman
        darkGrey, grey,
        //small table
        grey, grey, grey, grey, grey, grey, grey,
        //coffee table
        lightGrey, grey, grey, grey, grey, grey, grey, grey, grey,
        //carpet
        greyOmbre,
        //couch
        darkGrey, darkGrey, lightGrey, lightGrey, darkGrey, darkGrey, grey, grey, darkGrey, darkGrey, lightGrey, grey};
    return vector2array(vector_concat(scene_colors));
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // World model parameters
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glRotatef(theta, 0.0, 1.0, 0.0);
    //    glRotatef(theta, 1.0, 0.0, 0.0);
    
    GLfloat* vertices = init_scene();
    GLfloat* colors = init_color();
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    vertices);  // Pointer to memory location to read from
    
    //pass the color pointer
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   colors);     // Pointer to memory location to read from
    
    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, 4 * (planes + 12));
    
    glFlush();            //Finish rendering
    glutSwapBuffers();
    
    free(vertices);
    free(colors);
}

void idle_func() {
    theta = theta + 0.3;
    display_func();
}

int main (int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Vertex Arrays");
    
    setup();
    init_camera();
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    return 0;
}
