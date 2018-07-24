#ifndef MAIN_HPP
#define MAIN_HPP

struct POINT{
    int16_t x;
    int16_t y;
};

void ClearScreen(uint32_t* screen, int width, int heigth, uint32_t color);
void SetPixel(uint32_t* screen, int16_t x, int16_t y, uint32_t color);
void GenerateStars(POINT* starfield, int count, int width, int heigth);
void ScrollHorStarfield(POINT* starfield, int count, int width, int step);
void DrawStarfield(POINT* starfield, int count, uint32_t* screen, uint32_t color);

#endif // MAIN_HPP
