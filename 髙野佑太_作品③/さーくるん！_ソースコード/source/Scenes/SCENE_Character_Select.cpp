/******************************
* @author KAPITONOV IGOR
*	yahiro riki
* @date 2024-11-09
* 
* 
* 
*出撃ハンコの描画はButtonSwitch();で行っています
******************************/

#include "SCENE_Title.h"
#include "SCENE_Character_Select.h"
#include "../Game/Entities/character.h"
#include "../Game/Entities/player.h"
#include "../game_main.h"
#include  "../Core/DirectX/dx.h"
#include "../Game/Texture/texData.h"
#include "../Game/Sound/soundData.h"
#include "../main.h"

#include "../Game/Managers/bg.h"
#include "../Core/Input/controller_main.h"

#include "../Game/Systems/fade.h"
#include "../Game/Managers/scene.h"
#include "../Game/Sprite/sprite.h"

#include "../Game/Sprite/spriteGraph.h"
#include "../Game/Game Object/gameObject.h"
#include "../Game/Game Object/gameObjectGraph.h"
#include "../Game/Managers/gameManager.h"
#include "../Game/Managers/soundManager.h"
#include "../Game/Systems/camera.h"
#include "../Game/Sprite/spriteAnim.h"

#include "../Core/Input/keyboard.h"

#include "../Game/Systems/ui.h"
#include "../Game/Systems/ui_Game.h"
#include "../Game/Game Events/GE_Gaya.h"

Scene::Select::Characters Scene::Select::characters;
static ID3D11Buffer* g_VertexBuffer = nullptr;
std::vector<float> characterEasing(Scene::Select::characters.size, 0.0f);
std::vector<float> bukiEasing(Scene::Select::characters.size, 0.0f);
std::vector<float> startEasing(1, 0.0f);
int Scene::Select::selected_id;//保存

static AnimationPage g_PageState;
static e_MapAnimation g_MapState;
static AnimationFlags flags;
static TextureTypes textures;

//プッシュした回数
static int g_PushCount;
static int g_PushBackCount;

static float g_in;
static int character_id;
static float g_scaleTime;
static int stopTime;

//アニメーション
float easingValue = EasingInOutQuint(characterEasing[character_id]);
static XMFLOAT2 Offset = { 0.0f,(1.0f - easingValue) * SCREEN_WIDTH };//オフセット
float startXOffset = 0.0f;
float shakeAmount;

//関数//
void CharacterAnimation();
void CharacterType();
void CharacterStatusType(int character_id);
void ButtonSwitch();
void ColorAnimation();
void SelectCursor();
void MapAnimation();
void CharaEasingAnimation();

//線形補間(枠の移動)
static float g_NowPositionY = NOW_CURSOR_Y;
static float g_OldCursor = 0.0f;//古いポジション
static float g_Interpolation = 1.0f;//補間の割合を保存する変数
static float CursorPosition = 0.0f;

static TEX_DAT g_tex_map = {};
static TEX_DAT g_tex_light = {};
static TEX_DAT g_tex_cursor = {};
static TEX_DAT g_tex_map_cursor = {};
static TEX_DAT g_tex_map_pin = {};

//フクダ追加3/02
void UpdateProgress(float& progress, float moveTime);
Float2 F2Lerpshare(float startX, float startY, float endX, float endY, float progress, std::function<float(float)> easing);
float FLerpshare(float start, float end, float progress, std::function<float(float)> easing);
static float StampProgress;
static float StampProgress2;

void InitializeSceneCharacterSelect()
{
    g_in = 0;
    character_id = 0;
    Scene::Select::selected_id = -1;
    startXOffset = (1.0f - easingValue) * SCREEN_WIDTH;//startのアニメーション
    g_MapState = MAP_START;
    g_PageState = CHARACTER_NORMAL;
    flags = { false, false, false, false, false,false,false,false,false,false };
    Offset.y = (1.0f - easingValue) * SCREEN_WIDTH;
    Offset.x = 0.0f;
    shakeAmount = 0.0f;

    g_PushCount = 0;
    g_PushBackCount = 0;
    stopTime = 0.0f;

    //TEX_DATA系
    g_tex_map = GetTexData(TEX_TYPE_UI_MAP);
    //g_tex_light = GetTexData(TEX_TYPE_CHARACTER_LIGHT);
    g_tex_cursor = GetTexData(TEX_TYPE_CHARACTER_KAKKO);
    g_tex_map_cursor = GetTexData(TEX_TYPE_UI_CURSOR);
    g_tex_map_pin = GetTexData(TEX_TYPE_UI_PIN);

    //テクスチャ関連の設定
    textures.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    textures.textureColor = MkF4(1.0f, 1.0f, 1.0f, 0.0f);
    textures.textureAlpha = 0.0f;
    textures.statusType = TEX_TYPE_CHARACTER_KAITO_PAGE;
    textures.texType = TEX_TYPE_CHARACTER_KAITO_ON;
    textures.bukiType = TEX_TYPE_CHARACTER_KAITO_BUKI;
    textures.coffeeType = TEX_TYPE_UI_BUTTON_B_OFF;
    textures.hankoType = TEX_TYPE_UI_BUTTON_A_OFF;
    textures.g_Color = 1.0f;
    textures.g_Alpha = 1.0f;
    textures.g_switchAlpha = 1.0f;

    SetBg(BG_TYPE_NONE);

    UIReset();
    GameStopResetRequest();
    Keyboard_Initialize();

    //
    GameManagerSetSceneRequestCancel();
    {
        SOUND_ID bgm = GetSoundData("SOUND/BGM/Test_BGM_Title2");
        if (bgm != GetSoundBGMId())
        {
            //異なるBGMが流れている、
            //またはBGMが流れていない場合再生する。
            SoundBGMPlayRequest(bgm,
                (FRAME_ONE_MINUTE * 1)
                + (FRAME_ONE_SECOND * 30),
                1.0f);
        }
    }

    Scene::Select::characters.character.resize(Scene::Select::characters.size);
    characterEasing.assign(Scene::Select::characters.size, 0.0f);
    bukiEasing.assign(Scene::Select::characters.size, 0.0f);
    startEasing.assign(Scene::Select::characters.size, 0.0f);

    //2025/02/27 Hara
    //前のシーンがステージ選択じゃなかったら
    //カメラを初期化する
    if(GetSceneBefore() != SCENE_STAGESELECT)
    {
        ResetCamera();

        SetCameraPosAll(MkF3(0.0f, 0.0f, 0.0f));
        SetCameraRotAll(0.0f);
        SetCameraZoomAll(1.75f);
        //イージングを終了状態にしとく
        startEasing[0] = 1.0f;
    }
    else
    {
    }

    //2025/3/02/fukuda
    StampProgress = 0;
    StampProgress2 = 0;
}

void FinalizeSceneCharacterSelect()
{
}

void DrawSceneCharacterSelect()
{
    //カラーアニメーション
    ColorAnimation();
    
    {//デスク
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            0.0f,
            0.0f,
            0.0f
        );
        drawDatTmp.size = MkF3(3840.0f * 0.6f, 2160.0f * 0.6f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        drawDatTmp.color = textures.color;
        drawDatTmp.texNo = GetTexData(TEX_TYPE_UI_BACK).textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }

    //マップのアニメーションを行う
    MapAnimation();

    //TEX_DAT
    {//ライト
        g_tex_light.width = 3840.0f;
        g_tex_light.height = 2160.0f;

        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            0.0f,
            0.0f,
            0.0f
        );
        //drawDatTmp.size = MkF3(659.0f, 775.0f, 1.0f);
        drawDatTmp.size = MkF3(g_tex_light.width * 0.6f, g_tex_light.height * 0.6f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        drawDatTmp.color = textures.color;
        drawDatTmp.texNo = GetTexData(TEX_TYPE_CHARACTER_LIGHT).textureId;

        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }


    // キャラクターテクスチャを描画
    CharacterType();

    //キャラ選択時の矢印and枠
    SelectCursor();

    ButtonSwitch();//キャラクター選択後
}

void UpdateSceneCharacterSelect()
{
    //安全用
    if (IsFading()) return;

    if (g_MapState == MAP_OUT)
    {
        if (!flags.is_Stop)
        {
            if (!flags.is_Animation)
            {
                if (!flags.is_Operation)
                {
                    CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainLeftStickTrigger();
                    bool stickTrigger_UP = false;
                    bool stickTrigger_DOWN = false;

                    // スティックの入力を受け取る
                    if (stickTrigger_Dat.enable)
                    {
                        if (stickTrigger_Dat.stick.y > 0.4f)
                        {
                            stickTrigger_UP = true;
                        }
                        if (stickTrigger_Dat.stick.y < -0.4f)
                        {
                            stickTrigger_DOWN = true;
                        }
                    }

                    int previous_character_id = character_id; // 変更前のキャラクターIDを保存

                    if (!IsFading() && GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_UP) || stickTrigger_UP)
                    {
                        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2, 4.0f, 0.0f);

                        g_OldCursor = g_NowPositionY;//古い座標を保存
                        g_Interpolation = 0.0f;
                        Offset.x = 0.0f;

                        g_PageState = CHARACTER_UP;

                        if (character_id > 0) { character_id--; }
                        if (character_id != previous_character_id)
                        {
                            flags.is_Book_Animation = true;
                            characterEasing[character_id] = 0.0f;
                        }
                    }

                    if (!IsFading() && GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_DOWN) || stickTrigger_DOWN)
                    {
                        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2, 4.0f, 0.0f);

                        g_OldCursor = g_NowPositionY;//古い座標を保存
                        g_Interpolation = 0.0f;
                        Offset.x = 0.0f;

                        g_PageState = CHARACTER_DOWN;

                        if (character_id < 2) { character_id++; }

                        if (character_id != previous_character_id)
                        {
                            flags.is_Book_Animation = true;
                            characterEasing[character_id] = 0.0f;
                        }
                    }
                }

                if (!IsFading() && (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A) || Keyboard_IsKeyTrigger(KK_ENTER))
                    && !flags.is_Disp)
                {
                    g_PushCount++;
                    g_PushBackCount = 0;
                    flags.is_Color = true;
                    flags.is_Operation = true;
                    flags.is_Push = true;
                    flags.is_Start = true;
                    flags.Is_Hanko = true;

                    if (g_PushCount == PUSH_COUNT && flags.Is_Hanko == true)
                    {
                        flags.is_Disp = true;//ハンコをポン
                        // CameraShakeRequestAll(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_SMALL_POWER); // Fukuda
                        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_STAMP, 4.0f, 15);
                        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER2, 4.0f, 0);
                    }
                    else
                    {
                        //2025/05/17 Hara
                        //キャラセリフ
                        {
                            std::string gayaname = "";
                            switch (character_id)
                            {
                            case 0:
                                gayaname = ("characterchoice_kai");
                                break;
                            case 1:
                                gayaname = ("characterchoice_neo");
                                break;
                            case 2:
                                gayaname = ("characterchoice_kon");
                                break;
                            case 3:
                                gayaname = ("characterchoice_ca");
                                break;

                            default:
                                break;
                            }

                            GAYA_REQUEST_OPT opt = {};
                            opt.selfOnly = true;
                            GE_GayaStartRequest(gayaname, 0, opt);
                        }
                        SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER, 4.0f, 0.0f);
                    }
                }

                if (!IsFading() && GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_B) || Keyboard_IsKeyTrigger(KK_BACK) && flags.Is_Coffee == true)
                {//戻る
                    g_PushBackCount++;
                    g_PushCount = 0;
                    SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_EXIT, 4.0f, 0.0f);
                    GE_GayaCancelRequestAll();
                    flags.is_Color = false;
                    flags.is_Disp = false;
                    flags.is_Start = false;
                    flags.is_Operation = false;
                    flags.Is_Coffee = true;

                    bool becomeStageSelect = false;
                    if (flags.is_Push)
                    {
                        if (g_PushBackCount == PUSH_COUNT)
                        {
                            becomeStageSelect = true;
                            flags.is_Push = false;
                        }
                    }

                    else
                    {
                        becomeStageSelect = true;
                    }

                    if (becomeStageSelect)
                    {
                        if (GetMultiFlag() == MULTI_FLAG::LOG_IN
                            || IsSingleBossBattleFlag()
                            //&& WinSockGetType() == WINSOCK_CONNECT_TYPE::CLIENT
                        )
                        {
                            //マルチ参加者の場合
                            //タイトルへもどる
                            SetSceneFade(SCENE_TITLE, FADE_TYPE_STAR_WIPE);
                        }
                        else
                        {
                            //2025/02/27 Hara
                            //即移動
                            g_PageState = CHARACTER_OUT;
                   
                        }
                    }
                }
            }
        }
        CharacterAnimation();
    }

    
    //ボタンを押した時のABボタン
    if (flags.Is_Coffee)
    {//Bボタンを押したとき
        textures.g_switchAlpha -= 0.2f;

        if (textures.g_switchAlpha < 0.0f)
        {
            textures.g_switchAlpha = 1.0f;
            flags.Is_Coffee = false;
        }
    }
    else if (flags.Is_Hanko)
    {//Aボタンを押したとき
        textures.g_switchAlpha -= 0.2f;

        if (textures.g_switchAlpha < 0.0f)
        {
            textures.g_switchAlpha = 1.0f;
            flags.Is_Hanko = false;
        }
    }


    if (flags.is_Disp)
    {
        //2025/03/05 fukuda変更
        //スタンプアニメーション

        UpdateProgress(StampProgress, 20);

        if (StampProgress >= 1.0f)
        {
            if (StampProgress2 == 0.0f)
            {
                CameraShakeRequestAll(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_POWER);
            }
            UpdateProgress(StampProgress2, 20);
        }
        if (StampProgress2 >= 1.0f)
        {
            flags.is_Stop = true;

            //ディレイ
            stopTime++;
            //2025/02/27 Hara
            //キャラセリフ
            if (stopTime == 1)
            {
                //std::string gayaname = "";
                //switch (character_id)
                //{
                //case 0:
                //    gayaname = ("characterchoice_kai");
                //    break;
                //case 1:
                //    gayaname = ("characterchoice_neo");
                //    break;
                //case 2:
                //    gayaname = ("characterchoice_kon");
                //    break;
                //case 3:
                //    gayaname = ("characterchoice_ca");
                //    break;

                //default:
                //    break;
                //}

                //GAYA_REQUEST_OPT opt = {};
                //opt.selfOnly = true;
                //GE_GayaStartRequest(gayaname, 0, opt);
                GE_GayaCancelRequestAll();
            }

            if (stopTime > 55.0f)
            {
                Scene::Select::selected_id = character_id;//キャラクターのIDを保存/決定
                stopTime = 0.0f;
                flags.is_Start = false;
                bool started = SetSceneFade(SCENE_GAME, FADE_TYPE_STAR_WIPE);
            }
        }
    }
}

void CharacterAnimation()
{
    //キャラクターテクスチャを描画
    for (size_t i = 0; i < Scene::Select::characters.size; i++)
    {
        float t = EasingInOutCubic(characterEasing[i]);
        float buki_t = EasingInOutBounce(bukiEasing[i]);

        shakeAmount = sin(bukiEasing[i] * 1.5f) * EasingInOutBounce(buki_t) * 1.1f;

        switch (g_PageState)
        {
        case CHARACTER_NORMAL:
            if (t < 0.7f)
            {
                if (flags.is_Book_Animation)
                {
                    Offset.y = t * SCREEN_WIDTH * 2;       
                }
                Offset.x += shakeAmount;
            }
            else
            {
                Offset.y = (t - 0.5f) * SCREEN_WIDTH * 2 - SCREEN_WIDTH; 
            }

            CharaEasingAnimation();

            break;
        case CHARACTER_DOWN:

            if (character_id == i)
            {// キャラクター選択時のアニメーション
                if (t < 0.7f)
                {
                    Offset.y = t * SCREEN_WIDTH * 2;
                    Offset.x += shakeAmount;
                }
                else
                {
                    Offset.y = (t - 0.5f) * SCREEN_WIDTH * 2 - SCREEN_WIDTH;
                }
            }
            CharaEasingAnimation();

            break;
        case CHARACTER_UP:

            if (character_id == i)
            {// キャラクター選択時のアニメーション
                if (t < 0.7f)
                {
                    Offset.y = -t * SCREEN_WIDTH * 2;
                    Offset.x += shakeAmount;
                }
                else
                {
                    Offset.y = -(t - 0.5f) * SCREEN_WIDTH * 2 + SCREEN_WIDTH;
                }
            }
            CharaEasingAnimation();

            break;
        case CHARACTER_OUT:

            if (character_id == i)
            {// キャラクター選択時のアニメーション
                Offset.y = (t - 0.5f) * SCREEN_WIDTH * 2 - SCREEN_WIDTH;
            }

            characterEasing[character_id] -= 0.012f;
            flags.is_Animation = true;

            if (characterEasing[character_id] < 0.4f)
            {
                characterEasing[character_id] = 0.0f;
                Offset.x = 0.0f;
                textures.textureAlpha = 0.0f;
                flags.is_Animation = false;
                g_MapState = MAP_BACK;
            }
            break;
        case CHARACTER_MAX:
            break;
        default:
            break;
        }
    }
}

// キャラクタータイプ
void CharacterStatusType(int character_id)
{
    for (size_t i = 0; i < Scene::Select::characters.size; i++)
    {
        //キャラの立ち絵とステータス
        switch (character_id)
        {
        case 0:
            textures.statusType = TEX_TYPE_CHARACTER_KAITO_PAGE;
            textures.bukiType = TEX_TYPE_CHARACTER_KAITO_BUKI;
            break;
        case 1:
            textures.statusType = TEX_TYPE_CHARACTER_NEO_PAGE;
            textures.bukiType = TEX_TYPE_CHARACTER_NEO_BUKI;
            break;
        case 2:
            textures.statusType = TEX_TYPE_CHARACTER_KONTO_PAGE;
            textures.bukiType = TEX_TYPE_CHARACTER_KONTO_BUKI;
            break;
        default:
            break;
        }
    }
}

//選択しているキャラクター
void CharacterType(void)
{
    XMFLOAT2 coordinate_initial = { NEW_INITIAL_X, NEW_INITIAL_Y };

    for (size_t i = 0; i < Scene::Select::characters.size; i++)
    {
        switch (i)
        {
        case 0:
            textures.texType = (character_id == 0) ? TEX_TYPE_CHARACTER_KAITO_ON : TEX_TYPE_CHARACTER_KAITO_OFF;
            break;
        case 1:
            textures.texType = (character_id == 1) ? TEX_TYPE_CHARACTER_NEO_ON : TEX_TYPE_CHARACTER_NEO_OFF;
            break;
        case 2:
            textures.texType = (character_id == 2) ? TEX_TYPE_CHARACTER_KONTO_ON : TEX_TYPE_CHARACTER_KONTO_OFF;
            break;
        default:
            break;
        }

        DRAW_POLYGON_DAT drawDatTmp = {};
        float adjustedX = coordinate_initial.x;
        float adjustedY = coordinate_initial.y + SPACING * i;
        float scale = 160.0f;

        {
            scale = (character_id == 0) ? 160.0f : 120.0f;//サイズを小さくする

            //720,-400
            if (i == 1)
            {
                adjustedX = 520.0f;
                adjustedY = -20.0f; // y座標を調整
                scale = (character_id == i) ? 130.0f : 100.0f;
            }
            else if (i == 2)
            {
                adjustedX = 465.0f;
                adjustedY = 100.0f; // y座標を調整
                scale = (character_id == i) ? 105.0f : 80.0f;
            }

            drawDatTmp.pos = MkF3(
                adjustedX,
                adjustedY,
                0.0f
            );

            drawDatTmp.size = MkF3(scale, scale, 1.0f);
            drawDatTmp.rot = 0.0f;
            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
            drawDatTmp.color = textures.textureColor;
            drawDatTmp.texNo = GetTexData(textures.texType).textureId;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
            drawDatTmp.texUV = { 1.0f, 1.0f };
            drawDatTmp.texWH = { 1.0f, 1.0f };
            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
            drawDatTmp.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = true;
            opt.billboard = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            DrawPolygonInCameraEx(drawDatTmp, opt);
        }

        {//履歴書/
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.pos = MkF3(
                -50,
                40 + Offset.y,//50
                Offset.y
            );
            drawDatTmp.size = MkF3(SCREEN_WIDTH * 0.47f, SCREEN_HEIGHT * 0.54f, 1.0f);
            drawDatTmp.rot = 0.0f;
            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
            drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            drawDatTmp.texNo = GetTexData(textures.statusType).textureId;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
            drawDatTmp.texUV = { 1.0f, 1.0f };
            drawDatTmp.texWH = { 1.0f, 1.0f };
            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
            drawDatTmp.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = true;
            opt.billboard = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            DrawPolygonInCameraEx(drawDatTmp, opt);
        }


        {//キャラクター武器
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.pos = MkF3(
                -405.0f- Offset.x,
                20 + +Offset.y,//50
                +Offset.y
            );
            drawDatTmp.size = MkF3(215.0f, 212.0f, 1.0f);
            drawDatTmp.rot = Offset.x;
            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
            drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
            drawDatTmp.texNo = GetTexData(textures.bukiType).textureId;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
            drawDatTmp.texUV = { 1.0f, 1.0f };
            drawDatTmp.texWH = { 1.0f, 1.0f };
            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
            drawDatTmp.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = true;
            opt.billboard = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
            DrawPolygonInCameraEx(drawDatTmp, opt);
        }
    }
}

//A,Bボタンを押したか
void ButtonSwitch(void)
{
    {//出撃タグ
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            -650,
            -610,
            0.0f
        );
        drawDatTmp.size = MkF3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.1f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        drawDatTmp.color = textures.textureColor;
        drawDatTmp.texNo = GetTexData(TEX_TYPE_CHARACTER_TITLE).textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }

    ///キャラクター決定時のハンコはココです。
    if (flags.is_Disp)
    {
        {//出撃ハンコ
            DRAW_POLYGON_DAT drawDatTmp = {};
            drawDatTmp.pos = MkF3(
                210,
                190,
                0.0f
            );
            Float2 size = F2Lerpshare(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, SCREEN_WIDTH * 0.12f, SCREEN_HEIGHT * 0.12f, StampProgress, EasingInOutQuad);
            drawDatTmp.size = MkF3(size.x, size.y, 1.0f);
            drawDatTmp.rot = 0.0f;
            drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
            drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, FLerpshare(0.0f, 1.0f, StampProgress, EasingInOutQuad));
            drawDatTmp.texNo = GetTexData(TEX_TYPE_UI_STUMP).textureId;
            drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
            drawDatTmp.texUV = { 1.0f, 1.0f };
            drawDatTmp.texWH = { 1.0f, 1.0f };
            drawDatTmp.startScale = MkF2(1.0f, 1.0f);
            drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
            drawDatTmp.modelNo = 0;

            DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
            opt.reverse = false;
            opt.fixed = false;
            opt.enableShade = true;
            opt.billboard = true;
            opt.depthBuffIgnore = true;
            opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;

            DrawPolygonInCameraEx(drawDatTmp, opt);
        }

        {//出撃ハンコ2//2025/03/05 Fukuda
            if (StampProgress >= 1.0f)
            {


                DRAW_POLYGON_DAT drawDatTmp = {};
                drawDatTmp.pos = MkF3(
                    210,
                    190,
                    0.0f
                );
                Float2 size = F2Lerpshare(SCREEN_WIDTH * 0.12f, SCREEN_HEIGHT * 0.12f, SCREEN_WIDTH * 0.36f, SCREEN_HEIGHT * 0.36f, StampProgress2, EasingOutQuint);
                drawDatTmp.size = MkF3(size.x, size.y, 1.0f);
                drawDatTmp.rot = 0.0f;
                drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
                drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0f, 0.0f, StampProgress2, EasingOutQuint));
                drawDatTmp.texNo = GetTexData(TEX_TYPE_UI_STUMP).textureId;
                drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
                drawDatTmp.texUV = { 1.0f, 1.0f };
                drawDatTmp.texWH = { 1.0f, 1.0f };
                drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
                drawDatTmp.modelNo = 0;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                opt.reverse = false;
                opt.fixed = false;
                opt.enableShade = true;
                opt.billboard = true;
                opt.depthBuffIgnore = true;
                opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;

                DrawPolygonInCameraEx(drawDatTmp, opt);
            }
        }
    }


    textures.coffeeType = flags.Is_Coffee ? TEX_TYPE_UI_BUTTON_B_ON : TEX_TYPE_UI_BUTTON_B_OFF;
    textures.hankoType = flags.Is_Hanko ? TEX_TYPE_UI_BUTTON_A_ON : TEX_TYPE_UI_BUTTON_A_OFF;

    //2025/02/27 hara
    //ifの外へ移動
    {//Coffee
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            -385,
            270,
            0.0f
        );
        drawDatTmp.size = MkF3(399 * 0.5f, 246 * 0.5f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

        drawDatTmp.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        drawDatTmp.texNo = GetTexData(textures.coffeeType).textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;//レイヤーを一番前にする
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }

    {//ハンコ
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            380,
            265,
            0.0f
        );
        drawDatTmp.size = MkF3(455 * 0.5f, 253 * 0.5f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        drawDatTmp.color = { 1.0f, 1.0f, 1.0f, 1.0f};
        drawDatTmp.texNo = GetTexData(textures.hankoType).textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }
}

void ColorAnimation(void)
{
    if (!IsFading())
    {//α値を上げて行く
        if (g_MapState == MAP_START||GetSceneBefore() != SCENE_STAGESELECT)
        {
            textures.textureAlpha += 0.03f;
            if (textures.textureAlpha >= 1.0f)
            {
                textures.textureAlpha = 1.0f;
            }

            textures.textureColor = { 1.0f,1.0f,1.0f, textures.textureAlpha };
        }

        if (g_PageState==CHARACTER_OUT)
        {
            textures.textureAlpha -= 0.06f;
            if (textures.textureAlpha < 0.0f)
            {
                textures.textureAlpha = 0.0f;
            }
            textures.textureColor = { 1.0f,1.0f,1.0f, textures.textureAlpha };
        }
    }

    if (flags.is_Color)
    {//画面を少し暗くする
        textures.g_Color -= 0.04f;
        textures.g_Alpha -= 0.04f;

        if (textures.g_Alpha <= 0.5f)
        {
            textures.g_Color = 0.5f;
            textures.g_Alpha = 0.5f;
        }
        if (flags.is_Color)
        {
            textures.color = MkF4(textures.g_Color, textures.g_Color, textures.g_Color, textures.g_Alpha);
        }
    }
    else
    {// 画面を元に戻す
        textures.g_Color += 0.05f;
        textures.g_Alpha += 0.05f;

        if (textures.g_Alpha >= 1.0f)
        {
            textures.g_Color = 1.0f;
            textures.g_Alpha = 1.0f;
        }
        textures.color = MkF4(textures.g_Color, textures.g_Color, textures.g_Color, textures.g_Alpha);
    }
}

//キャラ選択時の矢印と枠
void SelectCursor(void)
{
    float t = EasingInOutSine(g_Interpolation);

    if (character_id == 1 || character_id == 2)
    {//矢印上
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            700.0f,
            -375.0f,
            0
        );
        drawDatTmp.size = MkF3(200.0f * 0.25f, 200.0f * 0.25f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        drawDatTmp.color = textures.textureColor;
        drawDatTmp.texNo = GetTexData(TEX_TYPE_CHARACTER_SELECT_UP).textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }

    if (character_id == 0 || character_id == 1)
    {//矢印下
        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            425.0f,
            180.0f,
            0
        );
        drawDatTmp.size = MkF3(200.0f * 0.15f, 200.0f * 0.15f, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        drawDatTmp.color = textures.textureColor;
        drawDatTmp.texNo = GetTexData(TEX_TYPE_CHARACTER_SELECT_DOWN).textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }

    for (size_t i = 0; i < Scene::Select::characters.size; i++)
    {
        CursorPosition = g_OldCursor * (1.0f - t) + g_NowPositionY * t;//線形補間


        g_Interpolation += 0.03f;//速度の設定

        if (g_Interpolation > 1.0f)
        {
            g_Interpolation = 1.0f;
        }

        if (character_id == 0)
        {
            g_NowPositionY = NOW_CURSOR_Y;//-180,x720
        }
        if (character_id == 1)
        {
            g_NowPositionY = 75.0f;
        }
        if (character_id == 2)
        {
            g_NowPositionY = 250.0f;
        }

        //枠
        {
            g_tex_cursor.width = 223.0f;
            g_tex_cursor.height = 158.0f;
               
            DrawSpriteQuadColorRotateTexUv(
                780.0f,
                -100.0f + CursorPosition,
                g_tex_cursor.width, g_tex_cursor.height,
                0,
                textures.textureColor,
                g_tex_cursor.textureId,
                { 0.0f, 0.0f },
                { 1.0f, 1.0f }
            );
        }
    }
}

void MapAnimation(void)
{
    float t = EasingInOutQuint(startEasing[0]);

    if (g_MapState == MAP_START)
    {
        if (t < 0.45f)
        {
            startXOffset = -t * SCREEN_WIDTH * 2;
        }

        startEasing[0] += 0.01f;

        if (startEasing[0] > 0.45f)//1.0f
        {
            startEasing[0] = 1.0f;
            g_MapState = MAP_OUT;//OUTしたので切り替え
            SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT_SCENE_ENTER, 4.0f, 10);
        }
    }

    if (g_MapState == MAP_BACK)
    {
        if (t < 0.45f)
        {
            startXOffset = -t * SCREEN_WIDTH * 2;
        }

        //startEasing[0] -= 0.02f;
        startEasing[0] -= 0.2f;

        if (startEasing[0] < 0.0f)//1.0f
        {
            startEasing[character_id] = 1.0f;
           
            SetScene(SCENE_STAGESELECT);
            g_MapState = MAP_NONE;
            
        }
    }

    {//マップ
        g_tex_map.width = 2590.0f * 0.52f;
        g_tex_map.height = 1634.0f * 0.52f;
        g_tex_map.textureId;

        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            -235.0f + startXOffset,
            116.0f,
            0
        );
        drawDatTmp.size = MkF3(g_tex_map.width, g_tex_map.height, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        drawDatTmp.texNo = g_tex_map.textureId;;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }

    {//マップのカーソル
        g_tex_map_cursor.width = 697.0f * 0.59f;
        g_tex_map_cursor.height = 641.0f * 0.59f;

        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(
            -277.5f + startXOffset,
            5.0f, 0.0f 
        );

        drawDatTmp.size = MkF3(g_tex_map_cursor.width, g_tex_map_cursor.height, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);
        drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        drawDatTmp.texNo = g_tex_map_cursor.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = { 1.0f, 1.0f };
        drawDatTmp.texWH = { 1.0f, 1.0f };
        drawDatTmp.startScale = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.modelNo = 0;

        DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
        opt.reverse = false;
        opt.fixed = false;
        opt.enableShade = true;
        opt.billboard = true;
        opt.depthBuffIgnore = true;
        opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
        DrawPolygonInCameraEx(drawDatTmp, opt);
    }

    {
        //マップのピン
        g_tex_map_pin.width = 25.0f;
        g_tex_map_pin.height = 25.0f;

        DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(-205.0f+startXOffset, 225.0f, 1.0f);
        drawDatTmp.size = MkF3(g_tex_map_pin.width, g_tex_map_pin.height, 1.0f);
        drawDatTmp.rot = 0.0f;
        drawDatTmp.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        drawDatTmp.texNo = g_tex_map_pin.textureId;
        drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;
        drawDatTmp.texUV = MkF2(0.0f, 0.0f);
        drawDatTmp.texWH = MkF2(1.0f, 1.0f);
        drawDatTmp.startScale = MkF2(1.0, 1.0f);
        drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);
        drawDatTmp.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
        DRAW_POLYGON_IN_CAMERA_EX_OPTION ropt = {};
        ropt.reverse = false;//反転
        ropt.fixed = false;//カメラの移動を無視する
        ropt.billboard = true;//ビルボード
        ropt.priority = DRAW_POLYGON_PRIORITY_NORMAL;//レイヤ
        ropt.depthBuffIgnore = true;//深度バッファ
        //描画
        DrawPolygonInCameraEx(drawDatTmp, ropt);
    }
}


//キャラクターイージング
void CharaEasingAnimation()
{
    characterEasing[character_id] += 0.012f;
    bukiEasing[character_id] += 0.005f;
    flags.is_Animation = true;

    if (characterEasing[character_id] > 0.4f)
    {
        flags.is_Animation = false;
    }

    if (characterEasing[character_id] > 0.65f)
    {//途中でキャラID変更
        if (g_PageState!= CHARACTER_NORMAL)
        {
            CharacterStatusType(character_id);
        }
    }
    if (characterEasing[character_id] > 1.0f)
    {
        characterEasing[character_id] = 1.0f;
        shakeAmount=0.0f;
        Offset.x = 0.0f;
        g_PageState = CHARACTER_MAX;
    }
}

void UpdateProgress(float& progress, float moveTime)
{
    progress += 1.0f / moveTime;
    if (progress > 1.0f)
    {
        progress = 1.0f;
    }
}
// 進行度から補間位置を求める関数（イージング対応版）
Float2 F2Lerpshare(float startX, float startY, float endX, float endY,
    float progress, std::function<float(float)> easing)
{
    float t = easing(progress);  // 進行度にイージング適用
    float x = startX * (1.0f - t) + endX * t;
    float y = startY * (1.0f - t) + endY * t;
    return MkF2(x, y);
}

// 進行度からfloat値を補間（数値版）
float FLerpshare(float start, float end,
    float progress, std::function<float(float)> easing)
{
    float t = easing(progress);  // 進行度にイージング適用
    return start * (1.0f - t) + end * t;
}