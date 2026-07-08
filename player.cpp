//========================================================================
// 
// プレイヤー [ player.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "player.h"

#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "texture.h"

#include "game.h"
#include "camera.h"
#include "model.h"
#include "motion.h"

#include "effect3D.h"
#include "particle3D.h"
#include "energyrock.h"
#include "tree.h"
#include "rock.h"
#include "ship.h"

#include "object3D.h"
#include "meshfield.h"

#include <iostream>
#include <fstream>
#include <string>

//************************************************************************
// マクロ定義
//************************************************************************
#define LAND_MOVEMENT	(D3DXVECTOR3(0.6f, 0.6f, 0.6f))			// 移動量(地上)
#define AIR_MOVEMENT	(D3DXVECTOR3(0.1f, 0.1f, 0.1f))			// 移動量(空中)
#define MOVE_INERTIA	(0.05f)									// 移動量の慣性
#define ROT				(D3DXVECTOR3(0.05f, 0.05f, 0.05f))		// 向き移動量
#define MODEL_ROT		(D3DX_PI * 0.05f)						// モデルの傾き具合
#define MODEL_ROT_X		(0.04f)									// モデルの傾きカウントX方向
#define MODEL_ROT_Z		(0.005f)								// モデルの傾きカウントZ方向
#define MAX_ROTADD		(1.0f)									// 最大追加角度
#define MIN_ROTADD		(-1.0f)									// 最小追加角度
#define TIRE_ROT		(0.15f)									// タイヤの回り具合
#define JUMP_HEIGHT		(4.0f)									// ジャンプの高さ
#define JUMP_ADD		(0.02f)									// ジャンプの変化量
#define MAX_JUMP		(1.0f)									// ジャンプ量の最大値
#define GRAVITY			(-0.3f)									// 重力
#define MAX_ENERGY		(100.0f)								// 最大所持エネルギー
#define FIRST_ENERGY	(80.0f)									// 初期所持エネルギー
#define ONE_ENERGY		(5.0f)									// 鉱石ひとつあたりのエネルギー
#define MINUS_ENERGY	(90)									// エネルギー減少の間隔
#define UNCLIMB_SLOPE	(0.4f)									// 登れない傾斜の角度

//========================================================================
// プレイヤークラスの生成処理
//========================================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULLチェック
		// プレイヤーの生成
		pPlayer = new CPlayer;
	}

	if (pPlayer != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pPlayer->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! プレイヤーの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pPlayer->SetType(TYPE_PLAYER);

		return pPlayer;
	}

	OutputDebugStringA("! ! ! プレイヤーの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// プレイヤークラスのコンストラクタ
//========================================================================
CPlayer::CPlayer(const int nPriority) :CObject(nPriority)
{
	// プレイヤークラスの値をクリア
	memset(&m_apFileName[0], NULL, sizeof m_apFileName);
	memset(&m_apModel[0], NULL, sizeof m_apModel);
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_pos = DEFAULT_VECTER3;
	m_posOld = DEFAULT_VECTER3;
	m_move = DEFAULT_VECTER3;
	m_rot = DEFAULT_VECTER3;
	m_rotDest = DEFAULT_VECTER3;
	m_scale = DEFAULT_VECTER3;
	m_col = COLOR_WHITE;
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_fJumpHigh = 0.0f;
	m_fEnergy = 0.0f;
	m_nEnergyCounter = 0;
	m_state = STATE_NONE;
	m_nCounterState = 0;
	m_bJump = false;
	m_bMove = false;
	m_bLand = false;
	m_bAct = false;
	m_bDisp = true;
}

//========================================================================
// プレイヤークラスのデストラクタ
//========================================================================
CPlayer::~CPlayer()
{
}

//========================================================================
// プレイヤークラスの初期化処理(オーバーロード)
//========================================================================
HRESULT CPlayer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// プレイヤーの情報の初期化
	m_pos = pos;
	m_posOld = pos;
	m_rot = rot;
	m_scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fEnergy = FIRST_ENERGY;
	m_state = STATE_APPEAR;

	// モーションを生成/初期化
	if (m_pMotion == NULL)
	{// NULLチェック
		m_pMotion = CMotion::Create();
	}

	// プレイヤーモデルを設定
	if (FAILED(SetModel("data\\player.txt")))
	{// もし失敗したら
		OutputDebugStringA("! ! ! プレイヤーモデルの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// モーションに必要なモデルの設定
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	// 初期モーションの設定
	m_pMotion->Set(MOTIONTYPE_NEUTRAL, false, 0);

	return S_OK;
}

//========================================================================
// プレイヤークラスの終了処理
//========================================================================
void CPlayer::Uninit(void)
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

	// モーションの破棄
	if (m_pMotion != NULL)
	{// NULLチェック
		// 終了処理
		m_pMotion->Uninit();

		delete m_pMotion;
		m_pMotion = NULL;
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// プレイヤークラスの更新処理
//========================================================================
void CPlayer::Update(void)
{
	// ローカル変数
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CDebugProc* pDebugProc = CManager::GetDebugProc();					// デバッグ表示の取得
	CMeshField* pMeshField = CGame::GetMeshField();						// メッシュフィールドの取得
	CShip* pShip = CGame::GetShip();						// 船の取得
	D3DXVECTOR3 pos = CPlayer::GetPosition();				// プレイヤーの位置	
	D3DXVECTOR3 rot = CPlayer::GetRotation();				// プレイヤーの向き
	D3DXVECTOR2 move = { 0.0f,0.0f };						// XZ方向に動いているかどうかの判断用

	D3DXVECTOR3 UpperPosOff = m_apModel[1]->GetPosOffC();	// 上半身の位置(オフセット保存)
	D3DXVECTOR3 UpperPos = m_apModel[1]->GetPosOff();		// 上半身の位置(オフセットをいじる)

	float fHeight = 0.0f;		// 地面の高さ
	D3DXVECTOR2 polygonIdx = { -1.0f,-1.0f };		// ポリゴン番号

	bool bLand = false;			// オブジェクトへの着地判定
	bool bHead = false;			// オブジェクトへの頭ぶつかり判定

	pDebugProc->Print("\n*** プレイヤー ***\n");
	pDebugProc->Print("位置 : %f %f %f\n", pos.x, pos.y, pos.z);
	pDebugProc->Print("エネルギー残量:%f\n", m_fEnergy);

	// 操作
#ifdef ENABLE_EXPLANATION
	pDebugProc->Print("\n\n*** 操作説明 ***\n");
	pDebugProc->Print("\n[キーボード]\n");
	pDebugProc->Print("WASD:移動\n");
	pDebugProc->Print("Space:ジャンプ(長押しで高さ変化)\n");
	pDebugProc->Print("Enter:エネルギー回収(鉱石の近くで)\n");
	pDebugProc->Print("IJKL/マウス右クリック:カメラ操作\n");
	pDebugProc->Print("BackSpace:リセット\n");
	pDebugProc->Print("\n[コントローラー]\n");
	pDebugProc->Print("左スティック/十字キー:移動\n");
	pDebugProc->Print("A:ジャンプ(長押しで高さ変化)\n");
	pDebugProc->Print("X:エネルギー回収(鉱石の近くで)\n");
	pDebugProc->Print("右スティック:カメラ操作\n");
	pDebugProc->Print("START:リセット\n");
#endif

	// 前回の位置を保存
	m_posOld = m_pos;

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

		break;

	case STATE_FALL:		// 転び状態
		pDebugProc->Print("状態 : 転び状態\n");

		break;

	case STATE_DEATH:		// 死亡状態
		pDebugProc->Print("状態 : 死亡状態\n");

		break;
	}

	// 移動処理
	if (m_state == STATE_NORMAL)
	{// 通常状態のとき
		if (Movement(rot) == true)
		{// 移動している
			if (m_bJump == false)
			{// ジャンプしていない
				// モーションを設定
				m_pMotion->Set(MOTIONTYPE_MOVE, true, 20);
			}

			// 移動するとエネルギーが減る
			m_nEnergyCounter++;
		}
		else if (m_bJump == false && m_bLand == false && m_bAct == false)
		{// 移動していない
			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_NEUTRAL, true, 20);
		}
	}

	if (m_bJump == false && m_state == STATE_NORMAL)
	{// ジャンプしていないとき
		static float fMinusEnergy = 0.0f;

		if (pInputKeyboard->GetPress(DIK_SPACE) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_A) == true)
		{// ジャンプキー長押し
			// ジャンプ量を上げる
			m_fJumpHigh += JUMP_ADD;

			if (m_fJumpHigh > MAX_JUMP)
			{// 最大値に固定
				m_fJumpHigh = MAX_JUMP;
			}
			else
			{// 最大値ではないときだけ
				// モデルの位置を変更
				UpperPos.y -= JUMP_ADD;

				// 消費するエネルギー量を増やす
				fMinusEnergy += 0.1f;
			}
		}

		if (pInputKeyboard->GetRelease(DIK_SPACE) == true || pInputJoypad->GetRelease(0, CInputJoypad::JOYKEY_A) == true)
		{// ジャンプ
			m_bJump = true;
			m_move.y = JUMP_HEIGHT + (JUMP_HEIGHT * m_fJumpHigh) - ((JUMP_HEIGHT * 0.8f) * (m_fEnergy * 0.01f));

			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_JUMP, true, 20);

			// ジャンプ量リセット
			m_fJumpHigh = 0.0f;
			UpperPos.y = UpperPosOff.y;

			// エネルギーを消費する
			m_fEnergy -= fMinusEnergy;
			fMinusEnergy = 0.0f;		// リセット
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true || pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true)
	{// 位置回転リセット
		pos = DEFAULT_VECTER3;
		m_posOld = DEFAULT_VECTER3;
		rot = DEFAULT_VECTER3;
		m_rotDest = DEFAULT_VECTER3;
		m_move = DEFAULT_VECTER3;

		m_nEnergyCounter = 0;
		m_fEnergy = FIRST_ENERGY;

		m_state = STATE_NORMAL;

		// モーションを設定
		m_pMotion->Set(MOTIONTYPE_NEUTRAL, true, 20);
	}

	// 位置に移動量を加算
	pos += m_move;

	// 重力
	m_move.y += GRAVITY;

	if (m_bJump == false)
	{// 地上では慣性
		// 移動量に慣性
		m_move.x += (0.0f - m_move.x) * MOVE_INERTIA;
		m_move.z += (0.0f - m_move.z) * MOVE_INERTIA;
	}

	// XZ方向への移動量(動いているかいないか)※Y座標のみの変化はとらない
	move = D3DXVECTOR2(m_move.x, m_move.z);

	// 目的の向きを修正
	float fmoveAngle = m_rotDest.y - rot.y;

	// 向きを調整
	CorrectAngle(&m_rotDest.y, fmoveAngle);

	// 角度を慣性ありで加算
	rot.y += (m_rotDest.y - rot.y) * 0.1f;

	// ポリゴン番号を取得
	polygonIdx = pMeshField->GetPolygonIdx(pos);

	pDebugProc->Print("傾斜 : %f\n", pMeshField->GetSlope(pos, polygonIdx));

	if (m_bJump == false)
	{// 地上にいるときだけ
		// 傾斜によって進む距離を調整
		pos = m_posOld + ((pos - m_posOld) * pMeshField->GetSlope(pos, polygonIdx));
	}

	// 乗っているポリゴン番号を再計算
	polygonIdx = pMeshField->GetPolygonIdx(pos);

	// 地面の高さを取得
	fHeight = pMeshField->GetHeight(pos, polygonIdx);

	if (fHeight == ERROR_HEIGHT)
	{// 無効な高さだったら
		fHeight = 0.0f;
	}

	if (pos.y <= fHeight)
	{// 地面にめり込んだときだけ
		pos.y = fHeight;

		if (pMeshField->GetSlope(pos, polygonIdx) <= UNCLIMB_SLOPE && m_bJump == false)
		{// 傾斜の角度的に登れない/地面にいる
			pos = m_posOld;
		}
	}

	// 当たり判定
	CEnergyRock* pEnergyRock = CEnergyRock::Collision(&pos, &m_posOld, &m_move, m_fRadius, m_fHeight);
	CTree::Collision(&pos, &m_posOld, &m_move, m_fRadius, m_fHeight);
	CRock::Collision(&pos, &m_posOld, &m_move, m_fRadius, m_fHeight, &bLand, &bHead);
	pShip->Collision(&pos, &m_posOld, &m_move, m_fRadius, m_fHeight, &bLand);

	if (pos.y <= fHeight || bLand == true)
	{// 地面との当たり判定
		if (m_bJump == true && m_state == STATE_NORMAL)
		{// 着地
			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_LANDING, true, 20);
			m_bLand = true;
		}

		if (pos.y < fHeight && bHead == true)
		{// 地面にめり込んで頭もぶつかっていたら
			pos = m_posOld;		// 進めない
		}

		m_move.y = 0.0f;
		m_bJump = false;

		if (m_bLand == true && m_pMotion->GetType() == MOTIONTYPE_LANDING && m_pMotion->IsFinish() == true)
		{// 着地モーションが終わった
			m_bLand = false;
			m_bAct = false;

			m_state = STATE_NORMAL;
		}
	}
	else if (pos.y > fHeight + 5.0f && bLand == false && m_state == STATE_NORMAL)
	{// オブジェクトの上でもない空中
		m_bJump = true;

		// モーションを設定
		m_pMotion->Set(MOTIONTYPE_JUMP, true, 20);
	}

	// 転ぶ
	if (m_bJump == true)
	{// 空中にいるとき
		float headLine = m_apModel[3]->GetMtxWorld()._42 - 5.0f;

		if (headLine < fHeight)
		{// 上半身が先に地面についた
			m_state = STATE_FALL;

			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_FALL, true, 20);

			m_bLand = true;

			m_move.y = 0.0f;
			m_bJump = false;

			m_fEnergy -= ONE_ENERGY;
		}
	}

	if (m_bLand == true && m_pMotion->GetType() == MOTIONTYPE_FALL && m_pMotion->IsFinish() == true)
	{// 転びモーションが終わった
		m_bLand = false;
		m_bAct = false;

		m_state = STATE_NORMAL;
	}

	if (pos.y <= fHeight || bLand == true)
	{// 地上
		if (D3DXVec2Length(&move) >= 0.4f)
		{// 移動している
			CParticle3D::Create(pos, 1, 3, 1.0f, -0.2f, 0.1f, CEffect3D::TYPE_NORMAL_NULL, CParticle3D::TYPE_NORMAL, 2, 2.0f, true);
		}
	}

	if (pEnergyRock != NULL &&
		m_bJump == false && m_state == STATE_NORMAL)
	{// エネルギー鉱物と当たっているとき/空中ではないとき
		if (pInputKeyboard->GetTrigger(DIK_E) == true || pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_B) == true)
		{// 回収するキーを押した
			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_ACTION, true, 20);

			m_bAct = true;
			m_bLand = false;
		}
	}

	if (m_bAct == true && m_pMotion->GetType() == MOTIONTYPE_ACTION && m_pMotion->IsFinish() == true)
	{// アクションモーションが終わった
		m_bAct = false;

		// とる
		pEnergyRock->Minus(1);

		// エネルギー回収
		m_fEnergy += ONE_ENERGY;
	}

	if (pInputKeyboard->GetTrigger(DIK_H) == true)
	{// エネルギー鉱石生成
		CEnergyRock::Create(pos, DEFAULT_VECTER3);
	}

	// エネルギー減少
	if (m_nEnergyCounter > MINUS_ENERGY)
	{// 一定時間移動し続けると減少
		if (m_bJump == false)
		{// 地上
			m_fEnergy -= 2.0f - pMeshField->GetSlope(pos, polygonIdx);		// 減らす
		}
		else
		{// 空中
			m_fEnergy -= 1.0f;
		}

		m_nEnergyCounter = 0;	// リセット
	}

	if (m_fEnergy > MAX_ENERGY)
	{// 最大値に調整
		m_fEnergy = MAX_ENERGY;
	}
	else if (m_fEnergy < 0.0f)
	{// 最小値に調整
		m_fEnergy = 0.0f;

		m_state = STATE_DEATH;

		// モーションを設定
		m_pMotion->Set(MOTIONTYPE_DEATH, true, 20);

#ifndef _DEBUG		// リリースビルドならリザルト画面へ
		// 画面遷移
		CManager::SetMode(CScene::MODE_RESULT);

		return;
#endif
	}

	// 位置/向きを適用
	SetPosition(pos);
	SetRotation(rot);

	// モデルの位置を適用
	m_apModel[1]->SetPosOff(UpperPos);

	// モーションの更新
	m_pMotion->Update();
}

//========================================================================
// プレイヤークラスの描画処理
//========================================================================
void CPlayer::Draw(void)
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
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;				// 現在のレンダリング保存用
	D3DVIEWPORT9 viewportDef;								// 現在のビューポート保存用
	D3DXMATRIX mtxViewDef, mtxProjectionDef;				// 現在のマトリックス保存用

	// 現在のレンダリングターゲットを取得（保存）
	pDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファを取得（保存）
	pDevice->GetDepthStencilSurface(&pZBuffDef);

	// 現在のビューポートを取得（保存）
	pDevice->GetViewport(&viewportDef);

	// 現在のビューマトリックスを取得（保存）
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewDef);

	// 現在のプロジェクションマトリックスを取得（保存）
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

#ifdef MALTITARGET_RENDERING
	// レンダリングターゲットを変更
	CManager::GetRenderer()->ChangeTarget(D3DXVECTOR3(m_pos.x, m_pos.y + 10.0f, m_pos.z - 15.0f), m_pos, D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// レンダリングターゲット用テクスチャのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
#endif

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

	// レンダリングターゲットをもとに戻す
	pDevice->SetRenderTarget(0, pRenderDef);

	// Zバッファを元に戻す
	pDevice->SetDepthStencilSurface(pZBuffDef);

	// ビューポートを元に戻す
	pDevice->SetViewport(&viewportDef);

	// ビューマトリックスを元に戻す
	pDevice->SetTransform(D3DTS_VIEW, &mtxViewDef);

	// プロジェクションマトリックスを元に戻す
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionDef);
}

//========================================================================
// 位置設定
//========================================================================
void CPlayer::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置を変更
	m_pos = pos;
}

//========================================================================
// スケール設定
//========================================================================
void CPlayer::SetScale(const D3DXVECTOR3 scale)
{
	// スケールを変更
	m_scale = scale;
}

//========================================================================
// 角度設定
//========================================================================
void CPlayer::SetRotation(const D3DXVECTOR3 rot)
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
void CPlayer::SetColor(const D3DXCOLOR col)
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
// プレイヤーの近くにいるかを判定
//========================================================================
bool CPlayer::IsNear(const D3DXVECTOR3 pos, const float fRadius)
{
	// ローカル変数
	D3DXVECTOR3 dist;
	dist = pos - m_pos;

	if (D3DXVec3Length(&dist) <= fRadius)
	{// 近い
		return true;
	}

	return false;
}

//========================================================================
// プレイヤーの移動処理
//========================================================================
bool CPlayer::Movement(const D3DXVECTOR3 rot)
{
	// ローカル変数
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CDebugProc* pDebugProc = CManager::GetDebugProc();					// デバッグ表示の取得

	D3DXVECTOR3 UnderRotOff = m_apModel[0]->GetRotOffC();	// 下半身の傾き(オフセット保存)
	D3DXVECTOR3 UnderRot = m_apModel[0]->GetRotOff();		// 下半身の傾き(オフセットをいじる)
	D3DXVECTOR3 UpperPosOff = m_apModel[1]->GetPosOffC();	// 上半身の位置(オフセット保存)
	D3DXVECTOR3 UpperPos = m_apModel[1]->GetPosOff();		// 上半身の位置(オフセットをいじる)
	D3DXVECTOR3 UpperRotOff = m_apModel[1]->GetRotOffC();	// 上半身の傾き(オフセット保存)
	D3DXVECTOR3 UpperRot = m_apModel[1]->GetRotOff();		// 上半身の傾き(オフセットをいじる)
	D3DXVECTOR3 TireRotOff = m_apModel[2]->GetRotOffC();	// タイヤの傾き(オフセット保存)
	D3DXVECTOR3 TireRot = m_apModel[2]->GetRotOff();		// タイヤの傾き(オフセットをいじる)

	D3DXVECTOR3 UnderRotDest = { 0.0f,0.0f,0.0f };			// 下半身の目的の傾き
	D3DXVECTOR3 UpperRotDest = { 0.0f,0.0f,0.0f };			// 上半身の目的の傾き

	bool bMove = false;
	int nValueH, nValueV;
	static float fRotCounterX = 0.0f;		// 傾きの追加角度X
	static float fRotCounterZ = 0.0f;		// 傾きの追加角度Z

	// 8方向移動時のスピード
	float fCustomSpeed = (1.5f - (m_fEnergy * 0.01f)) * ((fRotCounterZ >= MAX_ROTADD || fRotCounterZ <= MIN_ROTADD) ? 0.1f : 1.0f) * 0.5f;
	float fSpeedX = (m_bJump ? AIR_MOVEMENT.x : LAND_MOVEMENT.x) * fCustomSpeed;
	float fSpeedZ = (m_bJump ? AIR_MOVEMENT.z : LAND_MOVEMENT.z) * fCustomSpeed;

	if (pInputJoypad->GetStick(0, CInputJoypad::JOYKEY_LEFTSTICK, &nValueH, &nValueV) == true)
	{// スティック移動
		float fSpeed = (m_bJump ? -(float)nValueV * 0.0000183f / 6.0f : -(float)nValueV * 0.0000183f) * fCustomSpeed;		// スピード

		m_move.x += sinf(rot.y) * fSpeed;
		m_move.z += cosf(rot.y) * fSpeed;

		// 進んだ方向にモデルを傾ける
		UnderRotDest.x = (MODEL_ROT + fRotCounterX) * ((nValueV < 0) ? 1 : -1);
		UnderRotDest.z = (MODEL_ROT + fRotCounterZ) * ((nValueH < 0) ? -1 : 1);
		UpperRotDest.x = MODEL_ROT * ((nValueV < 0) ? 1 : -1);
		UpperRotDest.z = MODEL_ROT * ((nValueH < 0) ? -1 : 1);
		TireRot.x += TIRE_ROT * ((nValueV < 0) ? 1 : -1);

		// 進んだ方向に角度を向ける
		m_rotDest.y = rot.y + ((float)nValueH * 0.00001f * ((nValueV < 0) ? -1 : 1));

		if (nValueV != 0 && m_bJump == true)
		{// 前後にも動いている/空中
			fRotCounterX += MODEL_ROT_X;		// カウンター加算
		}

		if (nValueH == 0)
		{// 左右に動いてない
			fRotCounterZ = 0.0f;	// リセット
		}
		else
		{// 左右にも動いている
			fRotCounterZ += MODEL_ROT_Z;		// カウンター加算
		}

		bMove = true;
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_UP) == true)	// ↓8方向移動
	{// 奥に移動
		if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_LEFT) == true)
		{// 左奥に移動
			m_move.x += sinf(D3DX_PI * 0.25f - rot.y) * fSpeedX;
			m_move.z += cosf(D3DX_PI * 0.75f + rot.y) * fSpeedZ;

			// 進んだ方向にモデルを傾ける
			UnderRotDest.z = -MODEL_ROT + fRotCounterZ;
			UpperRotDest.z = -MODEL_ROT;

			// 進んだ方向に角度を向ける
			m_rotDest.y = rot.y + (-D3DX_PI * 0.15f);

			fRotCounterZ += -MODEL_ROT_Z;		// カウンター加算
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_RIGHT) == true)
		{// 右奥に移動
			m_move.x += sinf(-D3DX_PI * 0.25f - rot.y) * fSpeedX;
			m_move.z += cosf(-D3DX_PI * 0.75f + rot.y) * fSpeedZ;

			// 進んだ方向にモデルを傾ける
			UnderRotDest.z = MODEL_ROT + fRotCounterZ;
			UpperRotDest.z = MODEL_ROT;

			// 進んだ方向に角度を向ける
			m_rotDest.y = rot.y + (D3DX_PI * 0.15f);

			fRotCounterZ += MODEL_ROT_Z;		// カウンター加算
		}
		else if (pInputKeyboard->GetPress(DIK_W) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_UP) == true)
		{// 奥に移動
			m_move.x += sinf(D3DX_PI * 1.0f + rot.y) * fSpeedX;
			m_move.z += cosf(D3DX_PI * 1.0f + rot.y) * fSpeedZ;

			fRotCounterZ = 0.0f;	// リセット
		}

		// 進んだ方向にモデルを傾ける
		UnderRotDest.x = -MODEL_ROT + fRotCounterX;
		UpperRotDest.x = -MODEL_ROT;
		TireRot.x += -TIRE_ROT;

		if (m_bJump == true)
		{// 空中
			fRotCounterX += -MODEL_ROT_X;		// カウンター加算
		}

		bMove = true;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_DOWN) == true)
	{// 手前に移動
		if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_LEFT) == true)
		{// 左手前に移動
			m_move.x += sinf(D3DX_PI * 0.75f - rot.y) * fSpeedX;
			m_move.z += cosf(D3DX_PI * 0.25f + rot.y) * fSpeedZ;

			// 進んだ方向にモデルを傾ける
			UnderRotDest.z = -MODEL_ROT + fRotCounterZ;
			UpperRotDest.z = -MODEL_ROT;

			// 進んだ方向に角度を向ける
			m_rotDest.y = rot.y + (D3DX_PI * 0.15f);

			fRotCounterZ += -MODEL_ROT_Z;		// カウンター加算
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_RIGHT) == true)
		{// 右手前に移動
			m_move.x += sinf(-D3DX_PI * 0.75f - rot.y) * fSpeedX;
			m_move.z += cosf(-D3DX_PI * 0.25f + rot.y) * fSpeedZ;

			// 進んだ方向にモデルを傾ける
			UnderRotDest.z = MODEL_ROT + fRotCounterZ;
			UpperRotDest.z = MODEL_ROT;

			// 進んだ方向に角度を向ける
			m_rotDest.y = rot.y + (-D3DX_PI * 0.15f);

			fRotCounterZ += MODEL_ROT_Z;		// カウンター加算
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_DOWN) == true)
		{// 手前に移動
			m_move.x += sinf(D3DX_PI * 0.0f + rot.y) * fSpeedX;
			m_move.z += cosf(D3DX_PI * 0.0f + rot.y) * fSpeedZ;

			fRotCounterZ = 0.0f;	// リセット
		}

		// 進んだ方向にモデルを傾ける
		UnderRotDest.x = MODEL_ROT + fRotCounterX;
		UpperRotDest.x = MODEL_ROT;
		TireRot.x += TIRE_ROT;

		if (m_bJump == true)
		{// 空中
			fRotCounterX += MODEL_ROT_X;		// カウンター加算
		}

		bMove = true;
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_LEFT) == true)
	{// 左に移動
		// 進んだ方向にモデルを傾ける
		UnderRotDest.z = -MODEL_ROT + fRotCounterZ;
		UpperRotDest.z = -MODEL_ROT;

		fRotCounterZ += -MODEL_ROT_Z;		// カウンター加算

		bMove = true;
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_RIGHT) == true)
	{// 右に移動
		// 進んだ方向にモデルを傾ける
		UnderRotDest.z = MODEL_ROT + fRotCounterZ;
		UpperRotDest.z = MODEL_ROT;

		fRotCounterZ += MODEL_ROT_Z;		// カウンター加算

		bMove = true;
	}
	else
	{// 動いてない
		// オフセットに戻す
		UpperRotDest = UpperRotOff;
		UnderRotDest.x = MODEL_ROT + fRotCounterX;
		UnderRotDest.z = UnderRotOff.z;

		fRotCounterZ = 0.0f;	// リセット

		bMove = false;
	}

	if (m_bJump == false)
	{// 着地している
		fRotCounterX = 0.0f;	// リセット
	}

	if (fRotCounterZ > MAX_ROTADD)
	{// 追加角度の最大値
		fRotCounterZ = MAX_ROTADD;
	}

	if (fRotCounterZ < MIN_ROTADD)
	{// 追加角度の最小値
		fRotCounterZ = MIN_ROTADD;
	}

	// 向きを調整
	CorrectAngle(&UpperRotDest.x, UpperRotDest.x - UpperRot.x);
	CorrectAngle(&UpperRotDest.z, UpperRotDest.z - UpperRot.z);
	CorrectAngle(&UnderRotDest.x, UnderRotDest.x - UnderRot.x);
	CorrectAngle(&UnderRotDest.z, UnderRotDest.z - UnderRot.z);

	// モデルの傾きを加算
	UnderRot += (UnderRotDest - UnderRot) * 0.1f;
	UpperRot += (UpperRotDest - UpperRot) * 0.1f;

	// モデルの向きを適用
	m_apModel[0]->SetRotOff(UnderRot);
	m_apModel[1]->SetRotOff(UpperRot);
	m_apModel[2]->SetRotOff(TireRot);

	m_bMove = bMove;		// 保存

	return bMove;
}

//========================================================================
// 外部ファイルからモデルデータを読み込む
//========================================================================
HRESULT CPlayer::SetModel(const char* pFilename)
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

	// モーションセット用の変数
	CMotion::INFO info = {};	// モーション情報
	int nLoop = 0;				// ループするかどうか
	int nNumKey = 0;			// キーの総数
	int nFrame = 0;				// 再生フレーム数
	int nCntMotion = 0;			// モーション番号
	CMotion::KEY key = {};		// キー要素

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

			if (line.find("MOTIONSET") != std::string::npos)
			{// モーションの設定
				file >> line;				// 次の文字列

				if (line.find("#") != std::string::npos)
				{// コメント
					std::getline(file, line);

					// 次の文字列を読み込む
					file >> line;			// 次の文字列
				}

				if (line.find("LOOP") != std::string::npos)
				{// ループするかどうか
					file >> line;				// =
					file >> nLoop;				// ループするかどうか

					// ループの真偽を代入
					if (nLoop == 0)
					{// false
						info.bLoop = false;
					}
					else if (nLoop == 1)
					{// true
						info.bLoop = true;
					}

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				if (line.find("NUM_KEY") != std::string::npos)
				{// キーの総数
					file >> line;				// =
					file >> nNumKey;			// キーの総数

					// キーの総数を代入
					info.nNumKey = nNumKey;

					file >> line;				// 次の文字列

					if (line.find("#") != std::string::npos)
					{// コメント
						std::getline(file, line);

						// 次の文字列を読み込む
						file >> line;			// 次の文字列
					}
				}

				// キー読み込み
				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{
					if (line.find("KEYSET") != std::string::npos)
					{// KEYSET
						file >> line;				// 次の文字列

						if (line.find("#") != std::string::npos)
						{// コメント
							std::getline(file, line);

							// 次の文字列を読み込む
							file >> line;			// 次の文字列
						}
					}

					if (line.find("FRAME") != std::string::npos)
					{// 再生フレーム数
						file >> line;				// =
						file >> nFrame;				// 再生フレーム数

						info.aKeyInfo[nCntKey].nFrame = nFrame;

						file >> line;				// 次の文字列

						if (line.find("#") != std::string::npos)
						{// コメント
							std::getline(file, line);

							// 次の文字列を読み込む
							file >> line;			// 次の文字列
						}
					}

					// パーツごとに読み込み
					for (int nCntPartsKey = 0; nCntPartsKey < nNumParts; nCntPartsKey++)
					{
						if (line.find("KEY") != std::string::npos)
						{// KEY
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
							file >> key.fPosX;			// 位置(オフセット)x
							file >> key.fPosY;			// 位置(オフセット)y
							file >> key.fPosZ;			// 位置(オフセット)y

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
							file >> key.fRotX;			// 向き(オフセット)x
							file >> key.fRotY;			// 向き(オフセット)y
							file >> key.fRotZ;			// 向き(オフセット)y

							file >> line;				// 次の文字列

							if (line.find("#") != std::string::npos)
							{// コメント
								std::getline(file, line);

								// 次の文字列を読み込む
								file >> line;			// 次の文字列
							}
						}

						if (line.find("END_KEY") != std::string::npos)
						{// END_KEY
							file >> line;				// 次の文字列
						}

						// モーション情報に代入
						info.aKeyInfo[nCntKey].aKey[nCntPartsKey] = key;
					}

					if (line.find("END_KEYSET") != std::string::npos)
					{// END_KEYSET
						file >> line;				// 次の文字列
					}
				}

				nCntMotion++;

				// モーション情報を設定
				m_pMotion->SetInfo(info);

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