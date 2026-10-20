// ===================================================
// GOE_Character_Enemy_Rucs.h キャラクターオブジェクト
// 
// hara sougo  2025/04/06
// ===================================================
#ifndef _GOE_CHILD_CHARACTER_ENEMY_RUCS_H_
#define _GOE_CHILD_CHARACTER_ENEMY_RUCS_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Enemy.h"

class GOE_Character_Enemy_Rucs : public GOE_Character_Enemy
{
	typedef GOE_Character_Enemy
		SuperGOE; //親クラスにあだ名をつける

private:
    static constexpr const char* BEAM_ATK_COL_NAME = "COL_ATK_ENEMY_BOSS_KONTORIUSU_BEAM";

    static constexpr const char* HARD_STATUS_NAME = "STATUS_ENEMY_RUCS_HARD";

    enum class ActionState {
        Idle = 0,
        Attack,
    };
    enum class AnimState {
        Idle = 0,
        Shoot,
        Dead,
    };

    enum class DrawType {
        Default = 0,
        Shadow,
        CutInShadow,
        WarpIn,
        WarpOut,
        Dead,
    };
    //static constexpr const char* BLADE_ATK_COL_NAME = "COL_ATK_ENEMY_BOSS_INPERIUSU_BLADE";

    static constexpr int FRAME_BEAM_LIFETIME = 300;

    static constexpr int MODEL_NUM = 3;
    static constexpr float MODEL_SCA = 1.5f * 0.1f;

    void update_action_pattern();
    void update_action_pattern_normal();
    void update_action_pattern_hard();

    void update_start();
    void draw_start();
    void update_attack();
    void draw_attack();

    void drawBody(int i, int flashnum, Float3 shakePos,
        DrawType type
    );

    void shootBeam(Float3 pos, Float3 vec, float scale, int type=0);

protected:
    int m_StartedFrameCount = 0;

    float m_CurrentAxisPrivate = 0;

    float m_CurrentAxisHead = 0;
    Float3 m_HeadVec = {};
    Float3 m_HeadVecBuf = {};


    TEX_DAT m_Tex_Dis = {};
    TEX_DAT m_Tex_Warp = {};

    TEX_DAT m_Tex_Shadow = {};
    TEX_DAT m_Tex_Model_Blade = {};

    TEX_DAT m_Tex_Model = {};
    unsigned int m_ModelIndex[MODEL_NUM] = {};
    Float3 m_ModelTransPos[MODEL_NUM] = {};
    Float3 m_ModelTransRot[MODEL_NUM] = {};
    Float3 m_ModelTransSca[MODEL_NUM] = {};
    DPD_SHADER_TYPE m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY;

    Float3 m_BeforeVecBuf = {};

    int m_AttackActNum = 0;
    int m_AttackAct_Frame = -1;


    AnimState m_Anim_State = {};
    int m_Anim_Frame = -1;
    int m_Anim_FrameMax = 0;
    float m_Anim_Mod = 0;
    bool m_Anim_NoEndReset = false;


    //bool m_StartFlag = false;
    int m_StartFrame = 0;
    bool m_StartedFlag = false;

    ActionState m_CurrentState = ActionState::Idle;
    int m_ActCycle = 0;
    int m_ActCycleNum = 0;

    Float3 m_TargetVec = {};
    float m_TargetDistance = 0;
    int m_ActionCooldown = 0;

    //
    //ワープ
    int m_WarpFrame = -1;
    int m_WarpFrameMax = -1;
    Float3 m_WarpPosBefore = {};
    float m_WarpAxisBefore = {};
    Float3 m_WarpPosAfter = {};
    float m_WarpAxisAfter = {};

    //
    //シンプル移動
    int m_SimpleMoveFrame = -1;
    int m_SimpleMoveFrameMax = -1;
    Float3 m_SimpleMovePosBefore = {};
    float m_SimpleMoveAxisBefore = {};
    Float3 m_SimpleMovePosAfter = {};
    float m_SimpleMoveAxisAfter = {};

    //
    //振り向き
    Float3 m_TurnVecAfter = {};
    float m_TurnSpeed = 0;

    int m_Dead_Frame = 0;
    //int m_Dead_FrameMax = 120;
    int m_Dead_FrameMax = 90;

    bool m_ExitFlag = false;
public:
    static constexpr const char* CHARACTER_NAME = "ENEMY_RUCS";

    GOE_Character_Enemy_Rucs(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Character_Enemy_Rucs() {};

	virtual void Init() override;
	virtual void FirstFrame() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void UpdateActive() override;

	virtual void Draw() override;

	virtual void Dead() override;

    virtual void CalcDamage(
        GAME_OBJECT_DAMAGE_RESULT* output,
        const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
        int damage,
        GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
        GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
    ) override;

	virtual void DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr) override;
	virtual void DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr) override;

	virtual void BlockHit(GAME_OBJECT_POSMOD_RESULT pmr) override;

    //void StartBattle();

    void StartAnim(AnimState aniSte, int maxF, bool noEndReset=false)
    {
        m_Anim_State = aniSte;
        m_Anim_Mod - 0.0f;
        m_Anim_Frame = 0;
        m_Anim_FrameMax = maxF;

        m_Anim_NoEndReset = noEndReset;
    }
    void StopAnim()
    {
        m_Anim_Mod - 0.0f;
        m_Anim_Frame = -1;
    }

    void StartExit();

    void StartTurn(float afterAxis, float speed);
    void StartTurn(Float2 afterVec, float speed);

    void StartMove(Float3 afterPos, float afterAxis, int maxF);
    void StartWarp(Float3 afterPos, float afterAxis, int maxF,
        bool safeAreaIgnore=false);
};


#endif