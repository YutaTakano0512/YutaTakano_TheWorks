// ===================================================
// GOE_Character_Player_KN.h キャラクターオブジェクト
// 
// hara sougo  2025/01/19
// ===================================================
#ifndef _GOE_CHARACTER_PLAYER_KN_H_
#define _GOE_CHARACTER_PLAYER_KN_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Player.h"

// ===================================================
// マクロ定義
// ===================================================

//
// クラス
//
class GOE_Character_Player_KN : public GOE_Character_Player
{
	typedef GOE_Character_Player
		SuperGOE; //親クラスにあだ名をつける

public:

private:
    //通常攻撃の判定参照
    static constexpr auto N_ATTACK_TEST_ATK_COL_NAME = ("COL_ATK_PLAYER_KN_NORAL_ATTACK");

    //レーザーの判定参照
    static constexpr auto KAKO_BEAM_COL_NAME = ("COL_ATK_PLAYER_KN_BEAM_ATTACK");

    //ボムの判定参照
    static constexpr auto SKILL_BOMB_COL_NAME = ("COL_ATK_PLAYER_KN_SKILL_BOMB");

    //ハンマーの衝撃波の判定参照
    static constexpr auto HAMMER_SHOCKWAVE_COL_NAME = ("COL_ATK_PLAYER_KN_HAMMER_SHOCKWAVE");

    //かこぽん初激の判定参照
    static constexpr auto KAKOPON_BEFORE_KN_COL_NAME = ("COL_ATK_KAKOPON_BEFORE_KN");

    static constexpr auto N_ATTACK_SHOCKWAVE_GENERAL_TAG = ("N_ATTACK_SHOCKWAVE_GENERAL_TAG");
    static constexpr auto N_ATTACK_LASER_GENERAL_TAG = ("N_ATTACK_LASER_GENERAL_TAG");
    static constexpr auto N_ATTACK_BOMBLASER_GENERAL_TAG = ("N_ATTACK_BOMBLASER_GENERAL_TAG");

    //通常攻撃のフレーム
    static constexpr int WALK_FRAME = 30;

    //static constexpr int ATTACK_CHARGE_START_FRAME = 20;
    static constexpr int ATTACK_CHARGE_START_FRAME = 10;
    //static constexpr int ATTACK_CHARGE_MAX_FRAME = 160 + ATTACK_CHARGE_START_FRAME;
    //static constexpr float ATTACK_CHARGE_LV1_MOD = 0.3f;
    //static constexpr float ATTACK_CHARGE_LV2_MOD = 0.6f;
    //static constexpr float ATTACK_CHARGE_LV3_MOD = 1.0f;
    static constexpr int ATTACK_CHARGE_MAX_FRAME = 80 + ATTACK_CHARGE_START_FRAME;
    static constexpr float ATTACK_CHARGE_LV1_MOD = 0.3f;
    //static constexpr float ATTACK_CHARGE_LV2_MOD = 0.5f;//無効化
    static constexpr float ATTACK_CHARGE_LV3_MOD = 1.0f;

    //static constexpr float ATTACK_CHARGE_LV1_ATK_MOD = 2.0f;
    //static constexpr float ATTACK_CHARGE_LV2_ATK_MOD = 3.0f;
    //static constexpr float ATTACK_CHARGE_LV3_ATK_MOD = 5.0f;
    static constexpr float ATTACK_CHARGE_LV1_ATK_MOD = 1.0f;
    //static constexpr float ATTACK_CHARGE_LV2_ATK_MOD = 4.0f;
    static constexpr float ATTACK_CHARGE_LV3_ATK_MOD = 3.0f;

    static constexpr int ATTACK_START_FRAME = 8;
    static constexpr int ATTACK_FRAME = 8;

    static constexpr int ATTACK_FTH_FRAME = 10;
    static constexpr int ATTACK_TOTAL_FRAME = ATTACK_START_FRAME + ATTACK_FRAME + ATTACK_FTH_FRAME;

    static constexpr int ATTACK_COOLDOWN_FRAME = ATTACK_TOTAL_FRAME + 20;

    static constexpr int KAKOPON_COOLDOWN_FRAME = 30 * 2.0f;

    //static constexpr float MOVE_FRICTION_MOD = 0.25f;
    //static constexpr float MOVE_SPEED_MOD = 0.275f;
    //static constexpr float MOVE_SPEED_MOD_KAKO = 1.0f;
    static constexpr float MOVE_FRICTION_MOD = 0.16f;
    //static constexpr float MOVE_SPEED_MOD = 0.135f;
    static constexpr float MOVE_SPEED_MOD = 0.145f;
    static constexpr float MOVE_SPEED_MOD_KAKO = 1.0f;


    static constexpr int BOMB_SET_FRAME = 24;
    static constexpr float BOMB_AREA = ONE_BLOCK * 16.0f;

    static constexpr int BOMB_EXPLOSION_START_FRAME = FRAME_ONE_SECOND * 4;
    static constexpr int BOMB_EXPLOSION_START_BEFORE_FRAME = BOMB_EXPLOSION_START_FRAME - 80;
    static constexpr int BOMB_EXPLOSION_STARTING_FRAME = 40;
    static constexpr int BOMB_EXPLOSION_ATTACK_FRAME = 180;
    static constexpr int BOMB_EXPLOSION_AFTER_FRAME = 90;
    static constexpr int BOMB_EXPLOSION_TOTAL_FRAME = 
        BOMB_EXPLOSION_START_FRAME 
        + BOMB_EXPLOSION_ATTACK_FRAME 
        + BOMB_EXPLOSION_AFTER_FRAME
        ;

protected:
    TEX_DAT m_Tex_Main_Attack[8] = {};
    TEX_DAT m_Tex_Main_Walk[8] = {};
    TEX_DAT m_Tex_Main_Draw = {};
    TEX_DAT m_Tex_Main_Idle = {};
    TEX_DAT m_Tex_Main_Down = {};
    
    unsigned int m_Tex_BeamCharge = 0;
    unsigned int m_Tex_Beam1 = 0;
    unsigned int m_Tex_Beam2 = 0;
    unsigned int m_Tex_Beam3 = 0;
    unsigned int m_Tex_Beambg = 0;
    unsigned int m_Model_Beam = 0;
    unsigned int m_Tex_ShockWave = 0;
    unsigned int m_Model_ShockWave = 0;

    unsigned int m_Tex_EnergyBall = 0;
    unsigned int m_Tex_AttackArea = 0;
    unsigned int m_Tex_Wave1 = 0;
    //unsigned int m_Tex_Wave2 = 0;
    unsigned int m_Model_Wave = 0;

    int m_AxisSpriteIndex = 0;

    bool m_IsDrawLaser = false;
    //int m_HammerAttackFrame = -1;
    bool m_HammerAttackStartFlag = false;
    int m_HammerAttackChargeFrame = 0;
    int m_HammerAttackAnimFrame = -1;

    GAME_OBJECT_COLLISION_ATTACK_CHILD m_BombAttackCol = {};
    int m_BombSetFrame = -1;
    int m_BombActiveFrame = -1;

    //現存するボム
    GAME_OBJECT_IDENTIFIER m_SkillBomb = {};
    //std::list<GAME_OBJECT_IDENTIFIER> m_SkillBombs = { };

    //KAKOPON_SET m_KakoDatBuf = {};

public:
    GOE_Character_Player_KN(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Character_Player_KN() {};

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