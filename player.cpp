//========================================================================
// 
// プレイヤー [player.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "player.h"

#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "texture.h"

#include "camera.h"
#include "model.h"
#include "motion.h"
#include "effect3D.h"
#include "particle3D.h"
#include "energyrock.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define LAND_MOVEMENT	(D3DXVECTOR3(0.6f, 0.6f, 0.6f))			// 移動量(地上)
#define AIR_MOVEMENT	(D3DXVECTOR3(0.1f, 0.1f, 0.1f))			// 移動量(空中)
#define MOVE_INERTIA	(0.1f)									// 移動量の慣性
#define ROT				(D3DXVECTOR3(0.05f, 0.05f, 0.05f))		// 向き移動量
#define ONE_LINE		(100)									// ファイルの一行として読み取る文字数
#define MODEL_ROT		(D3DX_PI * 0.05f)						// モデルの傾き具合
#define TIRE_ROT		(0.15f)									// タイヤの回り具合
#define JUMP_HEIGHT		(4.0f)									// ジャンプの高さ
#define JUMP_ADD		(0.02f)									// ジャンプの変化量
#define MAX_JUMP		(1.0f)									// ジャンプ量の最大値
#define GRAVITY			(-0.3f)									// 重力

//========================================================================
// プレイヤークラスの生成処理
//========================================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}

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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_fJumpHigh = 0.0f;
	m_bJump = false;
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
	m_rot = rot;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
	D3DXVECTOR3 pos = CPlayer::GetPosition();				// プレイヤーの位置	
	D3DXVECTOR3 rot = CPlayer::GetRotation();				// プレイヤーの向き

	D3DXVECTOR3 UpperPosOff = m_apModel[1]->GetPosOffC();	// 上半身の位置(オフセット保存)
	D3DXVECTOR3 UpperPos = m_apModel[1]->GetPosOff();		// 上半身の位置(オフセットをいじる)

	// 移動処理
	if (Movement(rot) == true)
	{// 移動している
		if (m_bJump == false)
		{// ジャンプしていない
			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_MOVE, true, 20);
		}
	}
	else if(m_bJump == false && m_bLand == false && m_bAct == false)
	{// 移動していない
		// モーションを設定
		m_pMotion->Set(MOTIONTYPE_NEUTRAL, true, 20);
	}

	if (m_bJump == false)
	{// ジャンプしていないとき
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
			}
		}

		if (pInputKeyboard->GetRelease(DIK_SPACE) == true || pInputJoypad->GetRelease(0, CInputJoypad::JOYKEY_A) == true)
		{// ジャンプ
			m_bJump = true;
			m_move.y = JUMP_HEIGHT + (JUMP_HEIGHT * m_fJumpHigh);

			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_JUMP, true, 20);

			// ジャンプ量リセット
			m_fJumpHigh = 0.0f;
			UpperPos.y = UpperPosOff.y;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true || pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true)
	{// 位置回転リセット
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	if (pos.y < 0.0f)
	{// 地面との当たり判定
		if (m_bJump == true)
		{// 着地
			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_LANDING, true, 20);
			m_bLand = true;
		}

		pos.y = 0.0f;
		m_move.y = 0.0f;
		m_bJump = false;

		if (m_bLand == true && m_pMotion->GetType() == MOTIONTYPE_LANDING && m_pMotion->IsFinish() == true)
		{// 着地モーションが終わった
			m_bLand = false;
			m_bAct = false;
		}
	}

	// 目的の向きを修正
	float fmoveAngle = m_rotDest.y - rot.y;

	// 向きを調整
	if (fmoveAngle > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (fmoveAngle < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2;
	}

	// 角度を慣性ありで加算
	rot.y += (m_rotDest.y - rot.y) * 0.1f;

	// 当たり判定
	if (CollisionEnergyRock(pos) == true && m_bJump == false)
	{// エネルギー鉱物と当たっているとき/空中ではないとき
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{// 回収するキーを押した
			// モーションを設定
			m_pMotion->Set(MOTIONTYPE_ACTION, true, 20);
			m_bAct = true;
		}
	}

	if (m_bAct == true && m_pMotion->GetType() == MOTIONTYPE_ACTION && m_pMotion->IsFinish() == true)
	{// アクションモーションが終わった
		m_bAct = false;
	}

	// 位置/向きを適用
	SetPosition(pos);
	SetRotation(rot);

	// モデルの位置を適用
	m_apModel[1]->SetPosOff(UpperPos);

	// モーションの更新
	m_pMotion->Update();

	pDebugProc->Print("%d\n", m_pMotion->GetType());
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

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

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
	if (m_rot.x > D3DX_PI)
	{
		m_rot.x -= D3DX_PI * 2.0f;
	}
	else if (m_rot.x < -D3DX_PI)
	{
		m_rot.x += D3DX_PI * 2.0f;
	}

	// Y向きを調整
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// Z向きを調整
	if (m_rot.z > D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}
	else if (m_rot.z < -D3DX_PI)
	{
		m_rot.z += D3DX_PI * 2.0f;
	}
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
// プレイヤーの移動処理
//========================================================================
bool CPlayer::Movement(const D3DXVECTOR3 rot)
{
	// ローカル変数
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得

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

	if (pInputJoypad->GetStick(0, CInputJoypad::JOYKEY_LEFTSTICK, &nValueH, &nValueV) == true)
	{// スティック移動
		float fSpeed = -(float)nValueV * 0.0000183f;		// スピード

		m_move.x += sinf(rot.y) * (m_bJump ? fSpeed / 6.0f : fSpeed);
		m_move.z += cosf(rot.y) * (m_bJump ? fSpeed / 6.0f : fSpeed);

		// 進んだ方向にモデルを傾ける
		UnderRotDest.x = MODEL_ROT * ((nValueV < 0) ? 1 : -1);
		UnderRotDest.z = MODEL_ROT * ((nValueH < 0) ? -1 : 1);
		UpperRotDest.x = MODEL_ROT * ((nValueV < 0) ? 1 : -1);
		UpperRotDest.z = MODEL_ROT * ((nValueH < 0) ? -1 : 1);
		TireRot.x += TIRE_ROT * ((nValueV < 0) ? 1 : -1); 

		// 進んだ方向に角度を向ける
		m_rotDest.y = rot.y + ((float)nValueH * 0.00001f * ((nValueV < 0) ? -1 : 1));

		bMove = true;
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_UP) == true)	// ↓8方向移動
	{// 奥に移動
		if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_LEFT) == true)
		{// 左奥に移動
			m_move.x += sinf(D3DX_PI * 0.25f - rot.y) * (m_bJump ? AIR_MOVEMENT.x : LAND_MOVEMENT.x);
			m_move.z += cosf(D3DX_PI * 0.75f + rot.y) * (m_bJump ? AIR_MOVEMENT.z : LAND_MOVEMENT.z);

			// 進んだ方向にモデルを傾ける
			UnderRotDest.z = -MODEL_ROT;
			UpperRotDest.z = -MODEL_ROT;

			// 進んだ方向に角度を向ける
			m_rotDest.y = rot.y + (-D3DX_PI * 0.15f);
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_RIGHT) == true)
		{// 右奥に移動
			m_move.x += sinf(-D3DX_PI * 0.25f - rot.y) * (m_bJump ? AIR_MOVEMENT.x : LAND_MOVEMENT.x);
			m_move.z += cosf(-D3DX_PI * 0.75f + rot.y) * (m_bJump ? AIR_MOVEMENT.z : LAND_MOVEMENT.z);

			// 進んだ方向にモデルを傾ける
			UnderRotDest.z = MODEL_ROT;
			UpperRotDest.z = MODEL_ROT;

			// 進んだ方向に角度を向ける
			m_rotDest.y = rot.y + (D3DX_PI * 0.15f);
		}
		else if (pInputKeyboard->GetPress(DIK_W) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_UP) == true)
		{// 奥に移動
			m_move.x += sinf(D3DX_PI * 1.0f + rot.y) * (m_bJump ? AIR_MOVEMENT.x : LAND_MOVEMENT.x);
			m_move.z += cosf(D3DX_PI * 1.0f + rot.y) * (m_bJump ? AIR_MOVEMENT.z : LAND_MOVEMENT.z);
		}

		// 進んだ方向にモデルを傾ける
		UnderRotDest.x = -MODEL_ROT;
		UpperRotDest.x = -MODEL_ROT;
		TireRot.x += -TIRE_ROT;

		bMove = true;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_DOWN) == true)
	{// 手前に移動
		if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_LEFT) == true)
		{// 左手前に移動
			m_move.x += sinf(D3DX_PI * 0.75f - rot.y) * (m_bJump ? AIR_MOVEMENT.x : LAND_MOVEMENT.x);
			m_move.z += cosf(D3DX_PI * 0.25f + rot.y) * (m_bJump ? AIR_MOVEMENT.z : LAND_MOVEMENT.z);

			// 進んだ方向にモデルを傾ける
			UnderRotDest.z = -MODEL_ROT;
			UpperRotDest.z = -MODEL_ROT;

			// 進んだ方向に角度を向ける
			m_rotDest.y = rot.y + (D3DX_PI * 0.15f);
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_RIGHT) == true)
		{// 右手前に移動
			m_move.x += sinf(-D3DX_PI * 0.75f - rot.y) * (m_bJump ? AIR_MOVEMENT.x : LAND_MOVEMENT.x);
			m_move.z += cosf(-D3DX_PI * 0.25f + rot.y) * (m_bJump ? AIR_MOVEMENT.z : LAND_MOVEMENT.z);

			// 進んだ方向にモデルを傾ける
			UnderRotDest.z = MODEL_ROT;
			UpperRotDest.z = MODEL_ROT;

			// 進んだ方向に角度を向ける
			m_rotDest.y = rot.y + (-D3DX_PI * 0.15f);
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_DOWN) == true)
		{// 手前に移動
			m_move.x += sinf(D3DX_PI * 0.0f + rot.y) * (m_bJump ? AIR_MOVEMENT.x : LAND_MOVEMENT.x);
			m_move.z += cosf(D3DX_PI * 0.0f + rot.y) * (m_bJump ? AIR_MOVEMENT.z : LAND_MOVEMENT.z);
		}

		// 進んだ方向にモデルを傾ける
		UnderRotDest.x = MODEL_ROT;
		UpperRotDest.x = MODEL_ROT;
		TireRot.x += TIRE_ROT;

		bMove = true;
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_LEFT) == true)
	{// 左に移動
		// 進んだ方向にモデルを傾ける
		UpperRotDest.z = -MODEL_ROT;

		bMove = true;
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_RIGHT) == true)
	{// 右に移動
		// 進んだ方向にモデルを傾ける
		UpperRotDest.z = MODEL_ROT;

		bMove = true;
	}
	else
	{// 動いてない
		// オフセットに戻す
		UpperRotDest = UpperRotOff;
		UnderRotDest = UnderRotOff;

		bMove = false;
	}

	// モデルの傾きを加算
	UnderRot += (UnderRotDest - UnderRot) * 0.1f;
	UpperRot += (UpperRotDest - UpperRot) * 0.1f;

	// モデルの向きを適用
	m_apModel[0]->SetRotOff(UnderRot);
	m_apModel[1]->SetRotOff(UpperRot);
	m_apModel[2]->SetRotOff(TireRot);

	return bMove;
}

//========================================================================
// エネルギー鉱物との当たり判定
//========================================================================
bool CPlayer::CollisionEnergyRock(const D3DXVECTOR3 pos)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIORITY_NUM; nCntPri++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject* pObj;

			// オブジェクトを取得
			pObj = GetObject(nCntPri, nCntObj);

			if (pObj != NULL)
			{// NULLチェック
				CObject::TYPE type;

				// オブジェクトの種類を取得
				type = pObj->GetType();

				if (type == CObject::TYPE_ENERGYROCK)
				{// エネルギー鉱物オブジェクトなら当たり判定する
					D3DXVECTOR3 posRock, dist;

					// エネルギー鉱物の位置を取得
					posRock = pObj->GetPosition();

					// 距離を計算
					dist = pos - posRock;

					if ((D3DXVec3Length(&dist) < ENERGYROCK_RADIUS + m_fRadius) &&
						pos.y < posRock.y + ENERGYROCK_HEIGHT && pos.y + m_fHeight > posRock.y)
					{// エネルギー鉱物と重なった
						CParticle3D::Create(posRock, 10, 5, 5.0f, 0.0f, CEffect3D::TYPE_BLENDADD,
							CParticle3D::TYPE_NORMAL, 10, 3.0f);

						return true;
					}
				}
			}
		}
	}

	return false;
}

//========================================================================
// 外部ファイルからモデルデータを読み込む
//========================================================================
HRESULT CPlayer::SetModel(const char* pFilename)
{
	// ローカル変数宣言
	FILE* pFile;
	char aString[256] = {};				// ファイルのテキスト読み込み
	char aTrash[256] = {};				// ごみ箱
	char aModelName[64][256] = {};		// モデルの名前

	// テクスチャ読み込み用の変数
	int nNumTexture = 0;
	char aTextureName[64][256] = {};		// テクスチャの名前

	// キャラクターセット用の変数
	int nNumParts = 0;		// 読み込むパーツ数
	float fRadius = 0.0f;	// キャラクターの半径
	float fHeight = 0.0f;	// キャラクターの高さ
	float fMove = 0.0f;		// キャラクターの移動量
	float fJump = 0.0f;		// キャラクターのジャンプ量
	D3DXVECTOR2 Blowoff = D3DXVECTOR2(0.0f, 0.0f);		// 吹っ飛び量
	D3DXVECTOR2 Somersault = D3DXVECTOR2(0.0f, 0.0f);	// バク宙量

	// パーツセット用の変数
	int nIdx = -1;			// モデル番号
	int nIdxParent = -1;	// 親のモデル番号
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き

	// モーションセット用の変数
	CMotion::INFO info = {};	// モーション情報
	int nLoop = 0;				// ループするかどうか
	int nNumKey = 0;			// キーの総数
	int nFrame = 0;				// 再生フレーム数
	int nCntMotion = 0;			// モーション番号
	CMotion::KEY key = {};		// キー要素

	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%[^SCRIPT]", &aTrash[0]);

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (aString[0] == '#')
			{// コメントは無視
				fgets(&aTrash[0], ONE_LINE, pFile);

				continue;
			}

			if (strcmp(&aString[0], "NUM_MODEL") == 0)
			{// モデル数の読み込み
				fscanf(pFile, " = %d", &m_nNumModel);

				continue;
			}

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{// モデルの名前読み込み
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{
					if (nCntModel > 0)
					{// 2回目以降のMODEL_FILENAMEを読み込んでおく
						if (strcmp(&aString[0], "MODEL_FILENAME") != 0)
						{// コメントの代わりに読み込んだ場合はなし
							fscanf(pFile, "%s", &aTrash[0]);
						}
					}

					fscanf(pFile, " = %s", &aModelName[nCntModel][0]);		// モデルのパス
					m_apFileName[nCntModel] = &aModelName[nCntModel][0];

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);
					}
				}

				continue;
			}

			if (strcmp(&aString[0], "NUM_TEXTURE") == 0)
			{// テクスチャ数の読み込み
				fscanf(pFile, " = %d", &nNumTexture);

				continue;
			}

			if (strcmp(&aString[0], "TEXTURE_FILENAME") == 0)
			{// テクスチャの名前読み込み
				for (int nCntTexture = 0; nCntTexture < nNumTexture; nCntTexture++)
				{
					if (nCntTexture > 0)
					{// 2回目以降のTEXTURE_FILENAMEを読み込んでおく
						if (strcmp(&aString[0], "TEXTURE_FILENAME") != 0)
						{// コメントの代わりに読み込んだ場合はなし
							fscanf(pFile, "%s", &aTrash[0]);
						}
					}

					fscanf(pFile, " = %s", &aTextureName[nCntTexture][0]);		// テクスチャのパス

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);
					}
				}

				continue;
			}

			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{// キャラクターの設定
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp(&aString[0], "NUM_PARTS") == 0)
				{// 読み込むパーツ数
					fscanf(pFile, " = %d", &nNumParts);

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				if (strcmp(&aString[0], "RADIUS") == 0)
				{// キャラクターの半径
					fscanf(pFile, " = %f", &m_fRadius);

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				if (strcmp(&aString[0], "HEIGHT") == 0)
				{// キャラクターの高さ
					fscanf(pFile, " = %f", &m_fHeight);

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				if (strcmp(&aString[0], "MOVE") == 0)
				{// キャラクターの移動量
					fscanf(pFile, " = %f", &fMove);

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				if (strcmp(&aString[0], "JUMP") == 0)
				{// キャラクターのジャンプ量
					fscanf(pFile, " = %f", &fJump);

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				if (strcmp(&aString[0], "BLOWOFF") == 0)
				{// キャラクターの吹っ飛び量
					fscanf(pFile, " = %f %f", &Blowoff.x, &Blowoff.y);

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				if (strcmp(&aString[0], "SOMERSAULT") == 0)
				{// キャラクターのバク宙量
					fscanf(pFile, " = %f %f", &Somersault.x, &Somersault.y);

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				// パーツ読み込み
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{
					if (strcmp(&aString[0], "PARTSSET") == 0)
					{// PARTSSET
						fscanf(pFile, "%s", &aString[0]);
					}

					if (strcmp(&aString[0], "INDEX") == 0)
					{// モデル番号
						fscanf(pFile, " = %d", &nIdx);

						fscanf(pFile, "%s", &aString[0]);

						if (aString[0] == '#')
						{// コメント無視
							fgets(&aTrash[0], ONE_LINE, pFile);

							// 次の文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);
						}
					}

					if (strcmp(&aString[0], "PARENT") == 0)
					{// 親のモデル番号
						fscanf(pFile, " = %d", &nIdxParent);

						fscanf(pFile, "%s", &aString[0]);

						if (aString[0] == '#')
						{// コメント無視
							fgets(&aTrash[0], ONE_LINE, pFile);

							// 次の文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);
						}
					}

					if (strcmp(&aString[0], "POS") == 0)
					{// パーツの位置(オフセット)
						fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);

						fscanf(pFile, "%s", &aString[0]);

						if (aString[0] == '#')
						{// コメント無視
							fgets(&aTrash[0], ONE_LINE, pFile);

							// 次の文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);
						}
					}

					if (strcmp(&aString[0], "ROT") == 0)
					{// パーツの向き
						fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);

						fscanf(pFile, "%s", &aString[0]);

						if (aString[0] == '#')
						{// コメント無視
							fgets(&aTrash[0], ONE_LINE, pFile);

							// 次の文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);
						}
					}

					if (strcmp(&aString[0], "END_PARTSSET") == 0)
					{// END_PARTSSET
						fscanf(pFile, "%s", &aString[0]);
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

			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{// モーションの設定
				fscanf(pFile, "%s", &aString[0]);

				if (aString[0] == '#')
				{// コメント無視
					fgets(&aTrash[0], ONE_LINE, pFile);

					// 次の文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
				}

				if (strcmp(&aString[0], "LOOP") == 0)
				{// ループするかどうか
					fscanf(pFile, " = %d", &nLoop);

					// ループの真偽を代入
					if (nLoop == 0)
					{// false
						info.bLoop = false;
					}
					else if (nLoop == 1)
					{// true
						info.bLoop = true;
					}

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				if (strcmp(&aString[0], "NUM_KEY") == 0)
				{// キーの総数
					fscanf(pFile, " = %d", &nNumKey);

					// キーの総数を代入
					info.nNumKey = nNumKey;

					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{// コメント無視
						fgets(&aTrash[0], ONE_LINE, pFile);

						// 次の文字列を読み込む
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				// キー読み込み
				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{
					if (strcmp(&aString[0], "KEYSET") == 0)
					{// KEYSET
						fscanf(pFile, "%s", &aString[0]);

						if (aString[0] == '#')
						{// コメント無視
							fgets(&aTrash[0], ONE_LINE, pFile);

							// 次の文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);
						}
					}

					if (strcmp(&aString[0], "FRAME") == 0)
					{// 再生フレーム数
						fscanf(pFile, " = %d", &nFrame);

						info.aKeyInfo[nCntKey].nFrame = nFrame;

						fscanf(pFile, "%s", &aString[0]);

						if (aString[0] == '#')
						{// コメント無視
							fgets(&aTrash[0], ONE_LINE, pFile);

							// 次の文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);
						}
					}

					// パーツごとに読み込み
					for (int nCntPartsKey = 0; nCntPartsKey < nNumParts; nCntPartsKey++)
					{
						if (strcmp(&aString[0], "KEY") == 0)
						{// KEY
							fscanf(pFile, "%s", &aString[0]);

							if (aString[0] == '#')
							{// コメント無視
								fgets(&aTrash[0], ONE_LINE, pFile);

								// 次の文字列を読み込む
								fscanf(pFile, "%s", &aString[0]);
							}
						}

						if (strcmp(&aString[0], "POS") == 0)
						{// パーツの位置(オフセット)
							fscanf(pFile, " = %f %f %f", &key.fPosX, &key.fPosY, &key.fPosZ);

							fscanf(pFile, "%s", &aString[0]);

							if (aString[0] == '#')
							{// コメント無視
								fgets(&aTrash[0], ONE_LINE, pFile);

								// 次の文字列を読み込む
								fscanf(pFile, "%s", &aString[0]);
							}
						}

						if (strcmp(&aString[0], "ROT") == 0)
						{// パーツの向き
							fscanf(pFile, " = %f %f %f", &key.fRotX, &key.fRotY, &key.fRotZ);

							fscanf(pFile, "%s", &aString[0]);

							if (aString[0] == '#')
							{// コメント無視
								fgets(&aTrash[0], ONE_LINE, pFile);

								// 次の文字列を読み込む
								fscanf(pFile, "%s", &aString[0]);
							}
						}

						if (strcmp(&aString[0], "END_KEY") == 0)
						{// END_KEY
							fscanf(pFile, "%s", &aString[0]);
						}

						// モーション情報に代入
						info.aKeyInfo[nCntKey].aKey[nCntPartsKey] = key;
					}

					if (strcmp(&aString[0], "END_KEYSET") == 0)
					{// END_KEYSET
						fscanf(pFile, "%s", &aString[0]);
					}
				}

				nCntMotion++;

				// モーション情報を設定
				m_pMotion->SetInfo(info);

				continue;
			}

			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{// ファイルの読み込みが完了
				break;
			}
		}

		fclose(pFile);
	}
	else
	{// ファイルが開けなかった場合
		OutputDebugStringA("! ! ! ファイルを開けませんでした ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}