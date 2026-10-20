// ===================================================
// GOE_Character_Player.h キャラクターオブジェクト
// 
// hara sougo  2024/11/25
// ===================================================
#ifndef _GOE_CHARACTER_PLAYER_H_
#define _GOE_CHARACTER_PLAYER_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"
#include "../../Entities/character.h"

#include "GOE_Character.h"

// ===================================================
// マクロ定義
// ===================================================

//
// クラス
//
class GOE_Character_Player : public GOE_Character
{
	typedef GOE_Character
		SuperGOE; //親クラスにあだ名をつける

public:
    static constexpr int KAKOPON_DAT_NUM = 3;
    static constexpr int KAKOPON_INK_GAGE_MEMORY_NUM = 3;
    static constexpr int KAKOPON_INK_GAGE_MAX = 100000;
    static constexpr int KAKOPON_INK_GAGE_ONE = KAKOPON_INK_GAGE_MAX / KAKOPON_INK_GAGE_MEMORY_NUM;
    //static constexpr int KAKOPON_INK_GAGE_HEAL = KAKOPON_INK_GAGE_ONE / (FRAME_ONE_SECOND * 5);
    static constexpr int KAKOPON_INK_GAGE_HEAL = KAKOPON_INK_GAGE_ONE / (FRAME_ONE_SECOND * 4);
    static constexpr int KAKOPON_INK_GAGE_HEAL_ATTACK = KAKOPON_INK_GAGE_MAX / 20;
    static constexpr int KAKOPON_INK_GAGE_COST = KAKOPON_INK_GAGE_MAX / 120;
private:

	//通常攻撃の判定参照
	//後でキャラごとに変えられるようにする
	static constexpr auto N_ATTACK_TEST_ATK_COL_NAME = ("COL_ATK_PLAYER_TEST_NORAL_ATTACK");

	//囲んでポンの攻撃判定データのファイル名
	static constexpr auto KAKOPON_GENERAL_ATK_COL_NAME = ("COL_ATK_KAKOPON_GENERAL");

    static constexpr int DAMAGE_INV_FRAME = FRAME_ONE_SECOND * 2;

	//通常攻撃のフレーム
	static constexpr int WALK_FRAME = 36;

	static constexpr int ATTACK_START_FRAME = 6;
	static constexpr int ATTACK_FRAME = 6;

    static constexpr int ATTACK_FTH_FRAME = 10;
    static constexpr int ATTACK_TOTAL_FRAME = ATTACK_START_FRAME + ATTACK_FRAME + ATTACK_FTH_FRAME;

	static constexpr int ATTACK_COOLDOWN_FRAME = ATTACK_TOTAL_FRAME + 50;

	static constexpr int KAKOPON_COOLDOWN_FRAME = 60;

    //かこポンの頂点を配置する間隔
    //static constexpr float KAKOPON_ONE_LINE_DISTANE = 56.0f;
    static constexpr float KAKOPON_ONE_LINE_DISTANE = 192.0f;

    //かこぽん中のスピード補正
    static constexpr float KAKOPON_WALK_SPEED_MOD = 1.2f;

    //かこポン線 頂点最大数
    static constexpr int KAKOPON_VERTEX_MAX = 256;


    enum class KAKOPON_GRAPHIC {
        Main,
    };

    enum class KAKOPON_TYPE {
        Main,
        Heal
    };

    //かこポン頂点データ
    struct KAKOPON_VERTEX {
        bool enable;
        Float2 pos;
        bool isEndLine;
    };

    struct KAKOPON_DATA {
        //わっかが完成したか
        bool isConnected;

        //わっかの中央座標
        Float2 connectCentroid;
        float connectAreaSize;

        //つながってからのフレーム
        int frame;
        int frameMax;

        //頂点
        KAKOPON_VERTEX vertex[KAKOPON_VERTEX_MAX];
        int vertexNum;

        //内部のオブジェクト
        std::vector<GAME_OBJECT_IDENTIFIER> objs;

        //タイプ
        KAKOPON_TYPE type;
        //見た目
        KAKOPON_GRAPHIC graphic;

        bool isFirstObjectCheck; //外から操作する
    };

    //かこポン描いてるときに使うデータ
    struct KAKOPON_LINEDRAW {

        //入力フラグ
        int vertexDrawInputFlag;

        //頂点の配置中フラグ
        bool vertexDrawFlag;

        //以前配置した頂点の座標
        Float2 vertexBeforePos;

        //頂点を配置していいか
        bool vertexAddFlag;

        //わっかの中をチェックしたか
        //bool isVertexObjectCheck;

        //線を描いているかのフラグ
        bool isLineDrawing;

        //後隙フレーム
        int afterTimerFrame;
    };

    struct KAKOPON_SET {
        KAKOPON_DATA dat;
        KAKOPON_LINEDRAW lineDraw;
    };


    //かこポン攻撃
    void kakopon_objects_attack(KAKOPON_DATA dat);

    //かこポンの処理
    void init_kakoponDat(KAKOPON_DATA* dat);
    void update_kakoponDat(KAKOPON_DATA* dat);
    void draw_kakoponDat(KAKOPON_DATA* dat);

    void update_kakopon_action(bool keyPush, KAKOPON_DATA* dat, KAKOPON_LINEDRAW* linedraw);

protected:
    unsigned int m_TexId_AttackEffect_s = -1;
    Int2 m_AnimCount_AttackEffect_s = { 3, 3 };

    unsigned int m_TexId_kakoponEffect_Trail = -1;
    unsigned int m_TexId_kakoponEffect_Add = -1;
    unsigned int m_TexId_kakoponEffect_Aura = -1;

    unsigned int m_TexId_kakoponEffect_Add_Heal = -1;

	int m_AttackCooldown = 0;

    Float2 m_AxisVec = {};

    bool m_IsDrawing = false;
    bool m_IsIdling = false;
    bool m_IsWalkAnim = false; //アニメーション依存 立ち止まっているときもtrueになる
    bool m_IsNormalAttack = false;

    bool m_IsWalking = false;

    int m_InkGage_DrawBefore = 0;

    int m_InkGage = 0;
    int m_InkGageMax = 0;

    //かこポンデータ
    KAKOPON_SET m_KakoDatList[KAKOPON_DAT_NUM] = {};
    //KAKOPON_DATA m_KakoDat_Main = {};
    //KAKOPON_LINEDRAW m_KakoponLineDraw_Main = {};

    //KAKOPON_DATA m_KakoDat_SubTest = {};
    //KAKOPON_LINEDRAW m_KakoponLineDraw_SubTest = {};

public:
	GOE_Character_Player(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Character_Player() {};

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

    virtual Float2 GetKakoponVertexNum();

    virtual Int2 GetKakoponInkGage();

	//仮
	//static Float2 GetKakoponVertexNum();
};

#endif