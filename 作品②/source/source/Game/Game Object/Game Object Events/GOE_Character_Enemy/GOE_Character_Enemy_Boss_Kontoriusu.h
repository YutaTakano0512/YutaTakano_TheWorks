// ===================================================
// GOE_Character_Enemy_Boss_Kontoriusu.h キャラクターオブジェクト
// 
// hara sougo  2025/01/25
// ===================================================
#ifndef _GOE_CHILD_CHARACTER_ENEMY_BOSS_KONTORIUSU_H_
#define _GOE_CHILD_CHARACTER_ENEMY_BOSS_KONTORIUSU_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Enemy.h"

class GOE_Character_Enemy_Boss_Kontoriusu : public GOE_Character_Enemy
{
	typedef GOE_Character_Enemy
		SuperGOE; //親クラスにあだ名をつける

private:
    static constexpr const char* BEAM_ATK_COL_NAME = "COL_ATK_ENEMY_BOSS_KONTORIUSU_BEAM";

    static constexpr const char* NORMAL_STATUS_NAME = "STATUS_ENEMY_BOSS_KONTORIUSU";
    static constexpr const char* HARD_STATUS_NAME = "STATUS_ENEMY_BOSS_KONTORIUSU_HARD";

    static constexpr int ATTACK_HAMMER_BEFORE_FIXED_FRAME = 20;

    static constexpr int ATTACK_HAMMER_BEFORE_FRAME_STARTPOSE = ATTACK_HAMMER_BEFORE_FIXED_FRAME;

    static constexpr double ATTACK_HAMMER_ATTACKING_PRESS_MOD = 0.375;
    static constexpr double ATTACK_HAMMER_ATTACKING_PRESSIN_MOD = 0.5;
    static constexpr double ATTACK_HAMMER_ATTACKING_PRESSOUT_MOD = 0.5;

    static constexpr int ATTACK_HAMMER_BEFORE_FRAME = 30;
    static constexpr int ATTACK_HAMMER_ATTACK_FRAME = 70;

    static constexpr int FRAME_GRANADE_START = 30;
    static constexpr int FRAME_GRANADE_AFTER = 70;

    static constexpr int ATTACK_BEAM_BEFORE_FIXED_FRAME = 20;

    static constexpr int ATTACK_LASER_TARGET_START_FRAME = 10;
    static constexpr int ATTACK_LASER_TARGET_ENDBEFORE_FRAME = 20;

    static constexpr float PHASE2_HP_BORDER_N = 0.5f;
    static constexpr float PHASE2_HP_BORDER_H = 0.7f;

    enum class ActionState {
        Idle = 0,
        Hammer,
        Shoot,
        Fireworks,
        Granade,
        Laser,
        SuperLaser,

        Attack,
        Test,
    };
    enum class AnimState {
        Idle = 0,
        HammerBefore,
        Hammer,
        GranadeBefore,
        Granade,
        Shoot,
        ShootBefore,
        ShootAfter,

        StartBefore,
        Start,
        End,

        Appeal_0,
        Appeal_1,
        Appeal_2,
    };

    enum class DrawType {
        Default = 0,
        Shadow,
        CutInShadow,
        WarpIn,
        WarpOut,
        Dead,
        FastMoveAfterEffect,
    };
    enum class HeadState {
        None = 0,
        Idle,
        W,
        Eye,
        Num_0,
        Num_1,
        Num_2,
        Num_3,
        Num_4,
        Num_5,
        Num_6,
        Num_7,
        Num_8,
        Num_9,

        Max,

    };
    //static constexpr const char* BLADE_ATK_COL_NAME = "COL_ATK_ENEMY_BOSS_INPERIUSU_BLADE";

    static constexpr int FRAME_BEAM_LIFETIME = 300;

    static constexpr int MODEL_NUM = 8;
    static constexpr float MODEL_SCA = 1.2f;

    void update_action_pattern();
    void update_action_pattern_normal();
    void update_action_pattern_normal_base1();
    void update_action_pattern_hard();
    void update_action_pattern_hard_base1();

    void safe_area_move();

    void event_playerPosSet();
    void become_start();
    void update_start();
    void draw_start();

    void update_test();
    void draw_test();

    void update_attack();
    void draw_attack();

    void hammer_attack(int attackPointIndex);
    void hammer_attack_all();
    void update_hammer();

    void hammer_attack_area_draw();
    void draw_hammer();


    void fireworks_shoot();
    void update_fireworks();
    void draw_fireworks();

    void laser_shoot();
    void update_laser();
    void draw_laser();
    void update_superlaser();
    void draw_superlaser();

    void update_granade();
    void draw_granade();

    void drawBody(int i, int flashnum, Float3 shakePos,
        DrawType type,
        int childIndex = -1
    );

    void shootBeam(
        Float3 pos,
        Float3 vec,
        float scale,
        int lifetime= FRAME_BEAM_LIFETIME,
        int type=0
    );

    void set_hammer_visible(bool b);

    void startFastMove(Float3 afterPos, float afterAxis, int maxF,
        bool safeAreaIgnore = false,
        std::function<void()> middlefunc = nullptr
    );
    void startWarp(Float3 afterPos, float afterAxis, int maxF,
        bool safeAreaIgnore = false,
        std::function<void()> middlefunc = nullptr
    );
protected:
    int m_RetryCount = 0;
    bool m_IsHard = false;
    int m_StartedFrameCount = 0;

    bool m_phase2Flag = false;

    float m_CurrentAxisPrivate = 0;

    float m_CurrentAxisHead = 0;
    Float3 m_HeadVec = {};
    Float3 m_HeadVecBuf = {};

    int m_ShaderIndex_CutInName_Vertex = 0;
    int m_ShaderIndex_CutInName_Pixel = 0;

    TEX_DAT m_Tex_StartingEff_Bk1 = {};
    TEX_DAT m_Tex_StartingEff_Bk2 = {};
    TEX_DAT m_Tex_StartingEff_Name = {};

    unsigned int m_Tex_AttackArea = 0;
    unsigned int m_Model_Wave = 0;

    TEX_DAT m_Tex_Dis = {};
    TEX_DAT m_Tex_Warp = {};

    TEX_DAT m_Tex_Shadow = {};
    TEX_DAT m_Tex_Model_Blade = {};

    TEX_DAT m_Tex_cursor_000 = {};
    TEX_DAT m_Tex_cursor_010 = {};
    TEX_DAT m_Tex_cursor_020 = {};

    HeadState m_HeadState = HeadState::Idle;
    TEX_DAT m_Tex_Model_Head[(int)HeadState::Max] = {};

    TEX_DAT m_Tex_Model = {};
    unsigned int m_ModelIndex[MODEL_NUM] = {};
    Float3 m_ModelTransPosStart[MODEL_NUM] = {};
    Float3 m_ModelTransRotStart[MODEL_NUM] = {};
    Float3 m_ModelTransScaStart[MODEL_NUM] = {};
    Float3 m_ModelTransPos[MODEL_NUM] = {};
    Float3 m_ModelTransRot[MODEL_NUM] = {};
    Float3 m_ModelTransSca[MODEL_NUM] = {};
    DPD_SHADER_TYPE m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY;

    Float3 m_BeforeVecBuf = {};

    int m_WazaCooldown = 100;

    int m_AttackActNum = 0;
    int m_AttackAct_Frame = -1;

    int m_TestActNum = 0;
    int m_TestAct_Frame = -1;

    bool m_IsPosCfx = false;
    Float3 m_PosCfx = {};


    AnimState m_Anim_State = {};
    int m_Anim_Frame = -1;
    int m_Anim_FrameMax = 0;
    float m_Anim_Mod = 0;
    bool m_Anim_NoEndReset = false;

    bool m_HammerVisibleFlag = false;
    int m_HammerVisibleFrame = 0;
    int m_HammerVisibleFrameMax = 1;


    bool m_StartFlag = false;
    int m_StartFrame = 0;
    bool m_StartedFlag = false;

    int m_StartMotionBeforeShaderVS = -1;
    int m_StartMotionBeforeShaderPS = -1;
    bool m_StartMotionShaderFlag = false;

    ActionState m_CurrentState = ActionState::Idle;
    int m_ActCycle = 0;
    int m_ActCycleNum = 0;

    Float3 m_TargetVec = {};
    float m_TargetDistance = 0;
    int m_ActionCooldown = 0;

    //ハンマー
    int   m_HammerFrame = -1;

    int   m_HammerWarpNum = 0;
    int   m_HammerWarpHastenFrame = 0;

    float m_HammerBeforeSpeedMod = 1.0f;

    int   m_HammerWaveNum = 0;
    float m_HammerWaveAngle = 0.0f;
    int   m_HammerWaveLifetime = 80;
    float m_HammerWaveScale = 1.0f;
    float m_HammerWaveSpeed = 70.0f;
    float m_HammerWaveGapAngleStart = 0.0f;
    float m_HammerWaveGapAngleEnd = 0.0f;

    bool  m_HammerNoTurnFlag = false;
    bool  m_HammerAfterWarpFlag = false;
    bool  m_HammerSuperFlag = false;

    bool  m_HammerQuakeFlag = false;
    int   m_HammerQuakeSternFrame = FRAME_ONE_SECOND * 5;
    float m_HammerQuakeArea = ONE_BLOCK * 10.0f;

    bool  m_HammerRumblingFlag = false;
    int   m_HammerRumblingNum = 3;
    float m_HammerRumblingArea = ONE_BLOCK * 10.0f;
    float m_HammerRumblingDistance = ONE_BLOCK * 10.0f;
    int   m_HammerRumblingDelay = 30;

    //
    std::vector<Float3> m_HammerAttackPoint{ };
    int   m_HammerAttackStartFrame = -1;

    //花火
    int   m_FireworksFrame = -1;
    int   m_FireworksWarpNum = 0;
    int   m_FireworksWarpHastenFrame = 0;
    int   m_FireworksBeforeFrame = 50;
    int   m_FireworksBulletNum = 1;
    float m_FireworksBulletAngle = 0.0f;
    int   m_FireworksBulletLifetime = FRAME_BEAM_LIFETIME;
    float m_FireworksBulletScale = 1.0f;
    float m_FireworksBulletSpeed = 0.1f;
    bool  m_FireworksNoTurnFlag = false;
    bool  m_FireworksLaserFlag = false;
    int   m_FireworksLaserDelay = FRAME_ONE_SECOND * 2;
    std::vector<std::pair<Float3, float>> m_FireworksWarpPos = {};
    Float3 m_FireworksWarpPosBuf = {};

    //
    //グレネード
    int m_GranadeFrame = -1;
    int m_GranadeAfterFrame = -1;
    std::vector<GAME_OBJECT_IDENTIFIER> m_Granade_Objects = {};

    //
    //あにひれいたー
    int m_SuperLaserFrame = -1;
    int m_SuperLaserAfterFrame = -1;
    int m_SuperLaserVariant = 0;
    std::vector<GAME_OBJECT_IDENTIFIER> m_SuperLaser_Objects = {};

    //laser
    int   m_LaserFrame = -1;
    int   m_LaserWarpNum = 0;
    int   m_LaserWarpHastenFrame = 0;
    int   m_LaserBeforeFrame = 50;
    int   m_LaserBulletNum = 1;
    float m_LaserBulletAngle = 0.0f;
    int   m_LaserBulletLifetime = FRAME_BEAM_LIFETIME;
    float m_LaserBulletScale = 1.0f;
    float m_LaserBulletSpeed = 5.0f;
    int   m_LaserBulletRapidNum = 1;
    int   m_LaserBulletRapidDelay = 10;
    bool  m_LaserNoTurnFlag = false;

    //
    //ワープ
    int m_WarpFrame = -1;
    int m_WarpFrameMax = -1;
    Float3 m_WarpPosBefore = {};
    float m_WarpAxisBefore = {};
    bool m_WarpPosCfxBefore = false;
    Float3 m_WarpPosAfter = {};
    float m_WarpAxisAfter = {};
    std::function<void()> m_WarpMiddleFunc = nullptr;

    //
    //高速移動
    int m_FastMoveFrame = -1;
    int m_FastMoveFrameMax = -1;
    Float3 m_FastMovePosBefore = {};
    float m_FastMoveAxisBefore = {};
    bool m_FastMovePosCfxBefore = false;
    Float3 m_FastMovePosAfter = {};
    float m_FastMoveAxisAfter = {};
    std::function<void()> m_FastMoveMiddleFunc = nullptr;

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
    int m_Dead_FrameMax = 120;

public:
    static constexpr const char* CHARACTER_NAME = "ENEMY_BOSS_KONTORIUSU";

    GOE_Character_Enemy_Boss_Kontoriusu(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Character_Enemy_Boss_Kontoriusu() {};

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

    void BecomeHard();
    void StartBattle();

    void StartAnim(AnimState aniSte, int maxF, bool noEndReset=false)
    {
        m_Anim_State = aniSte;
        m_Anim_Mod = 0.0f;
        m_Anim_Frame = 0;
        m_Anim_FrameMax = maxF;

        m_Anim_NoEndReset = noEndReset;
    }
    void StopAnim()
    {
        m_Anim_Mod = 0.0f;
        m_Anim_Frame = -1;
    }


    void StartTurn(float afterAxis, float speed);
    void StartTurn(Float2 afterVec, float speed);

    void StartMove(Float3 afterPos, float afterAxis, double maxF,
        bool distanceOneF = false);
    void StartWarp(Float3 afterPos, float afterAxis, int maxF,
        bool safeAreaIgnore = false,
        std::function<void()> middlefunc = nullptr
    );
    void StartWarpCfx(
        bool posCfxEnable,
        Float3 afterPos, float afterAxis, int maxF,
        std::function<void()> middlefunc = nullptr
    );
};


#endif