// ===================================================
// GOE_Character_Player.h キャラクターオブジェクト
// 
// hara sougo  2024/11/25
// ===================================================
#ifndef _GOE_CHARACTER_PLAYER_H_
#define _GOE_CHARACTER_PLAYER_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"
#include "../../Entities/character.h"

#include "GOE_Character.h"

// ===================================================
// マクロ定義
// ===================================================

struct GAME_OBJECT_COLLISION_ATTACK_CHILD;

//
// クラス
//
class GOE_Character_Player : public GOE_Character
{
	typedef GOE_Character
		SuperGOE; //親クラスにあだ名をつける

public:
    static constexpr int KAKOPON_DAT_NUM = 3;
    //static constexpr int KAKOPON_INK_GAGE_MEMORY_NUM = 3;
    static constexpr int KAKOPON_INK_GAGE_MEMORY_NUM = 8;
    static constexpr int KAKOPON_INK_GAGE_MAX = 100000;
    static constexpr int KAKOPON_INK_GAGE_ONE = KAKOPON_INK_GAGE_MAX / KAKOPON_INK_GAGE_MEMORY_NUM;

    //static constexpr int KAKOPON_INK_GAGE_HEAL = KAKOPON_INK_GAGE_ONE / (FRAME_ONE_SECOND * 5);
    //static constexpr int KAKOPON_INK_GAGE_HEAL = KAKOPON_INK_GAGE_ONE / (FRAME_ONE_SECOND * 4);
    //static constexpr int KAKOPON_INK_GAGE_HEAL = KAKOPON_INK_GAGE_MAX / (FRAME_ONE_SECOND * 12);
    //static constexpr int KAKOPON_INK_GAGE_HEAL = KAKOPON_INK_GAGE_MAX / (FRAME_ONE_SECOND * 2);
    static constexpr int KAKOPON_INK_GAGE_HEAL = KAKOPON_INK_GAGE_MAX / (FRAME_ONE_SECOND * 1.25);
    //static constexpr int KAKOPON_INK_GAGE_HEAL_ATTACK = KAKOPON_INK_GAGE_MAX / 20;
    static constexpr int KAKOPON_INK_GAGE_HEAL_ATTACK = KAKOPON_INK_GAGE_MAX / 18;

    //static constexpr int KAKOPON_INK_GAGE_COST = KAKOPON_INK_GAGE_MAX / 120;
    //static constexpr int KAKOPON_INK_GAGE_COST = KAKOPON_INK_GAGE_MAX / 60;
    static constexpr int KAKOPON_INK_GAGE_COST = KAKOPON_INK_GAGE_MAX / 50;

    //かこポン線 頂点最大数
    static constexpr int KAKOPON_VERTEX_MAX = 256;

    enum class KAKOPON_GRAPHIC {
        Main,
        Main_Kai,
        Main_Neo,
        Main_Ca,
        Main_Kn,

        Heal,
        Buff,
        DeBuff,
    };

    enum class KAKOPON_TYPE {
        Main,
        Heal,
        BuffAtk
    };

    //かこポン頂点データ
    struct KAKOPON_VERTEX {
        //bool enable;
        Float2 pos;
        bool isEndLine;
        char enableFrame;
    };

    struct KAKOPON_DATA_ATKBUF {
        GAME_OBJECT_COLLISION_ATTACK_CHILD col;
        int level;
    };
    struct KAKOPON_DATA {
        //わっかが完成したか
        bool isConnected;

        //わっかの中央座標
        Float2 connectCentroid;
        float connectAreaSize;

        //つながってからのフレーム
        int frame;
        int frameMax;
        //つながってからの描画フレーム
        int frameDraw;

        //頂点
        KAKOPON_VERTEX vertex[KAKOPON_VERTEX_MAX];
        int vertexNum;

        //内部のオブジェクト
        std::vector<GAME_OBJECT_IDENTIFIER> objs;

        //タイプ
        KAKOPON_TYPE type;
        //見た目
        KAKOPON_GRAPHIC graphic;

        bool isFirstObjectCheck; //外から操作する
        //成立したか 外から操作する
        bool IsSuccess;

        //攻撃のデータを保存するバッファ 外から操作する
        KAKOPON_DATA_ATKBUF atkBuf;
    };

    //かこポン描いてるときに使うデータ
    struct KAKOPON_LINEDRAW {

        //入力フラグ
        int vertexDrawInputFlag;
        int vertexDrawInputFlagBefore;

        //頂点の配置中フラグ
        bool vertexDrawFlag;
        int vertexDrawCancelF;

        //以前配置した頂点の座標
        Float2 vertexBeforePos;

        //頂点を配置していいか
        bool vertexAddFlag;

        //わっかの中をチェックしたか
        //bool isVertexObjectCheck;

        //線を描いているかのフラグ
        bool isLineDrawing;

        //後隙フレーム
        int afterTimerFrame;
    };

    struct KAKOPON_SET {
        KAKOPON_DATA dat;
        KAKOPON_LINEDRAW lineDraw;
    };

protected:
	//囲んでポンの攻撃判定データのファイル名
	//static constexpr auto KAKOPON_GENERAL_ATK_COL_NAME = ("COL_ATK_KAKOPON_GENERAL");

    static constexpr int DAMAGE_INV_FRAME = FRAME_ONE_SECOND * 2;

    //かこポンの頂点を配置する間隔
    //static constexpr float KAKOPON_ONE_LINE_DISTANE = 56.0f;
    //static constexpr float KAKOPON_ONE_LINE_DISTANE = 192.0f;
    //static constexpr float KAKOPON_ONE_LINE_DISTANE = 384.0f;
    static constexpr float KAKOPON_ONE_LINE_DISTANE = 350.0f;

    //かこぽん中のスピード補正
    //static constexpr float KAKOPON_WALK_SPEED_MOD = 1.2f;

    static constexpr auto N_ATTACK_GENERAL_TAG = ("ATK_GENERAL");


    //かこポン攻撃
    void kakopon_attack_setBuf(KAKOPON_DATA* dat);
    struct KAKOPON_ATTACK_OBJECT_ST {
        GAME_OBJECT* obj;
        GAME_OBJECT_COLLISION_RECIEVE_CHILD colRev;
        float dist;
    };
    bool is_kakopon_objects_attack_target(GAME_OBJECT* ob);
    void kakopon_objects_attack(KAKOPON_DATA* dat);

    Float2 kakoponCalSetVertexPos();

    //かこポンの処理
    void init_kakoponDat(KAKOPON_SET* set);
    void update_kakoponDat(KAKOPON_SET* set);
    void draw_kakoponDat(KAKOPON_SET* set);

    void draw_kakoponDat_Aura(
        DRAW_POLYGON_DAT& dsdat,
        Float3 postmp,
        float rottmp,
        int index,
        KAKOPON_DATA* dat
    );

    void update_kakopon_action(bool keyPush, KAKOPON_DATA* dat, KAKOPON_LINEDRAW* linedraw);

    void kakopon_final_update();

    virtual void calc_kakopon_damage(
        GAME_OBJECT_COLLISION_ATTACK_CHILD* output
    ) {};

    void become_stern_motion();

    //-------------------------------------------

    unsigned int m_TexId_AttackEffect_s = -1;
    Int2 m_AnimCount_AttackEffect_s = { 3, 3 };

    //unsigned int m_TexId_kakoponEffect_Trail = -1;
    unsigned int m_TexId_kakoponEffect_Add = -1;
    unsigned int m_TexId_kakoponEffect_Aura = -1;

    unsigned int m_TexId_kakoponEffect_Trail_Kai[4] = {};
    unsigned int m_TexId_kakoponEffect_Trail_Neo[4] = {};
    unsigned int m_TexId_kakoponEffect_Trail_Kn[4] = {};
    unsigned int m_TexId_kakoponEffect_Add_Kn = -1;

    unsigned int m_TexId_kakoponEffect_Ground = -1;

    //unsigned int m_TexId_kakoponEffect_General[5] = {};
    unsigned int m_TexId_kakoponEffect_General[2] = {};
    unsigned int m_TexId_kakoponEffect_General2[2] = {};
    unsigned int m_TexId_kakoponEffect_Kn[2] = {};
    unsigned int m_TexId_kakoponEffect_Heal[2] = {};
    unsigned int m_TexId_kakoponEffect_Buff[2] = {};
    unsigned int m_TexId_kakoponEffect_DeBuff[2] = {};

    //unsigned int m_TexId_kakoponEffect_Add_Heal = -1;
    //unsigned int m_TexId_kakoponEffect_Add_Buff = -1;
    //unsigned int m_TexId_kakoponEffect_Add_DeBuff = -1;

    unsigned int m_Model_StanStar = 0;

    Float3 m_BuffEfPos = {};

	int m_AttackCooldown = 0;

    float m_MoveSpeedMod = 1.0f;
    float m_MoveSpeedModKako = 1.2f;

    float m_KakoAddDis = 0.0f; //かこポンの線を引くときに足される距離
	//囲んでポンの攻撃判定データのファイル名
    std::string m_KakoDamName = ("COL_ATK_KAKOPON_GENERAL");
    float m_KakoDamAddMod = 1.0f; //かこポンのダメージの伸びの良さ
    float m_KakoInkCostMod = 1.0f; //かこポンのインクゲージの減りやすさ

    Float4 m_ThemeColor = {};

    Float2 m_AxisVec = {};

    bool m_IsDrawing = false;
    bool m_IsIdling = false;
    bool m_IsWalkAnim = false; //アニメーション依存 立ち止まっているときもtrueになる
    bool m_IsNormalAttack = false;

    bool m_IsWalking = false;


    int m_HealingFrame = 0;
    bool m_IsHealIgnore = false;

    float m_BuffAttackMod = 1.0;
    int m_BuffAttackFrame = 0;
    float m_BuffSpeedMod = 1.0;
    int m_BuffSpeedFrame = 0;
    float m_BuffInkCostMod = 1.0;
    int m_BuffInkCostFrame = 0;

    int m_SkillCooldownFrame = 0;
    int m_SkillCooldownFrameMax = 0;

    int m_InkGage_DrawBefore = 0;

    int m_InkGage = 0;
    int m_InkGageMax = 0;

    //かこポンデータ
    KAKOPON_SET m_KakoDatList[KAKOPON_DAT_NUM] = {};
    //KAKOPON_DATA m_KakoDat_Main = {};
    //KAKOPON_LINEDRAW m_KakoponLineDraw_Main = {};

    //KAKOPON_DATA m_KakoDat_SubTest = {};
    //KAKOPON_LINEDRAW m_KakoponLineDraw_SubTest = {};

    Float3 m_KakoFirstPos = {};

    float m_TexIconMain_Mod = 1.0f;
    TEX_DAT m_TexIconMain_N = {};
    TEX_DAT m_TexIconMain_Pinch = {};
    TEX_DAT m_TexIconMain_Out = {};
    TEX_DAT m_TexIconSub = {};
    TEX_DAT m_TexIconSkillOn = {};
    TEX_DAT m_TexIconSkillOff = {};

    bool m_PinchFlag = false;
    bool m_EnemyManyDefeatFlag = false;

    int m_MoveRequWalfFrame = 0;

    int m_DamageFrame = -1;
    int m_DefeatFrame = 0;
    int m_DefeatFrameAnim = 0;
    int m_DefeatFrameAnimMax = 0;
    int m_DamageSternFrame = -1;
    int m_DamageSternFrameMax = 0;
    bool m_DamageSternEffectFlag = false;
    bool m_DamageSternAnimInLoopFlag = false;
    int m_DeadNum = 0;
    bool m_GameoverDecision = false;
    TEX_DAT m_Tex_DamageEffect = {};
    TEX_DAT m_Tex_ScKakoEffect = {};

    int m_EffectQuestionCooldown = 0;

    int m_OnajiCharaNum = 0;

    bool m_Flag_Ttrl_Attack = false;
    bool m_Flag_Ttrl_Kako = false;
    bool m_Flag_Ttrl_Move = false;
    bool m_Flag_Ttrl_Skill = false;

    //カメラに動いてますよアピールするフラグ
    int m_CameraMoveActionFlag = 0;

    static constexpr int movieVisibleFrameMax = 40;
    int m_movieVisibleFrame = movieVisibleFrameMax;

    Float3 m_MoveInputVectorCurrent = {};
    Float3 m_MoveInputVectorBefore = {};
public:
	GOE_Character_Player(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Character_Player() {};

	virtual void Init() override;
	virtual void FirstFrame() override;
	virtual void Uninit() override;
	virtual void Update() override;
    virtual void UpdateActive() override;
    virtual void UpdateActiveLate();

	virtual void Draw() override;

	virtual void Dead() override;

	virtual void DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr) override;
	virtual void DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr) override;

	virtual void BlockHit(GAME_OBJECT_POSMOD_RESULT pmr) override;

    virtual void CalcAttackerDamage(
        GAME_OBJECT_DAMAGE_RESULT* output,

        const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
        int damage,
        GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
        GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
    ) override;

    virtual void HpBarReset();

    virtual Float2 GetKakoponVertexNum();

    virtual Int2 GetKakoponInkGage();

    void WalkRequest(
        int frame,
        Float3 pos,
        std::function<float(float)> easingX = nullptr,
        std::function<float(float)> easingY = nullptr,
        std::function<float(float)> easingZ = nullptr,
        bool isBlockCollisionIgnore = false
    ); 
    bool IsEventWalking()
    {
        return m_MoveRequWalfFrame > 0;
    }

    void SetOnajiCharaNum(int b) {
        m_OnajiCharaNum = b;
    };
    virtual void SetCharaSubGraphic() = 0;

    void BecomeRevive();

    bool IsDefeat() const; 
    void SetGameoverDecision(bool b) {
        m_GameoverDecision = b;
    };

    void HealingFlag();
    bool IsHealIgnore() const {
        return m_IsHealIgnore;
    };
    void BuffAttackFlag(float mod);
    float GetBuffAttackMod() const;
    void BuffSpeedFlag(float mod);
    float GetBuffSpeedMod() const;
    void BuffInkCostFlag(float mod);
    float GetBuffInkCostMod() const;

    void SetCameraMoveActionFlag(int n=60);
    int GetCameraMoveActionFlag() const;

    bool IsInput_NormalAttack() const;
    bool IsInput_NormalAttackPress() const;
    bool IsInput_KakoMain() const;
    bool IsInput_KakoSub() const;

    int GetDamageFrame() const {
        return m_DamageFrame;
    };
    bool IsPinch() const;

    virtual void SetDownPoseStart() = 0;
    virtual void SetDownPose() = 0;
    virtual void SetStern(int frame, bool enableEffect = false);

    virtual void SetKnockBack(float power, int sternf, GAME_OBJECT* atkrObj);

    KAKOPON_SET GetKakoMain() const {
        return m_KakoDatList[0];
    };
    Float3 GetKakoFirstPos() const {
        return m_KakoFirstPos;
    };
    bool IsDrawing() const {
        return m_IsDrawing;
    };

    void EffectQuestion();

    virtual void InitResetAction();
    void ResetAction();

    void SetSkillCooldownFrame(int n) {
        m_SkillCooldownFrame = n;
        m_SkillCooldownFrameMax = n;
    };
    void ResetSkillCooldown() {
        m_SkillCooldownFrame = 0;
    };

    int GetSkillCooldownFrame() const {
        return m_SkillCooldownFrame;
    };
    int GetSkillCooldownFrameMax() const {
        return m_SkillCooldownFrameMax;
    };

    float GetTex_IconMain_Mod() const {
        return m_TexIconMain_Mod;
    };
    TEX_DAT GetTex_IconMain_N() const {
        return m_TexIconMain_N;
    };
    TEX_DAT GetTex_IconMain_Pinch() const {
        return m_TexIconMain_Pinch;
    };
    TEX_DAT GetTex_IconMain_Out() const {
        return m_TexIconMain_Out;
    };

    TEX_DAT GetTex_IconSub() const {
        return m_TexIconSub;
    };
    TEX_DAT GetTex_IconSkillOn() const {
        return m_TexIconSkillOn;
    };
    TEX_DAT GetTex_IconSkillOff() const {
        return m_TexIconSkillOff;
    };

    Float4 GetThemeColor() const {
        return m_ThemeColor;
    };

    void SetFlagTtrlAttack() {
        m_Flag_Ttrl_Attack = true;
    };
    void SetFlagTtrlKako() {
        m_Flag_Ttrl_Kako = true;
    };
    void SetFlagTtrlMove() {
        m_Flag_Ttrl_Move = true;
    };
    void SetFlagTtrlSkill() {
        m_Flag_Ttrl_Skill = true;
    };

    bool GetFlagTtrlAttack() const {
        return m_Flag_Ttrl_Attack;
    };
    bool GetFlagTtrlKako() const {
        return m_Flag_Ttrl_Kako;
    };
    bool GetFlagTtrlMove() const {
        return m_Flag_Ttrl_Move;
    };
    bool GetFlagTtrlSkill() const {
        return m_Flag_Ttrl_Skill;
    };
	//仮
	//static Float2 GetKakoponVertexNum();
};

#endif