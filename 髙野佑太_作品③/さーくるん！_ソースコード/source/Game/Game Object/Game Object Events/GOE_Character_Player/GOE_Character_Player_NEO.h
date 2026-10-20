// ===================================================
// GOE_Character_Player_NEO.h キャラクターオブジェクト
// 
// hara sougo  2025/01/26
// ===================================================
#ifndef _GOE_CHARACTER_PLAYER_NEO_H_
#define _GOE_CHARACTER_PLAYER_NEO_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Player.h"

// ===================================================
// マクロ定義
// ===================================================

//
// クラス
//
class GOE_Character_Player_NEO : public GOE_Character_Player
{
	typedef GOE_Character_Player
		SuperGOE; //親クラスにあだ名をつける

public:

private:

    //通常攻撃の判定参照
    static constexpr auto N_ATTACK_TEST_ATK_COL_NAME = ("COL_ATK_PLAYER_NEO_NORAL_ATTACK");
    static constexpr auto N_ATTACK_TEST_ATK_COL2_NAME = ("COL_ATK_PLAYER_NEO_NORAL_ATTACK_2");

    //通常攻撃のフレーム
    static constexpr int WALK_FRAME = 60;

    static constexpr int ATTACK_START_FRAME = 1;
    static constexpr int ATTACK_FRAME = 32;

    static constexpr int ATTACK_FTH_FRAME = 3;
    static constexpr int ATTACK_TOTAL_FRAME = ATTACK_START_FRAME + ATTACK_FRAME + ATTACK_FTH_FRAME;

    static constexpr int ATTACK_COOLDOWN_FRAME = ATTACK_TOTAL_FRAME + 10;

    static constexpr int KAKOPON_COOLDOWN_FRAME = 30;


    //static constexpr float MOVE_FRICTION_MOD = 0.12f;
    static constexpr float MOVE_FRICTION_MOD = 0.1f;
    //static constexpr float MOVE_SPEED_MOD = 0.116;
    //static constexpr float MOVE_SPEED_MOD = 0.112;
    //static constexpr float MOVE_SPEED_MOD = 0.12;
    //static constexpr float MOVE_SPEED_MOD = 0.125;
    static constexpr float MOVE_SPEED_MOD = 0.155;
    static constexpr float MOVE_SPEED_MOD_KAKO = 1.4f;
    //static constexpr float MOVE_SPEED_MOD = 1.3f;
    //static constexpr float MOVE_SPEED_MOD_KAKO = 1.5f;

    static constexpr int KAKOPON_BUFFPOINT_MAX = 100000;
    static constexpr float KAKOPON_BUFFPOINT_ADDMOD = 5000.0f;
    static constexpr int KAKOPON_BUFFPOINT_COST = 80;
    static constexpr int KAKOPON_BUFFPOINT_DAMAGE_COST = KAKOPON_BUFFPOINT_MAX / 2;

    static constexpr int KAKOPON_BUFFPOINT_COST_COOLDOWM_MAX = FRAME_ONE_SECOND * 10;
    static constexpr float KAKOPON_BUFFPOINT_LVADDMOD = 0.25f;
    static constexpr int KAKOPON_BUFFPOINT_MAXLV = 4;

protected:
    virtual void calc_kakopon_damage(
        GAME_OBJECT_COLLISION_ATTACK_CHILD* output
    ) override;
protected:
    TEX_DAT m_Tex_Main_Attack[8] = {};
    TEX_DAT m_Tex_Main_Walk[8] = {};
    TEX_DAT m_Tex_Main_Draw[8] = {};
    TEX_DAT m_Tex_Main_Idle = {};
    TEX_DAT m_Tex_Main_Down = {};

    //std::map<GAME_OBJECT*, bool> m_BuffInObj = {};

    int m_KakoponBuffPoint = 0;
    int m_KakoponBuffPointCostCoolDown = 0;

    int m_AxisSpriteIndex = 0;

public:
    GOE_Character_Player_NEO(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Character_Player_NEO() {};

	virtual void Init() override;
	virtual void FirstFrame() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void UpdateActive() override;
    virtual void UpdateActiveLate()  override;

	virtual void Draw() override;

	virtual void Dead() override;

	virtual void DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr) override;
	virtual void DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr) override;

	virtual void BlockHit(GAME_OBJECT_POSMOD_RESULT pmr) override;

    virtual void InitResetAction() override;
    virtual void SetDownPoseStart() override;
    virtual void SetDownPose() override;

    virtual void SetCharaSubGraphic() override;

};

#endif