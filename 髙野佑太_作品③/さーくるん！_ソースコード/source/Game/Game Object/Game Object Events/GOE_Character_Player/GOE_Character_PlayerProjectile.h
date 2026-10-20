// ===================================================
// GOE_Character_PlayerProjectile.h オブジェクトイベントエフェクト
// 
// hara sougo  2025/01/12
// ===================================================
#ifndef _GOE_CHARACTER_PLAYERPROJECTILE_H_
#define _GOE_CHARACTER_PLAYERPROJECTILE_H_


#include "../../../Systems/sys_general.h"

#include "../../gameObject.h"

#include "../GOE_Base.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体定義
// ===================================================


class GOE_Character_PlayerProjectile : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける
public:
    enum class Type
    {
        None,
        ContriusBomb,
        ContriusShockWave,
    };
private:

protected:
    unsigned int m_Tex_EfBuff_Flow = 0;
    unsigned int m_Tex_EfBuff_Flare = 0;

    unsigned int m_Model_Beam_Misile = 0;

    unsigned int m_Tex_ShockWaveSprite = 0;
    unsigned int m_Tex_ShockWaveFlow = 0;
    unsigned int m_Model_ShockWaveFlow = 0;

    Float4 m_ColorGenral = { 1.0f, 1.0f, 1.0f, 1.0f };
    bool m_ColorSetFlag = false;

    Type m_type = Type::None;

    GAME_OBJECT_IDENTIFIER m_ObjOwner = {};

public:
    GOE_Character_PlayerProjectile(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Character_PlayerProjectile() {};

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

    void SetOwner(GAME_OBJECT_IDENTIFIER goi)
    {
        m_ObjOwner = goi;
    }
    void SetType(Type type)
    {
        m_type = type;
    }
    void SetColor(Float4 color)
    {
        m_ColorSetFlag = true;
        m_ColorGenral = color;
    }

    static GOE_Character_PlayerProjectile* CreateProjectile(
        Type type,
        Float3 startPos,
        Float3 startVel
    );
};

#endif