// ===================================================
// GOE_Character_Enemy_Boss_Inperiusu.h キャラクターオブジェクト
// 
// hara sougo  2024/12/10
// ===================================================
#ifndef _GOE_CHILD_CHARACTER_ENEMY_BOSS_INPERIUSU_H_
#define _GOE_CHILD_CHARACTER_ENEMY_BOSS_INPERIUSU_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Enemy.h"

class GOE_Character_Enemy_Boss_Inperiusu : public GOE_Character_Enemy
{
	typedef GOE_Character_Enemy
		SuperGOE; //親クラスにあだ名をつける

private:
    static constexpr const char* HARD_STATUS_NAME = "STATUS_ENEMY_BOSS_INPERIUSU_HARD";

    enum class ActionState {
        Idle = 0,
        Blade,
        Beam,
        Misile,
        MachineGun,
        MachineGunHand,
        EnergyBallBit,
        Granade,
        EnemyCall,

        SuperBeam,
        Phase2Start,
        HealEvent,
        WaitEvent,
    };
    enum class AnimState {
        Idle = 0,
		BladeBefore,
		Blade,
        MachinegunBefore,
        Machinegun,
        MachinegunHandBefore,
        MachinegunHand,
		BeamBefore,
		Beam,
		GranadeBefore,
		Granade,
        GuruguruBefore,
        Guruguru,
        //MisileBefore,
        Misile,
        SuperBeamBefore,
        SuperBeam,

        Damage,
        Damage2,

        //
        Stating_ArmR_Purge,
        Stating_ArmR_Up,
        Stating_ArmL_Purge,
        Stating_ArmL_Up,
        Stating_Body1_Up,
        Stating_Head_Up,

        Stating_All_Purge,
        Stating_All_Up,
    };
    enum class DrawType {
        Default = 0,
        Shadow,
        CutInShadow,
        WarpIn,
        WarpOut,
        Dead,
        Mask,
    };
    static constexpr const char* BLADE_ATK_COL_NAME = "COL_ATK_ENEMY_BOSS_INPERIUSU_BLADE";
    static constexpr const char* BEAM_ATK_COL_NAME = "COL_ATK_ENEMY_BOSS_INPERIUSU_BEAM";
    static constexpr const char* MISILE_ATK_COL_NAME = "COL_ATK_ENEMY_BOSS_INPERIUSU_MISILE";
	static constexpr const char* MACHINEGUN_ATK_COL_NAME = "COL_ATK_ENEMY_BOSS_INPERIUSU_MACHINEGUN";

    static constexpr int ATTACK_BLADE_BEFORE_FRAME_STARTPOSE = 60;
    static constexpr int ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE = 7;
    //static constexpr int ATTACK_BLADE_ATTACKING_FRAME_STARTPOSE = 8;
    static constexpr int ATTACK_BLADE_BEFORE_FIXED_FRAME = 50;
    static constexpr int ATTACK_BLADE_BEFORE_FRAME = 100;
    //static constexpr int ATTACK_BLADE_ATTACK_FRAME = 64;
    //static constexpr int ATTACK_BLADE_ATTACK_FRAME = 90;
    static constexpr int ATTACK_BLADE_ATTACK_FRAME = 90;
    //static constexpr int ATTACK_BLADE_FRAME = ATTACK_BLADE_BEFORE_FRAME + ATTACK_BLADE_ATTACK_FRAME;
    //static constexpr int ATTACK_BLADE_EFFECT_START_FRAME = 32;
    static constexpr int ATTACK_BLADE_EFFECT_START_FRAME = 5;
    static constexpr int ATTACK_BLADE_ATTACKING_FRAME = 8;
    //static constexpr int ATTACK_BLADE_ATTACK_START_FRAME = 10;
    static constexpr int ATTACK_BLADE_ATTACK_START_FRAME = ATTACK_BLADE_BEFORE_FRAME;
    

    static constexpr int ANIMCOUNT_BEAM_CHARGE_X = 16;
    static constexpr int ANIMCOUNT_BEAM_CHARGE_Y = 16;
    static constexpr int ANIMCOUNTMAX_BEAM_CHARGE = ANIMCOUNT_BEAM_CHARGE_X * ANIMCOUNT_BEAM_CHARGE_Y;

	static constexpr int ATTACK_BEAM_BEFORE_FIXED_FRAME = 20;

	//static constexpr int FRAME_BEAM_SHOOT_START = ANIMCOUNTMAX_BEAM_CHARGE * 0.35f;
	//static constexpr int FRAME_BEAM_SHOOT_END = ANIMCOUNTMAX_BEAM_CHARGE * 0.9f;
    static constexpr int FRAME_BEAM_SHOOT_START = 120;
    //static constexpr int FRAME_BEAM_SHOOT_START = 130;
	//static constexpr int FRAME_BEAM_SHOOT_END = FRAME_BEAM_SHOOT_START + 180;
	static constexpr int FRAME_BEAM_SHOOT_END = FRAME_BEAM_SHOOT_START + 60 * 4;
	static constexpr int FRAME_BEAM_SHOOT_START_BEFORE = FRAME_BEAM_SHOOT_START - 90;
    static constexpr int FRAME_BEAM_AFTER = 30; //80;

	static constexpr int FRAME_MISILE_START = 30;
	static constexpr int FRAME_MISILE_AFTER = FRAME_MISILE_START + 120;

    static constexpr int MISILE_HOMING_FLAG = 1;
    static constexpr int MISILE_OSOI_FLAG = 2;
    static constexpr int MISILE_HOMING_FALL_FLAG = 3;

    static constexpr int FRAME_MISILE_LIFETIME = FRAME_ONE_SECOND * 8;

	//static constexpr int FRAME_MACHINEGUN_SHOOT_START = ANIMCOUNTMAX_BEAM_CHARGE * 0.35f;
	//static constexpr int FRAME_MACHINEGUN_SHOOT_END = ANIMCOUNTMAX_BEAM_CHARGE * 0.9f;
	static constexpr int FRAME_MACHINEGUN_START = 30;
	static constexpr int FRAME_MACHINEGUN_SHOOT_START = 
		FRAME_MACHINEGUN_START + ANIMCOUNTMAX_BEAM_CHARGE * 0.35f;
	static constexpr int FRAME_MACHINEGUN_SHOOT_END = 
		FRAME_MACHINEGUN_START + ANIMCOUNTMAX_BEAM_CHARGE * 0.9f;
	static constexpr int FRAME_MACHINEGUN_AFTER = 60;

	static constexpr int FRAME_GRANADE_START = 30;
	static constexpr int FRAME_GRANADE_AFTER = 70;

    static constexpr int SUPER_BEAM_LEGABEAM_IN_FRAME = 90;
    static constexpr int SUPER_BEAM_BARRIER_IN_FRAME = 24;
    //static constexpr float SUPER_BEAM_HP_BORDER = 0.75f;
    static constexpr float SUPER_BEAM_HP_BORDER = 0.7f;

    //アナイアレイター暴発後、絶対に残るHP
    static constexpr int SUPER_BEAM_DAMAGE_MINHP = 100;
    //static constexpr float SUPER_BEAM_AFTER_WEAKNESS_MOD = 3.0f;
    static constexpr float SUPER_BEAM_AFTER_WEAKNESS_MOD = 2.0f;
    //static constexpr int SUPER_BEAM_AFTER_STARN = FRAME_ONE_SECOND * 10.0f;
    static constexpr int SUPER_BEAM_AFTER_STARN = FRAME_ONE_SECOND * 8.0f;
    static constexpr int SUPER_BEAM_AFTER_STARN_END = 140;

    static constexpr int WAZA_COOLDOWN_1 = FRAME_ONE_SECOND * 1.5f;
    static constexpr int WAZA_COOLDOWN_2 = FRAME_ONE_SECOND * 0.5f;

    //static constexpr float HARD_HP_BORDER = 0.6f;
    //static constexpr float HARD_HP_BORDER = 0.7f;
    static constexpr float HARD_HP_BORDER = 0.6f;
    //static constexpr int HARD_HP_MIN = 10;
    static constexpr int HARD_HP_MIN = 0;

    static constexpr int MODEL_NUM = 6;
    static constexpr float MODEL_SCA = 1.5f;

    static constexpr int MIRRORGE_STATE_INFRAME_MAX = 30;
    static constexpr int MIRRORGE_STATE_MAX = 16;
    struct MIRRORGE_STATE {
        bool enable;
        int inFrame;

        Float3 pos;
        Float3 rot;
        Float3 sca;
        float axis;
    };

    void update_action_pattern();
    void update_action_pattern_normal();
    void update_action_pattern_hard();
    void update_action_pattern_hard1();
    void update_action_pattern_hard2();

    void event_playerPosSet();
    void become_start();
    void update_start();
    void draw_start();

    int zako_powerup();
    int zako_powerdown();
    void safe_area_move();

    void update_waitevent();
    bool update_healevent(const GAME_OBJECT_IDENTIFIER& sourcePlayer);

    void update_blade();
    void draw_blade();

	void beam_object_update();
	void beam_object_delete();
    void update_beam();
    void draw_beam();
    void draw_beam_child(Float3 mainPosition);

	void update_misile_object();
	void draw_misile_object();
    void misile_object_delete();
    void update_misile();
    void draw_misile();

    void event_super_beam_before_movieFlag();
    void update_super_beam();
    void draw_super_beam();

    void update_machinegun();
    void draw_machinegun();

    void update_machinegun_hand();
    void draw_machinegun_hand();

    void update_energyballbit();
    void draw_energyballbit();

    void update_granade();
    void draw_granade();

    void update_enemyCall();
    void draw_enemyCall();

    void update_phase2Start();
    void draw_phase2Start();
    void phase2StartEvent(bool isfast);

    void drawBody(int i, int flashnum, Float3 shakePos,
        DrawType type
    );
    void drawBodyChild(int ChildIndex, int i, int flashnum, Float3 shakePos,
        DrawType type
    );

    void deadstop_event();
    void set_blade_visible(bool b);

protected:
    int m_RetryCount = 0;
    bool m_IsHard = false;
    bool m_HardPhase2Flag = false;
    int m_Phase2StartFrame = -1;
    bool m_Phase2StartPlayerDamagedFlag = false;
    char m_Phase2HpPhase = 0;
    GAME_OBJECT_IDENTIFIER m_Phase2StartSourcePlayerObject = {};

    int m_StartedFrameCount = 0;

    float m_CurrentAxisPrivate = 0;

    float m_CurrentAxisHead = 0;
    Float3 m_HeadVec = {};
    Float3 m_HeadVecBuf = {};

    int m_ShaderIndex_CutInName_Vertex = 0;
    int m_ShaderIndex_CutInName_Pixel = 0;

    int m_bgmIn = 0;
    int m_bgmInH = 0;
    int m_bgmLoop = 0;
    int m_bgmLoopH = 0;
    //TEX_DAT m_Tex_Main_Idle[8] = {};
    //TEX_DAT m_Tex_Main_Blade[8] = {};
    //TEX_DAT m_Tex_Main_Kidou = {};
    //TEX_DAT m_Tex_Main_Beam1 = {};
    //TEX_DAT m_Tex_Main_Beam2 = {};

    TEX_DAT m_Tex_WarningPoint = {};
    unsigned int m_Model_StanStar = {};
    TEX_DAT m_Tex_Dis = {};
    TEX_DAT m_Tex_Warp = {};

    TEX_DAT m_Tex_StartingEff_Eye = {};
    TEX_DAT m_Tex_StartingEff_Eye2 = {};
    TEX_DAT m_Tex_StartingEff_Bk1 = {};
    TEX_DAT m_Tex_StartingEff_Bk2 = {};
    TEX_DAT m_Tex_StartingEff_Name = {};
    TEX_DAT m_Tex_StartingEff_NameH = {};

    TEX_DAT m_Tex_Model = {};
    TEX_DAT m_Tex_Model_Blade = {};
    TEX_DAT m_Tex_Model_Mask = {};
    TEX_DAT m_Tex_Model_MaskGray = {};
    unsigned int m_ModelIndex[MODEL_NUM] = {};
    Float3 m_ModelTransPos[MODEL_NUM] = {};
    Float3 m_ModelTransRot[MODEL_NUM] = {};
    Float3 m_ModelTransSca[MODEL_NUM] = {};
    DPD_SHADER_TYPE m_shaderType = DPD_SHADER_TYPE_METAL_ENEMY;
    Float4 m_ModelColor = { 1.0f, 1.0f, 1.0f, 1.0f };

    unsigned int m_Model_Bit = {};

    AnimState m_Anim_State = {};
    int m_Anim_Frame = -1;
    int m_Anim_FrameMax = 0;
    float m_Anim_Mod = 0;
    bool m_Anim_NoEndReset = false;


    MIRRORGE_STATE m_MirrorgeState[MIRRORGE_STATE_MAX] = {};


    bool m_StartFlag = false;
    int m_StartFrame = 0;
    bool m_StartedFlag = false;

    int m_StartEyeEffFrame = 0;
    int m_StartEyeEffFrameMax = 0;

    int m_BodyTexMaskFrame = -1;
    int m_BodyTexMaskFrameMax = 0;

    int m_BodyTexMaskFrame_Init = -1;
    int m_BodyTexMaskFrameMax_Init = FRAME_ONE_SECOND * 1.0f;
    int m_BodyTexColorFrame_Init = -1;
    int m_BodyTexColorFrameMax_Init = FRAME_ONE_SECOND * 1.0f;
    bool m_BodyTexMaskInitFlag = false;

    int m_FlashScreenMaskFrame = -1;
    int m_FlashScreenMaskFrameMax = 0;
    int m_KakoEfScreenMaskFrame = -1;
    int m_KakoEfScreenMaskFrameMax = 0;

    bool m_BladeVisibleFlag = false;
    int m_BladeVisibleFrame = 0;
    int m_BladeVisibleFrameMax = 1;

    //----------------------------------
    ActionState m_CurrentState = ActionState::Idle;
    int m_ActCycle = 0;
    int m_ActCycleNum = 0;
    Float3 m_ActionStdPos[3] = {
        { 0.0f, ONE_BLOCK * -16.0f, 0.0f},
        { ONE_BLOCK * 38.0f, ONE_BLOCK * -16.0f, 0.0f },
        { ONE_BLOCK * -38.0f, ONE_BLOCK * -16.0f, 0.0f }
    };
    int m_WazaCooldown = WAZA_COOLDOWN_1;


    //
    //待機イベント
    int m_WaitEventFrame = -1;
    int m_WaitEventFrameMax = -1;


    //
    //回復イベント
    int m_HealEventFrame = -1;
    GAME_OBJECT_IDENTIFIER m_HealEventItemObj = {};
    bool m_HealEventAttackFlag = false;


    //ブレード
    unsigned int m_TexId_AttackEffect_s = -1;
    static constexpr Int2 m_AnimCount_AttackEffect_s = { 5, 5 };
	//static constexpr Float2 m_Size_AttackEffect_s = {
	//	516.0f * 1.9f,
	//	516.0f * 1.9f
	//};
    //static constexpr Float2 m_Size_AttackEffect_s = {
    //    516.0f * 2.5f,
    //    516.0f * 2.5f
    //};
    static constexpr Float2 m_Size_AttackEffect_s = {
        516.0f * 2.75f,
        516.0f * 2.75f
    };
    int m_BladeFrame = -1;
    int m_BladeWarpNum = 0;
    int m_BladeWarpHastenFrame = 0;
    float m_BladeBeforeSpeedMod = 1.0f;
    int m_BladeWaveNum = 0;
    float m_BladeWaveAngle = 0.0f;
    bool m_BladeNoTurnFlag = false;
    bool m_BladeAfterWarpFlag = false;
    bool m_BladeSuperFlag = false;
    bool m_BladeMirrogeFlag = false;
    bool m_BladeSuperMirrogeFlag = false;

    unsigned int m_Tex_Warning[3] = {};

    //多方向レーザー
    unsigned int m_Tex_BeamCharge = 0;
    unsigned int m_Tex_Beam1 = 0;
    unsigned int m_Tex_Beam2 = 0;
    unsigned int m_Tex_Beam3 = 0;
    unsigned int m_Model_Beam = 0;
	int m_BeamFrame = -1;
    float m_BeamRot = 0.0f;
    float m_BeamNum = 4;
    bool m_BeamIsOutLine = false;
    bool m_BeamShockWaveType = false;
    float m_BeamOutLineRangeMod = 0;
    bool m_BeamNoInitMove = false;
    bool m_BeamAfterWarpFlag = false;
    int m_BeamRotatePattarn = 0;
    //std::list<GAME_OBJECT_IDENTIFIER> m_BeamObjects = {};
    std::vector<std::vector<GAME_OBJECT_IDENTIFIER>> m_BeamObjects = {};
    float m_BeamBgmVolumeBuf = 1;


    unsigned int m_Tex_Barrier_Scroll1 = 0;
    unsigned int m_Tex_Barrier_Scroll2 = 0;
    unsigned int m_Tex_Barrier_Fresnel1 = 0;
    unsigned int m_Tex_Barrier_Fresnel2 = 0;
    unsigned int m_Tex_Barrier_Ground1 = 0;
    unsigned int m_Tex_Barrier_Ground2 = 0;
    unsigned int m_Tex_Barrier_Bg = 0;
    unsigned int m_Model_Sphere = 0;


    //
    //ミサイル
    
    //unsigned int m_Tex_Misile[AXIS_2D_MAX] = {};
    unsigned int m_Tex_Misile_Model = 0;
    unsigned int m_Model_Misile = 0;
    unsigned int m_Tex_Misile_Fire_Model = 0;
    unsigned int m_Model_Fire_Misile = 0;
    bool m_MisileMeteorFlag = false;

    int m_MisileFrame = -1;
    //int m_Misile_Object_Count = 0;
    //std::map<int, GAME_OBJECT_IDENTIFIER> m_Misile_Objects = {};
    std::vector<GAME_OBJECT_IDENTIFIER> m_Misile_Objects = {};


    //エマージェンシーコール
    int m_EnemyCallFrame = -1;
    //現存する敵キャラ
    std::vector<GAME_OBJECT_IDENTIFIER> m_CallEnemies = { };

    //
    //アナイアレイター
    TEX_DAT m_Tex_SuperBeamChargeRing = {};
    int m_SuperBeamLimitF = FRAME_ONE_SECOND * 45.0f;
    //int m_SuperBeamLimitF = FRAME_ONE_SECOND * 15.0f;
    bool m_SuperBeamFinish = false;

    Float3 m_SuperBeamPosition = { 0.0f, ONE_BLOCK * -16.0f, 0.0f };
    int m_SuperBeamStartFrame = -1;
    bool m_SuperBeamStartIsNear = false;

    int m_SuperBeamFrame = -1;
    int m_SuperBeamCountDownFrame = -1;
    int m_SuperBeamAfterFrame = -1;

    std::string m_SuperBeamGayaId = "";

    float m_SuperBeamBgmVolumeBuf = 1;
    int m_SuperBeamShootBgFrame = 0;
    int m_SuperBeamShootBgFrameMax = 0;

    TEX_DAT m_Tex_ScreenBreak = {};
    int m_SuperBeamScreenBreakFrame = 0;
    int m_SuperBeamScreenBreakFrameMax = 0;

    bool m_SuperBeamFirstFlag = false;
    bool m_SuperBeamBreakFlag = false;

    bool m_SuperBeamSuccessFlag = false;
    bool m_SuperBeamSuccessWeaknessFlag = false;

    //現存する敵キャラ
    std::vector<GAME_OBJECT_IDENTIFIER> m_SuperBeamEnemiesBuf = { };

    std::vector<GAME_OBJECT_IDENTIFIER> m_SuperBeamEnemies = { };
    std::vector<Float3> m_SuperBeamEnemiesPos = { };
    std::vector<int> m_SuperBeamEnemiesFrame = { };

    std::vector<GAME_OBJECT_IDENTIFIER> m_SuperBeamEnemiesSub = { };

    bool m_SuperBeamBarrierFlag = false;
    int m_SuperBeamBarrierFrame = 0;

    //
    //でタレントマシンガン
    int m_MachineGunFrame = -1;
    Float3 m_MachineGunShootPos[2] = {};
    Float3 m_MachineGunShootVec[2] = {};
    int m_MachineGunHandFrame = -1;
    Float3 m_MachineGunHandShootPos[1] = {};
    Float3 m_MachineGunHandShootVec[1] = {};


    //
    //Оビットレーザー
    int m_EnergyBallBitFrame = -1;

    //
    //グレネード
    int m_GranadeFrame = -1;
    int m_GranadeAfterFrame = -1;
    bool m_GranadeSuperFlag = false;
    std::vector<GAME_OBJECT_IDENTIFIER> m_Granade_Objects = {};

	//
	//ワープ
	int m_WarpFrame = -1;
	int m_WarpFrameMax = -1;
	Float3 m_WarpPosBefore = {};
	float m_WarpAxisBefore = {};
	Float3 m_WarpPosAfter = {};
	float m_WarpAxisAfter = {};

	//
	//ジェット
	int m_JetFrame = -1;
	int m_JetFrameMax = -1;
	Float3 m_JetPosBefore = {};
	float m_JetAxisBefore = {};
	Float3 m_JetPosAfter = {};
	float m_JetAxisAfter = {};
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

    //----------------------------------

    Float3 m_TargetVec = {};
    float m_TargetDistance = 0;

    int m_ActionCooldown = 0;
    int m_StarnFrame = 0;

    //int m_StartMotionBeforeShader = 0;
    int m_StartMotionBeforeShaderVS = 0;
    int m_StartMotionBeforeShaderPS = 0;
    bool m_StartMotionShaderFlag = false;

    TEX_DAT m_Tex_ScKakoEffect = {};
    TEX_DAT m_Tex_Defeat = {};

    TEX_DAT m_Tex_cursor_000 = {} ;
    TEX_DAT m_Tex_cursor_010 = {} ;
    TEX_DAT m_Tex_cursor_020 = {} ;

    TEX_DAT m_Tex_Shadow = {};

	unsigned int m_Tex_EnergyBall = 0;
	unsigned int m_Tex_PowerBg = 0;
	unsigned int m_Tex_Vignette = 0;

    int m_Dead_Frame = 0;
    int m_Dead_FrameMax = 360;

public:
    static constexpr const char* CHARACTER_NAME = "ENEMY_BOSS_INPERIUSU";

	GOE_Character_Enemy_Boss_Inperiusu(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Character_Enemy_Boss_Inperiusu() {};

    void BecomeHard();

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

    void SetRetryCount(int num)
    {
        m_RetryCount = num;
    };
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
    void ResetAnim()
    {
        for (int i = 0; i < MODEL_NUM; i++)
        {
            m_ModelTransPos[i] = {};
            m_ModelTransRot[i] = {};
        }
    }
    void StartBodyTexMask(int maxF)
    {
        m_BodyTexMaskFrame = 0;
        m_BodyTexMaskFrameMax = maxF;
    }



	void StartTurn(float afterAxis, float speed);
	void StartTurn(Float2 afterVec, float speed);

    void StartMove(Float3 afterPos, float afterAxis, double maxF,
        bool distanceOneF = false);
	void StartWarp(Float3 afterPos, float afterAxis, int maxF,
        bool safeAreaIgnore=false);
	void StartJet(Float3 afterPos, float afterAxis, int maxF);

    void MirrorgeStateReset()
    {
        for (int i = 0; i < MIRRORGE_STATE_MAX; i++)
        {
            m_MirrorgeState[i].enable = false;
            m_MirrorgeState[i].inFrame = 0;
            m_MirrorgeState[i].pos = {};
            m_MirrorgeState[i].rot = {};
            m_MirrorgeState[i].sca = {};
            m_MirrorgeState[i].axis = 0.0f;
        }
    }
    void MirrorgeStateStartSet(int index, float axis, Float3 pos, Float3 rot, Float3 sca)
    {
        if (index < 0 || index >= MIRRORGE_STATE_MAX) return;
        m_MirrorgeState[index].enable = true;
        //m_MirrorgeState[index].inFrame = 0;
        m_MirrorgeState[index].pos = pos;
        m_MirrorgeState[index].rot = rot;
        m_MirrorgeState[index].sca = sca;
        m_MirrorgeState[index].axis = axis;
    }
    void MirrorgeStateEndSet(int index)
    {
        if (index < 0 || index >= MIRRORGE_STATE_MAX) return;
        m_MirrorgeState[index].enable = false;
        //m_MirrorgeState[index].inFrame = MIRRORGE_STATE_INFRAME_MAX;
    }
};


#endif