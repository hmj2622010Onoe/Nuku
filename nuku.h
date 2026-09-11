#pragma once

void InitGame(void);
void DrawImageEnlarge(int img, int x, int y, int enlargeX, int enlargeY);
void DrawImageRotateEnlarge(int img, int x, float y,double ExtRate,float Angle);

struct Weapon {
	int img;	// 画像
	//int se;	// 動かしたときのSE

	int level;	// 武器のレベル
	int iniAngle;	// 初期weaponAngle +
	int xPlus, yPlus;	// 初期x+,y+
	float size;	// 武器の大きさ default:15
	int rangeR, rangeL;	// 振れる幅 default:3
	int phase;	// 引き抜くのに必要な段階
	int weight;	// 武器の重さ
};

int LoadGraphWithCheck(const char* file);
int LoadSoundMemWithCheck(const char* file);
