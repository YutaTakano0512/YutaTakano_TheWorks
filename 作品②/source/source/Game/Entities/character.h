// ===================================================
// character.h キャラクター管理
// 
// hara sougo  2024/11/11
// ===================================================

#ifndef _CHARACTER_H_
#define _CHARACTER_H_


#include "../Systems/sys_general.h"
#include "../../Core/Input/controller_main.h"
#include "../Game Object/gameObject.h"

// ===================================================
// マクロ定義
// ===================================================
#define CHARACTER_DATA_FOLDER_PATH		("rom/character_data") //キャラクターのデータをまとめたフォルダの名前

#define CHARACTER_FOLDER_PATH			("characters") //キャラクターをまとめたフォルダの名前
#define CHARACTER_FILE_EXT				("dat") //キャラクターファイルの拡張子

#define CHARACTER_COL_FOLDER_PATH		("character_collisions") //キャラクターの判定をまとめたフォルダの名前
#define CHARACTER_COL_ATK_FOLDER_PATH	("attack") //キャラクターの攻撃判定をまとめたフォルダの名前
#define CHARACTER_COL_GRAB_FOLDER_PATH	("grab") //キャラクターのつかみ判定をまとめたフォルダの名前
#define CHARACTER_COL_REC_FOLDER_PATH	("recieve") //キャラクターのくらい判定をまとめたフォルダの名前

#define CHARACTER_STATUS_FOLDER_PATH	("character_status") //キャラクターのステータスをまとめたフォルダの名前

//デフォルトのくらい判定名
#define CHARACTER_DEFAULT_REC_COL_NAME	("COL_REC_GENERAL_0")

//void motionColDatInit(void);

namespace Character
{
	typedef unsigned int PLAY_MOTION_ID;
	// ===================================================
	// 定数定義
	// ===================================================
	constexpr int TXT_MAX				= 64; //文字列の長さ

	constexpr int STATUS_MAX			= 128; //キャラのステータス最大数
	constexpr int MOTION_COL_DAT_MAX	= 128; //キャラの判定の最大量

	constexpr int MOTION_MAX			= 32; //キャラのモーションの最大数
	//constexpr int MOTION_MAX			= 16; //キャラのモーションの最大数
	constexpr int MOTION_FRAME_MAX		= 64; //キャラのモーションのフレーム最大数
	//constexpr int MOTION_FRAME_MAX	= 256; //キャラのモーションのフレーム最大数
	constexpr int MOTION_COL_MAX		= 16; //キャラのモーションの1F内の判定最大数

	constexpr float DEFAULT_SIZE = 720.0f; //デフォルトのサイズ

    constexpr float GRAVITY = (0.0f);	//重力加速度
    constexpr float FRICTION = (0.40f);	//摩擦力

	// ===================================================
	// 列挙体宣言
	// ===================================================

	enum MOTION_COL_TYPE {
		MOTION_COL_TYPE_ATK = 0,
		MOTION_COL_TYPE_GRAB,
		MOTION_COL_TYPE_REC,

		MOTION_COL_TYPE_MAX,
	};


	// ===================================================
	// 構造体宣言
	// ===================================================

	//ステータスの詳細データ
	struct STATUS_DAT {
		bool use;

		char name[TXT_MAX]; //名前 (参照時 識別に使います)

		GAME_OBJECT_STATUS status;
		bool family[GAME_OBJECT_FAMILY_MAX];

        std::string defaultColAtkName;
		float scale;
		Float3 pos;
	};


	//判定の詳細データ
	struct MOTION_COL_DAT {
		bool use;

		char name[TXT_MAX]; //名前 (参照時 識別に使います)

		MOTION_COL_TYPE type; //判定の種類

		//↓攻撃の威力とかの保存をする
		union {
			GAME_OBJECT_COLLISION_ATTACK_CHILD atkCol;
			GAME_OBJECT_COLLISION_RECIEVE_CHILD recCol;
		};
	};

	//
	//=================================================
	//

	//モーションの判定
	struct MOTION_COL {
		bool use;

		Float2 pos;
		Float2 size;

		//std::string type;
		MOTION_COL_TYPE colType; //参照する判定の判定の種類
		char colName[TXT_MAX]; //参照する判定の詳細（名前）

		//判定のグループナンバー
		//この判定が攻撃判定だった場合、
		//攻撃がヒットした後、
		//同じグループ番号の判定が再ヒットしなくなる
		int group;
	};

	//モーションデータ (1F分)
	struct MOTION_FRAME_LOAD {
		bool use;

		int spriteNumber; //参照するスプライトの番号

		MOTION_COL collision[MOTION_COL_MAX]; //モーションの判定
	};
	struct MOTION_FRAME {
		int spriteNumber; //参照するスプライトの番号

		std::vector<MOTION_COL> collision; //モーションデータ
	};

	//モーションデータ
	struct MOTION_LOAD {
		bool use;

		//std::string name;
		char name[TXT_MAX]; //モーションの名前
		int maxFrame; //全体フレーム

		MOTION_FRAME_LOAD frame[MOTION_FRAME_MAX]; //フレームデータ
	};
	struct MOTION {
		std::string name; //モーションの名前
		int maxFrame; //全体フレーム

		std::vector<MOTION_FRAME> frame; //モーションデータ
	};

	//キャラクターデータ
	struct MAIN_LOAD {
		bool use;

		//std::string name;
		char name[TXT_MAX]; //名前
		//std::string sprite;
		char sprite[TXT_MAX]; //スプライト(ファイルパス)
		int spriteXCnt; //分割数横
		int spriteYCnt; //分割数縦

		MOTION_LOAD motion[MOTION_MAX]; //モーションデータ

		char statusName[TXT_MAX]; //参照するステータスの詳細（名前）
	};
	struct MAIN {
		bool use;

		std::string name; //名前
		std::string sprite; //スプライト(ファイルパス)
		int spriteXCnt; //分割数横
		int spriteYCnt; //分割数縦

		std::vector<MOTION> motion; //モーションデータ

		std::string statusName; //参照するステータスの詳細（名前）
	};

	//--------------------------------------------------

	//キャラクターオブジェクトに持たせるデータ

	//再生中のモーション
	struct OBJECT_DAT_PLAYING_MOTION {
		const MOTION* motion;
		int maxFrame;
		int frame;
		bool loop;

		int const* frameReference;

		PLAY_MOTION_ID playKey;
		bool enable;
	};

	//キャラクターオブジェクトに持たせるデータ
	struct OBJECT_DAT {
		//const MAIN* data;
		int dataIndex;

		//固有のパラメータとかを書き加えていく

		TEX_DAT texture; //テクスチャ

		//入力
		CONTROLLER_INPUT_DAT inputDat;

		//再生中のアニメーションのデータ
		OBJECT_DAT_PLAYING_MOTION playingMotion;

		bool reverse;
		bool reverseRequest; //外から変更される値

		//void* privateVariable; //このオブジェクト独自で使う変数の格納場所
	};


	// ===================================================
	// プロトタイプ宣言
	// ===================================================
	void Init(void);
	void Uninit(void);

	//引数の文字列とファイル名が一致したキャラクターを読み込み、
	//そのポインターを返す。読み込めなかった場合は nullptr を返す。
	//使い終わったら必ずUnloadしてください。
	MAIN_LOAD* Load(std::string name);

	//読み込んだキャラクターデータの解放
	void Unload(MAIN_LOAD*);

	//キャラクターデータをもとにオブジェクトを生成する
	GAME_OBJECT* CreateCharacterGameObject(Character::MAIN_LOAD* charaDat);
	GAME_OBJECT* CreateCharacterGameObject(std::string name);

	//キャラクターデータを取得する
	const Character::MAIN* GetCharacterMainData(int);
	void CharacterMainDataClearCache(void);

	Character::STATUS_DAT GetStatusDataFindByName(std::string name);

	//キャラクターのモーションを名前から探して
	//インデックスを返す。
	//見つからなかった場合は -1 を返す
	int GetCharacterMotionFindByName(const Character::MAIN* charaDat, std::string name);

	//キャラクターのモーションを再生する
	Character::PLAY_MOTION_ID PlayMotionCharacterGameObject(Character::OBJECT_DAT* charObj,
		int index,

		bool loop=false,
		int const* frameRef=nullptr,
		int maxFrame=-1
	);

	Character::OBJECT_DAT* GetObjectDataCharacterGameObject(GAME_OBJECT* obj);

	Character::MOTION_COL_DAT GetMotionColDataFindByName(Character::MOTION_COL_TYPE mtype, std::string name);
    Character::STATUS_DAT GetStatusDataFindByName(std::string name);
    void SetStatusData(GAME_OBJECT* obj, Character::STATUS_DAT sd);
	

	//プレイヤーの入力取得
	//キャラクターの入力に関しては
	//controller_mainの関数ではなく
	//こちらを使用してください
	bool GetCtrlPress(Character::OBJECT_DAT* charObj, CONTROLLER_BUTTON_TYPE button);
	bool GetCtrlTrigger(Character::OBJECT_DAT* charObj, CONTROLLER_BUTTON_TYPE button);
	bool GetCtrlRelease(Character::OBJECT_DAT* charObj, CONTROLLER_BUTTON_TYPE button);
	Float2 GetCtrlStickLeft(Character::OBJECT_DAT* charObj);
	Float2 GetCtrlStickRight(Character::OBJECT_DAT* charObj);

	CONTROLLER_STICK_DAT GetCtrlStickLeftTrigger(Character::OBJECT_DAT* charObj);
	CONTROLLER_STICK_DAT GetCtrlStickRightTrigger(Character::OBJECT_DAT* charObj);

	CONTROLLER_STICK_DAT GetCtrlStickLeftPress(Character::OBJECT_DAT* charObj);
	CONTROLLER_STICK_DAT GetCtrlStickRightPress(Character::OBJECT_DAT* charObj);

	CONTROLLER_STICK_DAT GetCtrlStickLeftRelease(Character::OBJECT_DAT* charObj);
	CONTROLLER_STICK_DAT GetCtrlStickRightRelease(Character::OBJECT_DAT* charObj);
}


#endif //_CHARACTER_H_