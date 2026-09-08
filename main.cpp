#include "DxLib.h"
#include "nuku.h"	// ヘッダーファイルをインクルード

// 定数の定義
const int WIDTH = 1200, HEIGHT = 720;	// ウィンドウの幅と高さのピクセル数
const int FPS = 60;
const int PLAYER_POWER_MAX = 100;

// 武器の種類(全部使うかは不明)
int imgSword, imgScythe, imgSpear, imgAxe, imgKatana, imgMightyHammer,imgPickaxe,imgArrow,imgBayonet,imgChainsaw;
int imgCutterBlade, imgCyberSword, imgDarkSword, imgDragonKeyring, imgFireSword, imgKey, imgKitchenKnife, imgLaserSaber, imgMagicHand, imgNaginata;
int imgPencil, imgPixelSword, imgRapier, imgScissors, imgScrewdriver, imgShinai, imgShovel, imgSpatula, imgToySword, imgUmbrella;

int imgGround,imgTable, imgSky;
int weaponAngle = 0;
int weaponOut = 0;
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
		ClearDrawScreen();
		DrawImageEnlarge(imgSky, WIDTH / 2, HEIGHT / 1.4, 30, 20);
		DrawImageRotateEnlarge(imgSword, WIDTH/2, HEIGHT*1.5+(weaponOut/80.0f),15.0f,weaponAngle/10.0f);
		DrawImageEnlarge(imgGround, WIDTH / 2, HEIGHT / 1.5, 30, 30);
		//DrawImageEnlarge(imgTable, WIDTH / 2, HEIGHT / 1.3, 10, 10);
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1)weaponAngle++;
		if (CheckHitKey(KEY_INPUT_LEFT) == 1)weaponAngle--;
		if (CheckHitKey(KEY_INPUT_UP) == 1)weaponOut--;
		if (CheckHitKey(KEY_INPUT_DOWN) == 1)weaponOut++;

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

// 角度、中心座標と大きさを指定して画像を表示する関数 ←こいつと格闘中！！
void DrawImageRotateEnlarge(int img, int x, int y, double ExtRate,int angle)
{
	int w, h;
	GetGraphSize(img, &w, &h);
	DrawRotaGraph2(x - (w / 2),y - (h/ 2), w/2 , h-(weaponOut/20) ,ExtRate,(angle * DX_PI_F/180.0f), img, true);
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