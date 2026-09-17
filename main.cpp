
#include "DxLib.h"
#include "nuku.h"	// ヘッダーファイルをインクルード
#include <string>
#include <cmath>	// 四捨五入用

// 定数の定義
const int WIDTH = 1200, HEIGHT = 720;	// ウィンドウの幅と高さのピクセル数
const int FPS = 60;
const int PLAYER_POWER_MAX = 100;

int mainX = (WIDTH / 10) * 7;	// 武器収集エリアにおいての中心X座標

int attackX = (WIDTH / 10) * 2;	// 攻撃エリアにおいての中心X座標

int imgIconD, imgIconDb, imgIconC, imgIconCm, imgIconS;

int imgGround,imgTable, imgSky,imgLight,imgField,imgFieldUI,imgFieldCover;

int imgEne, imgEneK, imgEneKGreen, imgEneKYellow,imgEneKRed;

int imgPixelSword2;


float weaponAngle = 0;	// 武器の角度
float weaponOut = 0;	// 武器の高さ（Y座標）
int power;
float rightSwordRange = 1;	// 武器の回転範囲　広げることが出来る
float leftSwordRange = 1;

float attackOut;	// 攻撃する武器が上に動く

int selBoxS=10;	// 選ぶ枠の大きさ
int lottery = 3;	// 抽選される武器の数
const int BoxLayWidth = 400;	// 箱を配置するスペースの大きさ
int layBoxS = 0;	// 箱の数が多いほど大きさが小さくなる幅
int nowSelBox = 0;	// 現在選ばれているBox
int lotteryMax = 9;	// 抽選の最大数
int box[32];

int upKeyCheck;	// 上矢印キーが押された瞬間を判別する
int leftKeyCheck;	
int rightKeyCheck;	
int downKeyCheck;	
int outCount;	// 上に上がる回数を設定する　このカウントによって状態を切り替える

int lotteryItem[6]={};

int pixelStyle = 6;	// ピクセル剣のアニメーションの状態を表す
int pixelOut = 0;	// ピクセル剣の高さ

int enemyMove = 0;	// 敵が上下に動く
int gamePhase = 1;	// 敵を倒すと進む
int enemyHP = 200;	// 敵のHP
int enemyMaxHP = 200;	// 敵の現フェーズのMAXHP
int enemyStyle = imgEneK;	// 敵の見た目
int bonusDamage = 0;	// 追加のダメージ

bool endRound = false;	// 剣が引き抜かれたことを表す
bool lotteryFlag = false;	// 抽選を行うか
bool selectTime = false;	// 選択できる時間か
bool attackTime = false;	// 攻撃できる時間か

int winningLine;	// ここまでくると引き抜ける値
int w, h;	// 画像のサイズ

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("ゲーム");	// ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32);	// ウィンドウの大きさとカラービット数の指定
	ChangeWindowMode(true);	// ウィンドウモードで起動
	if (DxLib_Init() == -1)return -1;	// ライブラリ初期化　エラーが起きたら終了0
	SetDrawScreen(DX_SCREEN_BACK);	// 描画面を裏画面にする

	InitGame();
	Weapon weapon = weaponData[0];
	
	while(1)
	{
		ClearDrawScreen();
		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255-(weaponOut/winningLine)* 255);	// ブレンドモード設定
		
		DrawImageEnlarge(imgSky, mainX, HEIGHT / 2, 40, 30);	// 空を描く

		DrawImageEnlarge(imgGround, mainX, HEIGHT / 2.1f, 33, 28);	// 地面を描く　武器より後面
		//Weapon weapon;
		//Weapon weapon = {imgSword,1,0,0,15,15.0f,3,3};
		// 武器を描く
		if (*weapon.img == imgPixelSword&&attackTime==false) {	// ピクセル剣の場合用のアニメーション
			if (weaponAngle<-23)pixelStyle = 0;
			if (-24 < weaponAngle && weaponAngle < -20)pixelStyle = 1;
			if (-19 < weaponAngle && weaponAngle < -15)pixelStyle = 2;
			if (-14 < weaponAngle && weaponAngle < -10)pixelStyle = 3;
			if (-9 < weaponAngle && weaponAngle < -5)pixelStyle = 4;
			if(-4<weaponAngle&&weaponAngle<5)pixelStyle = 5;
			if(4< weaponAngle && weaponAngle <10)pixelStyle = 6;
			if(9< weaponAngle && weaponAngle <15)pixelStyle = 7;
			if(14<weaponAngle && weaponAngle <20)pixelStyle = 8;
			if(19< weaponAngle && weaponAngle <25)pixelStyle = 9;
			if(24<weaponAngle)pixelStyle = 10;

			pixelOut = static_cast<int>(std::round(weaponOut / 2));	// 概ねドット状に上下させるため整数に変換
			// ドット専用の描画用　角度を使わない
			DrawImageAnimation(*weapon.img, mainX + weapon.xPlus, (HEIGHT * 1.4f + pixelOut*2) - weapon.yPlus, weapon.size, pixelStyle);
		}
		// 通常時の描画用
		else if(attackTime == false)DrawImageRotateEnlarge(*weapon.img, mainX + weapon.xPlus, (HEIGHT * 1.4f + weaponOut)-weapon.yPlus, weapon.size, weaponAngle+weapon.iniAngle);
		DrawImageEnlarge(imgGround, mainX, HEIGHT / 2.1f, 40, 30);	// 地面を描く　武器より前面

		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((weaponOut / winningLine) * 100));	// ブレンドモード設定
		//DrawImageEnlarge(imgLight, mainX, HEIGHT / 1.2, 20, 25);	// 光を描く　
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);	// ブレンドモードを元に戻す

		DrawImageEnlarge(imgField, mainX-100, HEIGHT / 2, 29, 40);
		DrawImageEnlarge(imgFieldCover, mainX-100, HEIGHT / 2, 29, 30);
		DrawImageEnlarge(imgFieldUI, mainX-100, HEIGHT / 2, 29, 30);
		if (gamePhase == 1)enemyStyle = imgEneK;
		DrawImageEnlarge(enemyStyle, attackX+22, HEIGHT / 8+enemyMove, 6+(gamePhase*2), 6 + (gamePhase * 2));
		float enePercent;
		enePercent = static_cast<float>(enemyHP)/ static_cast<int>(enemyMaxHP);
		DrawBoxAA(attackX -150+22,HEIGHT/8-60, attackX -150+22+(enePercent * 300),HEIGHT / 8 - 30,GetColor(255-(enePercent * 255),(enePercent*255),0),true,1);
		DrawBox(attackX -150+22,HEIGHT/8-60, attackX +172,HEIGHT / 8 - 30,GetColor(0,0,0),false,5);
		SetFontSize(24);
		DrawFormatString(attackX - 150 + 26, HEIGHT / 8-56 , GetColor(255,255,255), "%d",enemyHP);
		SetFontSize(56);
		DrawFormatString(attackX - 150 + 26, HEIGHT -100 , GetColor(255,255,255), "+%d",bonusDamage);
		GetGraphSize(*weapon.img, &w, &h);	// 現在の武器の大きさを取得
		winningLine = (h / 10) * weapon.phase;
		//DrawImageEnlarge(imgTable, WIDTH / 2, HEIGHT / 1.3, 10, 10);
		
		while (weaponOut - 0.5 > winningLine && endRound == false)	// 既定のラインより上であれば上昇を抑える
		{
			weaponOut -= 0.5f;
		}
		if (outCount == 0) {
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {	// 横矢印キーが押されたらそちらに傾かせる
				if (leftSwordRange > weaponAngle) {	// 回転範囲を下回っている場合は速く動かせる
					weaponAngle += 0.5f;
					if (CheckHitKey(KEY_INPUT_LEFT) == 0 && weaponOut <= winningLine)weaponOut += 0.07f / weapon.weight;	// 少し上にも動く
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

		if (upKeyCheck == 1&&endRound==false)	// 上矢印キーが押された瞬間にまだ引き抜いていなければ
		{
			if (weaponOut > winningLine && -5 < weaponAngle && weaponAngle < 5&&endRound==false)	// 十分上に引っ張られているかつ真ん中にある状態の場合、引き抜く
			{
				endRound = true;
				weaponAngle = 0;	// 角度を真上に
				outCount = 110;	// 110回分上昇させる
	
			}
			if(weaponOut <= winningLine){	// まだ十分に引っ張られていない場合
				// 回転範囲が高さの半分より広くなっていた場合　高さを上げる　　これによりさらに回転範囲を広げることができるようになる
				if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 || weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
					if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 && weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
						weaponOut += (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2))) / 10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2))) / weapon.weight;
						outCount = 10;	// 左右両方広がっていれば10回分上昇
					}
					else if (weapon.rangeL + weaponOut / 2 - leftSwordRange < 0.5 || weapon.rangeR + weaponOut / 2 - rightSwordRange < 0.5) {
						weaponOut += (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2))) / 10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2))) / weapon.weight;
						outCount = 2;	// 左右どちらか広がっていれば2回分上昇
					}
				}
			}
		}

		if (outCount > 50) {	// 50回以上上昇する場合(つまり引き抜いた時)はしばらく上げ下げを行う
			if ( outCount % 30 == 0)weaponOut -= 2;
			if (outCount % 30 < 6)weaponOut += 0.4f;
			outCount--;
		}
		else if (outCount > 0) {	// 上に上昇させる
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
			if (endRound == true&&selectTime==false&&attackTime==false)	// 引き抜いた後の処理
			{
				for (int i = 0; i < lotteryMax; i++)
				{
					box[i] = 0;
				}

				lottery = weapon.nextLottery;
				nowSelBox = lottery / 2;
				for (int i = 0; i < lottery; i++) {
					int select;
					do { select = rand() % (lotteryMax); } while (box[select] != 0);
					box[select] = 1;
					lotteryItem[i] = GetRand(lotteryMax);
				}
				//DrawBox(mainX - selBoxS + lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (nowSelBox + 1) - 2, HEIGHT / 1.2f - selBoxS + lottery * layBoxS - 2, mainX + selBoxS - lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (nowSelBox + 1) + 2, HEIGHT / 1.2f + selBoxS - lottery * layBoxS + 2, 0xffffff, false, 5);
				//int randomNum = GetRand(17);
				//randomNum = 21;
				attackTime = true;
				attackOut = HEIGHT;	
				//selectTime = true;
				//endRound = false;
			}
		}

		if (attackTime == true) {// 攻撃フェーズ
			if (*weapon.img == imgPixelSword) {
				DrawImageRotateEnlarge(imgPixelSword2, attackX, 0+ attackOut, 5.0f, 180);
			}
			else DrawImageRotateEnlarge(*weapon.img,attackX+30,0+attackOut, 5.0f,180);
			attackOut -=20.0f;
				weaponOut = 0;
			if (attackOut < HEIGHT / 8) {
				weaponAngle = 0;
				rightSwordRange = 1;
				leftSwordRange = 1;


				if (weapon.cProbability > GetRand(100)) {
					int cDamage;
					cDamage = weapon.cMultiplier * weapon.damage;
					enemyHP -= cDamage + bonusDamage;
				}
				else enemyHP -= weapon.damage+bonusDamage;

				bonusDamage += weapon.pPower;
				selectTime = true;
				attackTime = false;
				if (enemyHP <= 0) {
					gamePhase++;
					if (gamePhase > 4)gamePhase = 4;
					if (gamePhase == 2) {
						enemyMaxHP = 750;
						lotteryMax = 17;
						enemyStyle = imgEneKGreen;
					}
					if (gamePhase == 3) {
						enemyMaxHP = 3000;
						lotteryMax = 24;
						enemyStyle = imgEneKYellow;
					}
					if (gamePhase == 4) {
						enemyMaxHP = 10000;
						lotteryMax = 31;
						enemyStyle = imgEneKRed;
					}
					enemyHP = enemyMaxHP;
				}
			}
		}
		
		if (selectTime == true) {	// 選ぶフェーズ
			for (int i = 0; i < lottery; i++) {
				DrawBox(mainX - selBoxS + lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (i + 1), HEIGHT / 1.2f - selBoxS + lottery * layBoxS, mainX + selBoxS - lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (i + 1), HEIGHT / 1.2f + selBoxS - lottery * layBoxS, 0x404040, true, 1);
				DrawBox(mainX - selBoxS + lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (i + 1), HEIGHT / 1.2f - selBoxS + lottery * layBoxS, mainX + selBoxS - lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (i + 1), HEIGHT / 1.2f + selBoxS - lottery * layBoxS, 0x000000, false, 5);
				//DrawImageEnlarge(*weaponData[lotteryItem[i]].img,mainX + lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (i + 1),HEIGHT/4, (15 - lottery)/6, (15 - lottery)/6);
			}
			DrawBox(mainX - selBoxS + lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (nowSelBox + 1) - 2, HEIGHT / 1.2f - selBoxS + lottery * layBoxS - 2, mainX + selBoxS - lottery * layBoxS - BoxLayWidth / 2 + BoxLayWidth / (lottery + 1) * (nowSelBox + 1) + 2, HEIGHT / 1.2f + selBoxS - lottery * layBoxS + 2, 0xffffff, false, 5);
			DrawBox(mainX - 200, HEIGHT / 15, mainX + 200, HEIGHT / 2.2f, 0xeeeeee, true, 1);
			DrawImageEnlarge(imgIconD, mainX - 150, HEIGHT / 15+50, 5, 5);
			DrawImageEnlarge(imgIconDb, mainX - 150, HEIGHT / 15+140, 5, 5);
			DrawImageEnlarge(imgIconS, mainX - 150, HEIGHT / 15+230, 5, 5);
			DrawImageEnlarge(imgIconC, mainX+30, HEIGHT / 15 + 50, 5, 5);
			DrawImageEnlarge(imgIconCm, mainX+30, HEIGHT / 15 + 140, 5, 5);
			SetFontSize(50);
			int cDamage;
			cDamage = weapon.cMultiplier * weapon.damage;
			DrawFormatString(mainX - 100, HEIGHT / 15 + 25,GetColor(0,0,0),"%d",weapon.damage);
			DrawFormatString(mainX - 100, HEIGHT / 15 + 115,GetColor(0,0,0),"%d",weapon.pPower);
			DrawFormatString(mainX - 100, HEIGHT / 15 + 205,GetColor(0,0,0),"%d",weapon.nextLottery);
			DrawFormatString(mainX +80, HEIGHT / 15 + 25,GetColor(0,0,0),"%d",cDamage);
			DrawFormatString(mainX +80, HEIGHT / 15 + 115,GetColor(0,0,0),"%d％", weapon.cProbability);
			weapon = weaponData[lotteryItem[nowSelBox]];
			if (leftKeyCheck == 1) { nowSelBox--;  }	
			if (rightKeyCheck == 1) { nowSelBox++;  }
			if (nowSelBox > lottery-1)nowSelBox = lottery-1;
			if (nowSelBox < 0)nowSelBox = 0;
			if (upKeyCheck == 1) {
				endRound = false;
				selectTime = false;
			}
		}

		// 上矢印キーを離した瞬間に少し武器の高さを下げる
		if (upKeyCheck == -1)weaponOut -= (1 - (leftSwordRange - (weapon.rangeL + weaponOut / 2))) / 10 + (1 - (rightSwordRange - (weapon.rangeR + weaponOut / 2))) / weapon.weight;

		if (CheckHitKey(KEY_INPUT_UP) == 0) {	// 押した瞬間離した瞬間を判定する
			if (upKeyCheck > 0) upKeyCheck = -1;
			else upKeyCheck = 0;
		}
		else upKeyCheck++;

		if (CheckHitKey(KEY_INPUT_LEFT) == 0) {	// 押した瞬間離した瞬間を判定する
			if (leftKeyCheck > 0) leftKeyCheck = -1;
			else leftKeyCheck = 0;
		}
		else leftKeyCheck++;
		if (CheckHitKey(KEY_INPUT_RIGHT) == 0) {	// 押した瞬間離した瞬間を判定する
			if (rightKeyCheck > 0) rightKeyCheck = -1;
			else rightKeyCheck = 0;
		}
		else rightKeyCheck++;
		if (CheckHitKey(KEY_INPUT_DOWN) == 0) {	// 押した瞬間離した瞬間を判定する
			if (downKeyCheck > 0) downKeyCheck = -1;
			else downKeyCheck = 0;
		}
		else downKeyCheck++;

		if (CheckHitKey(KEY_INPUT_D) == 1 && CheckHitKey(KEY_INPUT_R) == 1 && CheckHitKey(KEY_INPUT_G) == 1)
		{
			weaponOut = winningLine;
			weaponAngle = 0;
		}
		
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
	imgScythe = LoadGraphWithCheck("image/Nuku_+LargeScythe.png");
	imgSpear = LoadGraphWithCheck("image/Nuku_+Spear.png");
	imgAxe = LoadGraphWithCheck("image/Nuku_Axe.png");
	imgKatana = LoadGraphWithCheck("image/Nuku_NewKatana.png");
	imgMightyHammer = LoadGraphWithCheck("image/Nuku_MightyHammer.png");
	imgPickaxe = LoadGraphWithCheck("image/Nuku_Pickaxe.png");
	imgArrow = LoadGraphWithCheck("image/Nuku_Arrow.png");
	imgBayonet = LoadGraphWithCheck("image/Nuku_Bayonet.png");
	imgChainsaw = LoadGraphWithCheck("image/Nuku_Chainsaw.png");
	imgCutterBlade = LoadGraphWithCheck("image/Nuku_CutterBlade.png");
	imgCyberSword = LoadGraphWithCheck("image/Nuku_+CyberSword.png");
	imgDarkSword = LoadGraphWithCheck("image/Nuku_+DarkSword.png");
	imgDragonKeyring = LoadGraphWithCheck("image/Nuku_+DragonKeyring.png");
	imgFireSword = LoadGraphWithCheck("image/Nuku_FireSword.png");
	imgKey = LoadGraphWithCheck("image/Nuku_Key.png");
	imgKitchenKnife = LoadGraphWithCheck("image/Nuku_+KichenKnife.png");
	imgLaserSaber = LoadGraphWithCheck("image/Nuku_LaserSaber.png");
	imgMagicHand = LoadGraphWithCheck("image/Nuku_+MagicHand.png");
	imgNaginata = LoadGraphWithCheck("image/Nuku_+Naginata.png"); 
	imgPencil = LoadGraphWithCheck("image/Nuku_+Pencil.png");
	imgPixelSword = LoadGraphWithCheck("image/Nuku_Anition_PixelSword.png");
	imgPixelSword2 = LoadGraphWithCheck("image/Nuku_PixelSword.png");
	imgRapier = LoadGraphWithCheck("image/Nuku_+Rapier.png");
	imgScissors = LoadGraphWithCheck("image/Nuku_Scissors.png");
	imgShinai = LoadGraphWithCheck("image/Nuku_+Shinai.png");
	imgShovel = LoadGraphWithCheck("image/Nuku_+Shovel.png");
	imgSpatula = LoadGraphWithCheck("image/Nuku_+Spatula.png");
	imgUmbrella = LoadGraphWithCheck("image/Nuku_+Umbrella.png");
	imgCutterKnife = LoadGraphWithCheck("image/Nuku_CutterKnife.png");
	imgWindmill = LoadGraphWithCheck("image/Nuku_Windmill.png");
	imgTheLegendary = LoadGraphWithCheck("image/Nuku_TheLegendary.png");
	imgMagicStaff = LoadGraphWithCheck("image/Nuku_MagicStaff.png");
		

	imgEne = LoadGraphWithCheck("image/Nuku_Ghost.png");
	imgEneK = LoadGraphWithCheck("image/Nuku_KnightGhost.png");
	imgEneKGreen = LoadGraphWithCheck("image/Nuku_KnightGhostGreen.png");
	imgEneKYellow = LoadGraphWithCheck("image/Nuku_KnightGhostYellow.png");
	imgEneKRed = LoadGraphWithCheck("image/Nuku_KnightGhostRed.png");
	
	imgIconD = LoadGraphWithCheck("image/Nuku_Icon damage.png");
	imgIconDb = LoadGraphWithCheck("image/Nuku_Icon upDamage.png");
	imgIconC = LoadGraphWithCheck("image/Nuku_Icon critical.png");
	imgIconCm = LoadGraphWithCheck("image/Nuku_Icon criticalMuluti.png");
	imgIconS = LoadGraphWithCheck("image/Nuku_IconSelect.png");


	imgTable = LoadGraphWithCheck("image/Nuku_Table.png");
	imgGround = LoadGraphWithCheck("image/Nuku_+Ground.png");
	imgSky = LoadGraphWithCheck("image/Nuku_Sky1.png");
	imgLight = LoadGraphWithCheck("image/Nuku_Light.png");

	imgField = LoadGraphWithCheck("image/Nuku_Battlefield.png");
	imgFieldUI = LoadGraphWithCheck("image/Nuku_BattlefieldUI.png");
	imgFieldCover = LoadGraphWithCheck("image/Nuku_BattlefieldCover.png");
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
	DrawRotaGraph2F(x - (w / 2.0f), y, w/2.0f , (h/2.0f+weaponOut)+20, ExtRate, (angle * DX_PI_F / 180.0f), img, true);
}

void DrawImageAnimation(int img, int x, float y, double ExtRate, int style)
{
	float w = 100;
	float h = 50;
	GetGraphSizeF(img, &w, &h);
	DrawRectRotaGraph2F(x-(100/2.0f), y+(50/2.0f), style * 100, 0, 100, 50, 100 / 2.0f, (50 + pixelOut*2.0f), ExtRate, 0.0, img, true);
	//DrawRotaGraph2F(x - (w / 3), y + (h / 2), style*100 , (h / 2 + weaponOut) + 20, ExtRate, 0, img, true);
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