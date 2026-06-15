// ===================================================
// GOE_Character_Enemy_Regatosu.h キャラクターオブジェクト
// 
// hara sougo  2024/12/10
// ===================================================
#ifndef _GOE_CHILD_CHARACTER_ENEMY_REGATOSU_H_
#define _GOE_CHILD_CHARACTER_ENEMY_REGATOSU_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Enemy.h"

class GOE_Character_Enemy_Regatosu : public GOE_Character_Enemy
{
	typedef GOE_Character_Enemy
		SuperGOE; //親クラスにあだ名をつける

private:
    static constexpr auto EXPLODE_ATK_COL_NAME = ("COL_ATK_ENEMY_REGATOSU_EXPLODE");

    static constexpr float MIRESU_BUFF_AREA = ONE_BLOCK * 20.0f * 1.25f;

    static constexpr float BUFF_SUPER_MOD_ATTACK = 2.0f;
    static constexpr float BUFF_SUPER_MOD_SPEED = 2.0f;
    static constexpr float BUFF_SUPER_MOD_GUARD = 0.5f;

    static constexpr int BUFF_SUPER_EXPLODE_FRAME = FRAME_ONE_SECOND * 12;
    //static constexpr int BUFF_SUPER_EXPLODE_FRAME = FRAME_ONE_SECOND * 6;
    static constexpr int BUFF_SUPER_EXPLODE_BEFORE_FRAME = BUFF_SUPER_EXPLODE_FRAME - 10;

protected:
    bool m_IsNoBuffMode = false;

    bool m_Anim_AttackLoop = false;

    unsigned int m_TexId_AreaEnemy  = 0;
    int m_AnimCountMax_AreaEnemy = 8;
    static constexpr Int2 m_AnimCount_AreaEnemy = { 3, 3 };

    //std::map<int, GAME_OBJECT*> m_BuffTargets = {};

    static constexpr Int2 m_AnimCount_Buff = { 6, 6 };
    bool m_IsBuffSuper = false;
    int m_BuffSuperFrame = 0;
    std::string m_ExploGayaId = "";

    unsigned int m_Model_Wave = 0;

    TEX_DAT m_Tex_Defeat = {};
    TEX_DAT m_Tex_EffWave1 = {};
    //TEX_DAT m_Tex_EffWave2 = {};

    TEX_DAT m_Tex_Main_Idle = {};
    TEX_DAT m_Tex_Main_Dam = {};
    TEX_DAT m_Tex_Main_Atk = {};
public:
    static constexpr const char* CHARACTER_NAME = "ENEMY_REGATOSU";

    GOE_Character_Enemy_Regatosu(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Character_Enemy_Regatosu() {};

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

    virtual void SetBuffSuper() { m_IsBuffSuper = true; };
    bool GetIsBuffSuper() const { return m_IsBuffSuper; };

    void SetNoBuff()
    {
        m_IsNoBuffMode = true;
    }
    void SetAnimAttackLoop()
    {
        m_Anim_AttackLoop = true;
    }
};


#endif