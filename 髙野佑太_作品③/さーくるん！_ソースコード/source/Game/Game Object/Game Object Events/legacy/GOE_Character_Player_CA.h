// ===================================================
// GOE_Character_Player_CA.h キャラクターオブジェクト
// 
// hara sougo  2025/01/19
// ===================================================
#ifndef _GOE_CHARACTER_PLAYER_CA_H_
#define _GOE_CHARACTER_PLAYER_CA_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Player.h"

// ===================================================
// マクロ定義
// ===================================================

//
// クラス
//
class GOE_Character_Player_CA : public GOE_Character_Player
{
	typedef GOE_Character_Player
		SuperGOE; //親クラスにあだ名をつける

public:

private:

    //通常攻撃の判定参照
    static constexpr auto N_ATTACK_TEST_ATK_COL_NAME = ("COL_ATK_PLAYER_TEST_NORAL_ATTACK");

    //通常攻撃のフレーム
    static constexpr int WALK_FRAME = 36;

    static constexpr int ATTACK_START_FRAME = 6;
    static constexpr int ATTACK_FRAME = 6;

    static constexpr int ATTACK_FTH_FRAME = 10;
    static constexpr int ATTACK_TOTAL_FRAME = ATTACK_START_FRAME + ATTACK_FRAME + ATTACK_FTH_FRAME;

    static constexpr int ATTACK_COOLDOWN_FRAME = ATTACK_TOTAL_FRAME + 50;

    static constexpr int KAKOPON_COOLDOWN_FRAME = 60;


protected:

public:
    GOE_Character_Player_CA(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Character_Player_CA() {};

	virtual void Init() override;
	virtual void FirstFrame() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void UpdateActive() override;

	virtual void Draw() override;

	virtual void Dead() override;

	virtual void DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr) override;
	virtual void DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr) override;

	virtual void BlockHit(GAME_OBJECT_POSMOD_RESULT pmr) override;

};

#endif