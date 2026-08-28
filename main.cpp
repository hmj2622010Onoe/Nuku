#include "DxLib.h"
#include "nuku.h"	// ヘッダーファイルをインクルード

// 定数の定義
const int WIDTH = 1200, HEIGHT = 720;	// ウィンドウの幅と高さのピクセル数
const int FPS = 60;
const int PLAYER_POWER_MAX = 100;

int imgHand, imgSword;

int imgGround,imgTable, imgSky;

int power;
int rightSwordRange = 2;
int leftSwordRange = 2;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("ゲーム");	// ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32);	// ウィンドウの大きさとカラービット数の指定
	ChangeWindowMode(true);	// ウィンドウモードで起動
	if (DxLib_Init() == -1)return -1;	// ライブラリ初期化　エラーが起きたら終了
	SetDrawScreen(DX_SCREEN_BACK);	// 描画面を裏画面にする

	InitGame();

	while(1)
	{
		//DrawImageEnlarge(imgSky, WIDTH / 2, HEIGHT / 1.4, 30, 20);

		//DrawImageEnlarge(imgHand, WIDTH / 2, HEIGHT / 1.3, 10, 10);
		//DrawImageEnlarge(imgGround, WIDTH / 2, HEIGHT / 1.5, 30, 30);
		DrawImageEnlarge(imgTable, WIDTH / 2, HEIGHT / 1.3, 10, 10);

		DrawImageRotateEnlarge(imgSword, 0, 0,10.0f,0);
		ScreenFlip();	// 裏画面の内容を表画面に反映させる
		WaitKey();
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
	imgHand = LoadGraphWithCheck("image/Nuku_Hand.png");
	imgSword = LoadGraphWithCheck("image/Nuku_Sword.png");

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
void DrawImageRotateEnlarge(int img, int x, int y, double ExtRate,int angle)
{
	int w, h;
	GetGraphSize(img, &w, &h);
	//DrawRotaGraph2(x - (w / 2), y - (h / 2), x, y,ExtRate,(angle*DX_PI_F/180.0f), img, true);
	DrawRotaGraph2(x , y, x, y,ExtRate,(angle*DX_PI_F/180.0f), img, true);
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