// ===================================================
// GOE_Character_Player_KAI.h キャラクターオブジェクト
// 
// hara sougo  2025/01/26
// ===================================================
#ifndef _GOE_CHARACTER_PLAYER_KAI_H_
#define _GOE_CHARACTER_PLAYER_KAI_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Player.h"

// ===================================================
// マクロ定義
// ===================================================

//
// クラス
//
class GOE_Character_Player_KAI : public GOE_Character_Player
{
	typedef GOE_Character_Player
		SuperGOE; //親クラスにあだ名をつける

public:

private:

    //通常攻撃の判定参照
    static constexpr auto N_ATTACK_TEST_ATK_COL_NAME = ("COL_ATK_PLAYER_KAI_NORAL_ATTACK");

    //通常攻撃のフレーム
    static constexpr int WALK_FRAME = 30;

    static constexpr int ATTACK_START_FRAME = 4;
    static constexpr int ATTACK_FRAME = 8;

    static constexpr int ATTACK_FTH_FRAME = 10;
    static constexpr int ATTACK_TOTAL_FRAME = ATTACK_START_FRAME + ATTACK_FRAME + ATTACK_FTH_FRAME;

    static constexpr int ATTACK_COOLDOWN_FRAME = ATTACK_TOTAL_FRAME + 20;

    static constexpr int KAKOPON_COOLDOWN_FRAME = 30;

    static constexpr float MOVE_FRICTION_MOD = 0.95f;
    static constexpr float MOVE_SPEED_MOD = 1.0f;
    //static constexpr float MOVE_SPEED_MOD_KAKO = 1.2f;
    static constexpr float MOVE_SPEED_MOD_KAKO = 1.5f;


    static constexpr float BUFF_MOD_SOLO = 2.0f;
    static constexpr float BUFF_MOD = 1.25f;

    static constexpr float BUFF_SPD_MOD_SOLO = 1.5f;
    static constexpr float BUFF_SPD_MOD = 1.1f;

    static constexpr float BUFF_INC_MOD_SOLO = 0.75f;
    static constexpr float BUFF_INC_MOD = 0.9f;

protected:
    TEX_DAT m_Tex_Main_Attack[8] = {};
    TEX_DAT m_Tex_Main_Walk[8] = {};
    TEX_DAT m_Tex_Main_Draw[8] = {};
    TEX_DAT m_Tex_Main_Idle = {};
    TEX_DAT m_Tex_Main_Down = {};

    std::map<GAME_OBJECT*, bool> m_BuffInObj = {};

    int m_AxisSpriteIndex = 0;

public:
    GOE_Character_Player_KAI(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Character_Player_KAI() {};

	virtual void Init() override;
	virtual void FirstFrame() override;
	virtual void Uninit() override;
	virtual void Update() override;
    virtual void UpdateActive() override;
    virtual void UpdateActiveLate() override;

	virtual void Draw() override;

	virtual void Dead() override;

	virtual void DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr) override;
	virtual void DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr) override;

	virtual void BlockHit(GAME_OBJECT_POSMOD_RESULT pmr) override;

    virtual void SetDownPoseStart() override;
    virtual void SetDownPose() override;

    virtual void SetCharaSubGraphic() override;
};

#endif