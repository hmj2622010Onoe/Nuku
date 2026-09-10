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

int imgGround,imgTable, imgSky;
float weaponAngle = 0;	// 武器の角度
float weaponOut = 0;	// 武器の高さ（Y座標）
int power;
float rightSwordRange = 1;	// 武器の回転範囲　広げることが出来る
float leftSwordRange = 1;

int upKeyCheck;	// 上矢印キーが押された瞬間を判別する
int outCount;	// 上に上がる回数を設定する　このカウントによって状態を切り替える

bool endRound = false;	// 剣が引き抜かれたことを表す

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("ゲーム");	// ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32);	// ウィンドウの大きさとカラービット数の指定
	ChangeWindowMode(true);	// ウィンドウモードで起動
	if (DxLib_Init() == -1)return -1;	// ライブラリ初期化　エラーが起きたら終了
	SetDrawScreen(DX_SCREEN_BACK);	// 描画面を裏画面にする

	InitGame();
	Weapon weapon = { imgSword,1,0,0,15,15.0f,3,3};

	while(1)
	{
		ClearDrawScreen();
		DrawImageEnlarge(imgSky, mainX, HEIGHT / 1.4f, 30, 20);	// 空を描く
		//Weapon weapon;
		//Weapon weapon = {imgSword,1,0,0,15,15.0f,3,3};
		// 武器を描く
		DrawImageRotateEnlarge(weapon.img, mainX + weapon.xPlus, (HEIGHT * 1.4f + weaponOut)-weapon.yPlus, weapon.size, weaponAngle+weapon.iniAngle);
		DrawImageEnlarge(imgGround, mainX, HEIGHT / 1.5f, 30, 30);	// 地面を描く　武器より前面
		//DrawImageEnlarge(imgTable, WIDTH / 2, HEIGHT / 1.3, 10, 10);
		
		if (outCount == 0) {
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {	// 横矢印キーが押されたらそちらに傾かせる
				if (leftSwordRange > weaponAngle) {	// 回転範囲を下回っている場合は速く動かせる
					weaponAngle += 0.5f;
					if (CheckHitKey(KEY_INPUT_LEFT) == 0)weaponOut += 0.02f;	// 少し上にも動く
				}
				else if (leftSwordRange < weapon.rangeL + weaponOut / 2) {	// 回転範囲を超えようとした場合、武器に設定された範囲まではゆっくり広げることが出来る
					weaponAngle += 0.1f;
					leftSwordRange += 0.1f;	// 回転と同時に回転範囲も広がる
				}
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1) {	// 上の反対バージョン
				if (-rightSwordRange < weaponAngle) {
					weaponAngle -= 0.5f;
					if (CheckHitKey(KEY_INPUT_RIGHT) == 0)weaponOut += 0.02f;
				}
				else if (rightSwordRange < weapon.rangeR + weaponOut / 2) {
					weaponAngle -= 0.1f;
					rightSwordRange += 0.1f;
				}
			}
		}

		if (upKeyCheck == -1)
		{
			if (weaponOut > 30 && -5 < weaponAngle && weaponAngle < 5)	// 十分上に引っ張られているかつ真ん中にある状態の場合、引き抜く
			{
				endRound = true;
				weaponAngle = 0;
				outCount = 100;
			}
			if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 || weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
				if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 && weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
					weaponOut += (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2))) / 10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2))) / 10;
					outCount = 10;
				}
				else if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 || weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
					weaponOut += (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2))) / 10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2))) / 10;
					outCount = 5;
				}
			}
		}

		if (outCount > 50) {
			weaponOut += 0.05;
			outCount--;
		}
		else if (outCount > 1) {
			outCount--;
			weaponOut += 0.05;
			weaponOut += (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2)))/10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2)))/10;
			if(weaponAngle==0)weaponOut += 0.5;
		}
		else if (outCount > 0) {
			outCount--;
			weaponOut += 0.05;
			weaponOut -= (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2)))/10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2)))/10;
		}
		else if(outCount==-1)
		{
			outCount = 0;
			rightSwordRange = (weapon.rangeR + weaponOut / 2) / 2.0f;
			leftSwordRange = (weapon.rangeL + weaponOut / 2) / 2.0f;
			if(endRound==true)
			{
				Weapon weapon = { imgKatana,1,0,0,15,15.0f,4,4 };
				weaponAngle = 0;	
				weaponOut = 0;	
				rightSwordRange = 1;
				leftSwordRange = 1;
				endRound = false;
			}
		}

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
	imgScythe = LoadGraphWithCheck("image/Nuku_Scythe.png");
	imgSpear = LoadGraphWithCheck("image/Nuku_Spear.png");
	imgAxe = LoadGraphWithCheck("image/Nuku_Axe.png");
	imgKatana = LoadGraphWithCheck("image/Nuku_Katana.png");
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