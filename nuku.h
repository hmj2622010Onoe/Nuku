#pragma once

void DrawImageEnlarge(int img, int x, int y, int enlargeX, int enlargeY);
void DrawImageRotateEnlarge(int img, int x, float y,double ExtRate,float Angle);
void DrawImageAnimation(int img, int x, float y, double ExtRate, int style);

void InitGame(void);

// 武器の種類(全部使うかは不明)
int imgSword, imgScythe, imgSpear, imgAxe, imgKatana, imgMightyHammer, imgPickaxe, imgArrow, imgBayonet, imgChainsaw;
int imgCutterBlade, imgCyberSword, imgDarkSword, imgDragonKeyring, imgFireSword, imgKey, imgKitchenKnife, imgLaserSaber, imgMagicHand, imgNaginata;
int imgPencil, imgPixelSword, imgRapier, imgScissors, imgScrewdriver, imgShinai, imgShovel, imgSpatula, imgToySword, imgUmbrella;
int imgCutterKnife, imgWindmill, imgTheLegendary, imgMagicStaff;


struct Weapon{
	int* img;	// 画像

	int level;	// 武器のレベル

	int damage;	// 武器の基礎ダメージ
	int pPower;	// 永続なダメージアップ
	float cMultiplier;	// クリティカルでのダメージ倍率
	int cProbability;		// クリティカルの確率(%)
	int nextLottery;	// 次回の武器抽選回数

	int iniAngle;	// 初期weaponAngle +
	int xPlus, yPlus;	// 初期x+,y+
	float size;	// 武器の大きさ default:15
	int rangeR, rangeL;	// 振れる幅 default:3
	int phase;	// 引き抜くのに必要な段階を決める
	int weight;	// 武器の重さ
};

Weapon weaponData[] = {
	{ &imgSword,	1,	30,	1,	1.5f,	20,	3,	0,	0,170,	10.0f,	3,	3,	7,	10},//0
	{ &imgShinai,	1,	25,	2,	1.5f,	10,	4,	0,	0,170,	10.0f,	3,	3,	7,	9},
	{ &imgSpear,	1,	30,	1,	1.2f,	60,	2,	0,	0,170,	10.0f,	3,	3,	7,	8 },
	{ &imgAxe,		1,	35,	0,	1.1f,	5,	2,	0,	-50,230,10.0f,	2,	2,	6,	13 },
	{ &imgScythe,	1,	25,	0,	2.5f,	20,	3,	0,	-50,170,10.0f,	6,	2,	6,	10 },
	{ &imgPickaxe,	1,	15,	4,	1.2f,	40,	3,	0,	0,170,	10.0f,	5,	5,	7,	10 },
	{ &imgArrow,	1,	10,	1,	4.0f,	80,	3,	0,	0,170,	10.0f,	2,	2,	7,	5 },
	{ &imgKitchenKnife,1,30,1,	1.2f,	10,	4,	0,	0,170,	10.0f,	4,	4,	7,	9 },
	{ &imgShovel,	1,	30,	2,	1.2f,	20,	2,	0,	0,170,	10.0f,	4,	4,	7,	10 },
	{ &imgCutterKnife,1,20,	3,	2.0f,	5,	3,	0,	0,170,	10.0f,	7,	7,	7,	8 },
	// 段階を超えるとLv2の武器が出やすくなる
	{ &imgMagicStaff,2,	25,	5,	1.1f,	5,	6,	0,	0,170,	10.0f,	2,	2,	7,	10 },//10
	{ &imgBayonet,	2,	30,	2,	2.0f,	15,	4,	0,	0,170,	10.0f,	3,	3,	7,	12 },
	{ &imgMagicHand,2,	20,	2,	3.0f,	80,	3,	0,	0,170,	10.0f,	2,	2,	7,	8 },
	{ &imgFireSword,2,	40,	2,	1.5f,	20,	3,	0,	0,170,	10.0f,	5,	5,	7,	15 },
	{ &imgKey,		2,	5,	10,	20.0f,	5,	2,	0,	0,170,	10.0f,	3,	3,	7,	15 },
	{ &imgPencil,	2,	15,	3,	2.0f,	25,	4,	0,	0,170,	10.0f,	3,	3,	7,	7 },
	{ &imgScissors,	2,	60,	1,	1.1f,	5,	2,	0,	0,170,	10.0f,	5,	5,	7,	20 },
	{ &imgRapier,	2,	40,	2,	1.4f,	60,	2,	0,	0,170,	10.0f,	4,	4,	7,	8 },
	// 段階を超えるとLv3の武器が出やすくなる
	{ &imgKatana,	3,	50, 3,	1.2f,	20, 4,	0,	0,170,	10.0f,	5,	3,	7,	9 },//18
	{ &imgWindmill,	3,	15,	5,	5.0f,	50,	5,	0,	0,170,	10.0f,	1,	1,	7,	8 },
	{ &imgCyberSword,3,	45, 2,	1.5f,	30,	3,	0,	0,170,	10.0f,	5,	5,	7,	9 },
	{ &imgPixelSword,3,	60, 1,	1.5f,	5,	2,	0,	50,150,	8.0f,	7,	7,	7,	15 },
	{ &imgNaginata,	3,	45,	0,	1.6f,	60,	2,	0,	0,170,	10.0f,	4,	4,	7,	15 },
	{ &imgSpatula,	3,	30,	7,	1.1f,	90,	5,	0,	0,170,	13.0f,	2,	2,	6,	15 },
	{ &imgUmbrella,	3,	2,	20,	1.0f,	100,3,	0,	0,170,	10.0f,	2,	2,	7,	10 },
	// 段階を超えるとLv4の武器が出やすくなる
	{ &imgMightyHammer,4,80,0,	1.1f,	25,	5,	0,	50,270,	10.0f,	1,	1,	4,	25 },//25
	{ &imgChainsaw,	4,	60,	5,	1.5f,	50,	4,	0,	-0,250,	10.0f,	7,	7,	6,	20 },
	{ &imgDarkSword,4,	70, 0,	2.0f,	30,	3,	0,	0,170,	10.0f,	5,	5,	7,	20 },
	{ &imgDragonKeyring,4,45,15,1.5f,	80,	6,	0,	0,170,	10.0f,	8,	8,	6,	15 },
	{ &imgLaserSaber,4,	75,	3,	1.2f,	40,	4,	0,	0,170,	10.0f,	5,	5,	7,	10 },
	{ &imgTheLegendary,4,180,-10,1.0f,	100,1,	0,	0,170,	10.0f,	10,	10,	7,	20 },
	{ &imgCutterBlade,4,50,	10,	3.0f,	15,	3,	0,	0,170,	10.0f,	7,	7,	7,	15 }
};

int LoadGraphWithCheck(const char* file);
int LoadSoundMemWithCheck(const char* file);
