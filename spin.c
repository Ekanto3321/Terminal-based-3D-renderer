#include <stdio.h>
#include <unistd.h> 
#include <math.h>
#include <string.h>

#define size 50
#define pi 3.14159

const int framerate = 30;
const float phi_s = 0.02; //phi spacings around x axis
const float theta_s = 0.02; //theta spacings around y axis
const float R1 = 2; //radius of the sphere
const float k1 = 200; //scaling factor
const float k2 = 22; //distance from viewfinder to object
const float R2 = 3; //radius of shadow

// char luminosity[] = " .c`',^:-~\";!i*+xoO0%#@&8MW$B%RQHNK9DX";
char luminosity[] = " .`',^:-~\";c!i*+xoO0%#@&8MW$BRQHNK9DX";
int intensity[50];
int len = 30;
float lv[3] = {-1,1,-1}; // default static lighting vector

char f_b[size][size]; //frame buffer 
float z_b[size][size]; //depth buffer

float frametime = (1000000/(float)framerate);

void init(){

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      f_b[i][j] = ' ';

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      z_b[i][j] = 0;
    
}

void sphere(){

  float max_lum = 0;
  float min_lum = 20;
  for (float phi = 0; phi < 2 * pi; phi+=phi_s){
    for (float theta = 0; theta < pi; theta+=theta_s){
      
      float sin_phi = sin(phi);
      float cos_phi = cos(phi);

      float sin_theta = sin(theta);
      float cos_theta = cos(theta);

      //semi circle
      float c_x = R1 * cos_theta; 
      float c_y = R1 * sin_theta;

      //full sphere
      float x = c_x;
      float y = c_y * cos_phi + 0;
      float z = k2 + 0 * cos_phi - c_y * sin_phi;
      
      float z_inv = 1.0 / z;

      // projection on picture plane
      int x_proj = (int) (size / 2 + k1 * z_inv * x) * 0.8;
      int y_proj = (int) (size / 2 + k1 * z_inv * y);


      //lighting calc

      // float lum = c_y * cos_phi + c_y * sin_phi; //normals [0 1 -1]
      // float lum = c_x + c_y * cos_phi + c_y * sin_phi; //normals [1 1 -1]

      float lum = c_x * lv[0] + ( c_y * cos_phi ) * lv[1] + ( -c_y * sin_phi ) * lv[2];

      if(lum>0){
      
        if(z_inv > z_b[x_proj][y_proj]){

          z_b[x_proj][y_proj] = z_inv;
          int lum_idx = lum*8; //8 is arbritary
          f_b[x_proj][y_proj] = luminosity[2+lum_idx];

          // if(lum_idx<min_lum)min_lum=lum_idx;
          // if(lum_idx>max_lum)max_lum=lum_idx;

        }
      }
      //flat shading
      // if(z_inv > z_b[x_proj][y_proj])
      //   z_b[x_proj][y_proj] = z_inv;
    }
    
  }

}


void draw(){

  printf("\x1b[2J\x1b[H"); // magic escape sequence
  int idx = 0;

  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++){
      
      for (int k = 0; k < len; k++){
        if(f_b[i][j]==luminosity[k])idx=k;
      }
      

      printf("\033[38;2;%d;%d;%dm%c ",255,intensity[idx],0,f_b[i][j]);
      // printf("\033[38;2;%d;%d;%dm%c\033[0m ", 255, 255 , 255, f_b[i]);

    }
    printf("\n");
    
  }
  
  usleep(frametime);

}
 
int main(){

  // len = strlen(luminosity);


  for (int i = len; i > 0; i--){
    intensity[i] = (255 * i) / (len - 1);
  }
  
  init();

  float i = 45;
  float j = 0;
  float k = 0;
  
  while (1){

    lv[0] = cos(i);

    lv[1] = -sin(i);

    // lv[2] = cos(k); 

    init();

    sphere();
    
    draw();
    i+=0.04;
    j+=0.04;
    k+=0.04;
    
  }

  return 0;
}

