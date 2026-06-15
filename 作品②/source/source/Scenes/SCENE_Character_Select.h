/******************************
* @author KAPITONOV IGOR
* @date 2024-11-09
******************************/

#ifndef _SCENE_SELECT_H_
#define _SCENE_SELECT_H_

#include <vector>

#include "../Core/DirectX/dx.h"
#include "../Game/Sprite/sprite.h"
#include "../Core/Input/keyboard.h"
#include "../Game/Entities/character.h"

#define NEW_INITIAL_X            (600.0f)//キャラ選択座標位置X
#define NEW_INITIAL_Y            (-180.0f)//キャラ選択座標位置Y
#define SPACING                      (280.0f)//キャラスペース
#define CHARACTER_SELECT_MAX        (3)//キャラクターの数
#define PUSH_COUNT                              (2)//プッシュ回数
#define NOW_CURSOR_X                             (692.0f)//初期のカーソル座標
#define NOW_CURSOR_Y                             (-90.0f)//初期のカーソル座標Y

enum AnimationPage
{
    CHARACTER_NORMAL = -1,
    CHARACTER_DOWN,
    CHARACTER_UP,
    CHARACTER_OUT,
    CHARACTER_MAX
};

enum e_MapAnimation
{
    MAP_NONE = -1,
    MAP_START,
    MAP_OUT,
    MAP_BACK,
    MAP_MAX
};

struct AnimationFlags
{
    bool is_Animation;       // アニメーションフラグ
    bool is_Disp;            // テクスチャの描画
    bool is_Color;           // 色いじいじ
    bool is_Push;
    bool is_Operation;       // 操作フラグ
    bool is_Book_Animation;  // アニメーションフラグ
    bool is_Start;//ON,OFF
    bool Is_Coffee;
    bool Is_Hanko;
    bool is_Stop;//操作ができないようにする
};

struct TextureTypes
{
    TEX_TYPE statusType;
    TEX_TYPE texType;
    TEX_TYPE bukiType;
    TEX_TYPE coffeeType;
    TEX_TYPE hankoType;
    Float4 color;          // 色
    Float4 textureColor;   // テクスチャのα値設定
    float g_Color;         // アニメーションのフレーム
    float g_Alpha;         // α値をいじる
    float textureAlpha;
    float g_switchAlpha;//coffeeとハンコのオンオフ
};


namespace Scene
{
    namespace Select
    {
        struct Characters
        {
            struct Character_data {};
            int size = CHARACTER_SELECT_MAX;
            std::vector<Character_data> character;
        };
        extern Characters characters;
     
        extern int selected_id; // 選択されたキャラクターIDを保存する
    }
}

void InitializeSceneCharacterSelect();
void FinalizeSceneCharacterSelect();
void DrawSceneCharacterSelect();
void UpdateSceneCharacterSelect();


#endif // _SCENE_SELECT_H_