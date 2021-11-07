#include <stdio.h>
#include <raylib.h>
#include <math.h>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT= 720;

inline Color hex_to_color(unsigned int color) {
	return Color{(color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff};
}

float lerp_pixel(int a, float a_val, int b, float b_val) {
	return (a + (b - a) * (1 - a_val) / (b_val - a_val));
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MEATBALLS!!!");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);
	
	int cell_size = 3;
	int cols = 1 + SCREEN_WIDTH / cell_size;
	int rows = 1 + SCREEN_HEIGHT / cell_size;
	
	float* field = new float[rows * cols];
	
	auto cell = [cols](float* field, size_t x, size_t y) -> float& {
		return field[y * cols + x];
	};
	
	bool draw_pixels = false;
	
#define NUM_BALLS 6
#define CELL(x, y) cell(field, x, y)
	
	Vector2 balls[NUM_BALLS];
	int radii[NUM_BALLS];
	Vector2 velocity[NUM_BALLS];
	for(int i = 0; i < NUM_BALLS; ++i) {
		balls[i] = {float(GetRandomValue(0, SCREEN_WIDTH)), float(GetRandomValue(0, SCREEN_HEIGHT))};
		radii[i] = GetRandomValue(30, 50);
		velocity[i] = {float(GetRandomValue(-2, 2)), float(GetRandomValue(-2, 2))};
	}
	
	const float THRESHOLD = 1.f;
	
	while(!WindowShouldClose()) {
		
		
		for(int i = 0; i < rows; ++i) {
			for(int j = 0; j < cols; ++j) {
				float s = 0;
				int x = j * cell_size;
				int y = i * cell_size;
				for(int n = 0; n <  NUM_BALLS; ++n) {
					s += (float)radii[n] / ( sqrtf( (x - balls[n].x) * (x - balls[n].x) + (y - balls[n].y) * (y - balls[n].y)));
				}
				CELL(j, i) = s;
			}
		}
		
		balls[0] = GetMousePosition();
		
		if(IsKeyPressed(KEY_SPACE)) draw_pixels = !draw_pixels;
		BeginDrawing();
		{
			ClearBackground(hex_to_color(0x181818ff));
			
			for(int i = 0; i < rows; ++i) {
				for(int j = 0; j < cols; ++j) {
					int x = j * cell_size;
					int y = i * cell_size;
					//DrawCircle(x, y, 2, WHITE);
					
					int a = CELL(j, i) > THRESHOLD; 
					int b = CELL(j + 1, i) > THRESHOLD; 
					int c = CELL(j + 1, i + 1) > THRESHOLD; 
					int d = CELL(j, i + 1) > THRESHOLD; 
					
					Vector2 p1 = {float(x), float(y)};
					Vector2 p2 = {float(x) + cell_size, float(y)};
					Vector2 p3 = {float(x) + cell_size, float(y) + cell_size};
					Vector2 p4 = {float(x), float(y) + cell_size};
					
					switch( d << 3 | c << 2 | b << 1 | a) {
						case 0: {
							//DrawCircleLines(x, y, cell_size, RED);
						}break;
						case 1: {
							Vector2 l1 = {p1.x, lerp_pixel(p1.y, CELL(j, i), p4.y, CELL(j, i + 1))};
							Vector2 l2 = {lerp_pixel(p1.x, CELL(j, i), p2.x, CELL(j + 1, i)), p1.y};
							DrawLineV(l1, l2, WHITE);
						} break;
						case 2: { 
							Vector2 l1 = {lerp_pixel(p1.x, CELL(j, i), p2.x, CELL(j + 1, i)), p2.y};
							Vector2 l2 = {p2.x, lerp_pixel(p2.y, CELL(j + 1, i), p3.y, CELL(j+1, i+1))};
							DrawLineV(l1, l2, WHITE);
						} break;
						case 3: { 
							Vector2 l1 = {p1.x, lerp_pixel(p1.y, CELL(j, i), p4.y, CELL(j, i + 1))};
							Vector2 l2 = {p2.x, lerp_pixel(p2.y, CELL(j + 1, i), p3.y, CELL(j + 1, i + 1))};
							DrawLineV(l1, l2, WHITE);
						} break;
						case 4: { 
							Vector2 l1 = {lerp_pixel(p4.x, CELL(j, i + 1), p3.x, CELL(j + 1, i + 1)), p4.y};
							Vector2 l2 = {p2.x, lerp_pixel(p2.y, CELL(j + 1, i), p3.y, CELL(j + 1, i + 1))};
							
							DrawLineV(l1, l2, WHITE);
						} break;
						case 5: { 
							Vector2 l1 = {lerp_pixel(p1.x, CELL(j, i), p2.x, CELL(j + 1, i)), p2.y};
							Vector2 l2 = {p2.x, lerp_pixel(p2.y, CELL(j + 1, i), p3.y, CELL(j+1, i+1))};
							
							Vector2 l3 = {p4.x, lerp_pixel(p1.y, CELL(j, i), p4.y, CELL(j, i + 1))};
							Vector2 l4 = {lerp_pixel(p4.x, CELL(j, i + 1), p3.x, CELL(j + 1, i + 1)), p4.y};
							
							DrawLineV(l1, l2, WHITE);
							DrawLineV(l3, l4, WHITE);
						} break;
						case 6: { 
							Vector2 l1 = {lerp_pixel(p1.x, CELL(j, i), p2.x, CELL(j + 1, i)), p2.y};
							Vector2 l2 = {lerp_pixel(p4.x, CELL(j, i + 1), p3.x, CELL(j + 1, i + 1)), p3.y};
							DrawLineV(l1, l2, WHITE);
						} break;
						case 7: { 
							Vector2 l1 = {p4.x, lerp_pixel(p1.y, CELL(j, i), p4.y, CELL(j, i + 1))};
							Vector2 l2 = {lerp_pixel(p4.x, CELL(j, i + 1), p3.x, CELL(j + 1, i + 1)), p4.y};
							
							DrawLineV(l1, l2, WHITE);
						} break;
						case 8: { 
							Vector2 l1 = {p4.x, lerp_pixel(p1.y, CELL(j, i), p4.y, CELL(j, i + 1))};
							Vector2 l2 = {lerp_pixel(p4.x, CELL(j, i + 1), p3.x, CELL(j + 1, i + 1)), p4.y};
							
							DrawLineV(l1, l2, WHITE);
						} break;
						case 9: { 
							Vector2 l1 = {lerp_pixel(p1.x, CELL(j, i), p2.x, CELL(j + 1, i)), p1.y};
							Vector2 l2 = {lerp_pixel(p4.x, CELL(j, i + 1), p3.x, CELL(j + 1, i + 1)), p4.y};
							DrawLineV(l1, l2, WHITE);
						} break;
						case 10: { 
							Vector2 l1 = {p1.x, lerp_pixel(p1.y, CELL(j, i), p4.y, CELL(j, i + 1))};
							Vector2 l2 = {lerp_pixel(p1.x, CELL(j, i), p2.x, CELL(j + 1, i)), p1.y};
							Vector2 l3 = {lerp_pixel(p4.x, CELL(j, i + 1), p3.x, CELL(j + 1, i + 1)), p4.y};
							Vector2 l4 = {p2.x, lerp_pixel(p2.y, CELL(j + 1, i), p3.y, CELL(j + 1, i + 1))};
							
							DrawLineV(l1, l2, WHITE);
							DrawLineV(l3, l4, WHITE);
						} break;
						case 11: { 
							Vector2 l1 = {lerp_pixel(p4.x, CELL(j, i + 1), p3.x, CELL(j + 1, i + 1)), p4.y};
							Vector2 l2 = {p2.x, lerp_pixel(p2.y, CELL(j + 1, i), p3.y, CELL(j + 1, i + 1))};
							
							DrawLineV(l1, l2, WHITE);
						} break;
						case 12: { 
							Vector2 l1 = {p4.x, lerp_pixel(p1.y, CELL(j, i), p4.y, CELL(j, i + 1))};
							Vector2 l2 = {p3.x, lerp_pixel(p2.y, CELL(j + 1, i), p3.y, CELL(j + 1, i + 1))};
							DrawLineV(l1, l2, WHITE);
						} break;
						case 13: { 
							Vector2 l1 = {lerp_pixel(p1.x, CELL(j, i), p2.x, CELL(j + 1, i)), p2.y};
							Vector2 l2 = {p2.x, lerp_pixel(p2.y, CELL(j + 1, i), p3.y, CELL(j+1, i+1))};
							DrawLineV(l1, l2, WHITE);
						} break;
						case 14: { 
							Vector2 l1 = {p1.x, lerp_pixel(p1.y, CELL(j, i), p4.y, CELL(j, i + 1))};
							Vector2 l2 = {lerp_pixel(p1.x, CELL(j, i), p2.x, CELL(j + 1, i)), p1.y};
							DrawLineV(l1, l2, WHITE);
						} break;
						
						case 15: {
							if(draw_pixels) DrawPixel(x, y, GREEN);
						}break;
					}
				}
			}
			
		}
		EndDrawing();
		
		
		for(int i = 0; i < NUM_BALLS; ++i) {
			balls[i].x += 5 * velocity[i].x;
			balls[i].y += 5 * velocity[i].y;
			
			if(balls[i].x + radii[i] >= SCREEN_WIDTH) {
				velocity[i].x *= -1;
				balls[i].x = SCREEN_WIDTH - radii[i];
			}
			if(balls[i].x - radii[i]<= 0) {
				velocity[i].x *= -1;
				balls[i].x = radii[i];
			}
			
			if(balls[i].y + radii[i] >= SCREEN_HEIGHT) {
				velocity[i].y *= -1;
				balls[i].y = SCREEN_HEIGHT - radii[i];
			}
			if(balls[i].y - radii[i]<= 0) {
				velocity[i].y *= -1;
				balls[i].y = radii[i];
			}
			
		}
		
	}
	return 0;
}