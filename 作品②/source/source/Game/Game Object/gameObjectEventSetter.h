// ===================================================
// gameObjectEventSetter.h ゲームオブジェクトイベントをセットする
// 
// hara sougo  2024/12/10
// ===================================================
#ifndef _GAME_OBJECT_EVENT_SETTER_H_
#define _GAME_OBJECT_EVENT_SETTER_H_


#include "../Systems/sys_general.h"
#include "gameObject.h"

//必要に応じて GameObjectEvent をインクルードする
#include "../Game Object/Game Object Events/GOE_Base.h"

#include "../Game Object/Game Object Events/GOE_Test.h"
#include "../Game Object/Game Object Events/GOE_General.h"
#include "../Game Object/Game Object Events/GOE_Effect.h"

#include "../Game Object/Game Object Events/GOE_Character.h"
#include "../Game Object/Game Object Events/GOE_Character_Player.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy.h"

//プレイヤー
#include "../Game Object/Game Object Events/GOE_Character_Player/GOE_Character_Player_KAI.h"
#include "../Game Object/Game Object Events/GOE_Character_Player/GOE_Character_Player_NEO.h"
#include "../Game Object/Game Object Events/GOE_Character_Player/GOE_Character_Player_CA.h"
#include "../Game Object/Game Object Events/GOE_Character_Player/GOE_Character_Player_KN.h"

//敵
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Miresu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Regatosu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Boss_Inperiusu.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Boss_Kontoriusu.h"

#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Rucs.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Satellite.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy/GOE_Character_Enemy_Miresu_Hyper.h"


//インライン関数
inline void GameObjectEventSet(GAME_OBJECT* obj)
{
	if (obj == nullptr) return;

	GAME_OBJECT_EVENTS* pGOE = &obj->events;
	
	std::string CharaName = "";
	if (obj->type == GAME_OBJECT_TYPE_CHARACTER && obj->tags.size() > 0)
	{
		CharaName = obj->tags[0];
	}

	//ゲームオブジェクトイベントをセットする
	//基タグ名などから決める
	
	//------------------------------------------
	// プレイヤー
	//------------------------------------------
	{

        if (CharaName.find("PLAYER_KAI") == 0)
        { //カイト
            pGOE->source = new GOE_Character_Player_KAI(obj);
        }
        else if (CharaName.find("PLAYER_NEO") == 0)
        { //ネオちゃん
            pGOE->source = new GOE_Character_Player_NEO(obj);
        }
        else if (CharaName.find("PLAYER_CA") == 0)
        { //CAくん
            pGOE->source = new GOE_Character_Player_CA(obj);
        }
        else if (CharaName.find("PLAYER_KN") == 0)
        { //コントリウス
            pGOE->source = new GOE_Character_Player_KN(obj);
        }

        else if (CharaName.find("PLAYER_TEST") == 0)
        { //テスト
            pGOE->source = new GOE_Character_Player_CA(obj);
        }
        //else if (CharaName.find("PLAYER_") == 0)
        //{ //デフォ
        //    pGOE->source = new GOE_Character_Player(obj);
        //}
	}

	//------------------------------------------
	// エネミー
	//------------------------------------------
	{

        //ミレス色違い
        if (CharaName == GOE_Character_Enemy_Miresu_Hyper::CHARACTER_NAME)
        {
            pGOE->source = new GOE_Character_Enemy_Miresu_Hyper(obj);
        }

		//ミレス
        if (
            CharaName == GOE_Character_Enemy_Miresu::CHARACTER_NAME_ATTACKER
            || CharaName == GOE_Character_Enemy_Miresu::CHARACTER_NAME_SHOOTER
        )
        {
            pGOE->source = new GOE_Character_Enemy_Miresu(obj);
        }

        //レガトス
        if (CharaName == GOE_Character_Enemy_Regatosu::CHARACTER_NAME)
        {
            pGOE->source = new GOE_Character_Enemy_Regatosu(obj);
        }

        //インぺリウス
        if (CharaName == GOE_Character_Enemy_Boss_Inperiusu::CHARACTER_NAME)
        {
            pGOE->source = new GOE_Character_Enemy_Boss_Inperiusu(obj);
        }

        //コントリウス
        if (CharaName == GOE_Character_Enemy_Boss_Kontoriusu::CHARACTER_NAME)
        {
            pGOE->source = new GOE_Character_Enemy_Boss_Kontoriusu(obj);
        }

        //ルクス
        if (CharaName == GOE_Character_Enemy_Rucs::CHARACTER_NAME)
        {
            pGOE->source = new GOE_Character_Enemy_Rucs(obj);
        }
        //サテライト
        if (CharaName == GOE_Character_Enemy_Satellite::CHARACTER_NAME)
        {
            pGOE->source = new GOE_Character_Enemy_Satellite(obj);
        }
	}


    //------------------------------------------
    // エフェクト
    //------------------------------------------
    if (obj->type == GAME_OBJECT_TYPE_EFFECT)
    {
        pGOE->source = new GOE_Effect(obj);
    }

    //------------------------------------------
    // デフォルト
    //------------------------------------------
    //if (pGOE->source == nullptr)
    //{
    //    pGOE->source = new GOE_General(obj);
    //}

	//if (name.find("ENEMY_") == 0)
	//{
	//	goe->Init /*			*/ = GOE_InitCharacterEnemyObject;
	//	goe->FirstFrame /*		*/ = GOE_FirstFrameCharacterEnemyObject;
	//	goe->Uninit /*			*/ = GOE_UninitCharacterEnemyObject;
	//	goe->Update /*			*/ = GOE_UpdateCharacterEnemyObject;
	//	goe->UpdateActive /*	*/ = GOE_UpdateActiveCharacterEnemyObject;
	//	goe->Draw /*			*/ = GOE_DrawCharacterEnemyObject;

	//	goe->Dead /*			*/ = GOE_DeadCharacterEnemyObject;
	//	goe->DamageAttacker /*	*/ = GOE_DamageAttackerCharacterEnemyObject;
	//	goe->DamageReciever /*	*/ = GOE_DamageRecieverCharacterEnemyObject;
	//	goe->BlockHit /*		*/ = GOE_BlockHitCharacterEnemyObject;
	//}
}


#endif