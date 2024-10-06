#include <stdbool.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <math.h>

#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define SCREEN_WIDTH 792
#define SCREEN_HEIGHT 792

Color colors[] = {GRAY, RED, GREEN, ORANGE, BLUE};

int world_map[MAP_WIDTH][MAP_HEIGHT] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,1,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,2,0,5,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,1,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


Vector2 world_to_screen(Vector2 world_coord, float factor){
  return (Vector2){.x = world_coord.x * factor, .y = world_coord.y * factor};
}


int main(void){

  // force alphas for the colors....
  for (int i=0; i<5; i++){
    Color c = colors[i];
    c.a = 1.0;
    colors[i]=c;
  }
  
  SetTargetFPS(30);
  
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting test");
  
  
  
  float player_angle = -PI/6;
  Vector2 player_pos = (Vector2){.x = 10, .y = 10};
  Vector2 player_dir = (Vector2){.x = cos(player_angle), .y = sin(player_angle) };
  float fov = PI/3;
 

RenderTexture2D rt2D = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

  float new_x, new_y;
  while (!WindowShouldClose()){
    
    if (IsKeyDown(KEY_Q)){
      break;
    }

    if (IsKeyDown(KEY_W)){
      Vector2 new_pos = Vector2Add(player_pos, Vector2Scale(player_dir, 0.2));
      if (world_map[(int)new_pos.y][(int)new_pos.x] == 0){
        player_pos = new_pos;
      }
    }

    if (IsKeyDown(KEY_S)){
      Vector2 new_pos = Vector2Add(player_pos, Vector2Scale(player_dir, -0.2));
      if (world_map[(int)new_pos.y][(int)new_pos.x] == 0){
        player_pos = new_pos;
      }
    }


    if (IsKeyDown(KEY_A)){
      player_angle -= 2*PI/180;
      if (player_angle < -PI) player_angle += 2*PI;
      player_dir = (Vector2){.x = cos(player_angle), .y = sin(player_angle) };
    }

    if (IsKeyDown(KEY_D)){
      player_angle += 2*PI/180;
      if (player_angle > PI) player_angle -= 2*PI;
      player_dir = (Vector2){.x = cos(player_angle), .y = sin(player_angle) };
    }

    if (IsKeyDown(KEY_LEFT)){
      float new_fov = fov - PI/180;
      fov = fmax(PI/180, new_fov);
    }

    if (IsKeyDown(KEY_RIGHT)){
      float new_fov = fov + PI/180;
      fov = fmin(2*PI, new_fov);
    }

    
    BeginTextureMode(rt2D);

    //        ClearBackground(BLACK);
    
    Color ceiling_color = (Color){.r = 99, .g = 164, .b = 207, .a = 255};
    
    Color floor_color = (Color){.r = 64, .g = 50, .b = 7, .a = 255};
    
     DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2, ceiling_color);
    DrawRectangle(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, floor_color);
    
    
    
    int cell_width = (int) SCREEN_WIDTH/MAP_WIDTH;
    int cell_height = (int) SCREEN_HEIGHT/ MAP_HEIGHT;
    
    
    /*       
    for (int y = 0; y < MAP_HEIGHT; y++){    
      for (int x = 0; x < MAP_WIDTH; x++){
        if (world_map[y][x] != 0){
          DrawRectangle(x*cell_width, y * cell_height, cell_width, cell_height, BLUE);
        }
      }
    }
    */
    
    
  
    int col = 0;
    float min_angle = 1000.0, max_angle = -1000.0;
    for (float angle = - fov/2 ; angle < fov/2 ; angle += fov/SCREEN_WIDTH){
      
      float tot_angle = angle + player_angle;
      Vector2 ray_unit_step_size = {fminf(sqrt(1+pow(sin(tot_angle)/cos(tot_angle),2)), 1e30), fminf(sqrt(1 + pow(cos(tot_angle)/sin(tot_angle), 2)), 1e30)};
      Vector2 map_cell = {(int) player_pos.x, (int) player_pos.y};
      Vector2 ray_length_1_d = {0};
      Vector2 step;
      Vector2 ray_start = player_pos;
      Vector2 ray_direction = Vector2Normalize( (Vector2){cos(tot_angle), sin(tot_angle)});
      bool found = false;
      float maxDist = 150;
      float distance = 0;
      
      if (ray_direction.x < 0){
        step.x = -1 ;
        ray_length_1_d.x = (ray_start.x - (float)map_cell.x) * ray_unit_step_size.x;
      } else{
        step.x = 1 ;
        ray_length_1_d.x = ( (float) (map_cell.x + 1)  - ray_start.x ) * ray_unit_step_size.x;
      }
      
      if (ray_direction.y < 0){
        step.y = 1 ;
        ray_length_1_d.y = (ray_start.y - (float)map_cell.y) * ray_unit_step_size.y;
      } else{
        step.y = -1 ;
        ray_length_1_d.y =  ((float)(map_cell.y + 1) - ray_start.y) * ray_unit_step_size.y;
      }

      int side;
      while (!found && distance < maxDist){
        if (ray_length_1_d.x < ray_length_1_d.y){
          map_cell.x += step.x;
          distance = ray_length_1_d.x;
          ray_length_1_d.x += ray_unit_step_size.x;
          side = 0;
        } else {
          map_cell.y -= step.y;
          distance = ray_length_1_d.y;
          ray_length_1_d.y += ray_unit_step_size.y;
          side = 1;
        }

        
        if (world_map[(int)map_cell.y][(int)map_cell.x] != 0){
          found = true;
          Vector2 intersection = Vector2Add (ray_start, Vector2Scale(ray_direction, distance));
                    
          DrawCircleV(world_to_screen(intersection, cell_height), 2, GREEN);
          //DrawLineEx(world_to_screen(player_pos, cell_height), world_to_screen(intersection, cell_height), 1, BLUE);

          float alpha = angle;// + player_angle;//player_angle - angle;

          float dx  = fabs(player_pos.x - intersection.x);
          float dy = fabs(player_pos.y - intersection.y);


          if (min_angle > angle) min_angle = angle;
          if (max_angle < angle) max_angle = angle;
          
          float scale = 0.1;
          float distance = sqrt((dx*dx + dy*dy));
          //          float d = dx*cos(angle) - dy * sin(angle); // Sanglard, Wolfenstein3D book, p.170
          float d = distance * cos(angle);
          if (d < 0.1) d  = 0.1;
          float height =  1000 / d;

          
          Color col_color = colors[world_map[(int)map_cell.y][(int)map_cell.x]-1];
          Vector3 color_hsv = ColorToHSV(col_color);
          color_hsv.z *= (2.0-side)/2;
          col_color = ColorFromHSV(color_hsv.x, color_hsv.y, color_hsv.z);
          static int flag = 0;
          if (flag<800){
            printf("col: %d, Color: %d, %d, %d, %d\n", col, col_color.r, col_color.g, col_color.b, col_color.a);
            flag += 1;
          }
            
          DrawLine(col, (float)SCREEN_HEIGHT/2 - height/2, col, (float)SCREEN_HEIGHT/2 + height/2, col_color);

          col++;
        }        
      }

      // draw player in 2D map
      Vector2 player_pos_scr = world_to_screen(player_pos, cell_width);
      DrawCircleV(player_pos_scr, 5, RED);
      DrawLine(player_pos_scr.x, player_pos_scr.y,
               player_pos_scr.x + 50*cos(player_angle) , player_pos_scr.y +  50*sin(player_angle), 
               WHITE);



      
    }

      // texts
       char text[10]; 
       sprintf(text, "%.3f\n", player_angle); 
       DrawText(text, 10, 772, 18, ORANGE); 


       sprintf(text, "%.3f\n", min_angle); 
       DrawText(text, 140, 772, 18, ORANGE); 

       sprintf(text, "%.3f\n", max_angle); 
       DrawText(text, 260, 772, 18, ORANGE);
       EndTextureMode();

       
       BeginDrawing();
       DrawTexturePro(rt2D.texture, (Rectangle){ 0.0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT }, (Rectangle) { 0.0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},(Vector2) {0, 0}, 0, WHITE );
       EndDrawing();
  }
  
  
  CloseWindow();
  
}
