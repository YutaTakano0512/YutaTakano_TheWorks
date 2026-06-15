// ===================================================
// ui_Confirm.cpp ui処理
// 
// hara sougo  2025/03/09
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "../Entities/character.h"
#include "../Entities/player.h"
#include "../Managers/soundManager.h"
#include "../Managers/gameManager.h"
#include "ui_Confirm.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

// ===================================================
// グローバル変数
// ===================================================

void UI_Confirm::Update()
{
    CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainLeftStickTrigger();

    bool stickTrigger_positive = false;
    bool stickTrigger_negative = false;

    //Lスティックの入力を受け取る
    if (stickTrigger_Dat.enable)
    {
        if (stickTrigger_Dat.stick.x > 0.4f)
        {
            stickTrigger_positive = true;
        }
        if (stickTrigger_Dat.stick.x < -0.4f)
        {
            stickTrigger_negative = true;
        }
    }

    m_Confirm_StateTrigger = StateTrigger::None;

    if (IsCancelTrigger())
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0);
        m_Confirm_cursorConfirmMenu = State::No;

        m_Confirm_StateTrigger = StateTrigger::No;
    }
    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_LEFT)
        || stickTrigger_negative)
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

        m_Confirm_cursorConfirmMenu = (State)((
            (int)m_Confirm_cursorConfirmMenu - 1 + (int)State::Max) % (int)State::Max
            );

        m_Confirm_old_button = m_Confirm_confirmMenuItems_ON_pos[static_cast<int>(m_Confirm_cursorConfirmMenu)].x;//現在の位置を保存する
        m_Confirm_Interpolation = 0.0f;
        m_Confirm_is_move = true;
    }

    if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_RIGHT)
        || stickTrigger_positive)
    {
        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);


        m_Confirm_cursorConfirmMenu = (State)(((int)m_Confirm_cursorConfirmMenu + 1) % 2);

        m_Confirm_old_button = m_Confirm_buttonZPositionX;
        m_Confirm_Interpolation = 0.0f;
        m_Confirm_is_move = true;
    }

    if (!m_Confirm_is_move)
    {//ボタン動かす
        m_Confirm_scaleTime += 0.05f;
        m_Confirm_buttonZScale = 1.0f + 0.05f * sin(m_Confirm_scaleTime);
    }

    //イージング
    float t = EasingInOutSine(m_Confirm_Interpolation);

    //線形補間
    m_Confirm_buttonZPositionX = m_Confirm_old_button * (1.0f - t) + m_Confirm_confirmMenuItems_ON_pos[static_cast<int>(m_Confirm_cursorConfirmMenu)].x * t;

    if (m_Confirm_is_move == true)
    {//trueになったら数秒語にfalse
        m_Confirm_move++;

        if (m_Confirm_move > 20.0f)
        {
            m_Confirm_is_move = false;
            m_Confirm_move = 0.0f;
        }
    }

    m_Confirm_Interpolation += 0.3f;//速度の設定

    if (m_Confirm_Interpolation > 1.0f)
    {
        m_Confirm_Interpolation = 1.0f;
    }

    if (IsDecisionTrigger())
    {
        //SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0);
        switch (m_Confirm_cursorConfirmMenu)
        {
        case State::Yes:
            m_Confirm_StateTrigger = StateTrigger::Yes;
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0);
            break;
        case State::No:
            m_Confirm_StateTrigger = StateTrigger::No;
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_CANCEL, 4.0f, 0);
            break;
        default:
            break;
        }
    }
}

void UI_Confirm::Draw()
{

    Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    DrawSpriteColorQuad(0, 0,
        GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).width,
        GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).height,
        GetTexData(TEX_TYPE_TITLE_CONFIRM_CHECK_Z).textureId,
        &color,
        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);

    if (m_Tex_Ttrl_Confirm_Text.enable)
    {
        DrawSpriteColorQuad(0.0f, -100.0f,
            m_Tex_Ttrl_Confirm_Text.width,
            m_Tex_Ttrl_Confirm_Text.height,
            m_Tex_Ttrl_Confirm_Text.textureId,
            &color,
            DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);
    }

    //ボタン
    DrawSpriteColorQuad(m_Confirm_buttonZPositionX,
        m_Confirm_confirmMenuItems_ON_pos[static_cast<int>(m_Confirm_cursorConfirmMenu)].y,
        GetTexData(TEX_TYPE_TITLE_BUTTON_Z).width * m_Confirm_buttonZScale,
        GetTexData(TEX_TYPE_TITLE_BUTTON_Z).height * m_Confirm_buttonZScale,
        GetTexData(TEX_TYPE_TITLE_BUTTON_Z).textureId,
        &color,
        DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);

    for (int i = 0; i < 2; i++)
    {
        float textScale = 0.8f;

        if (i == static_cast<int>(m_Confirm_cursorConfirmMenu))
        {
            textScale = m_Confirm_buttonZScale;
        }
        DrawSpriteColorQuad(m_Confirm_confirmMenuItems_OFF_pos[i].x,
            m_Confirm_confirmMenuItems_OFF_pos[i].y,
            GetTexData(m_Confirm_confirmMenuItems_OFF_Tex[i]).width * 0.8f,
            GetTexData(m_Confirm_confirmMenuItems_OFF_Tex[i]).height * 0.8f,
            GetTexData(m_Confirm_confirmMenuItems_OFF_Tex[i]).textureId,
            &color,
            DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);

        if (i == static_cast<int>(m_Confirm_cursorConfirmMenu))
        {
            DrawSpriteColorQuad(m_Confirm_confirmMenuItems_ON_pos[i].x,
                m_Confirm_confirmMenuItems_ON_pos[i].y,
                GetTexData(m_Confirm_confirmMenuItems_ON_Tex[i]).width * 1.1f,
                GetTexData(m_Confirm_confirmMenuItems_ON_Tex[i]).height * 1.1f,
                GetTexData(m_Confirm_confirmMenuItems_ON_Tex[i]).textureId, &color,
                DRAW_POLYGON_PRIORITY_FORCE_ALWAYS);
        }
    }
}

bool UI_Confirm::IsDecisionTrigger() const
{
    return GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A);
}
bool UI_Confirm::IsCancelTrigger() const
{
    return GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B);
}
