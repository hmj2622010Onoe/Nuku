#pragma once

void InitGame(void);
void DrawImageEnlarge(int img, int x, int y, int enlargeX, int enlargeY);
void DrawImageRotateEnlarge(int img, int x, int y,double ExtRate,int Angle);

int LoadGraphWithCheck(const char* file);
int LoadSoundMemWithCheck(const char* file);
