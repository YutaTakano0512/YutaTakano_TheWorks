// ===================================================
// GOE_Character_EnemyProjectile.h オブジェクトイベントエフェクト
// 
// hara sougo  2025/01/12
// ===================================================
#ifndef _GOE_CHARACTER_ENEMYPROJECTILE_H_
#define _GOE_CHARACTER_ENEMYPROJECTILE_H_


#include "../../../Systems/sys_general.h"

#include "../../gameObject.h"

#include "../GOE_Base.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体定義
// ===================================================


class GOE_Character_EnemyProjectile : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける
public:
    enum class Type
    {
        None,
        Beam,
        Granade,
        EnergyBallBit,
        BladeWave,
        EnergyBallBitKakusan,
        ExplosionShockWave,
        EnergyBallBitLaser,
        EnergyBallBomb,
    };
private:
    static constexpr auto ATK_COL_NAME_0 = ("COL_ATK_ENEMY_MIRESU_BEAM");

    static constexpr auto ATK_COL_NAME_BEAM_LARGE = ("COL_ATK_ENEMY_BOSS_INPERIUSU_BEAM");


    static constexpr int GRANADE_AREA_ENTER_FRAME = 12;
    static constexpr float GRANADE_EXPLO_AREA = ONE_BLOCK * 10.0f * 1.25f * 0.75f;

    static constexpr int BEAM_SHOOT_AFTER = 30;

    static constexpr int NUM_FLAG_MAX = 10;

protected:
    unsigned int m_Tex_Beam1 = 0;
    unsigned int m_Tex_Beam2 = 0;
    unsigned int m_Tex_Beam3 = 0;
    unsigned int m_Tex_Beambg = 0;

	unsigned int m_TexId_AreaEnemy = 0;
	unsigned int m_Tex_Beam = 0;
	unsigned int m_Model_Beam = 0;
    unsigned int m_Model_Beam_Misile = 0;
    unsigned int m_Tex_EnergyBall = 0;

    unsigned int m_Tex_Misile_Fire_Model = 0;
    unsigned int m_Model_Fire_Misile = 0;
    unsigned int m_Tex_Misile_Model = 0;
    unsigned int m_Model_Misile = 0;

    unsigned int m_Tex_AttackArea = 0;
    unsigned int m_Tex_BladeWave = 0;
    unsigned int m_Tex_BladeWaveNew = 0;
    unsigned int m_Tex_Wave1 = 0;
    //unsigned int m_Tex_Wave2 = 0;
    unsigned int m_Model_Wave = 0;

    unsigned int m_Tex_EfBuff_Flow = 0;
    unsigned int m_Tex_EfBuff_Flare = 0;

    unsigned int m_Tex_cursor_000 = 0;
    unsigned int m_Tex_cursor_010 = 0;
    unsigned int m_Tex_cursor_020 = 0;

    unsigned int m_Tex_Shadow = 0;

    unsigned int m_Tex_ShockWaveSprite = 0;
    unsigned int m_Tex_ShockWaveFlow = 0;
    unsigned int m_Tex_ShockWaveSprite_Gray = 0;
    unsigned int m_Tex_ShockWaveFlow_Gray = 0;
    unsigned int m_Model_ShockWaveFlow = 0;

    unsigned int m_Tex_EbBomb_ShockWave = 0;
    unsigned int m_Model_EbBomb_ShockWave = 0;


    Type m_type = Type::None;

	bool m_Init = false;
	bool m_Init2 = false;
    int m_Init2Frame = 0;

	Float3 m_HitPos = {};

	Float3 m_InitVel = {};

    int m_NumFlag[NUM_FLAG_MAX] = {};

    Float4 m_ColorGenral = { 1.0f, 1.0f, 1.0f, 1.0f };
    bool m_ColorSetFlag = false;

	GAME_OBJECT_IDENTIFIER m_ObjOwner = {};

    GAME_OBJECT_IDENTIFIER m_ObjTarget = {};

public:
    GOE_Character_EnemyProjectile(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Character_EnemyProjectile() {};

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

    void SetType(Type type)
    {
        m_type = type;
    }
    void SetOwner(GAME_OBJECT_IDENTIFIER goi)
    {
        m_ObjOwner = goi;
    }
    void SetTarget(GAME_OBJECT_IDENTIFIER goi)
    {
        m_ObjTarget = goi;
    }
    void SetHitPos(Float3 p)
    {
        m_HitPos = p;
    }
    void SetNumFlag(int n, int index=0)
    {
        if (index < 0 || index >= NUM_FLAG_MAX)
        {
            return; // 範囲外のインデックスは無視
        }
        m_NumFlag[index] = n;
    }

    void SetColor(Float4 color)
    {
        m_ColorSetFlag = true;
        m_ColorGenral = color;
    }

    static GOE_Character_EnemyProjectile* CreateProjectile(Type type,
        Float3 startPos,
        Float3 startVel
    );
};

#endif