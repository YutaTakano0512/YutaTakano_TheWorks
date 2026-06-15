// ===================================================
// ui_Confirm.h ui処理
// 
// hara sougo  2025/03/09
// ===================================================
#ifndef _UI_CONFIRM_H_
#define _UI_CONFIRM_H_


#include "../Systems/sys_general.h"


class UI_Confirm
{
public:
    enum class StateTrigger {
       None= 0,
       Yes,
       No,
    };
    enum class State {
        Yes = 0,
        No,
        Max
    };
private:
protected:

    StateTrigger m_Confirm_StateTrigger = StateTrigger::None;

    State m_Confirm_cursorConfirmMenu = State::No;

    //線形補間(枠の移動)
    float m_Confirm_old_button = 0.0f;//古いポジション
    float m_Confirm_Interpolation = 1.0f;//補間の割合を保存する変数
    float m_Confirm_buttonZPositionY = 0.0f;//移動
    float m_Confirm_buttonZPositionX = 0.0f;//移動

    //ボタンのサイズ
    float m_Confirm_buttonZScale = 1.0f;
    bool m_Confirm_is_move = false;//移動フラグ
    float m_Confirm_move = 0.0f;//ディレイ
    float m_Confirm_scaleTime = 0.0f;//時間の計測
    Float2 m_Confirm_confirmMenuItems_ON_pos[2] =
    {
        {-225.0f, 75.0f},
        {225.0f, 75.0f},
    };
    TEX_TYPE m_Confirm_confirmMenuItems_ON_Tex[2] =
    {
        TEX_TYPE_TITLE_CONFIRM_YESTEXT_ON,
        TEX_TYPE_TITLE_CONFIRM_NOTEXT_ON,
    };
    Float2 m_Confirm_confirmMenuItems_OFF_pos[2] =
    {
        {-225.0f, 75.0f},
        {225.0f, 75.0f},
    };
    TEX_TYPE m_Confirm_confirmMenuItems_OFF_Tex[2] =
    {
        TEX_TYPE_TITLE_CONFIRM_YESTEXT_OFF,
        TEX_TYPE_TITLE_CONFIRM_NOTEXT_OFF,
    };

    TEX_DAT m_Tex_Ttrl_Confirm_Text = {};

public:
    UI_Confirm() {};
    UI_Confirm(TEX_DAT TextTex) : m_Tex_Ttrl_Confirm_Text(TextTex) {};
    ~UI_Confirm() {};

    virtual void Update();
    virtual void Draw();

    void SetTextTex(TEX_DAT TextTex)
    {
        m_Tex_Ttrl_Confirm_Text = TextTex;
    }

    bool IsDecisionTrigger() const;
    bool IsCancelTrigger() const;
    State GetState() const
    {
        return m_Confirm_cursorConfirmMenu;
    }
    StateTrigger GetStateTrigger() const
    {
        return m_Confirm_StateTrigger;
    }
};



#endif