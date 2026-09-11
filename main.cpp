#include "DxLib.h"
#include "nuku.h"	// ヘッダーファイルをインクルード
#include <string>

// 定数の定義
const int WIDTH = 1200, HEIGHT = 720;	// ウィンドウの幅と高さのピクセル数
const int FPS = 60;
const int PLAYER_POWER_MAX = 100;

int mainX = (WIDTH / 10) * 7;	// 武器収集エリアにおいての中心X座標

// 武器の種類(全部使うかは不明)
int imgSword, imgScythe, imgSpear, imgAxe, imgKatana, imgMightyHammer,imgPickaxe,imgArrow,imgBayonet,imgChainsaw;
int imgCutterBlade, imgCyberSword, imgDarkSword, imgDragonKeyring, imgFireSword, imgKey, imgKitchenKnife, imgLaserSaber, imgMagicHand, imgNaginata;
int imgPencil, imgPixelSword, imgRapier, imgScissors, imgScrewdriver, imgShinai, imgShovel, imgSpatula, imgToySword, imgUmbrella;
int imgCutterKnife, imgWindmill, imgTheLegendary,imgMagicStaff;

int imgGround,imgTable, imgSky;
float weaponAngle = 0;	// 武器の角度
float weaponOut = 0;	// 武器の高さ（Y座標）
int power;
float rightSwordRange = 1;	// 武器の回転範囲　広げることが出来る
float leftSwordRange = 1;

int upKeyCheck;	// 上矢印キーが押された瞬間を判別する
int outCount;	// 上に上がる回数を設定する　このカウントによって状態を切り替える

bool endRound = false;	// 剣が引き抜かれたことを表す

int winningLine;	// ここまでくると引き抜ける値
int w, h;	// 画像のサイズ

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("ゲーム");	// ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32);	// ウィンドウの大きさとカラービット数の指定
	ChangeWindowMode(true);	// ウィンドウモードで起動
	if (DxLib_Init() == -1)return -1;	// ライブラリ初期化　エラーが起きたら終了
	SetDrawScreen(DX_SCREEN_BACK);	// 描画面を裏画面にする

	InitGame();
	Weapon weapon = { imgSword ,1,0,-100,170,10.0f,3,3,7,10};

	while(1)
	{
		ClearDrawScreen();
		DrawImageEnlarge(imgSky, mainX, HEIGHT / 1.4f, 30, 20);	// 空を描く
		//Weapon weapon;
		//Weapon weapon = {imgSword,1,0,0,15,15.0f,3,3};
		// 武器を描く
		DrawImageRotateEnlarge(weapon.img, mainX + weapon.xPlus, (HEIGHT * 1.4f + weaponOut)-weapon.yPlus, weapon.size, weaponAngle+weapon.iniAngle);
		DrawImageEnlarge(imgGround, mainX, HEIGHT / 1.5f, 30, 30);	// 地面を描く　武器より前面
		
		GetGraphSize(weapon.img, &w, &h);	// 現在の武器の大きさを取得
		winningLine = (h / 10) * weapon.phase;
		//DrawImageEnlarge(imgTable, WIDTH / 2, HEIGHT / 1.3, 10, 10);
		
		if(weaponOut-0.5>winningLine)
		{
			weaponOut -= 0.3f;
		}

		if (outCount == 0) {
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {	// 横矢印キーが押されたらそちらに傾かせる
				if (leftSwordRange > weaponAngle) {	// 回転範囲を下回っている場合は速く動かせる
					weaponAngle += 0.5f;
					if (CheckHitKey(KEY_INPUT_LEFT) == 0&& weaponOut <= winningLine)weaponOut += 0.07f/ weapon.weight;	// 少し上にも動く
				}
				else if (leftSwordRange < weapon.rangeL + weaponOut / 2) {	// 回転範囲を超えようとした場合、武器に設定された範囲まではゆっくり広げることが出来る
					weaponAngle += 0.1f;
					leftSwordRange += 0.1f;	// 回転と同時に回転範囲も広がる
				}
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1) {	// 上の反対バージョン
				if (-rightSwordRange < weaponAngle) {
					weaponAngle -= 0.5f;
					if (CheckHitKey(KEY_INPUT_RIGHT) == 0 && weaponOut <= winningLine)weaponOut += 0.07f / weapon.weight;
				}
				else if (rightSwordRange < weapon.rangeR + weaponOut / 2) {
					weaponAngle -= 0.1f;
					rightSwordRange += 0.1f;
				}
			}
		}

		if (upKeyCheck == 1&&endRound==false)
		{
			if (weaponOut > winningLine && -5 < weaponAngle && weaponAngle < 5)	// 十分上に引っ張られているかつ真ん中にある状態の場合、引き抜く
			{
				endRound = true;
				weaponAngle = 0;
				outCount = 100;
			}
			if(weaponOut <= winningLine){
				if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 || weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
					if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 && weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
						weaponOut += (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2))) / 10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2))) / weapon.weight;
						outCount = 10;
					}
					else if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 || weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
						weaponOut += (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2))) / 10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2))) / weapon.weight;
						outCount = 5;
					}
				}
			}
		}

		if (outCount > 50) {
			weaponOut += 0.05f;
			outCount--;
		}
		else if (outCount > 0) {
			outCount--;
			weaponOut += 0.02f;
			weaponOut += (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2)))/10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2)))/ weapon.weight;
			if(weaponAngle==0)weaponOut += 0.5;
		}
		else
		{
			outCount = 0;
			//rightSwordRange = (weapon.rangeR + weaponOut / 2) / 2.0f;
			//leftSwordRange = (weapon.rangeL + weaponOut / 2) / 2.0f;
			if(endRound==true)
			{
				int randomNum = GetRand(9);
				//randomNum = 32;
				// 武器のステータス決め　　名前　　　レベル		　X	  Y				右　左	   重さ
				//  									　初期角度			大きさ		 終了位置
				if(randomNum==0)weapon = { imgSword,	1,	0,	-100,170,	10.0f,	3,	3,	7,	10 };
				if(randomNum==1)weapon = { imgShinai,	1,	0,	-100,170,	10.0f,	3,	3,	7,	9 };
				if(randomNum==2)weapon = { imgSpear,	1,	0,	-100,170,	10.0f,	3,	3,	7,	8};
				if(randomNum==3)weapon = { imgAxe,		1,	0,	-50,230,	10.0f,	2,	2,	6,	13};
				if(randomNum==4)weapon = { imgScythe,	1,	0,	-150,170,	10.0f,	6,	2,	6,	10};
				if(randomNum==5)weapon = { imgPickaxe,	1,	0,	-100,170,	10.0f,	5,	5,	7,	10};
				if(randomNum==6)weapon = { imgArrow,	1,	0,	-100,170,	10.0f,	2,	2,	7,	5};
				if(randomNum==7)weapon = { imgKitchenKnife,1,0,-100,170,	10.0f,	4,	4,	7,	9};
				if(randomNum==8)weapon = { imgShovel,	1,	0,	-100,170,	10.0f,	4,	4,	7,	10 };
				if(randomNum==9)weapon = { imgCutterKnife,1,0,	-100,170,	10.0f,	7,	7,	7,	8 };

				if(randomNum==10)weapon = { imgCutterBlade,1,0,	-100,170,	10.0f,	7,	7,	7,	15};
				if(randomNum==11)weapon = { imgBayonet,	1,	0,	-100,170,	10.0f,	3,	3,	7,	12};
				if(randomNum==12)weapon = { imgMagicHand,1,	0,	-100,170,	10.0f,	2,	2,	7,	8 };
				if(randomNum==13)weapon = { imgFireSword,1,	0,	-100,170,	10.0f,	5,	5,	7,	15};
				if(randomNum==14)weapon = { imgKey,		1,	0,	-100,170,	10.0f,	3,	3,	7,	15};
				if(randomNum==15)weapon = { imgPencil,	1,	0,	-100,170,	10.0f,	3,	3,	7,	7 };
				if(randomNum==16)weapon = { imgScissors,1,	0,	-100,170,	10.0f,	5,	5,	7,	20 };
				if(randomNum==18)weapon = { imgScrewdriver,1,0,	-100,170,	15.0f,	3,	3,	7,	10 };
				if(randomNum==19)weapon = { imgToySword,1,	0,	-100,170,	13.0f,	1,	1,	5,	8 };
				if(randomNum==20)weapon = { imgRapier,	1,	0,	-100,170,	10.0f,	4,	4,	7,	8 };


				if(randomNum==21)weapon = { imgKatana,	1,	0,	-100,170,	10.0f,	5,	3,	7,	9};
				if(randomNum==22)weapon = { imgWindmill,1,	0,	-100,170,	10.0f,	1,	1,	7,	8 };
				if(randomNum==23)weapon = { imgCyberSword,1,0,	-100,170,	10.0f,	5,	5,	7,	9};
				if(randomNum==24)weapon = { imgPixelSword,1,0,	-100,170,	10.0f,	6,	6,	7,	15 };
				if(randomNum==25)weapon = { imgNaginata,1,	0,	-100,170,	10.0f,	4,	4,	7,	15 };
				if(randomNum==26)weapon = { imgSpatula,	1,	0,	-100,170,	13.0f,	2,	2,	6,	15 };
				if(randomNum==27)weapon = { imgUmbrella,1,	0,	-100,170,	10.0f,	2,	2,	7,	10 };


				if(randomNum==28)weapon = { imgMightyHammer,1,0,	-50,270,	10.0f,	1,	1,	4,	25};
				if(randomNum==29)weapon = { imgChainsaw,	1,	0,	-100,250,	10.0f,	7,	7,	6,	20};
				if(randomNum==30)weapon = { imgDarkSword,1,	0,	-100,170,	10.0f,	5,	5,	7,	20};
				if(randomNum==31)weapon = { imgDragonKeyring,1,0,-100,170,	10.0f,	8,	8,	7,	15};
				if(randomNum==32)weapon = { imgLaserSaber,1,0,	-100,170,	10.0f,	5,	5,	7,	10 };
				if(randomNum==33)weapon = { imgTheLegendary,1,0,-100,170,	10.0f,	10,	10,	7,	20 };
				if(randomNum==34)weapon = { imgMagicStaff,1,0,	-100,170,	10.0f,	2,	2,	7,	10 };
				 

				weaponOut = 0;	
				weaponAngle = 0;	
				rightSwordRange = 1;
				leftSwordRange = 1;
				endRound = false;
			}
		}

		if (upKeyCheck == -1)weaponOut -= (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2))) / 10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2))) / 5;

		/*if (weaponOut > 40) {
			outCount = 50;
			endRound = true;
		}*/


		if (CheckHitKey(KEY_INPUT_UP) == 0) {	// 押した瞬間離した瞬間を判定する
			if (upKeyCheck > 0) upKeyCheck = -1;
			else upKeyCheck = 0;
		}
		else upKeyCheck++;

		//if (CheckHitKey(KEY_INPUT_DOWN) == 1)weaponOut-=0.1;

		ScreenFlip();	// 裏画面の内容を表画面に反映させる
		//WaitKey();
		WaitTimer(1000 / FPS);	// 一定時間待つ
		if (ProcessMessage() == -1)break;	// Windowsから情報を受け取りエラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;	// ESCキーが押されたら終了
	}
	DxLib_End();	// DXライブラリ使用の終了処理
	return 0;
}

// 初期化用の関数
void InitGame(void)
{
	imgSword = LoadGraphWithCheck("image/Nuku_SwordSize.png");
	imgScythe = LoadGraphWithCheck("image/Nuku_LargeScythe.png");
	imgSpear = LoadGraphWithCheck("image/Nuku_Spear.png");
	imgAxe = LoadGraphWithCheck("image/Nuku_Axe.png");
	imgKatana = LoadGraphWithCheck("image/Nuku_NewKatana.png");
	imgMightyHammer = LoadGraphWithCheck("image/Nuku_MightyHammer.png");
	imgPickaxe = LoadGraphWithCheck("image/Nuku_Pickaxe.png");
	imgArrow = LoadGraphWithCheck("image/Nuku_Arrow.png");
	imgBayonet = LoadGraphWithCheck("image/Nuku_Bayonet.png");
	imgChainsaw = LoadGraphWithCheck("image/Nuku_Chainsaw.png");
	imgCutterBlade = LoadGraphWithCheck("image/Nuku_CutterBlade.png");
	imgCyberSword = LoadGraphWithCheck("image/Nuku_CyberSword.png");
	imgDarkSword = LoadGraphWithCheck("image/Nuku_DarkSword.png");
	imgDragonKeyring = LoadGraphWithCheck("image/Nuku_DragonKeyring.png");
	imgFireSword = LoadGraphWithCheck("image/Nuku_FireSword.png");
	imgKey = LoadGraphWithCheck("image/Nuku_Key.png");
	imgKitchenKnife = LoadGraphWithCheck("image/Nuku_KitchenKnife.png");
	imgLaserSaber = LoadGraphWithCheck("image/Nuku_LaserSaber.png");
	imgMagicHand = LoadGraphWithCheck("image/Nuku_MagicHand.png");
	imgNaginata = LoadGraphWithCheck("image/Nuku_Naginata.png"); 
	imgPencil = LoadGraphWithCheck("image/Nuku_Pencil.png");
	imgPixelSword = LoadGraphWithCheck("image/Nuku_PixelSword.png");
	imgRapier = LoadGraphWithCheck("image/Nuku_Rapier.png");
	imgScissors = LoadGraphWithCheck("image/Nuku_Scissors.png");
	imgScrewdriver = LoadGraphWithCheck("image/Nuku_Screwdriver.png");
	imgShinai = LoadGraphWithCheck("image/Nuku_Shinai.png");
	imgShovel = LoadGraphWithCheck("image/Nuku_Shovel.png");
	imgSpatula = LoadGraphWithCheck("image/Nuku_Spatula.png");
	imgToySword = LoadGraphWithCheck("image/Nuku_ToySword.png");
	imgUmbrella = LoadGraphWithCheck("image/Nuku_Umbrella.png");
	imgCutterKnife = LoadGraphWithCheck("image/Nuku_CutterKnife.png");
	imgWindmill = LoadGraphWithCheck("image/Nuku_Windmill.png");
	imgTheLegendary = LoadGraphWithCheck("image/Nuku_TheLegendary.png");
	imgMagicStaff = LoadGraphWithCheck("image/Nuku_MagicStaff.png");
		




	imgTable = LoadGraphWithCheck("image/Nuku_Table.png");
	imgGround = LoadGraphWithCheck("image/Nuku_Ground.png");
	imgSky = LoadGraphWithCheck("image/Nuku_Sky.png");
}

// 中心座標と大きさを指定して画像を表示する関数
void DrawImageEnlarge(int img, int x, int y, int enlargeX, int enlargeY)
{
	int w, h;
	GetGraphSize(img, &w, &h);
	DrawExtendGraph(x - (w / 2) * enlargeX, y - (h / 2) * enlargeY, x + (w / 2) * enlargeX, y + (h / 2) * enlargeY, img, true);
}

// 角度、中心座標と大きさを指定して画像を表示する関数
void DrawImageRotateEnlarge(int img, int x, float y, double ExtRate,float angle)
{
	float w, h;
	GetGraphSizeF(img, &w, &h);
	DrawRotaGraph2F(x - (w / 3), y+(h/ 2), w/2 , (h/2+weaponOut)+20, ExtRate, (angle * DX_PI_F / 180.0f), img, true);
}

int LoadGraphWithCheck(const char* file)
{
	int res = LoadGraph(file);
	if (res == -1) { MessageBox(GetMainWindowHandle(), file, "画像読み込みに失敗", MB_OK | MB_ICONSTOP); }
	return res;
}

int LoadSoundMemWithCheck(const char* file)
{
	int res = LoadSoundMem(file);
	if (res == -1) { MessageBox(GetMainWindowHandle(), file, "音声読み込みに失敗", MB_OK | MB_ICONSTOP); }
	return res;
}