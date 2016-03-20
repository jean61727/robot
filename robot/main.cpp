//
//  main.cpp
//  robot
//
//  Created by jeanlee on 3/12/16.
//  Copyright © 2016 jeanlee. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <cstdio>
#include <cstdlib>
#include "texture_loader.h"

#define MENU_TIMER_START 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3
#define MENU_ANIM 4
#define MENU_STOP 5
#define DIR_ROTATE_ANGLE 5
#define DIR_TRANSLATE_ANGLE 0.1


const GLubyte robot_head[] = {255,255, 255};
const GLubyte robot_torso_up[] = {255,199, 240};
const GLubyte robot_torso_down[]={255,199, 240};
const GLubyte robot_neck[] = {255,255, 255};
const GLubyte robot_arm_joint1[] = {0, 0, 0};
const GLubyte robot_arm_joint2[] = {255,199, 240};
const GLubyte robot_arm_arm1[] = {0, 0, 0};
const GLubyte robot_arm_arm2[] = {255,199, 240};
const GLubyte robot_eyes[] = {0,0,0};
const GLubyte robot_leg[] = {255,199, 240};

GLUquadricObj *disk=gluNewQuadric();
GLUquadricObj *neck=gluNewQuadric();
GLUquadricObj *torso_1=gluNewQuadric();
GLUquadricObj *cylinder=gluNewQuadric();

//animation control parts
struct para_anim{
    
    GLfloat degree = 0;
    GLint rotate_enable = 0;
    
}head_x, head_y ,left_leg_thigh, right_leg_thigh, right_arm_joint1, right_arm_joint2,left_arm_joint1,left_arm_joint2,torso_down,torso_up,neck_anim;

// initial parameters flags and counters
GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;
int anim_flag=0;
int flag=0; // control animation in head movements

// Stages of directions and animation define
GLfloat xRotated, yRotated, zRotated, xTranslate, yTranslate, zTranslate,angle;
enum Input_direction{still,up, down, right, left,trans_up,trans_left,trans_right,trans_down};
enum anim_set{nod,wave_right,wave_right2};
// initial direction and positions
Input_direction dir = still;
int anim_stage = nod;


GLuint texture_jpg;
GLuint texture_png;


// Print OpenGL context related information.
void dumpInfo(void){
    
    printf("Vendor: %s\n", glGetString (GL_VENDOR));
    printf("Renderer: %s\n", glGetString (GL_RENDERER));
    printf("Version: %s\n", glGetString (GL_VERSION));
    printf("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}
/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(138.0f/255.0f, 226.0f/255.0f, 255.0f/255.0f, 1.0f);//light BLue
    //glClearColor(255.0f/255.0f, 199.0f/255.0f, 240.0f/255.0f, 0.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
/*The initialization of all the parameters controlling the robot rotation and translation */
void init_all_parameters(){
    
    head_x.rotate_enable=0;
    head_y.rotate_enable=0;
    head_x.degree=0;
    head_y.degree=0;
    
    left_leg_thigh.rotate_enable=0;
    left_leg_thigh.degree=10;
    
    right_leg_thigh.rotate_enable=0;
    right_leg_thigh.degree=10;
    
    right_arm_joint1.rotate_enable=0;
    right_arm_joint1.degree=0;
    right_arm_joint2.rotate_enable=0;
    right_arm_joint2.degree=0;
    left_arm_joint1.rotate_enable=0;
    left_arm_joint1.degree=0;
    left_arm_joint2.rotate_enable=0;
    left_arm_joint2.degree=0;
    
    torso_down.rotate_enable=0;
    torso_down.degree=-0.26;
    
    torso_up.rotate_enable=0;
    torso_up.degree=0.25;
    neck_anim.rotate_enable=0;
    neck_anim.degree=0.3;
    
    if(anim_flag==1){
        left_leg_thigh.rotate_enable=1;
        right_leg_thigh.rotate_enable=1;
        right_leg_thigh.degree=50;
        left_leg_thigh.degree=-10;
        
        head_x.rotate_enable=1;
        head_y.rotate_enable=1;
        
        right_arm_joint1.rotate_enable=1;
        right_arm_joint2.rotate_enable=1;
        left_arm_joint1.rotate_enable=1;
        left_arm_joint2.rotate_enable=1;
        
        torso_down.rotate_enable=1;
        torso_down.degree=-0.26;
        
        torso_up.rotate_enable=1;
        torso_up.degree=0.25;
        
        neck_anim.rotate_enable=0;
        neck_anim.degree=0.3;
        
    }
    

}
/*All the function that defines the appearance of the robot*/
void Draw_eyes(){
    
    glPushMatrix();
    
    glPushMatrix();
    glColor3ubv(robot_eyes);
    glTranslatef( 0 ,0, 0 );
    glTranslatef( -0.25 ,1, 0.2);
    //glRotatef(90,1,0,0);
    glScalef( 1, 1, 1 );
    glutSolidSphere(0.43,50,50);
    glPopMatrix();
    
    glPushMatrix();
    glColor3ubv(robot_eyes);
    glTranslatef( 0 ,0, 0 );
    glTranslatef( 0.25 ,1, 0.2);
    //glRotatef(90,1,0,0);
    glScalef( 1, 1, 1 );
    glutSolidSphere(0.43,50,50);
    glPopMatrix();
    
    
    
    glPopMatrix();
}
void Draw_head(){
    
    
    
    glPushMatrix();

    
    glColor3ubv(robot_neck);
    //glTranslatef( 0,0.3,0);
    glTranslatef( 0,neck_anim.degree,0);
    glRotatef(-90,1,0,0);
    gluCylinder(neck, 0.4, 0.25, 0.07, 50,50);// (*quad,base,top,height,slices,slices,stacks)
    
    //gluCylinder(torso_2, 0.4, 0.25, 0.07, 50,50);
    //gluCylinder(torso_3, 0.4, 0.25, 0.07, 50,50);
    //gluDisk(disk, 0, 0.1, 50, 50);
    
    //glTranslatef( 0 ,0.15, 0 );
    //glRotatef(-90,1,0,0);
    //glScalef( 0.03, 0.03, 0.01 );
    //glutSolidTorus(2.5, 4,50,50);
    glPopMatrix();


    glPushMatrix();
    glRotatef(head_x.degree,1,0,0);
    glRotatef(head_y.degree,0,1,0);
    Draw_eyes();
    glColor3ubv(robot_head);
    glTranslatef( 0 ,1, 0 );
    glScalef( 1.05, 1, 1.05 );

    glutSolidSphere(0.7,50,50);

    glPopMatrix();


}
void Draw_neck(){}
void Draw_torso(){
    

    
    glPushMatrix();
    
    glColor3ubv(robot_neck);
    //glTranslatef( 0,0.3,0);
    glTranslatef( 0,neck_anim.degree,0);
    glRotatef(-90,1,0,0);
    gluCylinder(neck, 0.4, 0.25, 0.07, 50,50);
    glRotatef(90,1,0,0);
    
    glColor3ubv(robot_torso_up);
    //glTranslatef( 0,0.25,0);
    glTranslatef( 0,-0.05,0);
    glRotatef(-90,1,0,0);
    gluCylinder(torso_1, 0.37, 0.4, 0.05, 50,50);
    glRotatef(90,1,0,0);
    
    glTranslatef( 0,-0.05,0);
    glRotatef(-90,1,0,0);
    gluCylinder(torso_1, 0.34,0.37, 0.05, 50,50);
    glRotatef(90,1,0,0);
    
    glTranslatef( 0,-0.05,0);
    glRotatef(-90,1,0,0);
    gluCylinder(torso_1, 0.31,0.34, 0.05, 50,50);
    glRotatef(90,1,0,0);
    
    glTranslatef( 0,-0.05,0);
    glRotatef(-90,1,0,0);
    gluCylinder(torso_1, 0.28,0.31, 0.05, 50,50);
    glRotatef(90,1,0,0);
    
    glTranslatef( 0,-0.05,0);
    glRotatef(-90,1,0,0);
    gluCylinder(torso_1, 0.25,0.28, 0.05, 50,50);
    glRotatef(90,1,0,0);
    
    glTranslatef( 0,-0.1,0);
    glRotatef(-90,1,0,0);
    gluCylinder(torso_1, 0.27,0.25, 0.1, 50,50);
    gluDisk(disk, 0, 0.27, 50, 50);
    glRotatef(90,1,0,0);
    //glRotatef(90,1,0,0);
    
    //glRotatef(90,1,0,0);
    
   
    
    glPopMatrix();

}
void Draw_finger(GLfloat angle1, GLfloat angle2,GLfloat axis_x ,GLfloat axis_y,GLfloat axis_z,GLfloat x,GLfloat y, GLfloat z){
    
    glPushMatrix();
    glColor3ub(0, 0, 0);
    glTranslatef(x, y, z);
    //glutSolidCube(0.1);
    
    glPushMatrix();
    glRotatef(angle1, axis_x, axis_y, axis_z);
    glScalef(1.0f, 4.0f, 1.0f);
    glutSolidCube(0.05);
    
    glScalef(1.0f, 0.7f, 1.0f);
    glTranslatef(0,-0.04,0);
    glutSolidCube(0.03);
    
    glTranslatef(0,-0.03,0);
    glutSolidCube(0.04);
    
    glPopMatrix();
    
    /*glRotatef(angle1, axis_x, axis_y, axis_z);
    glTranslatef(0.3f, 0, 0);
    glRotatef(angle2, 0, 1, 0);
    glTranslatef(-0.04f, 0.0f, 0.0f);
    glScalef(2.0f, 1.0f, 1.0f);
    glutSolidSphere(0.2, 50, 50);
    
    
    glTranslatef(0.1f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    //glRotatef(50, 0, 0, 1);
    glScalef(1.0f, 1.0f, 1.0f);
    glutSolidSphere(0.15, 50, 50);*/
    
    glPopMatrix();
}
void Draw_Arm(){
    //angle=angle+1;

    //left-arm-1 joint
    glPushMatrix();
        glColor3ubv(robot_arm_joint1);
        glTranslatef( -0.3,0.12, 0 );
        glRotatef(90,0,1,0);
        glRotatef(-23,1,0,0);
        glScalef( 0.5, 0.5, 1 );
        glutSolidTorus(0.1,0.18,50,50);
       // glScalef( 1, 2, 2 );
    
    //left-arm-2 joint
        glColor3ubv(robot_arm_joint2);
        glTranslatef(0,0,-0.1);
        glutSolidTorus(0.1,0.13,50,50);
        glRotatef(-90,0,1,0);
        glRotatef(23,1,0,0);
        glScalef( 1, 2, 2 );
   //left-arm-3 arm-1
        glRotatef(90,0,1,0);
        glRotatef(-23,1,0,0);
        glColor3ubv(robot_arm_arm1);
        glTranslatef(0,0.03,-0.05);
        glRotatef(left_arm_joint1.degree,0,1,0);
        glutSolidSphere(0.05,50,50);
    
    //left-arm-3 arm-2
        glPushMatrix();
            glTranslatef(0,0,-0.1);
            //glRotatef(angle,0,1,0);
            //glRotatef(-50,1,0,0);
            glScalef(1,1,3);
            glutSolidCube(0.4/5);
            glScalef(1,1,1.0/3);
    
    // left-arm-4 hand-1
            glColor3ubv(robot_arm_arm2);
            glTranslatef(0,0,-0.16);
            //glRotatef(10,1,0,0);
            glutSolidSphere(0.06,50,50);
    // left-arm-4 hand-2
            glTranslatef(0,0,0.02);
            glScalef(1,1,0.5);
            glutSolidTorus(0.04,0.04,50,50);
            glScalef(0.7,1,2);
            glTranslatef(0,0,-0.2);
    
            gluCylinder(cylinder, 0.255, 0.08, 0.2, 50, 50);
            glTranslatef(0,0,-0.05);
            glRotatef(-5,1,0,0);
            glScalef(1,1,2);
            glutSolidTorus(0.04,0.24,50,50);
            glScalef(1,1,1.0/2);
    
            glTranslatef(0,0,-0.1);
            gluCylinder(cylinder, 0.31, 0.275, 0.1, 50, 50);
    
            glTranslatef(0,0,-0.4);
            gluCylinder(cylinder, 0.15, 0.31, 0.4, 50, 50);
            gluDisk(disk, 0, 0.15, 50, 50);
            glScalef(1/0.7,1,1.0/2);
            Draw_finger(90, 0, 1.0,0.0,0.0,0,-0.05, -0.1);
            Draw_finger(90, 0, 1.0,0.0,0.0,0,0.05, -0.1);
            //Draw_finger(90, 0,0.0,1.0,0.0,0,-0.2, 0.5);
            //Draw_finger(0, 0,0.0,0.0,1.0, 0,-1, 0);

        glPopMatrix();
    
    
    glPopMatrix();

}
void Draw_Right_Arm(){
    angle=angle+1;
    
    //left-arm-1 joint
    glPushMatrix();
    glColor3ubv(robot_arm_joint1);
    glTranslatef( 0.3,0.12, 0 );
    glRotatef(90,0,1,0);
    glRotatef(23,1,0,0);
    glScalef( 0.5, 0.5, 1 );
    glutSolidTorus(0.1,0.18,50,50);
    // glScalef( 1, 2, 2 );
    
    //left-arm-2 joint
    glColor3ubv(robot_arm_joint2);
    glTranslatef(0,0,0.1);
    glutSolidTorus(0.1,0.13,50,50);
    
    glRotatef(90,0,1,0);
    glRotatef(23,1,0,0);
    glScalef( 1, 2, 2 );
    //left-arm-3 arm-1
    glRotatef(90,0,1,0);
    glRotatef(-23,1,0,0);
    glColor3ubv(robot_arm_arm1);
    glTranslatef(0,0.03,-0.05);
    glRotatef(right_arm_joint1.degree,1*right_arm_joint1.rotate_enable,0,0);
    glutSolidSphere(0.05,50,50);
    
    //left-arm-3 arm-2
    glPushMatrix();
    glTranslatef(0,0,-0.1);
    //glRotatef(angle,0,1,0);
    //glRotatef(-50,1,0,0);

    glScalef(1,1,3);
    glutSolidCube(0.4/5);
    glScalef(1,1,1.0/3);
    
    

    
    // left-arm-4 hand-1
    glColor3ubv(robot_arm_arm2);
    glTranslatef(0,0,-0.16);
    //glRotatef(10,1,0,0);
    glutSolidSphere(0.06,50,50);
    // left-arm-4 hand-2
    glTranslatef(0,0,0.02);
    glRotatef(right_arm_joint2.degree,1*right_arm_joint2.rotate_enable,0,0);
    glScalef(1,1,0.5);
    glutSolidTorus(0.04,0.04,50,50);
    glScalef(0.7,1,2);
    glTranslatef(0,0,-0.2);
    
    gluCylinder(cylinder, 0.255, 0.08, 0.2, 50, 50);
    glTranslatef(0,0,-0.05);
    glRotatef(-5,1,0,0);
    glScalef(1,1,2);
    glutSolidTorus(0.04,0.24,50,50);
    glScalef(1,1,1.0/2);
    
    glTranslatef(0,0,-0.1);
    gluCylinder(cylinder, 0.31, 0.275, 0.1, 50, 50);
    
    glTranslatef(0,0,-0.4);
    gluCylinder(cylinder, 0.15, 0.31, 0.4, 50, 50);
    gluDisk(disk, 0, 0.15, 50, 50);
    glScalef(1/0.7,1,1.0/2);
    Draw_finger(90, 0, 1.0,0.0,0.0,0,-0.05, -0.1);
    Draw_finger(90, 0, 1.0,0.0,0.0,0,0.05, -0.1);
    //Draw_finger(90, 0,0.0,1.0,0.0,0,-0.2, 0.5);
    //Draw_finger(0, 0,0.0,0.0,1.0, 0,-1, 0);
    
    glPopMatrix();
    
    
    glPopMatrix();
}
void Draw_Leg(){
    //left-leg-1 joint
    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef( -0.18,-0.45,0.1 );
    
    glRotatef(55,1,0,0);
    glRotatef(-35,0,1,0);
    glScalef(1,1,1.25 );
    glutSolidTorus(0.03,0.1,50,50);
    glScalef(1,1,1/1.25 );
    //left-leg-1 leg
    //glTranslatef( 0,-0.1,0);
    //glRotatef(10,1,0,0);
    glRotatef(left_leg_thigh.degree,1*left_leg_thigh.rotate_enable,0,0);
    glTranslatef( 0,0.0,-0.05);
    gluCylinder(torso_1, 0.1,0.1, 0.2, 50,50);
    glTranslatef( 0,0,0.13);
    glRotatef(10,1,0,0);
    gluCylinder(torso_1, 0.1,0.13, 0.13, 50,50);
    
    //left-leg-1 leg
    glColor3ubv(robot_leg);
    glTranslatef(0,0,0.13);
    glRotatef(5,0,1,0);
    glutSolidTorus(0.05, 0.12,50,50);
    
    //glTranslatef(0.05,-0.05,0);
    glRotatef(10,0,1,0);
    glRotatef(8,1,0,0);
    gluCylinder(torso_1, 0.17,0.26, 0.32, 50,50);
    glTranslatef(0,0,0.32);
     gluDisk(disk, 0, 0.26, 50, 50);
    glRotatef(3,1,0,0);
    glutSolidTorus(0.05, 0.21,50,50);
    glRotatef(10,0,1,0);
    glRotatef(10,1,0,0);
    gluCylinder(torso_1, 0.17,0.3, 0.16, 50,50);
    glTranslatef(0,0,0.16);
    gluDisk(disk, 0, 0.3, 50, 50);
 

    glPopMatrix();
    
}
void Draw_Right_Leg(){
    //left-leg-1 joint
    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef( 0.18,-0.45,0.1 );
    
    glRotatef(55,1,0,0);
    glRotatef(35,0,1,0);
    glScalef(1,1,1.25 );
    glutSolidTorus(0.03,0.1,50,50);
    glScalef(1,1,1/1.25 );
    //left-leg-1 leg
    //glTranslatef( 0,-0.1,0);
    glRotatef(right_leg_thigh.degree,1*right_leg_thigh.rotate_enable,0,0);
    glTranslatef( 0,0.0,-0.05);
    gluCylinder(torso_1, 0.1,0.1, 0.2, 50,50);
    glTranslatef( 0,0,0.13);
    glRotatef(10,1,0,0);
    gluCylinder(torso_1, 0.1,0.13, 0.13, 50,50);
    
    //left-leg-1 leg
    glColor3ubv(robot_leg);
    glTranslatef(0,0,0.13);
    glRotatef(-5,0,1,0);
    glutSolidTorus(0.05, 0.12,50,50);
    
    //glTranslatef(0.05,-0.05,0);
    glRotatef(-10,0,1,0);
    glRotatef(8,1,0,0);
    gluCylinder(torso_1, 0.17,0.26, 0.32, 50,50);
    glTranslatef(0,0,0.32);
    gluDisk(disk, 0, 0.26, 50, 50);
    glRotatef(3,1,0,0);
    glutSolidTorus(0.05, 0.21,50,50);
    glRotatef(-10,0,1,0);
    glRotatef(10,1,0,0);
    gluCylinder(torso_1, 0.17,0.3, 0.16, 50,50);
    glTranslatef(0,0,0.16);
    gluDisk(disk, 0, 0.3, 50, 50);
    glTranslatef(0,0.05,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_png);
    glBegin(GL_QUADS);
    {
        glNormal3f(0, 0, 1);
        //glColor3f(1, 1, 1);
        glTexCoord2f(1.05, 1.01); glVertex3f(-0.18f, 0.15f, 0.01f);
        glTexCoord2f(1.05, 0.01); glVertex3f(-0.18f, -0.25f, 0.01f);
        glTexCoord2f(2.05, 0.01); glVertex3f(0.22f, -0.25f, 0.01f);
        glTexCoord2f(2.05, 1.01); glVertex3f(0.22f, 0.15f, 0.01f);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}
void Draw_cube(){
    
    glPushMatrix();
    glColor3ub(0,0,0);
    glTranslatef( 0 ,-0.2, 0 );
    glRotatef(-90,1,0,0);
    gluCylinder(cylinder, 0.2, 0.2, 0.3, 50,50);
    glRotatef(90,1,0,0);
    glPopMatrix();
}
void Draw_under_torso(){
   
    glPushMatrix();
    glColor3ub(255,255,255);
    /*glTranslatef( 0 ,-0.28, 0 );
    glRotatef(0,0,0,1);
    glScalef( 0.25, 0.15, 0.13 );
    //glutSolidDodecahedron();
    glutSolidIcosahedron();*/
    
    //glTranslatef( 0,-0.26,0);
    glTranslatef( 0,torso_down.degree,0);
    glRotatef(-90,1,0,0);
    glScalef( 1, 1, 1.25 );
    glutSolidTorus(0.1, 0.25,50,50);
    
    glTranslatef( 0,0,-0.2);
    gluCylinder(torso_1, 0.15, 0.3, 0.12, 50,50);
    //glRotatef(90,1,0,0);
    glTranslatef( 0,0,0.13);
    glScalef( 1, 1, 1/1.25 );
    glutSolidSphere(0.22,50,50);
    
    glPopMatrix();
        
    
}
void Draw_Robot(){

    
    Draw_head();
    //Draw_eyes();
    //Draw_neck();
    Draw_torso();
    //Left_arm
    Draw_Arm();
    //torso joint
    Draw_cube();
    Draw_under_torso();
    //Draw_Leg
    Draw_Leg();
    Draw_Right_Leg();
    Draw_Right_Arm();


}

/*set the rotation boundary of each animation */
para_anim set_boundary(GLfloat upper_bound, GLfloat lower_bound, para_anim component, GLfloat speed){
    
    if(component.degree <= lower_bound || component.rotate_enable == 1){
        component.degree += speed;
        component.rotate_enable = 1;
        if( component.degree> upper_bound){
            component.rotate_enable = 0;
        }
    }
    else{
        component.degree -= speed;
        
    }
    
    return component;
}
void animation(){
    
    if(anim_flag!=1){
    
    }
    
    if(anim_flag==1){
        
        left_leg_thigh=set_boundary(50, -10, left_leg_thigh, 1);
        right_leg_thigh=set_boundary(50, -10, right_leg_thigh, 1);
        
        left_arm_joint1= set_boundary(50, -10, left_arm_joint1, 1);
        //torso_up= set_boundary( 0.28, 0.24, torso_up, 0.0006);
        torso_down= set_boundary( -0.24, -0.28, torso_down, 0.0006);
        neck_anim= set_boundary( 0.31, 0.27, neck_anim, 0.0006);
        //printf("%d\n",left_arm_joint1.rotate_enable);
        
        switch (anim_stage) {
            case nod: // rotating head
                head_x=set_boundary(15, 0, head_x, 0.5);
                if(head_x.degree==0){
                    anim_stage++;
                }
                break;
                
            case wave_right:
                right_arm_joint1=set_boundary(60, 0, right_arm_joint1, 0.5);
                if(right_arm_joint1.degree==60){
                    anim_stage++;
                }
                break;
            case wave_right2:
                right_arm_joint2 = set_boundary(30, 0, right_arm_joint2, 0.5);
                if(flag==0){
                    head_x=set_boundary(15, 0, head_x, 0.5);
                    
                }else{
                     head_y=set_boundary(30, -30, head_y, 0.5);
                }
                if(head_x.degree<=0 && flag==0 && head_x.rotate_enable==0){
                    flag=1;
                
                }else if(head_y.degree<=-30 && flag==1 && head_y.rotate_enable==0){
                    flag=0;

                }


                break;
                
            default:
                break;
        }
        
       

        
        
        
    }
    
}

// GLUT callback. Called to draw the scene.
void My_Display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// clear color buffer, depth buffer
    
    glLoadIdentity();
    glTranslatef(0, 0, -4);
    glTranslatef(xTranslate, yTranslate, zTranslate);
    glRotatef(yRotated, 0.0f, 1.0f, 0.0f); // Rotate around the y and z axis.
    glRotatef(xRotated, 1.0f, 0.0f, 0.0f);
    
    animation();
    Draw_Robot();

    
    glutSwapBuffers();
}
void direction_move(void){
    
    switch (dir) {
        case up:
            xRotated += DIR_ROTATE_ANGLE;
            break;
        case down:
            xRotated -= DIR_ROTATE_ANGLE;
            break;
        case left:
            yRotated -= DIR_ROTATE_ANGLE;
            break;
        case right:
            yRotated += DIR_ROTATE_ANGLE;
            break;
        case trans_up:
            yTranslate += DIR_TRANSLATE_ANGLE;
            break;
        case trans_down:
            yTranslate -= DIR_TRANSLATE_ANGLE;
            break;
        case trans_left:
            xTranslate -= DIR_TRANSLATE_ANGLE;
            break;
        case trans_right:
            xTranslate += DIR_TRANSLATE_ANGLE;
            break;
        default:
            dir=still;
            break;
    }
    
    dir = still;
}
void My_Reshape(int width, int height){
    
    glViewport(0, 0, width, height);
    
    float viewportAspect = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat) width / height, 1, 3000.0 );
    //gluOrtho2D(-2.5 * viewportAspect, 2.5 * viewportAspect, -2.5, 2.5);
    gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void My_Timer(int val){
    
    timer_cnt++;
    glutPostRedisplay();
    
    if(timer_enabled){
        glutTimerFunc(timer_speed, My_Timer, val);
    }
    
}
void My_Mouse(int button, int state, int x, int y){
    
    if(state == GLUT_DOWN){
        printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
    }
    
    else if(state == GLUT_UP){
        printf("Mouse %d is released at (%d, %d)\n", button, x, y);
    }
}
void My_Keyboard(unsigned char key, int x, int y){
    
    switch (key) {
        case 'w'://up
            printf("Key %c is pressed at (%d, %d)\n", key, x, y);
            dir= trans_up;
            break;
        case 'a'://left
            dir= trans_left;
            break;
        case 's'://down
            dir= trans_down;
            break;
        case 'd'://right
            dir= trans_right;
            break;
        default:
            break;
    }
    
}
void My_SpecialKeys(int key, int x, int y){
    
    switch(key){
            
        case GLUT_KEY_F1:
            printf("F1 is pressed at (%d, %d)\n", x, y);
            break;
        case GLUT_KEY_PAGE_UP:
            printf("Page up is pressed at (%d, %d)\n", x, y);
            break;
        case GLUT_KEY_LEFT:
            printf("Left arrow is pressed at (%d, %d)\n", x, y);
            dir= left;
            break;
        case GLUT_KEY_RIGHT:
            printf("Right arrow is pressed at (%d, %d)\n", x, y);
            dir= right;
            break;
        case GLUT_KEY_UP:
            printf("UP arrow is pressed at (%d, %d)\n", x, y);
            dir= up;
            break;
        case GLUT_KEY_DOWN:
            printf("Down arrow is pressed at (%d, %d)\n", x, y);
            dir= down;
            break;
        default:
            printf("Other special key is pressed at (%d, %d)\n", x, y);
            break;
    }
}
void My_Menu(int id){
    
    switch(id){
            
        case MENU_TIMER_START:
            if(!timer_enabled){
                
                timer_enabled = true;
                glutTimerFunc(timer_speed, My_Timer, 0);
            }
            break;
        case MENU_TIMER_STOP:
            timer_enabled = false;
            break;
        case MENU_EXIT:
            exit(0);
            break;
        case MENU_ANIM:
            anim_flag=1;
            init_all_parameters();
            break;
        case MENU_STOP:
            anim_flag=0;
            anim_stage=nod;
            init_all_parameters();
            break;
        default:
            break;
    }
}

void SetLightSource(){
    
    GLfloat light_diffuse[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0};
    GLfloat light_position[] = {-60.00, 100.00, 100.0,0.0 };//(x,)
        //GLfloat a = 0.8;
    
    glEnable(GL_LIGHTING);                                 //開燈
    glEnable(GL_LIGHT0);
    // 設定發光體的光源的特性
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);      //環境光(Ambient Light)
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);      //散射光(Diffuse Light)
    glLightfv( GL_LIGHT0, GL_SPECULAR,light_specular);     //反射光(Specular Light)
    glLightfv( GL_LIGHT0, GL_POSITION,light_position);     //光的座標 //光源的位置

    //glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,a);
    
    glEnable(GL_DEPTH_TEST);                               //啟動深度測試
}
void SetMaterial(){
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT);
    GLfloat material_ambient[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat material_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat material_specular[]   = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat high_shininess[] = { 100.0f };
    
    
    glMaterialfv( GL_FRONT, GL_AMBIENT,  material_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE,  material_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}
void SetMenu(){
    
    // Create a menu and bind it to mouse right button.
    ////////////////////////////
    int menu_main = glutCreateMenu(My_Menu);
    int menu_anim = glutCreateMenu(My_Menu);
    
    glutSetMenu(menu_main);
    //glutAddSubMenu("Timer", menu_timer);
    glutAddSubMenu("Animation", menu_anim);
    glutAddMenuEntry("Exit", MENU_EXIT);
    
    
    glutSetMenu(menu_anim);
    glutAddMenuEntry("Anim1", MENU_ANIM);
    glutAddMenuEntry("Stop", MENU_STOP);

    
    glutSetMenu(menu_main);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    ////////////////////////////

}

void initTextures()
{
    // load jpg
    texture_data tdata = load_jpg("nthu.jpg"); // return width * height * 3 uchars
    if(tdata.data == 0)
    {
        // load failed
        return;
    }
    glGenTextures(1, &texture_jpg);
    glBindTexture(GL_TEXTURE_2D, texture_jpg);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata.data); // Use GL_RGB
    glGenerateMipmap(GL_TEXTURE_2D);
    free_texture_data(tdata);
    
    
    // load png
    tdata = load_png("/Users/jeanlee/Downloads/symbols.png"); // return width * height * 4 uchars
    if(tdata.data == 0)
    {
        // load failed
        return;
    }
    glGenTextures(1, &texture_png);
    glBindTexture(GL_TEXTURE_2D, texture_png);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data); // Use GL_RGBA
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    free_texture_data(tdata);
    
    
}
int main(int argc, char *argv[]){
    
    // Initialize GLUT and GLEW, then create a window.
    ////////////////////
    glutInit(&argc, argv);
    //glewInit();
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("HW1 Robot");
    // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
    dumpInfo();
    ////////////////////
    
    // Initialize OpenGL states.
    initGL();
    initTextures();
    
    // Set Menu
    SetMenu();
    init_all_parameters();
    // Register GLUT callback functions.
    ///////////////////////////////
    glutDisplayFunc(My_Display);
    glutReshapeFunc(My_Reshape);
    glutMouseFunc(My_Mouse);
    glutKeyboardFunc(My_Keyboard);
    glutSpecialFunc(My_SpecialKeys);
    glutTimerFunc(timer_speed, My_Timer, 0);
    glutIdleFunc(direction_move);
    ///////////////////////////////
    
    // Enter main event loop.
    //////////////
    SetLightSource();
    SetMaterial();
    
    

    glutMainLoop();
    //////////////
    return 0;
}
