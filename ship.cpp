//========================================================================
// 
// 船 [ ship.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "ship.h"

#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "texture.h"

#include "game.h"
#include "title.h"
#include "result.h"
#include "camera.h"
#include "model.h"

#include "particle3D.h"
#include "explosion.h"
#include "map_object.h"
#include "mountain.h"
#include "beach.h"
#include "player.h"
#include "UI_action.h"
#include "UI_jump_meter.h"
#include "UI_energy.h"
#include "UI_item.h"
#include "tutorial_txt.h"
#include "score.h"

#include <iostream>
#include <fstream>
#include <string>

//************************************************************************
// マクロ定義
//************************************************************************
#define NEAR_BUTTON			(30.0f)				// ボタンの近く

//========================================================================
// 船クラスの生成処理
//========================================================================
CShip* CShip::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CShip* pShip = NULL;

	if (pShip == NULL)
	{// NULLチェック
		// 船の生成
		pShip = new CShip;
	}

	if (pShip != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pShip->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 船の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pShip->SetType(TYPE_SHIP);

		return pShip;
	}

	OutputDebugStringA("! ! ! 船の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 船クラスのコンストラクタ
//========================================================================
CShip::CShip(const int nPriority) :CObject(nPriority)
{
	// 船クラスの値をクリア
	memset(&m_apFileName[0], NULL, sizeof m_apFileName);
	memset(&m_apModel[0], NULL, sizeof m_apModel);
	m_nNumModel = 0;
	m_pos = DEFAULT_VECTER3;
	m_move = DEFAULT_VECTER3;
	m_rot = DEFAULT_VECTER3;
	m_rotDest = DEFAULT_VECTER3;
	m_scale = DEFAULT_VECTER3;
	m_col = COLOR_WHITE;
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_state = STATE_NONE;
	m_nCounterState = 0;
	m_bDisp = true;
}

//========================================================================
// 船クラスのデストラクタ
//========================================================================
CShip::~CShip()
{
}

//========================================================================
// 船クラスの初期化処理(オーバーロード)
//========================================================================
HRESULT CShip::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 船の情報の初期化
	m_pos = pos;
	m_rot = rot;
	m_scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_state = STATE_APPEAR;

	// 船モデルを設定
	if (FAILED(SetModel("data\\ship.txt")))
	{// もし失敗したら
		OutputDebugStringA("! ! ! 船モデルの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// 船クラスの終了処理
//========================================================================
void CShip::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLチェック
			// 終了処理
			m_apModel[nCntModel]->Uninit();

			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// 船クラスの更新処理
//========================================================================
void CShip::Update(void)
{
	if (CManager::GetMode() == CScene::MODE_RESULT)
	{// リザルト
		return;
	}

	// ローカル変数
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CCamera* pCamera = CManager::GetCamera();							// カメラの取得
	CDebugProc* pDebugProc = CManager::GetDebugProc();		// デバッグ表示の取得
	CMapObject* pMapObj = NULL;								// マップオブジェクトの取得
	CPlayer* pPlayer = (CManager::GetMode() == CScene::MODE_GAME) ?
		CGame::GetPlayer() : CTitle::GetPlayer();			// プレイヤーの取得
	CActionUI* pActionUI = (CManager::GetMode() == CScene::MODE_GAME) ?
		CGame::GetActionUI() : CTitle::GetActionUI();		// アクションUIの取得
	CJumpMeterUI* pJumpMeterUI = (CManager::GetMode() == CScene::MODE_GAME) ?
		CGame::GetJumpMeterUI() : CTitle::GetJumpMeterUI();	// ジャンプUIの取得
	CEnergyUI* pEnergyUI = (CManager::GetMode() == CScene::MODE_GAME) ?
		CGame::GetEnergyUI() : CTitle::GetEnergyUI();		// エネルギーUIの取得
	CTutorialTxt* pTutorialTxt = NULL;						// チュートリアル文の取得
	CItemUI* pItemUI = CGame::GetItemUI();					// アイテムUIの取得
	CScore* pScore = CGame::GetScore();						// スコアの取得

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = DEFAULT_VECTER3;
	D3DXVECTOR3 DoorRot = m_apModel[5]->GetRotation();

	pDebugProc->Print("\n*** 船 ***\n");

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// ゲーム中だけ
		// ローカル変数
		pMapObj = CGame::GetMapObject();			// マップオブジェクトの取得
	}

	// 状態管理
	switch (m_state)
	{
	case STATE_NONE:		// 状態なし
		pDebugProc->Print("状態 : 状態なし\n");

		break;

	case STATE_WAIT:		// 待機状態
		pDebugProc->Print("状態 : 待機状態\n");

		break;

	case STATE_APPEAR:		// 出現状態
		pDebugProc->Print("状態 : 出現状態\n");

		m_nCounterState++;

		if (m_nCounterState > 10)
		{// 一定時間経った
			m_nCounterState = 0;
			m_state = STATE_NORMAL;
		}

		break;

	case STATE_NORMAL:		// 通常状態
		pDebugProc->Print("状態 : 通常状態\n");

		if (pMapObj != NULL && pMapObj->GetCollectObj() <= 0)
		{// マップ上の収集アイテムがなくなったら
			m_state = STATE_READY;
		}

		break;

	case STATE_READY:		// 準備完了状態
		pDebugProc->Print("状態 : 準備完了状態\n");

		if (pPlayer->IsNear(m_pos, NEAR_BUTTON) == true)
		{// 近くにいるとき
			pActionUI->SetFade(CActionUI::FADE_OUT);
			pActionUI->NearShip(true);

			if (pInputKeyboard->GetTrigger(DIK_E) == true || pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_B) == true)
			{// ボタンを押した
				m_state = STATE_CLOSE;

				return;
			}
		}
		else
		{// 近くじゃない
			pActionUI->NearShip(false);
			pActionUI->SetFade(CActionUI::FADE_IN);
		}

		break;

	case STATE_OPEN:		// 開く状態
		pDebugProc->Print("状態 : 開く状態\n");

		DoorRot.x += (m_apModel[5]->GetRotOffC().x - DoorRot.x) * 0.02f;

		m_apModel[5]->SetRotation(DoorRot);

		if (DoorRot.x >= m_apModel[5]->GetRotOffC().x - 0.01f && DoorRot.x <= m_apModel[5]->GetRotOffC().x + 0.01f)
		{// 目的の角度になった
			DoorRot.x = m_apModel[5]->GetRotOffC().x;

			m_state = STATE_NORMAL;
		}

		break;

	case STATE_CLOSE:		// 閉じる状態
		pDebugProc->Print("状態 : 閉じる状態\n");

		DoorRot.x += (0.0f - DoorRot.x) * 0.02f;

		m_apModel[5]->SetRotation(DoorRot);

		if (DoorRot.x >= -0.01f && DoorRot.x <= 0.01f)
		{// 目的の角度になった
			DoorRot.x = 0.0f;

			m_state = STATE_UP;
			pPlayer->SetDisp(false);
			pPlayer->SetState(CPlayer::STATE_NONE);
			pActionUI->SetDisp(false);
			pJumpMeterUI->SetDispAll(false);
			pEnergyUI->SetDisp(false);
			pEnergyUI->SetState(0);

			if (pItemUI != NULL && pScore != NULL)
			{// NULLチェック
				pItemUI->SetDispAll(false);
				pScore->SetDisp(false);
			}

			for (int nCnt = 0; nCnt < NUM_TUTORIALTXT; nCnt++)
			{
				pTutorialTxt = CTitle::GetTutorialTxt(nCnt);
				if (pTutorialTxt != NULL)
				{// NULLチェック
					pTutorialTxt->SetDisp(false);
				}
			}
		}

		break;

	case STATE_UP:			// 上昇状態
		pDebugProc->Print("状態 : 上昇状態\n");

		move.y += 0.7f;
		pos.y += move.y;

		// 位置を適用
		SetPosition(pos);

		if (pos.y > ((CManager::GetMode() == CScene::MODE_TITLE) ? 150.0f : 500.0f))
		{// ある程度高くなったら
			if (CManager::GetMode() == CScene::MODE_GAME)
			{// ゲーム中
				// 遷移フラグをON
				CGame::SetFadeEnable();

				// クリアフラグを立てる
				CResult::SetClear(true);
			}
			else if (CManager::GetMode() == CScene::MODE_TITLE)
			{// タイトル
				// 遷移フラグをON
				CTitle::SetFadeEnable();
			}
		}
		else
		{// 船を追う
			// カメラ設定
			pCamera->SetPosition(D3DXVECTOR3(pos.x, pCamera->GetPositionV().y, pos.z - 400.0f), pos, pCamera->GetRotation(), CCamera::TYPE_STOP);
		}

		CParticle3D::Create(pos, 1, 3, 10.0f, 0.2f, 0.01f, CEffect3D::TYPE_BLENDADD, CParticle3D::TYPE_HOMING, 300, 2.0f,
			1.0f, COLOR_ORANGE, 10.0f, true, NULL, D3DXVECTOR3(pos.x, pos.y - 100.0f, pos.z), 0.1f);

		break;

	case STATE_DOWN:		// 下降状態
		pDebugProc->Print("状態 : 下降状態\n");

		CMountain* pMountain = NULL;							// 山の取得
		CBeach* pBeach = NULL;									// 砂浜の取得

		float fHeightM = 0.0f;		// 山の地面の高さ
		D3DXVECTOR2 polygonIdxM = { -1.0f,-1.0f };		// ポリゴン番号

		float fHeightB = 0.0f;		// 砂浜の地面の高さ
		D3DXVECTOR2 polygonIdxB = { -1.0f,-1.0f };		// ポリゴン番号

		float fHeight = 0.0f;		// 地面の高さ

		// ローカル変数
		pMountain = CGame::GetMountain();			// 山の取得
		pBeach = CGame::GetBeach();					// 砂浜の取得

		DoorRot.x = 0.0f;
		m_apModel[5]->SetRotation(DoorRot);

		if (pMountain != NULL && pBeach != NULL)
		{// NULLチェック
			// 山のポリゴン番号を取得
			polygonIdxM = pMountain->GetPolygonIdx(pos);

			// 山の地面の高さを取得
			fHeightM = pMountain->GetHeight(pos, polygonIdxM);

			// 砂浜のポリゴン番号を取得
			polygonIdxB = pBeach->GetPolygonIdx(pos);

			// 砂浜の地面の高さを取得
			fHeightB = pBeach->GetHeight(pos, polygonIdxB);
		}

		// 最終的な高さ
		fHeight = (fHeightM >= fHeightB) ? fHeightM : fHeightB;

		if (fHeight == ERROR_HEIGHT)
		{// 無効な高さだったら
			fHeight = 0.0f;
		}

		if (pos.y > fHeight + 10.0f)
		{// 落ちる
			pos.y += (fHeight - pos.y) * 0.005f;

			// カメラ設定
			pCamera->SetPosition(D3DXVECTOR3(pos.x, fHeight + 100.0f, pos.z - 300.0f), pos, pCamera->GetRotation(), CCamera::TYPE_STOP);

			CParticle3D::Create(pos, 1, 3, 10.0f, 0.2f, 0.01f, CEffect3D::TYPE_BLENDADD, CParticle3D::TYPE_HOMING, 30, 2.0f,
				1.0f, COLOR_ORANGE, 10.0f, true, NULL, D3DXVECTOR3(pos.x, pos.y - 100.0f, pos.z), 0.1f);
		}
		else
		{// 止まる
			// カメラ設定
			pCamera->SetType(CCamera::TYPE_PLAYER);

			pos.y = fHeight;
			m_state = STATE_OPEN;

			CExplosion::Ray(pos, 20.0f, 2, 2.0f, COLOR_DARKGRAY);

			if (CManager::GetMode() == CScene::MODE_GAME)
			{// ゲーム中なら
				pPlayer->SetState(CPlayer::STATE_APPEAR);
				pPlayer->SetPosition(D3DXVECTOR3(pos.x, pos.y + 7.0f, pos.z));
				pPlayer->SetDisp(true);
				pActionUI->SetDisp(true);
				pJumpMeterUI->SetDispAll(true);
				pEnergyUI->SetDisp(true);
				pItemUI->SetDispWOCheck(true);
				pScore->SetDisp(true);
			}
		}

		// 位置を適用
		SetPosition(pos);

		break;
	}

	pDebugProc->Print("位置 : %f %f %f\n", pos.x, pos.y, pos.z);
}

//========================================================================
// 船クラスの描画処理
//========================================================================
void CShip::Draw(void)
{
	if (m_bDisp == false)
	{// 表示しない場合
		return;
	}

	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;					// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画（全パーツ分）
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLチェック
			// 描画処理
			m_apModel[nCntModel]->Draw();
		}
	}
}

//========================================================================
// 位置設定
//========================================================================
void CShip::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置を変更
	m_pos = pos;
}

//========================================================================
// スケール設定
//========================================================================
void CShip::SetScale(const D3DXVECTOR3 scale)
{
	// スケールを変更
	m_scale = scale;
}

//========================================================================
// 角度設定
//========================================================================
void CShip::SetRotation(const D3DXVECTOR3 rot)
{
	// 向きを変更
	m_rot = rot;

	// X向きを調整
	CorrectAngle(&m_rot.x, m_rot.x);

	// Y向きを調整
	CorrectAngle(&m_rot.y, m_rot.y);

	// Z向きを調整
	CorrectAngle(&m_rot.z, m_rot.z);
}

//========================================================================
// 頂点カラー設定
//========================================================================
void CShip::SetColor(const D3DXCOLOR col)
{
	// 色を変更
	m_col = col;

	// モデルの色を変更（全パーツ分）
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLチェック
			// 各モデルの色を変更
			m_apModel[nCntModel]->SetColor(col);
		}
	}
}

//========================================================================
// 当たり判定
//========================================================================
bool CShip::Collision(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, D3DXVECTOR3* move,
	const float fRadius, const float fHeight, bool* pLand)
{
	D3DXVECTOR3 dist;

	dist = *pos - m_pos;

	if (D3DXVec3Length(&dist) <= m_fRadius)
	{// 距離が近い
		// 各パーツの当たり判定
		for (int nCntPart = 0; nCntPart < m_nNumModel; nCntPart++)
		{
			if (nCntPart == 5 && m_state != STATE_CLOSE)
			{// 当たり判定しないもの
				continue;
			}

			D3DXVECTOR3 posPart =
				D3DXVECTOR3(m_apModel[nCntPart]->GetMtxWorld()._41, m_apModel[nCntPart]->GetMtxWorld()._42, m_apModel[nCntPart]->GetMtxWorld()._43);

			CorrectAngle(&m_rot.y, m_rot.y);

			if (m_apModel[nCntPart]->Collision(posPart, m_rot, m_scale, pos, posOld, move, fRadius, fHeight) == true)
			{// 当たっている
				*pLand = true;
			}
		}

		return true;
	}

	return false;
}

//========================================================================
// 外部ファイルからモデルデータを読み込む
//========================================================================
HRESULT CShip::SetModel(const char* pFilename)
{
	// ローカル変数宣言
	std::string filename = pFilename;	// 読み込むファイル名
	std::fstream file(filename);		// ファイル
	std::string line = {};				// テキストを読み込む

	// テクスチャ読み込み用の変数
	int nNumTexture = 0;				// テクスチャ総数
	int nCntTexture = 0;				// 
	std::string TextureName = {};		// テクスチャの名前

	// キャラクターセット用の変数
	int nCntModel = 0;				// 読み込んだモデル数
	char aModelName[64][256] = {};	// モデルの名前
	int nNumParts = 0;				// 読み込むパーツ数
	int nCntParts = 0;				// 読み込んだパーツ数
	float fRadius = 0.0f;			// キャラクターの半径
	float fHeight = 0.0f;			// キャラクターの高さ
	float fMove = 0.0f;				// キャラクターの移動量
	float fJump = 0.0f;				// キャラクターのジャンプ量
	D3DXVECTOR2 Blowoff = D3DXVECTOR2(0.0f, 0.0f);		// 吹っ飛び量
	D3DXVECTOR2 Somersault = D3DXVECTOR2(0.0f, 0.0f);	// バク宙量

	// パーツセット用の変数
	int nIdx = -1;			// モデル番号
	int nIdxParent = -1;	// 親のモデル番号
	D3DXVECTOR3 pos = DEFAULT_VECTER3;		// 位置
	D3DXVECTOR3 rot = DEFAULT_VECTER3;		// 向き

	if (file.is_open() == true)
	{// ファイルが開けた
		// ファイルの内容を読み込む
		while (file >> line)
		{
			if (line.find("#") != std::string::npos)
			{// コメント
				std::getline(file, line);

				continue;
			}

			if (line.find("NUM_MODEL") != std::string::npos)
			{// モデル数
				file >> line;				// =
				file >> m_nNumModel;		// 数字

				continue;
			}

			if (line.find("MODEL_FILENAME") != std::string::npos && nCntModel < m_nNumModel)
			{// モデルの名前
				file >> line;						// =
				file >> &aModelName[nCntModel][0];	// モデルの名前

				m_apFileName[nCntModel] = &aModelName[nCntModel][0];	// 格納

				nCntModel++;		// 読み込んだモデル数をカウントアップ

				continue;
			}

			if (line.find("NUM_TEXTURE") != std::string::npos)
			{// テクスチャ数
				file >> line;				// =
				file >> nNumTexture;		// 数字

				continue;
			}

			if (line.find("TEXTURE_FILENAME") != std::string::npos && nCntTexture < nNumTexture)
			{// テクスチャの名前
				file >> line;				// =
				file >> TextureName;		// テクスチャの名前

				nCntTexture++;		// 読み込んだテクスチャ数をカウントアップ

				continue;
			}

			if (line.find("CHARACTERSET") != std::string::npos)
			{// キャラクターの設定
				file >> line;				// NUM_PARTS

				if (line.find("NUM_PARTS") != std::string::npos)
				{// 読み込むパーツ数
					file >> line;				// =
					file >> nNumParts;			// パーツ数

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				if (line.find("RADIUS") != std::string::npos)
				{// キャラクターの半径
					file >> line;				// =
					file >> m_fRadius;			// 半径

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				if (line.find("HEIGHT") != std::string::npos)
				{// キャラクターの高さ
					file >> line;				// =
					file >> m_fHeight;			// 高さ

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				if (line.find("MOVE") != std::string::npos)
				{// キャラクターの移動量
					file >> line;				// =
					file >> fMove;				// 移動量

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				if (line.find("JUMP") != std::string::npos)
				{// キャラクターのジャンプ量
					file >> line;				// =
					file >> fJump;				// ジャンプ量

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				if (line.find("BLOWOFF") != std::string::npos)
				{// キャラクターの吹っ飛び量
					file >> line;				// =
					file >> Blowoff.x;			// 吹っ飛び量x
					file >> Blowoff.y;			// 吹っ飛び量y

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				if (line.find("SOMERSAULT") != std::string::npos)
				{// キャラクターのバク宙量
					file >> line;				// =
					file >> Somersault.x;		// バク宙量x
					file >> Somersault.y;		// バク宙量y

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				// パーツ読み込み
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{
					if (line.find("PARTSSET") != std::string::npos)
					{// PARTSSET
						file >> line;				// モデル番号
					}

					if (line.find("INDEX") != std::string::npos)
					{// モデル番号
						file >> line;				// =
						file >> nIdx;				// モデル番号

						file >> line;				// 次の文字列

						if (line.find("#") != std::string::npos)
						{// コメント
							std::getline(file, line);

							// 次の文字列を読み込む
							file >> line;			// 次の文字列
						}
					}

					if (line.find("PARENT") != std::string::npos)
					{// 親のモデル番号
						file >> line;				// =
						file >> nIdxParent;			// 親のモデル番号

						file >> line;				// 次の文字列

						if (line.find("#") != std::string::npos)
						{// コメント
							std::getline(file, line);

							// 次の文字列を読み込む
							file >> line;			// 次の文字列
						}
					}

					if (line.find("POS") != std::string::npos)
					{// パーツの位置(オフセット)
						file >> line;				// =
						file >> pos.x;				// 位置(オフセット)x
						file >> pos.y;				// 位置(オフセット)y
						file >> pos.z;				// 位置(オフセット)y

						file >> line;				// 次の文字列

						if (line.find("#") != std::string::npos)
						{// コメント
							std::getline(file, line);

							// 次の文字列を読み込む
							file >> line;			// 次の文字列
						}
					}

					if (line.find("ROT") != std::string::npos)
					{// パーツの向き
						file >> line;				// =
						file >> rot.x;				// 向き(オフセット)x
						file >> rot.y;				// 向き(オフセット)y
						file >> rot.z;				// 向き(オフセット)y

						file >> line;				// 次の文字列

						if (line.find("#") != std::string::npos)
						{// コメント
							std::getline(file, line);

							// 次の文字列を読み込む
							file >> line;			// 次の文字列
						}
					}

					if (line.find("END_PARTSSET") != std::string::npos)
					{// END_PARTSSET
						file >> line;				// 次の文字列
					}

					// モデルの生成
					m_apModel[nCntParts] = CModel::Create(pos, rot, m_apFileName[nIdx]);

					if (nIdxParent < 0 || nIdxParent >= MAX_MODEL)
					{// 範囲外の親インデックス
						// 親がない場合の設定
						m_apModel[nCntParts]->SetParent(NULL);
					}
					else
					{// 親がいる場合
						m_apModel[nCntParts]->SetParent(m_apModel[nIdxParent]);
					}
				}

				continue;
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{// ファイルの読み込みが完了
				break;
			}
		}

		// ファイルを閉じる
		file.close();
	}
	else
	{// ファイルが開けなかった
		OutputDebugStringA("! ! ! ファイルを開けませんでした ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}