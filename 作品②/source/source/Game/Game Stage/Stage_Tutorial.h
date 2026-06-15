// ===================================================
// Stage_Tutorial.h ステージ処理
// 
// hara sougo  2025/02/22
// ===================================================
#ifndef _STAGE_TUTORIAL_H_
#define _STAGE_TUTORIAL_H_


#include "../Systems/sys_general.h"
#include "../Game Object/gameObject.h"
#include "Stage_Base.h"

class Stage_Tutorial : public Stage_Base
{
    typedef Stage_Base
        SuperClass; //親クラスにあだ名をつける

private:

protected:

    TEX_DAT m_TexId_Yuka = {};
    unsigned int m_Model_Yuka = 0;
    unsigned int m_Model_Ring = 0;

    int m_ShaderIndex_Ring_Vertex = 0;
    int m_ShaderIndex_Ring_Pixel = 0;
    int m_ShaderIndex_Floor_Vertex = 0;
    int m_ShaderIndex_Floor_Pixel = 0;
    int m_ShaderIndex_Hikari_Vertex = 0;
    int m_ShaderIndex_Hikari_Pixel = 0;
public:

    Stage_Tutorial(Difficulty dif) : SuperClass(dif){};
    ~Stage_Tutorial() {};

    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Update() override;
    virtual void Draw() override;

    //virtual void a();
};



#endif