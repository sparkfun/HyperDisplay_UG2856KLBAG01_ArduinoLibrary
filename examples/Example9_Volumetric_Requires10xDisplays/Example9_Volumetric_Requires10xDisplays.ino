/*
  Make a 10-layer deep volumetric display witht he transparent graphical oled
  
  Big credit goes to M Rule at http://crawlingrobotfortress.blogspot.com for writing the 3D vector code 
  as well as the STL to header Python code. 
  
  Check out the blog post here: http://crawlingrobotfortress.blogspot.com/2015/12/better-3d-graphics-engine-on-arduino.html
  
  By: Owen Lyke
  SparkFun Electronics
  Date: March 1, 2019
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.
  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/15173

  

  The graphics library is like a 3-layer cake. Here they are from top-down
  https://github.com/sparkfun/SparkFun_HyperDisplay
  https://github.com/sparkfun/HyperDisplay_SSD1309_ArduinoLibrary
  https://github.com/sparkfun/HyperDisplay_UG2856KLBAG01_ArduinoLibrary

  Hardware Compatibility
    - The IO pins on this board are designed for use with 3.3V so if you are using a 5V microcontroller
      please use a level shifter. Note: Qwiic connectors on SparkFun dev boards are already at 3.3V
    - This display relies on a copy of graphics data in your microcontroller, a total of 1024 bytes. 
      That is half the RAM available on an Uno so it is easy to run into sinister low-memory related
      bugs. We reccomend using a micro with more memory like a SAMD21, Esp32, Teensy, etc.
  
  Hardware Connections:
  Option 1 (I2C):
    Connect using a Qwiic jumper if you have a Qwiic compatible board and you plan to use I2C

  Option 2 (SPI):
    Connect SCLK and MOSI to the SPI port of your choice (13 and 11 for SPI on Uno-like boards) 
    Also connect D/C and CS to two unused GPIO pins of your choice (and set the proper pin definitions below)
    Don't forget power - connect 3.3V and GND
*/
#include "type.h"
#include "HyperDisplay_UG2856KLBAG01.h"   // Your library can be installed here: http://librarymanager/All#SparkFun_Transparent_Graphical_OLED
                                          // The rest of the Layer Cake:         http://librarymanager/All#SparkFun_HyperDisplay_SSD1309
                                          //                                     http://librarymanager/All#SparkFun_HyperDisplay

// ----------------------------------------------
// meshes
// ----------------------------------------------
// uncomment 1 header to automatically load mesh
#include "meshes/mesh_cube.h"
//#include "meshes/mesh_cone.h"
//#include "meshes/mesh_sphere.h"
//#include "meshes/mesh_torus.h"
//#include "meshes/mesh_monkey.h"
//#include "meshes/mesh_pikachu.h"
//#include "meshes/mesh_skull.h"
//#include "meshes/mesh_stanBun.h"

//////////////////////////
//      User Setup      //
//////////////////////////
#define SERIAL_PORT Serial  
#define SPI_PORT SPI        // Used if USE_SPI == 1

#define RES_PIN 2           // Optional
//#define CS_PIN 26         // Unused b/c of the cs array
#define DC_PIN 5            // Used only if USE_SPI == 1



// Set up the rotation. If spin is true then this value is aded every time, otherwise these are the constant rotation values
uint8_t rotx = 1;
uint8_t roty = 2;
uint8_t rotz = 3;


bool spin = true;  // Whether or not to spin the model
uint32_t del = 60;  // Delay between redraws





// END USER SETUP



#define LUT(a) (long)(lut[a])// return value from LUT in PROGMEM

const uint8_t zSpacing = 5;

void set3D(bool SETclear, uint8_t x, uint8_t y, uint8_t z);
void bres3D(bool SETclear, uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2);


void draw_wireframe(void);
void clear_wireframe( void );


// ----------------------------------------------
// Matrix operation
// ----------------------------------------------
Matrix4 mMultiply(const Matrix4 &mat1, const Matrix4 &mat2);
Matrix4 mRotateX(const unsigned int angle);
Matrix4 mRotateY(const unsigned int angle);
Matrix4 mRotateZ(const unsigned int angle);
Matrix4 mTranslate(const long x, const long y, const long z);
Matrix4 mScale(const float ratio);


UG2856KLBAG01_SPI myTOLED0;  // Declare a SPI-based Transparent OLED object called myTOLED
UG2856KLBAG01_SPI myTOLED1;
UG2856KLBAG01_SPI myTOLED2;
UG2856KLBAG01_SPI myTOLED3;
UG2856KLBAG01_SPI myTOLED4;
UG2856KLBAG01_SPI myTOLED5;
UG2856KLBAG01_SPI myTOLED6;
UG2856KLBAG01_SPI myTOLED7;
UG2856KLBAG01_SPI myTOLED8;
UG2856KLBAG01_SPI myTOLED9;

UG2856KLBAG01_SPI oleds[] = {
 myTOLED0,
 myTOLED1,
 myTOLED2,
 myTOLED3,
 myTOLED4,
 myTOLED5,
 myTOLED6,
 myTOLED7,
 myTOLED8,
 myTOLED9,};

uint8_t cs[] = {
  26,
  27,
  14,
  12,
  13,
  4,
  17,
  16,
  22,
  19
};


Matrix4 m_world;
Vector3i mesh_rotation = {0, 0, 0};
Vector3i mesh_position = {0, 0, 0};


long rotatedNodes[NODECOUNT][3]; // make another array to hold rotated nodes
long nodesToClear[NODECOUNT][3]; // reduce redraw delay by storing the nodes to clear out



uint8_t numLayers = sizeof(oleds)/sizeof(UG2856KLBAG01_SPI);

const unsigned int lut[] = {         // 0 to 90 degrees fixed point COSINE look up table
  16384, 16381, 16374, 16361, 16344, 16321, 16294, 16261, 16224, 16182, 16135, 16082, 16025, 15964, 15897, 15825, 15749, 15668, 15582, 15491, 15395, 15295, 15190, 15081, 14967, 14848, 14725, 14598, 14466, 14329, 14188, 14043, 13894, 13740, 13582, 13420, 13254, 13084, 12910, 12732, 12550, 12365, 12175, 11982, 11785, 11585, 11381, 11173, 10963, 10748, 10531, 10310, 10086, 9860, 9630, 9397, 9161, 8923, 8682, 8438, 8191, 7943, 7691, 7438, 7182, 6924, 6663, 6401, 6137, 5871, 5603, 5334, 5062, 4790, 4516, 4240, 3963, 3685, 3406, 3126, 2845, 2563, 2280, 1996, 1712, 1427, 1142, 857, 571, 285, 0
};

// ----------------------------------------------
// SIN/COS from 90 degrees LUT
// ----------------------------------------------
long SIN(unsigned int angle) {
  angle += 90;
  if (angle > 450) return LUT(0);
  if (angle > 360 && angle < 451) return -LUT(angle-360);
  if (angle > 270 && angle < 361) return -LUT(360-angle);
  if (angle > 180 && angle < 271) return  LUT(angle-180);
  return LUT(180-angle);
}

long COS(unsigned int angle) {
  if (angle > 360) return LUT(0);
  if (angle > 270 && angle < 361) return  LUT(360-angle);
  if (angle > 180 && angle < 271) return -LUT(angle-180);
  if (angle > 90  && angle < 181) return -LUT(180-angle);
  return LUT(angle);
}





void setup() {

  SERIAL_PORT.begin(9600);

  SERIAL_PORT.println(F("Example9_Volumetric: Transparent Graphical OLED"));
  SERIAL_PORT.println(numLayers);

  SPI_PORT.begin();

  for(uint8_t indi = 0; indi < numLayers; indi++){
    oleds[indi].begin(cs[indi], DC_PIN, SPI_PORT);                  // Begin for SPI requires that you provide the CS and DC pin numbers
  }

}

void loop() {


  do{
    
    // rotation
    m_world = mRotateX(mesh_rotation.x);
    m_world = mMultiply(mRotateY(mesh_rotation.y), m_world);
    m_world = mMultiply(mRotateZ(mesh_rotation.z), m_world);

    // project nodes with world matrix
    Vector3i p;
    for (uint32_t i=0; i<NODECOUNT; i++) {
      p.x = (m_world.m[0][0] * (NODE(i,0) >> PSHIFT)+
             m_world.m[1][0] * (NODE(i,1) >> PSHIFT) +
             m_world.m[2][0] * (NODE(i,2) >> PSHIFT) +
             m_world.m[3][0]) / PRES;
      
      p.y = (m_world.m[0][1] * (NODE(i,0) >> PSHIFT) +
             m_world.m[1][1] * (NODE(i,1) >> PSHIFT) +
             m_world.m[2][1] * (NODE(i,2) >> PSHIFT) +
             m_world.m[3][1]) / PRES;
            
      p.z = (m_world.m[0][2] * (NODE(i,0) >> PSHIFT) +
             m_world.m[1][2] * (NODE(i,1) >> PSHIFT) +
             m_world.m[2][2] * (NODE(i,2) >> PSHIFT) +
             m_world.m[3][2]) / PRES;

      // store projected node
      rotatedNodes[i][0] = p.x + 30; //post-rotation translate
      rotatedNodes[i][1] = p.y + 30;
      rotatedNodes[i][2] = p.z + 25;

//      // default auto-rotation mode
//      mesh_rotation.x+=3;
//      mesh_rotation.y+=2;
//      mesh_rotation.z++;

    // default auto-rotation mode
    if(spin){
      mesh_rotation.x+= rotx;
      mesh_rotation.y+= roty;
      mesh_rotation.z+= rotz;
    }else{
      mesh_rotation.x = rotx;
      mesh_rotation.y = roty;
      mesh_rotation.z = rotz;
    }
  
      if (mesh_rotation.x > 360) mesh_rotation.x = 0;
      if (mesh_rotation.y > 360) mesh_rotation.y = 0;
      if (mesh_rotation.z > 360) mesh_rotation.z = 0;
    }
    
  }while(0); // While its not time yet to draw again?

  
//    for (uint32_t i=0; i<NODECOUNT; i++) {
//
//      // store projected node
//      rotatedNodes[i][0] = (NODE(i, 0) >> PSHIFT) + 20;
//      rotatedNodes[i][1] = (NODE(i, 1) >> PSHIFT) + 20;
//      rotatedNodes[i][2] = (NODE(i, 2) >> PSHIFT) + 20;
//    }



  // Draw

  
//  // For now let's just print the coordinates to serial
//  for(uint32_t indi = 0; indi < NODECOUNT; indi++){
//    Serial.print("Node: "); Serial.print(indi); 
//    Serial.print(", coords: ("); Serial.print( rotatedNodes[indi][0] );
//    Serial.print(", "); Serial.print( rotatedNodes[indi][1] );
//    Serial.print(", "); Serial.print( rotatedNodes[indi][2] );
//    Serial.print(")");
//    Serial.println(); 
//  }
//  Serial.println("------");

  
  clear_wireframe();
  for (uint32_t i=0; i<NODECOUNT; i++) {    // Store which nodes to clear  next time
    nodesToClear[i][0] = rotatedNodes[i][0];
    nodesToClear[i][1] = rotatedNodes[i][1];
    nodesToClear[i][2] = rotatedNodes[i][2];
  }

  draw_wireframe();

  delay(del);
  
}


void set3D(bool SETclear, uint8_t x, uint8_t y, uint8_t z){
  if( (z/zSpacing) < numLayers){// Only draw if z is small enough
//   if( (z % zSpacing) == 0 ){ oleds[z/zSpacing].pixelSet(x,y); } // Only draw if the point is on a representable z axis. 

//    uint32_t 

    if(SETclear){ oleds[z/zSpacing].pixelSet(x,y); } // may want to just draw at the nearest z-axis instead of requiring a correct z-axis  // 
    else{ oleds[z/zSpacing].pixelClear(x,y); } 
  }
}

void bres3D(bool SETclear, uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2){

  int16_t dx = abs(x2 - x1);
  int16_t dy = abs(y2 - y1);
  int16_t dz = abs(z2 - z1);

  int8_t xs = -1;
  int8_t ys = -1;
  int8_t zs = -1;

  if( x2 > x1 ){ xs = 1; }
  if( y2 > y1 ){ ys = 1; }
  if( z2 > z1 ){ zs = 1; }

  int32_t p1 = 0;
  int32_t p2 = 0;

  set3D(SETclear, x1, y1, z1);

  if( (dx >= dy) && (dx >= dz) ){
    p1 = 2 * dy - dx;
    p2 = 2 * dz - dx;
    while( x1 != x2 ){
      x1 += xs;
      if( p1 >= 0 ){
        y1 += ys;
        p1 -= 2 * dx;
      }
      if( p2 >= 0 ){
        z1 += zs;
        p2 -= 2* dx;
      }
      p1 += 2* dy;
      p2 += 2* dz;
      set3D(SETclear, x1, y1, z1);
    }
  }
  else if( (dy >= dx) && (dy >= dz) ){
    p1 = 2 * dx - dy;
    p2 = 2 * dz - dy;
    while( y1 != y2 ){
      y1 += ys;
      if( p1 >= 0 ){
        x1 += xs;
        p1 -= 2 * dy;
      }
      if( p2 >= 0 ){
        z1 += zs;
        p2 -= 2* dy;
      }
      p1 += 2* dx;
      p2 += 2* dz;
      set3D(SETclear, x1, y1, z1);
    }
  }
  else{
    p1 = 2 * dy - dz;
    p2 = 2 * dx - dz;
    while( z1 != z2 ){
      z1 += zs;
      if( p1 >= 0 ){
        y1 += ys;
        p1 -= 2 * dz;
      }
      if( p2 >= 0 ){
        x1 += xs;
        p2 -= 2* dz;
      }
      p1 += 2* dy;
      p2 += 2* dx;
      set3D(SETclear, x1, y1, z1);
    }
  }
}


void draw_wireframe( void ) {
  uint32_t i = TRICOUNT-1;
  do {
    // don't draw triangle with negative surface value
//    if (!is_hidden(n, i)) {
      // draw triangle edges - 0 -> 1 -> 2 -> 0

        if( isPositive(EDGE(i,0)) && isPositive(EDGE(i,1)) ){ bres3D(true, rotatedNodes[EDGE(i,0)][0], rotatedNodes[EDGE(i,0)][1], rotatedNodes[EDGE(i,0)][2], rotatedNodes[EDGE(i,1)][0], rotatedNodes[EDGE(i,1)][1], rotatedNodes[EDGE(i,1)][2]); }
        if( isPositive(EDGE(i,1)) && isPositive(EDGE(i,2)) ){ bres3D(true, rotatedNodes[EDGE(i,1)][0], rotatedNodes[EDGE(i,1)][1], rotatedNodes[EDGE(i,1)][2], rotatedNodes[EDGE(i,2)][0], rotatedNodes[EDGE(i,2)][1], rotatedNodes[EDGE(i,2)][2]); }
        if( isPositive(EDGE(i,2)) && isPositive(EDGE(i,0)) ){ bres3D(true, rotatedNodes[EDGE(i,2)][0], rotatedNodes[EDGE(i,2)][1], rotatedNodes[EDGE(i,2)][2], rotatedNodes[EDGE(i,0)][0], rotatedNodes[EDGE(i,0)][1], rotatedNodes[EDGE(i,0)][2]); }
        
        
        
//    }
  } while(i--);
}

void clear_wireframe( void ){
    uint32_t i = TRICOUNT-1;
  do {
        if( isPositiveClear(EDGE(i,0)) && isPositiveClear(EDGE(i,1)) ){ bres3D(false, nodesToClear[EDGE(i,0)][0], nodesToClear[EDGE(i,0)][1], nodesToClear[EDGE(i,0)][2], nodesToClear[EDGE(i,1)][0], nodesToClear[EDGE(i,1)][1], nodesToClear[EDGE(i,1)][2]); }
        if( isPositiveClear(EDGE(i,1)) && isPositiveClear(EDGE(i,2)) ){ bres3D(false, nodesToClear[EDGE(i,1)][0], nodesToClear[EDGE(i,1)][1], nodesToClear[EDGE(i,1)][2], nodesToClear[EDGE(i,2)][0], nodesToClear[EDGE(i,2)][1], nodesToClear[EDGE(i,2)][2]); }
        if( isPositiveClear(EDGE(i,2)) && isPositiveClear(EDGE(i,0)) ){ bres3D(false, nodesToClear[EDGE(i,2)][0], nodesToClear[EDGE(i,2)][1], nodesToClear[EDGE(i,2)][2], nodesToClear[EDGE(i,0)][0], nodesToClear[EDGE(i,0)][1], nodesToClear[EDGE(i,0)][2]); }
  } while(i--);
}


bool isPositive( uint32_t node ){
  bool retval = false;
  if( rotatedNodes[node][0]  < 0 ){ return retval; }
  if( rotatedNodes[node][1]  < 0 ){ return retval; }
  if( rotatedNodes[node][2]  < 0 ){ return retval; }
  retval = true;
  return retval;
}

bool isPositiveClear( uint32_t node ){
  bool retval = false;
  if( nodesToClear[node][0]  < 0 ){ return retval; }
  if( nodesToClear[node][1]  < 0 ){ return retval; }
  if( nodesToClear[node][2]  < 0 ){ return retval; }
  retval = true;
  return retval;
}

// ----------------------------------------------
// Matrix operation
// ----------------------------------------------
Matrix4 mMultiply(const Matrix4 &mat1, const Matrix4 &mat2) {
  Matrix4 mat;
  unsigned char r,c;
  for (c=0; c<4; c++)
    for (r=0; r<4; r++)
      mat.m[c][r] = pMultiply(mat1.m[0][r], mat2.m[c][0]) +
                    pMultiply(mat1.m[1][r], mat2.m[c][1]) +
                    pMultiply(mat1.m[2][r], mat2.m[c][2]) +
                    pMultiply(mat1.m[3][r], mat2.m[c][3]);
  return mat;
}

Matrix4 mRotateX(const unsigned int angle) {
  Matrix4 mat;
  mat.m[1][1] =  COS(angle);
  mat.m[1][2] =  SIN(angle);
  mat.m[2][1] = -SIN(angle);
  mat.m[2][2] =  COS(angle);
  return mat;
}

Matrix4 mRotateY(const unsigned int angle) {
  Matrix4 mat;
  mat.m[0][0] =  COS(angle);
  mat.m[0][2] = -SIN(angle);
  mat.m[2][0] =  SIN(angle);
  mat.m[2][2] =  COS(angle);
  return mat;
}

Matrix4 mRotateZ(const unsigned int angle) {
  Matrix4 mat;
  mat.m[0][0] =  COS(angle);
  mat.m[0][1] =  SIN(angle);
  mat.m[1][0] = -SIN(angle);
  mat.m[1][1] =  COS(angle);
  return mat;
}

Matrix4 mTranslate(const long x, const long y, const long z) {
  Matrix4 mat;
  mat.m[3][0] =  x << PSHIFT;
  mat.m[3][1] =  y << PSHIFT;
  mat.m[3][2] =  z << PSHIFT;
  return mat;
}

Matrix4 mScale(const float ratio) {
  Matrix4 mat;
  mat.m[0][0] *= ratio;
  mat.m[1][1] *= ratio;
  mat.m[2][2] *= ratio;
  return mat;
}
