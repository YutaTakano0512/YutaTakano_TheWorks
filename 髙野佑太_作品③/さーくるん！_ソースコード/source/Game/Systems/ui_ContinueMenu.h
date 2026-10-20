// ===================================================
// ui_ContinueMenu.h ui処理
// 
// hara sougo  2025/05/03
// ===================================================
#ifndef _UI_CONTINUEMENU_H_
#define _UI_CONTINUEMENU_H_


#include "../Systems/sys_general.h"

class UI_Confirm;

class UI_ContinueMenu
{
public:
    enum class StateMenu {
        Default = 0,
        Withdrawal,

        Max,
    };

    enum class State {
        Default = 0,

        Continue,
        Withdrawal,

        Exit,
    };
protected:
    bool exitflag = false;

    State state = State::Default;

    UI_Confirm conf_continue;
    UI_Confirm conf_withdrawal;


    StateMenu m_cursorConfirmMenu = StateMenu::Default;

    //線形補間(枠の移動)
    float m_old_button = 0.0f;//古いポジション
    float m_Interpolation = 1.0f;//補間の割合を保存する変数
    float m_buttonZPositionY = 0.0f;//移動
    float m_buttonZPositionX = 0.0f;//移動

    //ボタンのサイズ
    float m_buttonZScale = 1.0f;
    bool m_is_move = false;//移動フラグ
    float m_move = 0.0f;//ディレイ
    float m_scaleTime = 0.0f;//時間の計測

    Float2 m_MenuItems_ON_pos[2] =
    {
        {0.0f, -75.0f},
        {0.0f, 75.0f},
    };
    //TEX_TYPE m_MenuItems_ON_Tex[2] =
    //{
    //    TEX_TYPE_TITLE_NORMAL_ON,
    //    TEX_TYPE_TITLE_HARD_ON,
    //};
    Float2 m_MenuItems_OFF_pos[2] =
    {
        {0.0f, -75.0f},
        {0.0f, 75.0f},
    };
    //TEX_TYPE m_MenuItems_OFF_Tex[2] =
    //{
    //    TEX_TYPE_TITLE_NORMAL_OFF,
    //    TEX_TYPE_TITLE_HARD_OFF,
    //};

    //ポーズの画像
    TEX_DAT m_tex_pause_text = {};
    TEX_DAT m_tex_pause_retire_button = {};

    TEX_DAT m_tex_text_check_cho_continue = {};
    TEX_DAT m_tex_text_check_cho_exit = {};

    //TEX_DAT m_tex_btn_cont_on = {};
    //TEX_DAT m_tex_btn_cont_off = {};
    //TEX_DAT m_tex_btn_exit_on = {};
    //TEX_DAT m_tex_btn_exit_off = {};
    TEX_DAT m_tex_menubtn_items[2][2] = {};

public:

    void init();
    void uninit();

    bool update();

    void draw();

    State get_state()
    {
        return state;
    }
};



#endif