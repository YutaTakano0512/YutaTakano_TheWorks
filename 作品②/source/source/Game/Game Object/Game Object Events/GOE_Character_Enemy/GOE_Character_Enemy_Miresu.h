// ===================================================
// GOE_Character_Enemy_Miresu.h キャラクターオブジェクト
// 
// hara sougo  2024/12/10
// ===================================================
#ifndef _GOE_CHILD_CHARACTER_ENEMY_MIRESU_H_
#define _GOE_CHILD_CHARACTER_ENEMY_MIRESU_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Enemy.h"

class GOE_Character_Enemy_Miresu : public GOE_Character_Enemy
{
	typedef GOE_Character_Enemy
		SuperGOE; //親クラスにあだ名をつける

public:
    enum class Type
    {
        Default,
        Hard,
        Speed,
        Strong,
        Rapid,
        Runaway,
        Fission,
    };

private:
    static constexpr const char* BEAM_ATK_COL_NAME = "COL_ATK_ENEMY_MIRESU_BEAM";

    static constexpr int BUFF_FRAME_MAX = FRAME_ONE_SECOND * 10;

    static constexpr float BUFF_MOD_ATTACK = 1.8f;
    static constexpr float BUFF_MOD_SPEED  = 1.5f;
    static constexpr float BUFF_MOD_GUARD  = 0.7f;

    static constexpr float BUFF_SUPER_MOD_ATTACK = 2.0f;
    static constexpr float BUFF_SUPER_MOD_SPEED  = 2.0f;
    static constexpr float BUFF_SUPER_MOD_GUARD  = 0.5f;

    static constexpr int ANIMCOUNT_BEAM_CHARGE_X = 16;
    static constexpr int ANIMCOUNT_BEAM_CHARGE_Y = 16;
    static constexpr int ANIMCOUNTMAX_BEAM_CHARGE = ANIMCOUNT_BEAM_CHARGE_X * ANIMCOUNT_BEAM_CHARGE_Y;

    static constexpr int FRAME_BEAM_SHOOT_START = ANIMCOUNTMAX_BEAM_CHARGE * 0.35f;
    static constexpr int FRAME_BEAM_SHOOT_END = ANIMCOUNTMAX_BEAM_CHARGE * 0.45f;
    static constexpr int FRAME_BEAM_LIFETIME = 75;

    void update_action_attacker();
    void update_action_shooter();
    void update_action_hybrid();
protected:
    int m_CurrentAxisPrivate = 0;

    Float2 m_DefaultSpritePos = {};

    Type m_Type = Type::Default;
    std::string m_RareGayaId = "";

    GAME_OBJECT* m_BuffOwner = nullptr;
    int m_BuffFrame = 0;

    bool m_IsBuffSuper = false;

    bool m_IsRare = false;

    //unsigned int m_TestModel = 0;
    unsigned int m_Tex_BeamCharge = 0;

    unsigned int m_Tex_Beam = 0;
    unsigned int m_Model_Beam = 0;

    Float3 m_BeamShootVec = {};
    int m_BeamChargeFrame = 0;
    Float3 m_BeamTargetPos = {};

    //int m_Beam_Object_Count = 0;
    //std::map<int, GAME_OBJECT_IDENTIFIER> m_Beam_Objects = {};

    float m_SpeedMod = 1.0f;

    TEX_DAT m_Tex_Defeat = {};

    TEX_DAT m_Tex_Main_Idle = {};

    bool m_TypeRareStartEventFlag = false;
public:
    static constexpr const char* CHARACTER_NAME_ATTACKER = "ENEMY_MIRESU_ATTACKER";
    static constexpr const char* CHARACTER_NAME_SHOOTER = "ENEMY_MIRESU_SHOOTER";
    

	GOE_Character_Enemy_Miresu(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Character_Enemy_Miresu() {};

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

    virtual void SetBuff(GAME_OBJECT* buffOwner) { 
        m_BuffOwner = buffOwner; 
        m_BuffFrame = BUFF_FRAME_MAX; 
    };
    bool GetIsBuff() const { return m_BuffFrame > 1; };

    virtual void SetBuffBreak()
    {
        m_IsBuffSuper = false;
        m_BuffFrame = -1; 
    };
    bool GetIsBuffBreak() const { return m_BuffFrame == -1; };

    virtual void SetBuffSuper() 
    { 
        m_IsBuffSuper = true;
        m_BuffFrame = 0;
    };
    bool GetIsBuffSuper() const { return m_IsBuffSuper; };

    void SetType(Type type, bool isGeneral=false);
};


#endif