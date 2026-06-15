// ===================================================
// camera.cpp カメラ制御
// 
// hara sougo  2024/07/23
// ===================================================
#include "../../game_main.h"
#include "../Sprite/sprite.h"
#include "../Texture/texture.h"
#include "fade.h"
#include "../../Core/Input/controller_main.h"
#include "collision.h"

#include "../../Core/DirectX/dx.h"
#include "../../Core/DirectX/dx_camera.h"
#include "camera.h"

#include "../Game Object/gameObject.h"
#include "../Entities/Block/block.h"
#include "../Managers/gameManager.h"
#include "../Managers/scene.h"
#include "../../Scenes/SCENE_Game.h"
#include "../Game Stage/Stage_Base.h"
#include "../Entities/player.h"

#include "../Systems/ui_Game.h"


#include "../Game Object/Game Object Events/GOE_Character.h"
#include "../Game Object/Game Object Events/GOE_Character_Player.h"
#include "../Game Object/Game Object Events/GOE_Character_Enemy.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================
void cameraMoveSetEndTrans(int index);
void update_moveRequ(CAMERA_MOVE_REQUEST& moveRequ,
    Float3* posRef, float* rotRef, Float2* scaleRef, float* zoomRef
);

// ===================================================
// グローバル変数
// ===================================================
static CAMERA g_Camera[CAMERA_NUM] = {};

//static std::vector<CAMERA_DRAW_BUF> g_CameraDrawBuf[CAMERA_NUM] = {};
static CAMERA_DRAW_BUF g_CameraDrawBuf[CAMERA_NUM][CAMERA_DRAW_BUF_MAX] = {};

static int g_CameraCurrentIndex = 0;


void CameraAddDrawBuf(int index, CAMERA_DRAW_BUF buf)
{
    if (index < 0 || index >= CAMERA_NUM)
    {
        return;
    }
    //g_CameraDrawBuf[index].push_back(buf);
    for (int i = 0; i < CAMERA_DRAW_BUF_MAX; i++)
    {
        if (!g_CameraDrawBuf[index][i].enable)
        {
            g_CameraDrawBuf[index][i] = buf;
            g_CameraDrawBuf[index][i].enable = true;
            break;
        }
    }
}
CAMERA_DRAW_BUF* CameraGetDrawBuf(int index)
{
    if (index < 0 || index >= CAMERA_NUM)
    {
        return nullptr;
    }
    return g_CameraDrawBuf[index];
}


// ===================================================
// カメラの初期化
// ===================================================
void InitCamera(void)
{
	ResetCamera();

}
void ResetCamera(void)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CAMERA& camera = g_Camera[i];
        camera = {};
        camera.trans.pos = MkF3(0.0f, 0.0f, 0.0f);
        camera.trans.rot = 0.0f;
        camera.trans.scale = MkF2(1.0f, 1.0f);
        camera.trans.zoom = 1.0f;

        camera.shakePower = 1.0f;
        camera.shakeFrame = 0;
        camera.shakeFrameMax = 1;

        camera.goZoomRequ.objIndex = -1;


        //3D用の設定
        //camera.camera3D.fov = 1.0f;
        camera.camera3D.fov = 1.0f;
        //camera.camera3D.pos = MkF3(0.0f, 200.0f, -160.0f);
        camera.camera3D.pos = MkF3(0.0f, -70.0f, -90.0f);
        //camera.camera3D.pos = MkF3(0.0f, 6.0f, -10.0f);
        //camera.camera3D.pos = MkF3(0.0f, 8.0f, -8.0f);
        //camera.camera3D.pos = MkF3(0.0f, 8.0f, -0.01f);
        camera.camera3D.target = MkF3(0.0f, 0.0f, 0.0f);
        //camera.camera3D.up = MkF3(0.0f, -1.0f, 0.0f);
        //camera.camera3D.up = MkF3(0.0f, 1.0f, 0.0f);
        camera.camera3D.up = MkF3(0.0f, 0.0f, -1.0f);
        //camera.camera3D.scale = 0.01f;
        //camera.camera3D.scale = 0.1f;
        camera.camera3D.scale = 0.1f;

        //実際の設定はループ内でやる

        //DX_SetCameraInfo(
        //	camera.camera3D.fov,
        //	{
        //		camera.camera3D.pos.x,
        //		camera.camera3D.pos.y,
        //		camera.camera3D.pos.z
        //	},
        //	{
        //		camera.camera3D.target.x,
        //		camera.camera3D.target.y,
        //		camera.camera3D.target.z
        //	},
        //	{
        //		camera.camera3D.up.x,
        //		camera.camera3D.up.y,
        //		camera.camera3D.up.z
        //	},
        //	camera.camera3D.scale
        //);

        //g_CameraDrawBuf[i].clear();
        //g_CameraDrawBuf[i].shrink_to_fit();
        //g_CameraDrawBuf[i].reserve(512);
        for (int j = 0; j < CAMERA_DRAW_BUF_MAX; j++)
        {
            g_CameraDrawBuf[i][j] = {};
        }
    }
    g_CameraCurrentIndex = 0;
}


// ===================================================
// カメラの更新
// ===================================================
void UpdateCamera(void)
{
#if defined(_DEBUG) || defined(DEBUG)
    //デバッグ

    ////if(!WinSockClientIsEnable())
    //{
    //    static bool kp = false;
    //    if (Keyboard_IsKeyTrigger(KK_C))
    //    {
    //        if (!kp)
    //        {
    //            g_CameraCurrentIndex++;
    //            if (g_CameraCurrentIndex >= CAMERA_NUM)
    //            {
    //                g_CameraCurrentIndex = 0;
    //            }

    //            kp = true;
    //        }
    //    }
    //    else
    //    {
    //        kp = false;
    //    }
    //}
#endif

    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CAMERA& camera = g_Camera[i];

        DX_CAMERA_INFO cinfo = DX_GetCameraInfo(i);

        //カメラシェイクの更新
        if (camera.shakeFrame > 0)
        {
            camera.shakeFrame--;
        }


        float changeLerpMod = 1.0f;
        if (camera.modeChangeLerpFrame.x > 0)
        {
            changeLerpMod = 1.0f - std::max(std::min(
                (float)camera.modeChangeLerpFrame.x
                / camera.modeChangeLerpFrame.y
                , 1.0f), 0.0f)
            ;
            camera.modeChangeLerpFrame.x--;
        }

        //ムブリク処理

        //if (camera.mode == CAMERA_MODE_PLAYER_CHASE
        //    || camera.mode == CAMERA_MODE_PLAYER_CHASE_INSTANT
        //    || camera.mode == CAMERA_MODE_BOSS_CHASE
        //    || camera.mode == CAMERA_MODE_BOSSANDZAKO_CHASE
        //    )
        {
            bool isenable =
                (camera.mode == CAMERA_MODE_PLAYER_CHASE
                    || camera.mode == CAMERA_MODE_PLAYER_CHASE_INSTANT
                    || camera.mode == CAMERA_MODE_BOSS_CHASE
                    || camera.mode == CAMERA_MODE_BOSSANDZAKO_CHASE
                    || camera.mode == CAMERA_MODE_BOSS_CHASE_WIDE
                    || camera.mode == CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE
                    || camera.mode == CAMERA_MODE_BOSS_CHASE_EXWIDE
                    );
            bool isInstant = camera.mode == CAMERA_MODE_PLAYER_CHASE_INSTANT;

            bool isBoss =
                camera.mode == CAMERA_MODE_BOSS_CHASE
                || camera.mode == CAMERA_MODE_BOSSANDZAKO_CHASE
                || camera.mode == CAMERA_MODE_BOSS_CHASE_WIDE
                || camera.mode == CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE
                || camera.mode == CAMERA_MODE_BOSS_CHASE_EXWIDE
                ;
            bool isBossOnly = camera.mode == CAMERA_MODE_BOSS_CHASE;

            bool isBossAndZako =
                camera.mode == CAMERA_MODE_BOSSANDZAKO_CHASE
                ;
            bool isBossWide =
                camera.mode == CAMERA_MODE_BOSS_CHASE_WIDE
                ;
            bool isBossWideMiddle =
                camera.mode == CAMERA_MODE_BOSS_CHASE_WIDEMIDDLE
                ;
            bool isBossWideEx =
                camera.mode == CAMERA_MODE_BOSS_CHASE_EXWIDE
                ;

            Int2 stageSize = {};
            GAME_OBJECT* bossObj = nullptr;
            {
                Stage_Base* stg = GetCurrentStageSceneGame();
                if (stg != nullptr)
                {
                    stageSize = stg->GetStageSize();
                    if (isBoss)
                    {
                        bossObj = stg->GetBossObj();
                    }
                }
            }

            PLAYER* pl = GetPlayer(i);
            if (pl != nullptr)
            {
                GAME_OBJECT* plOb = pl->obj;
                if (plOb != nullptr)
                {
                    CAMERA_PLAYER_CHASE& cpc = camera.playerChase;

                    float spd = (abs(plOb->vel.x) + abs(plOb->vel.y) + abs(plOb->vel.z)) / 3;
                    bool iskako = false;
                    bool isMoveApi = false;

                    float mpo = 1.0f / 20.0f * changeLerpMod;
                    float mpota = 1.0f / 8.0f * changeLerpMod;
                    //float mpo = 1.0f / 28.0f;
                    //float mpota = 1.0f / 10.0f;
                    float mz = 1.0f / 60.0f * changeLerpMod;

                    Float3 cpostmp = {};
                    Float3 ctapostmp = {};
                    float czoom = CAMERA_PLAYER_CHASE_ZOOM;

                    Float3 plbeforeVec = {};
                    //float plbeforeVecSpd = 0;

                    GOE_Character_Player* goe =
                        dynamic_cast<GOE_Character_Player*>(plOb->events.source);  // ダウンキャスト
                    if (goe != nullptr)
                    { 
                        plbeforeVec = NormalizeVector(goe->GetBeforeVecBuf());
                        //plbeforeVecSpd = (abs(plbeforeVec.x) + abs(plbeforeVec.y) + abs(plbeforeVec.z)) / 3;

                        GOE_Character_Player::KAKOPON_SET kakomain = goe->GetKakoMain();

                        isMoveApi = (goe->GetCameraMoveActionFlag() > 0);
                        iskako = goe->IsDrawing() || kakomain.dat.IsSuccess 
                            || isMoveApi
                            ;

                        if (iskako)
                        //if (goe->IsDrawing())
                        {
                            Float3 plkakofpos = goe->GetKakoFirstPos();
                            Float2 po = CalculateCentroid(Float3ToFloat2(plOb->pos), Float3ToFloat2(plkakofpos));
                            float dis = CalculateDistance(plOb->pos, plkakofpos);

                            Float2 pfvec = {};
                            pfvec.x = plOb->pos.x - po.x;
                            pfvec.y = plOb->pos.y - po.y;
                            pfvec = NormalizeVector(pfvec);

                            cpostmp.x = po.x + pfvec.x * dis / 2;
                            cpostmp.y = po.y + pfvec.y * dis / 2;
                            cpostmp.z = 0.0f;

                            //ctapostmp = Float2ToFloat3(po);
                            //ctapostmp = cpostmp;

                            //czoom *= 0.75f;
                            //czoom *= 0.85f;
                            czoom *= 0.95f;

                            mpo *= 0.8f;

                            //cpostmp.y += 1000.0f;
                            //cpostmp.z += -1000.0f;
                            //ctapostmp.y += 50000.0f;
                            //ctapostmp.y += 10000.0f;
                            ctapostmp.z += 80.0f;

                            //mpo *= 0.08f + (dis * 1.2f);
                        }
                        else
                        {
                            cpostmp.x = plOb->pos.x;
                            cpostmp.y = plOb->pos.y;
                            cpostmp.z = plOb->pos.z;

                            //ctapostmp = plOb->pos;
                            //ctapostmp = cpostmp;

                            //float dis = CalculateDistance(plOb->pos, cpostmp);

                            //mpo = 1.0f / std::max(80.0f + dis * 0.003f - spd * 1.36f, 10.0f);
                        }
                        cpostmp.z += 12000.0f;
                        ctapostmp.z += -21000.0f;

                        bool isMoving = spd > 12.0f || isMoveApi;
                        if (isMoving)
                        {
                            if (isBossOnly)
                            {
                                czoom *= 1.0f;
                            }
                            else
                            {
                                czoom *= 0.85f;
                            }
                        }
                        else
                        {
                            czoom *= 1.45f;
                        }

                        {
                            float spdmod = spd / 18.0f;

                            float maxi = 1.0f / 80.0f;

                            float kakovecmod = iskako ? 1.15f : 1.0f;
                            float kakoymod = 0.0f;
                            if (iskako)
                            {
                                if (isBossOnly)
                                {
                                    //kakoymod = 500.0f;
                                    kakoymod = 600.0f;
                                }
                                else
                                {
                                    kakoymod = 900.0f;
                                }
                            }
                            else
                            {
                                if (isBossOnly)
                                {
                                    //kakoymod = -500.0f;
                                    kakoymod = 900.0f;
                                }
                            }
                            kakoymod += 1200.0f * spdmod;

                            float kakozmod = 0.0f;
                            if (iskako)
                            {
                                if (isBossOnly)
                                {
                                    //kakozmod = -1000.0f;
                                }
                                else
                                {
                                    kakozmod = -4000.0f;
                                }
                            }

                            if (isBossOnly && !isMoving)
                            {
                                kakozmod += 3000.0f;
                            }

                            float ax = (float)((plbeforeVec.x 
                                * BLOCK_WIDTH * (3.0f + 8.0f * spdmod) * kakovecmod)
                                - cpc.cameraPosPlayerAxisAdd.x);
                            float ay = (float)((plbeforeVec.y 
                                * BLOCK_WIDTH * (3.0f + 8.0f * spdmod) * kakovecmod + kakoymod)
                                - cpc.cameraPosPlayerAxisAdd.y);
                            float az = (float)((plbeforeVec.z
                                * BLOCK_WIDTH * (3.0f + 8.0f * spdmod) * kakovecmod + kakozmod)
                                - cpc.cameraPosPlayerAxisAdd.z);

                            cpc.cameraPosPlayerAxisAdd.x += (float)(ax * maxi);
                            cpc.cameraPosPlayerAxisAdd.y += (float)(ay * maxi);
                            cpc.cameraPosPlayerAxisAdd.z += (float)(az * maxi);
                        }

                        cpostmp.x += CAMERA_PLAYER_CHASE_POS_X + cpc.cameraPosPlayerAxisAdd.x;
                        cpostmp.y += CAMERA_PLAYER_CHASE_POS_Y + cpc.cameraPosPlayerAxisAdd.y;
                        cpostmp.z += CAMERA_PLAYER_CHASE_POS_Z + cpc.cameraPosPlayerAxisAdd.z;


                        if (stageSize.x > 0 && stageSize.y > 0)
                        {
                            float stageW = BLOCK_WIDTH 
                                * stageSize.x
                                - (SCREEN_WIDTH * 1 * (1.0f / czoom) / 2);
                            float stageH = (BLOCK_HIGHT 
                                * stageSize.y + BLOCK_HIGHT * 12.0f)
                                - (SCREEN_HEIGHT * 1 * (1.0f / czoom) / 2);

                            if (cpostmp.x < stageW / -2)
                            {
                                cpostmp.x = stageW / -2;
                            }
                            if (cpostmp.x > stageW / 2)
                            {
                                cpostmp.x = stageW / 2;
                            }
                            if (cpostmp.y < stageH / -2)
                            {
                                cpostmp.y = stageH / -2;
                            }
                            if (cpostmp.y > stageH / 2)
                            {
                                cpostmp.y = stageH / 2;
                            }
                        }

                        //ctapostmp.x = cpostmp.x - CAMERA_PLAYER_CHASE_POS_X;
                        //ctapostmp.y = cpostmp.y - CAMERA_PLAYER_CHASE_POS_Y;
                        //ctapostmp.z = cpostmp.z - CAMERA_PLAYER_CHASE_POS_Z;
                        //ctapostmp.x = cpostmp.x * 0.75f;
                        //ctapostmp.y = cpostmp.y * 0.75f;
                        //ctapostmp.z = cpostmp.z * 0.75f;
                        //ctapostmp.x = cpostmp.x;
                        //ctapostmp.y = cpostmp.y - 10;
                        //ctapostmp.z = cpostmp.z + 10;
                        ctapostmp.x += cpostmp.x;
                        ctapostmp.y += cpostmp.y - 10;
                        ctapostmp.z += cpostmp.z + 10;

                        if (isBoss)
                        {
                            if (bossObj != nullptr)
                            {
                                ctapostmp = bossObj->pos;
                                ctapostmp.z = 0.0f;

                                ctapostmp.x += + cpc.cameraPosPlayerAxisAdd.x;
                                ctapostmp.y += + cpc.cameraPosPlayerAxisAdd.y;
                                ctapostmp.z += + cpc.cameraPosPlayerAxisAdd.z;
                            }
                            //cpostmp.z += 18000.0f;
                            //ctapostmp.y += 9000.0f;
                            //cpostmp.z += 16000.0f;
                            //ctapostmp.y += 8000.0f;

                            Float2 po = Float3ToFloat2(cpostmp);
                            float dis = 0;

                            Float2 pfvec = {};

                            if (bossObj != nullptr)
                            {
                                po = CalculateCentroid(
                                    Float3ToFloat2(cpostmp),
                                    Float3ToFloat2(bossObj->pos)
                                );
                                dis = CalculateDistance(cpostmp, bossObj->pos);

                                pfvec.x = po.x - cpostmp.x;
                                pfvec.y = po.y - cpostmp.y;
                                pfvec = NormalizeVector(pfvec);
                            }

                            if (isBossOnly && bossObj != nullptr)
                            {
                                cpostmp.x += pfvec.x * dis / 30;
                                cpostmp.y += pfvec.y * dis / 30;
                                //cpostmp.z += -2600.0f;
                                //cpostmp.z += 5000.0f;
                                cpostmp.z += 2000.0f;

                                mpo *= 0.85f;

                                //czoom *= 1.35f;
                                //czoom *= 0.9f;
                                //czoom *= 0.95f;
                                czoom *= 0.92f;

                                //ctapostmp.z += -17000.0f;
                                //ctapostmp.z += -18000.0f;
                                ctapostmp.z += -20000.0f;
                            }

                            if (isBossAndZako)
                            {
                                cpostmp.y += 500.0f;
                                cpostmp.z += -8000.0f;

                                ctapostmp.z += -25000.0f;
                                //czoom *= 0.7f;
                                czoom *= 1.1f;

                                mpo *= 1.25f;
                            }

                            if (isBossWide)
                            {
                                //////cpostmp.y += -2400.0f;
                                ////cpostmp.y += -1000.0f;
                                //////cpostmp.z += -8000.0f;
                                ////cpostmp.z += 2000.0f;

                                //cpostmp.y += -1500.0f;
                                //cpostmp.z += 2000.0f;

                                ////ctapostmp.y += -2400.0f;
                                ////ctapostmp.y += -1000.0f;
                                ////ctapostmp.z += -17000.0f;
                                //ctapostmp.y += -1500.0f;
                                //ctapostmp.z += -4000.0f;
                                ////czoom *= 0.52f;
                                //czoom *= 0.45f;
                                //if (isMoving)
                                //{
                                //    czoom *= 1.3f;
                                //}

                                ////mpo *= 1.25f;
                                ////mpo *= 0.85f;
                                //mpo *= 0.8f;

                                //cpostmp.x += pfvec.x * dis / 30;
                                //cpostmp.y += pfvec.y * dis / 30;
                                cpostmp.z += 2000.0f;

                                //cpostmp.y += -2800.0f;

                                Float2 addpvec = NormalizeVector(MkF2(
                                    plOb->pos.x,
                                    plOb->pos.y
                                ));
                                cpostmp.x += addpvec.x * 300.0f;
                                cpostmp.y += addpvec.y * 600.0f;
                                cpostmp.y += -1600.0f;

                                mpo *= 0.7f;
                                mpota *= 0.18f;

                                //czoom *= 0.4f;
                                czoom *= 0.35f;
                                if (isMoving)
                                {
                                    czoom *= 1.4f;
                                }

                                ctapostmp.z += -100.0f;
                            }

                            if (isBossWideMiddle)
                            {
                                ////Float2 po = CalculateCentroid(
                                ////    Float3ToFloat2(cpostmp),
                                ////    Float3ToFloat2(bossObj->pos)
                                ////);
                                ////float dis = CalculateDistance(cpostmp, bossObj->pos);

                                ////Float2 pfvec = {};
                                ////pfvec.x = po.x - cpostmp.x;
                                ////pfvec.y = po.y - cpostmp.y;
                                ////pfvec = NormalizeVector(pfvec);

                                ////cpostmp.x += pfvec.x * dis / 30;
                                ////cpostmp.y += pfvec.y * dis / 30;

                                ////cpostmp.z += 2000.0f;

                                ////mpo *= 0.85f;

                                ////czoom *= 0.7f;

                                ////ctapostmp.z += -15000.0f;

                                ////cpostmp.y += 500.0f;
                                //cpostmp.y += -1000.0f;
                                ////cpostmp.z += -8000.0f;
                                //cpostmp.z += 2000.0f;

                                //cpostmp.x += pfvec.x * dis / 30;
                                //cpostmp.y += pfvec.y * dis / 30;
                                //cpostmp.y += -100.0f;
                                ////cpostmp.z += -2600.0f;
                                ////cpostmp.z += 5000.0f;
                                //cpostmp.z += 2000.0f;

                                ////ctapostmp.y += -2400.0f;
                                ////ctapostmp.y += -1000.0f;
                                ////ctapostmp.z += -29000.0f;
                                //ctapostmp.y += -100.0f;
                                //ctapostmp.z += -16000.0f;
                                ////czoom *= 0.7f;
                                //czoom *= 0.55f;
                                //if (isMoving)
                                //{
                                //    czoom *= 1.3f;
                                //}

                                ////mpo *= 1.25f;
                                ////mpo *= 0.6f;
                                //mpo *= 0.9f;

                                //cpostmp.x += pfvec.x * dis / 30;
                                //cpostmp.y += pfvec.y * dis / 30;
                                cpostmp.z += 2000.0f;

                                cpostmp.y += -1200.0f;

                                //mpo *= 0.75f;
                                mpo *= 0.65f;
                                mpota *= 0.15f;

                                czoom *= 0.48f;
                                if (isMoving)
                                {
                                    czoom *= 1.4f;
                                }

                                ctapostmp.z += -22000.0f;
                            }

                            if (isBossWideEx)
                            {
                                cpostmp.z += 2000.0f;

                                Float2 addpvec = NormalizeVector(MkF2(
                                    plOb->pos.x,
                                    plOb->pos.y
                                ));
                                cpostmp.x += addpvec.x * 300.0f;
                                cpostmp.y += addpvec.y * 600.0f;
                                cpostmp.y += -1600.0f;

                                cpostmp.x *= 0.8f;
                                cpostmp.y *= 0.8f;
                                ctapostmp.x *= 0.75f;
                                ctapostmp.y *= 0.75f;

                                //mpo *= 0.7f;
                                //mpota *= 0.18f;
                                //mpo *= 1.75f;
                                //mpota *= 0.75f;
                                mz *= 1.5f;
                                mpo *= 1.0f;
                                mpota *= 0.5f;

                                czoom *= 0.275f;
                                if (isMoving)
                                {
                                    czoom *= 1.4f;
                                }

                                ctapostmp.z += -100.0f;
                            }
                        }
                    }

                    //camera.trans.target = plOb->pos;

                    {
                        float ax = (float)(ctapostmp.x - cpc.cameraTargetBuf.x);
                        float ay = (float)(ctapostmp.y - cpc.cameraTargetBuf.y);
                        float az = (float)(ctapostmp.z - cpc.cameraTargetBuf.z);

                        cpc.cameraTargetBuf.x += (float)(ax * mpota);
                        cpc.cameraTargetBuf.y += (float)(ay * mpota);
                        cpc.cameraTargetBuf.z += (float)(az * mpota);

                        //camera.trans.target.x = ctapostmp.x;
                        //camera.trans.target.y = ctapostmp.y;
                        //camera.trans.target.z = ctapostmp.z;

                        //// 0よりしたまでついていかない
                        //if (camera.trans.target.y > 0)
                        //{
                        //    camera.trans.target.y = 0;
                        //}
                    }

                    {
                        float ax = (float)((cpostmp.x) - cpc.cameraPosBuf.x);
                        float ay = (float)((cpostmp.y) - cpc.cameraPosBuf.y);
                        float az = (float)((cpostmp.z) - cpc.cameraPosBuf.z);

                        cpc.cameraPosBuf.x += (float)(ax * mpo);
                        cpc.cameraPosBuf.y += (float)(ay * mpo);
                        cpc.cameraPosBuf.z += (float)(az * mpo);

                        //// 0よりしたまでついていかない
                        //if (camera.trans.pos.y > 0)
                        //{
                        //    camera.trans.pos.y = 0;
                        //}

                        if (isenable)
                        {
                            float addzoom = (float)(czoom - camera.trans.zoom) * mz;
                            camera.trans.zoom += addzoom * (addzoom > 0 ? 0.3f : 1.0f);
                            //cameraTransZoom = EasingOutQuart( cameraTransZoom +  addzoom* (addzoom > 0 ? 0.16f : 1.0f));
                        }
                    }


                    cpc.playerBeforeSpd = spd;
                    //cpc.playerBeforeVecSpd = plbeforeVecSpd;
                    cpc.playerBeforeVec = plbeforeVec;

                    if (isenable)
                    {
                        if (isInstant)
                        {
                            camera.trans.pos = cpostmp;
                        }
                        else
                        {
                            float dis = CalculateDistance(
                                Float3ToFloat2(camera.trans.pos),
                                Float3ToFloat2(cpc.cameraPosBuf)
                            );
                            //float dis = CalculateDistance(cameraTransPos, cpc.cameraPosBuf);

                            if (cpc.cameraPosBufModdingFlag)
                            {
                                float ax = (float)(cpc.cameraPosBuf.x - (camera.trans.pos.x));
                                float ay = (float)(cpc.cameraPosBuf.y - (camera.trans.pos.y));
                                float az = (float)(cpc.cameraPosBuf.z - (camera.trans.pos.z));

                                camera.trans.pos.x += (float)(ax * mpo);
                                camera.trans.pos.y += (float)(ay * mpo);
                                camera.trans.pos.z += (float)(az * mpo);

                                if (dis <= 100.0f)
                                {
                                    cpc.cameraPosBufModdingFlag = false;
                                }
                            }
                            else
                            {
                                if (dis > 1000.0f)
                                {
                                    cpc.cameraPosBufModdingFlag = true;
                                }
                                else
                                {
                                    float mpo2 = 1.0f / 4;
                                    //cameraTransPos = cpc.cameraPosBuf;
                                    float ax = (float)(cpc.cameraPosBuf.x - (camera.trans.pos.x));
                                    float ay = (float)(cpc.cameraPosBuf.y - (camera.trans.pos.y));
                                    float az = (float)(cpc.cameraPosBuf.z - (camera.trans.pos.z));

                                    camera.trans.pos.x += (float)(ax * mpo2);
                                    camera.trans.pos.y += (float)(ay * mpo2);
                                    camera.trans.pos.z += (float)(az * mpo2);
                                }
                            }
                        }
                    }


                    if (isenable)
                    {
                        if (isInstant)
                        {
                            camera.trans.target = ctapostmp;
                        }
                        else
                        {
                            float dis = CalculateDistance(
                                Float3ToFloat2(camera.trans.target),
                                Float3ToFloat2(cpc.cameraTargetBuf)
                            );
                            //float dis = CalculateDistance(camera.trans.target, cpc.cameraTargetBuf);

                            if (cpc.cameraTargetBufModdingFlag)
                            {
                                float ax = (float)(cpc.cameraTargetBuf.x - (camera.trans.target.x));
                                float ay = (float)(cpc.cameraTargetBuf.y - (camera.trans.target.y));
                                float az = (float)(cpc.cameraTargetBuf.z - (camera.trans.target.z));

                                camera.trans.target.x += (float)(ax * mpo);
                                camera.trans.target.y += (float)(ay * mpo);
                                camera.trans.target.z += (float)(az * mpo);

                                if (dis <= 100.0f)
                                {
                                    cpc.cameraTargetBufModdingFlag = false;
                                }
                            }
                            else
                            {
                                if (dis > 1000.0f)
                                {
                                    cpc.cameraTargetBufModdingFlag = true;
                                }
                                else
                                {
                                    float mpo2 = 1.0f / 4;
                                    //camera.trans.target = cpc.cameraTargetBuf;
                                    float ax = (float)(cpc.cameraTargetBuf.x - (camera.trans.target.x));
                                    float ay = (float)(cpc.cameraTargetBuf.y - (camera.trans.target.y));
                                    float az = (float)(cpc.cameraTargetBuf.z - (camera.trans.target.z));

                                    camera.trans.target.x += (float)(ax * mpo2);
                                    camera.trans.target.y += (float)(ay * mpo2);
                                    camera.trans.target.z += (float)(az * mpo2);
                                }
                            }
                        }
                    }


                    cpc.cameraPosProvision = camera.trans.pos;
                    cpc.cameraTargetProvision = camera.trans.target;
                    cpc.cameraZoomProvision = camera.trans.zoom;

                    //if (isenable)
                    //{
                    //    camera.trans.pos = cpc.cameraPosProvision;
                    //    camera.trans.target = cpc.cameraTargetProvision;
                    //    camera.trans.zoom = cpc.cameraZoomProvision;
                    //}
                }    
            }
        }


        update_moveRequ(camera.moveRequ, &camera.trans.pos, &camera.trans.rot, &camera.trans.scale, &camera.trans.zoom);
        update_moveRequ(camera.moveRequTarget, &camera.trans.target, nullptr, nullptr, nullptr);

        if (camera.targetRef != nullptr)
        {
            camera.trans.target = (*camera.targetRef);
        }

        //ふらりく処理
        if (camera.flashRequ.frame > 0)
        {
            camera.flashRequ.frame--;
        }

        {
            CAMERA_GAME_OBJECT_ZOOM_REQUEST* gr = &camera.goZoomRequ;

            if (gr->zoom > 0.0f)
            {
                GAME_OBJECT* obj = GetGameObject(gr->objIndex);
                if (IsActiveGameObject(obj) && obj->createFrame == gr->objCreateFrame)
                {
                    gr->pos = obj->pos;
                }
            }

            if (gr->isUse)
            {
                if (gr->zoom < 1.0f)
                {
                    gr->zoom += (gr->speed > 0 ? gr->speed : 0.1f);
                    if (gr->zoom > 1.0f)
                    {
                        gr->zoom = 1.0f;
                    }
                }
            }
            else
            {
                if (gr->zoom > 0)
                {
                    gr->zoom -= (gr->speed > 0 ? gr->speed : 0.1f);
                    if (gr->zoom < 0)
                    {
                        gr->zoom = 0.0f;
                    }
                }
            }
        }

        if (camera.goZoomDelayRequ.frame > 0)
        {
            camera.goZoomDelayRequ.frame--;
            if (camera.goZoomDelayRequ.frame <= 0)
            {
                CameraZoomResetRequest(i, camera.goZoomDelayRequ.dat.speed, camera.goZoomDelayRequ.dat.easing);
            }
        }

        {
            CAMERA_REVERSE_REQUEST* rr = &camera.revRequ;

            if (rr->isUse)
            {
                if (rr->mod < 1.0f)
                {
                    rr->mod += (rr->speed > 0 ? rr->speed : 0.1f);
                    if (rr->mod > 1.0f)
                    {
                        rr->mod = 1.0f;
                    }
                }
            }
            else
            {
                if (rr->mod > 0)
                {
                    rr->mod -= (rr->speed > 0 ? rr->speed : 0.1f);
                    if (rr->mod < 0)
                    {
                        rr->mod = 0.0f;
                    }
                }
            }
        }

        //モード変更リクエスト
        if (camera.modeSetRequ.frame > 0)
        {
            camera.modeSetRequ.frame--;
            if (camera.modeSetRequ.frame <= 0)
            {
                //camera.mode = camera.modeSetRequ.mode;
                SetCameraMode(i, camera.modeSetRequ.mode);
            }
        }

        {
            {
                double sptmp = (double)camera.shakeFrame / (camera.shakeFrameMax < 1.0f ? 1.0f : camera.shakeFrameMax);
                double xtmp = sin(7.6f * (double)camera.shakeFrame) * sptmp * camera.shakePower;
                double ytmp = cos(5.6f * (double)camera.shakeFrame) * sptmp * camera.shakePowerY;

                camera.cameraDrawDat.shakePos.x = (float)xtmp;
                camera.cameraDrawDat.shakePos.y = (float)ytmp;
            }

            {
                Float2 postmp = MkF2(camera.trans.pos.x, camera.trans.pos.y);
                float zoomtmp = camera.trans.zoom;

                CAMERA_GAME_OBJECT_ZOOM_REQUEST* gr = &camera.goZoomRequ;
                if (gr->zoom > 0)
                {
                    Float2 objpostmp = MkF2(gr->pos.x, gr->pos.y);
                    float endscaletmp = gr->zoomScale;

                    float modtmp = gr->zoom;
                    if (gr->easing != nullptr)
                    {
                        modtmp = gr->easing(modtmp);
                    }

                    //postmp.x = objpostmp.x * modtmp + postmp.x * (1.0f - modtmp);
                    //postmp.y = objpostmp.y * modtmp + postmp.y * (1.0f - modtmp);
                    //scaletmp.x = endscaletmp.x * modtmp + scaletmp.x * (1.0f - modtmp);
                    //scaletmp.y = endscaletmp.y * modtmp + scaletmp.y * (1.0f - modtmp);

                    camera.cameraDrawDat.zoomAddPos.x = (objpostmp.x - postmp.x) * modtmp;
                    camera.cameraDrawDat.zoomAddPos.y = (objpostmp.y - postmp.y) * modtmp;
                    camera.cameraDrawDat.zoomAddScale = (endscaletmp - zoomtmp) * modtmp;

                    //DebugPrintf("aaa %f", modtmp);
                }
            }

            {
                CAMERA_REVERSE_REQUEST* rr = &camera.revRequ;
                if (rr->mod > 0)
                {
                    float modtmp = rr->mod;
                    if (rr->easing != nullptr)
                    {
                        modtmp = rr->easing(modtmp);
                    }
                    camera.cameraDrawDat.revScaleY = (1.0f - modtmp) * 2.0f - 1.0f;
                }
            }
        }

        //深度設定を保持
        bool deSave = DXDepthStencilStateDepthEnable();
        if (deSave)
        {
            Float3 vec = MkF3(1.0f, 1.0f, 1.0f);
            {
                vec.x = cinfo.CameraScalingRaw.x / CAMERA_3D_MOD;
                vec.y = cinfo.CameraScalingRaw.y / CAMERA_3D_MOD;
                vec.z = cinfo.CameraScalingRaw.z / CAMERA_3D_MOD;
                //vec = NormalizeVector(vec);
                //vec = vec * 1;
            }

            Float3 posTargettmp = MkF3(
                camera.trans.target.x * vec.x,
                camera.trans.target.y * vec.y,
                camera.trans.target.z * vec.z
            );
            //Float3 posTargettmp = MkF3(
            //    camera.trans.target.x * vec.x,
            //    camera.trans.target.y * vec.y,
            //    camera.trans.target.z * vec.z
            //);

            Float3 postmp = MkF3(
                (camera.trans.pos.x) * vec.x,
                (camera.trans.pos.y) * vec.y,
                (camera.trans.pos.z) * vec.z
            );
            //Float3 postmp = MkF3(
            //    (camera.trans.pos.x) * vec.x,
            //    (camera.trans.pos.y) * vec.y,
            //    (camera.trans.pos.z) * vec.z
            //);
            Float3 scaletmp = MkF3(camera.trans.scale.x, camera.trans.scale.y, 1.0f);
            float zoomtmp = camera.trans.zoom;

            DX_SetCameraInfo(
                i,
                camera.camera3D.fov,
                {
                    camera.camera3D.pos.x + postmp.x,
                    camera.camera3D.pos.y + postmp.y,
                    camera.camera3D.pos.z + postmp.z
                },
                {
                    camera.camera3D.target.x + posTargettmp.x,
                    camera.camera3D.target.y + posTargettmp.y,
                    camera.camera3D.target.z + posTargettmp.z
                },
                {
                    camera.camera3D.up.x,
                    camera.camera3D.up.y,
                    camera.camera3D.up.z
                },
                camera.camera3D.scale * (camera.trans.zoom + camera.cameraDrawDat.zoomAddScale)
            );
            //DebugPrintf("%f\n", camera.trans.zoom);

            //float x = -camera.trans.pos.x * camera.trans.scale.x * camera.trans.zoom * 0.024f;
            //float y = +camera.trans.pos.y * camera.trans.scale.y * camera.trans.zoom * 0.024f;
            //DX_SetCameraInfo(
            //	camera.camera3D.fov,
            //	{
            //		camera.camera3D.pos.x + x,
            //		camera.camera3D.pos.y + y,
            //		camera.camera3D.pos.z
            //	},
            //	{
            //		camera.camera3D.target.x + x,
            //		camera.camera3D.target.y + y,
            //		camera.camera3D.target.z
            //	},
            //	{
            //		camera.camera3D.up.x,
            //		camera.camera3D.up.y,
            //		camera.camera3D.up.z
            //	},
            //	camera.camera3D.scale * (1.0f / camera.trans.zoom)
            //);
        }
    }

	//テスト
	/*if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_LEFT))
	{
		CameraShakeRequest(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_POWER);
		CameraMoveRequest(
			60,
			MkF2(0.0f, 0.0f),
			0.0f,
			MkF2(1.0f, 1.0f),
			1.0f,
			EasingInOutCubic,
			CAMERA_MOVE_TYPE_SMOOTH
		);
	}
	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_UP))
	{
		CameraShakeRequest(CAMERA_SHAKE_MIDIUM_FRAME, CAMERA_SHAKE_MIDIUM_POWER);
	}
	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_RIGHT))
	{
		CameraShakeRequest(CAMERA_SHAKE_LARGE_FRAME, CAMERA_SHAKE_LARGE_POWER);
		CameraMoveRequest(
			60,
			MkF2(0.0f, 0.0f),
			0.0f,
			MkF2(1.0f, 1.0f),
			1.0f,
			EasingInOutCubic,
			CAMERA_MOVE_TYPE_FADE
		);
	}
	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_DOWN))
	{
		CameraShakeRequest(CAMERA_SHAKE_EXLARGE_FRAME, CAMERA_SHAKE_EXLARGE_POWER);
	}

	if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_MINUS))
	{
		if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_LEFT))
		{
			CameraMoveRequest(
				60,
				MkF2(0.0f, 0.0f),
				0.0f,
				MkF2(1.0f, 1.0f),
				1.0f,
				EasingInOutCubic,
				CAMERA_MOVE_TYPE_SMOOTH
			);
		}
		else if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_RIGHT))
		{
			CameraMoveRequest(
				60,
				MkF2(0.0f, 0.0f),
				0.0f,
				MkF2(1.0f, 1.0f),
				1.0f,
				EasingInOutCubic,
				CAMERA_MOVE_TYPE_FADE
			);
		}
		else
		{
			CameraMoveRequest(
				60,
				MkF2(0.0f, 0.0f),
				0.0f,
				MkF2(1.0f, 1.0f),
				1.0f,
				EasingInOutCubic,
				CAMERA_MOVE_TYPE_NORMAL
			);
		}
	}

	Float2 slickR = GetControllerMainLeftStick();
	if (slickR.x != 0.0f || slickR.y != 0.0f)
	{
		camera.trans.pos.x += slickR.x * 12.0f * (1.0f / camera.trans.zoom);
		camera.trans.pos.y += -slickR.y * 12.0f * (1.0f / camera.trans.zoom);
	}

	if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_L))
	{
		camera.trans.rot += 0.5f;
	}
	if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_R))
	{
		camera.trans.rot -= 0.5f;
	}

	if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_ZL))
	{
		camera.trans.zoom += 0.02f;
	}
	if (GetControllerMainPress(CONTROLLER_BUTTON_TYPE_ZR))
	{
		camera.trans.zoom -= 0.02f;
	}*/
}

// ===================================================
// カメラの終了処理
// ===================================================
void UninitCamera(void)
{
}

// ===================================================
// カメラの描画
// ===================================================
void DrawCamera(void)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CAMERA& camera = g_Camera[i];
        if (camera.flashRequ.frame > 0)
        {
            //Float4 color = camera.flashRequ.color;
            POLYGON_RELEASE_OPTION opt = camera.flashRequ.opt;

            float alpha = (float)camera.flashRequ.frame / camera.flashRequ.frameMax;
            if (camera.flashRequ.easing != nullptr)
            {
                opt.color.w *= camera.flashRequ.easing(alpha);
            }
            else
            {
                opt.color.w *= alpha;
            }

            //DRAW_SPRITE_BUFFER dsb = {};
            //SpriteSetUp(&dsb);
            //SpriteShape(&dsb,
            //    MkF2(-1.0f, -1.0f),
            //    0.0f,
            //    MkF2((float)SCREEN_WIDTH + 2.0f, (float)SCREEN_HEIGHT + 2.0f)
            //);

            //SpriteReleaseEx(&dsb, opt);
            {
                DRAW_POLYGON_DAT dsdat = {};
                dsdat.pos.x = -1.0f;
                dsdat.pos.y = -1.0f;
                dsdat.pos.z = 0.0f;

                dsdat.size.x = (float)SCREEN_WIDTH + 2.0f;
                dsdat.size.y = (float)SCREEN_HEIGHT + 2.0f;
                dsdat.size.z = 1.0f;

                dsdat.rot = 0.0f;

                dsdat.color = opt.color;

                dsdat.texNo = opt.texNo;
                dsdat.loadTexType = opt.loadTexType;

                dsdat.texUV = opt.texUV;
                dsdat.texWH = opt.texWH;

                dsdat.startPos = {
                    0.0f,
                    0.0f
                };
                dsdat.startRot = 0;
                dsdat.startScale = { 1.0f, 1.0f };
                dsdat.startScale3D = { 1.0f, 1.0f, 1.0f };

                dsdat.shaderOpt = opt.shaderOpt;

                DRAW_POLYGON_IN_CAMERA_EX_OPTION option = {};
                option.reverse = false;
                option.fixed = opt.fixed;
                option.depthBuffIgnore = opt.depthBuffIgnore;
                option.priority = opt.priority;
                option.shaderType = opt.shaderType;

                DrawPolygonInCameraSingle(i, dsdat, option);
            }
        }
    }
}

void DrawCameraBufferRelease()
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        //g_CameraDrawBuf[i].clear();
        //g_CameraDrawBuf[i].shrink_to_fit();
        //g_CameraDrawBuf[i].reserve(512);

        //if (i == g_CameraCurrentIndex)
        //{
        //    //for (int j = 0; j < g_CameraDrawBuf[i].size(); j++)
        //    //{
        //    //    PolygonRelease(
        //    //        &g_CameraDrawBuf[i][j].dpb,
        //    //        g_CameraDrawBuf[i][j].pro
        //    //    );
        //    //}
        //    for (int j = 0; j < CAMERA_DRAW_BUF_MAX; j++)
        //    {
        //        if (g_CameraDrawBuf[i][j].enable)
        //        {
        //            PolygonRelease(
        //                &g_CameraDrawBuf[i][j].dpb,
        //                g_CameraDrawBuf[i][j].pro,
        //                true
        //            );
        //        }
        //    }
        //}
        DrawCameraBufferDrawOnly();
        DrawCameraBufferReleaseOnly();

        //for (int j = 0; j < CAMERA_DRAW_BUF_MAX; j++)
        //{
        //    g_CameraDrawBuf[i][j].enable = false;
        //}
        //g_CameraDrawBuf[i].clear();
    }
}
void DrawCameraBufferDrawOnly()
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        if (i == g_CameraCurrentIndex)
        {
            for (int j = 0; j < CAMERA_DRAW_BUF_MAX; j++)
            {
                if (g_CameraDrawBuf[i][j].enable)
                {
                    PolygonRelease(
                        &g_CameraDrawBuf[i][j].dpb,
                        g_CameraDrawBuf[i][j].pro,
                        true
                    );
                }
            }
        }
    }
}
void DrawCameraBufferReleaseOnly()
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        for (int j = 0; j < CAMERA_DRAW_BUF_MAX; j++)
        {
            g_CameraDrawBuf[i][j].enable = false;
        }
        //g_CameraDrawBuf[i].clear();
    }
}


void DrawSpriteInCameraSingle(
    int index,
    float x, float y, float w, float h,
    float rot,
    Float4 color,
    unsigned int texNo,
    Float2 texUV, Float2 texWH,
    DRAW_POLYGON_PRIORITY priority,
    DPD_SHADER_TYPE shadertype,
    DPD_SHADER_TYPE_EX_OPT shadertypeExOpt
)
{
    DRAW_POLYGON_DAT drawDatTmp = {};
    drawDatTmp.pos = MkF3(x, y, 0);
    drawDatTmp.size = MkF3(w, h, 1.0f);

    drawDatTmp.rot = rot;

    drawDatTmp.color = color;

    drawDatTmp.texNo = texNo;
    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

    drawDatTmp.texUV = texUV;
    drawDatTmp.texWH = texWH;
    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    opt.reverse = false;
    opt.fixed = true;
    opt.enableShade = false;
    opt.priority = priority;
    opt.shaderType = shadertype;
    opt.shaderTypeEx = shadertypeExOpt;
    DrawPolygonInCameraSingle(index, drawDatTmp, opt);
}
void DrawSpriteInCamera(
    float x, float y, float w, float h,
    float rot,
    Float4 color,
    unsigned int texNo,
    Float2 texUV, Float2 texWH,
    DRAW_POLYGON_PRIORITY priority,
    DPD_SHADER_TYPE shadertype,
    DPD_SHADER_TYPE_EX_OPT shadertypeExOpt
)
{
    DRAW_POLYGON_DAT drawDatTmp = {};
    drawDatTmp.pos = MkF3(x, y, 0);
    drawDatTmp.size = MkF3(w, h, 1.0f);

    drawDatTmp.rot = rot;

    drawDatTmp.color = color;

    drawDatTmp.texNo = texNo;
    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

    drawDatTmp.texUV = texUV;
    drawDatTmp.texWH = texWH;
    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    opt.reverse = false;
    opt.fixed = true;
    opt.enableShade = false;
    opt.priority = priority;
    opt.shaderType = shadertype;
    opt.shaderTypeEx = shadertypeExOpt;
    DrawPolygonInCameraEx(drawDatTmp, opt);
}

void DrawSpriteInCamera(DRAW_SPRITE_DAT drawDat,
	bool reverse, bool fixed, bool enableShade
)
{
	DRAW_POLYGON_DAT drawDatTmp = {};
	drawDatTmp.pos = MkF3(drawDat.x, drawDat.y, drawDat.z);
	drawDatTmp.size = MkF3(drawDat.w, drawDat.h, 1.0f);

	drawDatTmp.rot = drawDat.rot;
	drawDatTmp.rot3D = drawDat.rot3D;

	drawDatTmp.color = drawDat.color;

	drawDatTmp.texNo = drawDat.texNo;
	drawDatTmp.loadTexType = drawDat.loadTexType;

	drawDatTmp.texUV = drawDat.texUV;
	drawDatTmp.texWH = drawDat.texWH;

	drawDatTmp.startPos = drawDat.startPos;
	drawDatTmp.startPos3D = drawDat.startPos3D;

	drawDatTmp.startRot = drawDat.startRot;
	drawDatTmp.startRot3D = drawDat.startRot3D;

	drawDatTmp.startScale = drawDat.startScale;
	drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

	drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_QUAD;

    //drawDatTmp.model = nullptr;
    drawDatTmp.modelNo = 0;

	drawDatTmp.shaderOpt = drawDat.shaderOpt;

	DrawPolygonInCamera(drawDatTmp,
		reverse,
		fixed,
		enableShade
	);
}

void DrawPolygonInCamera(DRAW_POLYGON_DAT drawDat,
	bool reverse,
	bool fixed,
	bool enableShade
)
{
	DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
	opt.reverse = reverse;
	opt.fixed = fixed;
	opt.enableShade = enableShade;
    opt.priority = DRAW_POLYGON_PRIORITY_NORMAL;
	DrawPolygonInCameraEx(drawDat, opt );
}

XMMATRIX DrawPolygonInCameraCalMatrix(
    DRAW_POLYGON_BUFFER& dpb,
    POLYGON_RELEASE_OPTION& ropt,

    int index,
    DRAW_POLYGON_DAT drawDat,
    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt
)
{
    int ccount = index;

    CAMERA& camera = g_Camera[ccount];

    Float2 shakePos = MkF2(0.0f, 0.0f);

    if (!opt.fixed)
    {
        shakePos.x = camera.cameraDrawDat.shakePos.x;
        shakePos.y = camera.cameraDrawDat.shakePos.y;
    }

    //DRAW_POLYGON_BUFFER dpb = {};
    //if (drawDat.model != nullptr)
    //{
    //    PolygonSetUp(&dpb, drawDat.model);
    //}
    if (drawDat.modelNo != 0)
    {
        PolygonSetUp(&dpb, drawDat.modelNo);
    }
    else
    {
        PolygonSetUp(&dpb, drawDat.vertex);
    }

    dpb.cameraIndex = ccount;

    //dpb.posGameCamera.x = camera.trans.pos.x + camera.cameraDrawDat.zoomAddPos.x;
    //dpb.posGameCamera.y = camera.trans.pos.y + camera.cameraDrawDat.zoomAddPos.y;

    //dpb.posPivot = drawDat.startPos3D;
    //dpb.posPivot.x += drawDat.startPos.x;
    //dpb.posPivot.y += drawDat.startPos.y;

    //if (opt.billboard)
    //{
    //	DX_CAMERA_INFO cameraInfo = DX_GetCameraInfo();

    //	{
    //		Float3 vec = MkF3(
    //			cameraInfo.CameraVector.x,
    //			cameraInfo.CameraVector.y,
    //			cameraInfo.CameraVector.z
    //		);

    //		float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    //		float pitch = std::asin(vec.y / length) * 180.0f / SYS_GENERAL_PI;

    //		float yaw = std::atan2(vec.z, vec.x) * 180.0f / SYS_GENERAL_PI;

    ///*		PolygonShape(&dpb,
    //			MkF3(0.0f, 0.0f, 0.0f),
    //			MkF3(
    //				-pitch,
    //				yaw,
    //				0.0f
    //			),
    //			MkF3(1.0f, 1.0f, 1.0f)
    //		);*/
    //	}
    //	{
    //		Float3 vec = MkF3(
    //			(dpb.posPivot.x + drawDat.pos.x) - (camera.trans.pos.x + camera.camera3D.pos.x),
    //			(dpb.posPivot.y + drawDat.pos.y) - (camera.trans.pos.y + camera.camera3D.pos.y),
    //			(dpb.posPivot.z + drawDat.pos.z) - (camera.camera3D.pos.z)
    //		);
    //		//Float3 nv = NormalizeVector(MkF3(
    //		//	(dpb.posPivot.x + drawDat.pos.x) - (camera.trans.pos.x),
    //		//	(dpb.posPivot.y + drawDat.pos.y) - (camera.trans.pos.y)
    //		//));
    //		//camera.camera3D.scale

    //		//float rx = atan2(nv.z, nv.y) / SYS_GENERAL_PI;
    //		//float ry = atan2(nv.y, nv.x) / SYS_GENERAL_PI;

    //		//PolygonShape(&dpb,
    //		//	MkF3(0.0f, 0.0f, 0.0f),
    //		//	MkF3(
    //		//		90.0f,
    //		//		0.0f,
    //		//		90.0f
    //		//	),
    //		//	MkF3(1.0f, 1.0f, 1.0f)
    //		//);

    //		//PolygonShape(&dpb,
    //		//	MkF3(0.0f, 0.0f, 0.0f),
    //		//	MkF3(
    //		//		90.0f * -rx - 90.0f,
    //		//		90.0f * ry,
    //		//		0.0f
    //		//	),
    //		//	MkF3(1.0f, 1.0f, 1.0f)
    //		//);

    //		float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    //		float pitch = std::asin(vec.y / length) * 180.0f / SYS_GENERAL_PI;

    //		float yaw = std::atan2(vec.z, vec.x) * 180.0f / SYS_GENERAL_PI;

    //		//PolygonShape(&dpb,
    //		//	MkF3(0.0f, 0.0f, 0.0f),
    //		//	MkF3(
    //		//		//0.0f + 90.0f * -cameraInfo.CameraVector.y,
    //		//		//90.0f * cameraInfo.CameraVector.x,
    //		//		abs(yaw),
    //		//		-abs(pitch),
    //		//		0.0f
    //		//	),
    //		//	MkF3(1.0f, 1.0f, 1.0f)
    //		//);
    //	}
    //}

    //if (dpb.billboard)
    //{
    //	DX_CAMERA_INFO cameraInfo = DX_GetCameraInfo();

    //	XMMATRIX invView = {};
    //	invView = XMMatrixInverse(nullptr, cameraInfo.CameraView);

    //	invView.r[3].m128_f32[0] = 0.0f;
    //	invView.r[3].m128_f32[1] = 0.0f;
    //	invView.r[3].m128_f32[2] = 0.0f;

    //	dpb.pro *= invView;
    //}


    if (opt.reverse)
    {
        PolygonShape(&dpb,
            MkF3(0.0f, 0.0f, 0.0f),
            MkF3(0.0f, 0.0f, 0.0f),
            MkF3(-1.0f, 1.0f, 1.0f)
        );
    }

    //PolygonShape(&dpb,
    //	MkF3(drawDat.pos.x, drawDat.pos.y, 0.0f),
    //	MkF3(0.0f, 0.0f, drawDat.rot),
    //	MkF3(drawDat.size.x, drawDat.size.y, 1.0f)
    //);
    //PolygonShape(&dpb,
    //	MkF3(0.0f, 0.0f, drawDat.pos.z),
    //	drawDat.rot3D,
    //	MkF3(1.0f, 1.0f, drawDat.size.z)
    //);

    if (opt.billboard)
    {
        dpb.billboard = true;
    }

    PolygonShape(&dpb,
        MkF3(drawDat.pos.x, drawDat.pos.y, drawDat.pos.z),
        MkF3(drawDat.rot3D.x, drawDat.rot3D.y, drawDat.rot3D.z + drawDat.rot),
        MkF3(drawDat.size.x, drawDat.size.y, drawDat.size.z),
        true
    );

    //PolygonShape(&dpb,
    //	Float2ToFloat3(drawDat.startPos),
    //	MkF3(0.0f, 0.0f, drawDat.startRot),
    //	Float2ToFloat3(drawDat.startScale, 1.0f)
    //);
    //PolygonShape(&dpb,
    //	drawDat.startPos3D,
    //	drawDat.startRot3D,
    //	MkF3(1.0f, 1.0f, 1.0f)
    //);

    PolygonShape(&dpb,
        MkF3(drawDat.startPos.x + drawDat.startPos3D.x,
            drawDat.startPos.y + drawDat.startPos3D.y,
            drawDat.startPos3D.z
        ),
        MkF3(drawDat.startRot3D.x,
            drawDat.startRot3D.y,
            drawDat.startRot3D.z + drawDat.startRot
        ),
        MkF3(
            drawDat.startScale.x * drawDat.startScale3D.x,
            drawDat.startScale.y * drawDat.startScale3D.y,
            drawDat.startScale3D.z
        ),
        true
    );

    Float2 postmp = MkF2(camera.trans.pos.x, camera.trans.pos.y);
    Float3 scaletmp = MkF3(camera.trans.scale.x, camera.trans.scale.y, 1.0f);
    float zoomtmp = camera.trans.zoom;
    if (!opt.fixed)
    {
        {
            if (camera.goZoomRequ.zoom > 0)
            {
                postmp.x += camera.cameraDrawDat.zoomAddPos.x;
                postmp.y += camera.cameraDrawDat.zoomAddPos.y;
                zoomtmp += camera.cameraDrawDat.zoomAddScale;
            }
        }

        bool deSave = DXDepthStencilStateDepthEnable();

        PolygonShape(&dpb,
            MkF3(-postmp.x + shakePos.x, -postmp.y + shakePos.y, 0.0f),

            MkF3(0.0f, 0.0f, 0.0f),
            MkF3(1.0f, 1.0f, 1.0f)
        );
        if (!deSave)
        {
            PolygonShape(&dpb,
                MkF3(0.0f, 0.0f, 0.0f),
                MkF3(0.0f, 0.0f, 0.0f),
                MkF3(zoomtmp, zoomtmp, 1.0f)
            );
        }
        PolygonShape(&dpb,
            MkF3(0.0f, 0.0f, 0.0f),
            MkF3(0.0f, 0.0f, -camera.trans.rot),
            MkF3(scaletmp.x, scaletmp.y, scaletmp.z)
        );


        {
            if (camera.revRequ.mod > 0)
            {
                PolygonShape(&dpb,
                    MkF3(0.0f, 0.0f, 0.0f),
                    MkF3(0.0f, 0.0f, 0.0f),
                    MkF3(1.0f, camera.cameraDrawDat.revScaleY, 1.0f)
                );
            }
        }

        if (deSave)
        {
            //最終的に反転
            PolygonShape(&dpb,
                MkF3(0.0f, 0.0f, 0.0f),
                MkF3(0.0f, 0.0f, 0.0f),
                MkF3(1.0f, -1.0f, 1.0f)
            );
        }

    }


    //SpriteRelease(&dpb,
    //	drawDat.color,
    //	drawDat.texNo,
    //	drawDat.texUV,
    //	drawDat.texWH,
    //	drawDat.loadTexType,
    //	fixed
    //);

    ropt.color = drawDat.color;
    ropt.texNo = drawDat.texNo;
    ropt.texUV = drawDat.texUV;
    ropt.texWH = drawDat.texWH;
    ropt.loadTexType = drawDat.loadTexType;
    ropt.fixed = opt.fixed;
    ropt.cameraFix = opt.cameraFix;
    ropt.priority = opt.priority;
    ropt.depthBuffIgnore = opt.depthBuffIgnore;

    //ropt.enableShade = opt.enableShade;
    ropt.shaderType = opt.shaderType;
    ropt.shaderTypeEx = opt.shaderTypeEx;
    //ropt.billboard = opt.billboard;

    ropt.shaderOpt = drawDat.shaderOpt;

    ////フラッシュ中は無効化
    //if (camera.flashRequ.frame > 0)
    //{
    //    ropt.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NONE;
    //    ropt.shaderOpt.outline = DPD_SHADER_OPT_OUTLINE_NONE;
    //}

    //PolygonRelease(&dpb, ropt);

    XMMATRIX polMat = PolygonReleaseMat(&dpb, ropt);

    //DebugPrintf("%f %f", polMat.r[3].m128_f32[0], polMat.r[3].m128_f32[1]);

    return polMat;
}

CAMERA_DRAW_BUF DrawPolygonInCameraSingle(int index,
    DRAW_POLYGON_DAT drawDat,
    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt,
    DRAW_POLYGON_IN_CAMERA_EX_MAT_EX* polMatref
)
{
    DRAW_POLYGON_BUFFER dpb = {};

    POLYGON_RELEASE_OPTION ropt = {};

    XMMATRIX polMat;
    //if (polMatref != nullptr)
    //{
    //    dpb = polMatref->dpb;
    //    ropt = polMatref->ropt;
    //    polMat = polMatref->mat;
    //}
    //else
    {
        polMat = DrawPolygonInCameraCalMatrix(
            dpb,
            ropt,
            index,
            drawDat,
            opt
        );
    }

    if (polMatref != nullptr)
    {
        if (polMatref->isFixed)
        {
            polMat.r[3].m128_f32[0] = polMatref->offset.x;
            polMat.r[3].m128_f32[1] = polMatref->offset.y;
        }
        else
        {
            polMat.r[3].m128_f32[0] += polMatref->offset.x;
            polMat.r[3].m128_f32[1] += polMatref->offset.y;
        }
    }

    CAMERA_DRAW_BUF bufTmp = {};
    //if (
    //    polMat.r[3].m128_f32[0] < -SCREEN_WIDTH / 2 || polMat.r[3].m128_f32[0] > SCREEN_WIDTH / 2
    //    || polMat.r[3].m128_f32[1] < -SCREEN_HEIGHT / 2 || polMat.r[3].m128_f32[1] > SCREEN_HEIGHT / 2)
    //{
    //}
    //else
    {
        dpb.pro = polMat;

        {
            bufTmp.dpb = dpb;
            bufTmp.pro = ropt;
            CameraAddDrawBuf(index, bufTmp);
        }
    }
    return bufTmp;
}

void DrawPolygonInCameraEx(DRAW_POLYGON_DAT drawDat,
    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt
)
{
    for (int ccount = 0; ccount < CAMERA_NUM; ccount++)
    {
        DrawPolygonInCameraSingle(ccount,
            drawDat,
            opt
        );
    }
}



Float2 WorldPosToCameraPos(int index, Float3 wp)
{
    CAMERA& camera = g_Camera[index];
    DX_CAMERA_INFO cinfo = DX_GetCameraInfo(index);

	Float2 cposBase = {};
	////
	//////bool deSave = DXDepthStencilStateDepthEnable();

	//////if (deSave)
	//////{
	//////	DX_CAMERA_INFO dxCinfo = DX_GetCameraInfo();

	//////	cposBase.x = wp.x - camera.trans.pos.x - camera.cameraDrawDat.zoomAddPos.x;
	//////	cposBase.y = wp.y - camera.trans.pos.y - camera.cameraDrawDat.zoomAddPos.y;

	//////	cposBase.x *= (camera.trans.zoom * camera.trans.scale.x) + camera.cameraDrawDat.zoomAddScale;
	//////	cposBase.y *= (camera.trans.zoom * camera.trans.scale.y) + camera.cameraDrawDat.zoomAddScale;

	//////	Float3 cposBase2 = Float2ToFloat3(cposBase);
	//////	{
	//////		float rotRad = atan2(dxCinfo.CameraVector.y, dxCinfo.CameraVector.z) + CalculateDegToRad(45);
	//////		float si = sinf(rotRad),
	//////			co = cosf(rotRad);

	//////		{
	//////			float tmpX, tmpY;
	//////			//x = (xcosθ - ysinθ) + posX
	//////			//y = (xsinθ + ycosθ) + posY
	//////			tmpX = (cposBase2.y * co - cposBase2.z * si);
	//////			tmpY = (cposBase2.y * si + cposBase2.z * co);

	//////			cposBase2.y = tmpX;
	//////			cposBase2.z = tmpY;
	//////		}
	//////	}
	//////	cposBase.x = cposBase2.x;
	//////	cposBase.y = cposBase2.y;

	//////	//cposBase.x = wp.x - camera.trans.pos.x;
	//////	//cposBase.y = wp.y - camera.trans.pos.y;

	//////	//XMMATRIX mWorld = {};
	//////	//mWorld *= XMMatrixTranslation(cposBase.x, cposBase.y, 0.0f); //本来の位置
	//////	////mWorld *= dxCinfo.CameraScaling;

	//////	//mWorld *= dxCinfo.CameraView;
	//////	//mWorld *= dxCinfo.CameraPos;

	//////	//cposBase.x = mWorld.r[3].m128_f32[0];
	//////	//cposBase.y = mWorld.r[3].m128_f32[1];
	//////	 
	//////	//cposBase.x += -SCREEN_WIDTH / 2 * (1.0f - dxCinfo.CameraVector.z) * (1.0f - dxCinfo.CameraVector.y);
	//////			
	//////	//cposBase.y += -SCREEN_HEIGHT / 2 * (1.0f - dxCinfo.CameraVector.x) * (1.0f - dxCinfo.CameraVector.z);
	//////	



	//////	//cposBase.x = wp.x;
	//////	//cposBase.y = wp.y;


	//////	//cposBase.x += -camera.trans.pos.x * (1.0f - dxCinfo.CameraVector.y);
	//////	//cposBase.x += -camera.trans.pos.x * (1.0f - dxCinfo.CameraVector.z);
	//////	//		
	//////	//cposBase.y += -camera.trans.pos.y * (1.0f - dxCinfo.CameraVector.x);
	//////	//cposBase.y += -camera.trans.pos.y * (1.0f - dxCinfo.CameraVector.z);

	//////	//cposBase.x *= (1.0f - dxCinfo.CameraVector.x);
	//////	//cposBase.y *= (1.0f - dxCinfo.CameraVector.y);
	//////	//{
	//////	//	float rotRad = CalculateDegToRad(opt.startRot.x);
	//////	//	float si = sinf(rotRad),
	//////	//		co = cosf(rotRad);

	//////	//	{
	//////	//		float tmpX, tmpY;
	//////	//		//x = (xcosθ - ysinθ) + posX
	//////	//		//y = (xsinθ + ycosθ) + posY
	//////	//		tmpX = (cposBase.y * co - cposBase.z * si);
	//////	//		tmpY = (cposBase.y * si + cposBase.z * co);

	//////	//		posTmp.y = tmpX;
	//////	//		posTmp.z = tmpY;
	//////	//	}
	//////	//}
	//////}
	//////else
	//////{
	//////	cposBase.x = wp.x - camera.trans.pos.x - camera.cameraDrawDat.zoomAddPos.x;
	//////	cposBase.y = wp.y - camera.trans.pos.y - camera.cameraDrawDat.zoomAddPos.y;

	//////	cposBase.x *= (camera.trans.zoom * camera.trans.scale.x) + camera.cameraDrawDat.zoomAddScale;
	//////	cposBase.y *= (camera.trans.zoom * camera.trans.scale.y) + camera.cameraDrawDat.zoomAddScale;
	//////}

	////cposBase.x = wp.x - camera.trans.pos.x - camera.cameraDrawDat.zoomAddPos.x;
	////cposBase.y = wp.y - camera.trans.pos.y - camera.cameraDrawDat.zoomAddPos.y;

	////cposBase.x *= (camera.trans.zoom * camera.trans.scale.x) + camera.cameraDrawDat.zoomAddScale;
	////cposBase.y *= (camera.trans.zoom * camera.trans.scale.y) + camera.cameraDrawDat.zoomAddScale;

 //   //wp.x *= camera.trans.zoom;
 //   //wp.y *= camera.trans.zoom;
 //   //wp.x /= CAMERA_3D_MOD;
 //   //wp.y /= CAMERA_3D_MOD;

 //   float w = SCREEN_WIDTH / 2.0f;
 //   float h = SCREEN_HEIGHT / 2.0f;
 //   XMMATRIX viewport = {
 //   w, 0, 0, 0,
 //   0, -h, 0, 0,
 //   0, 0, 1, 0,
 //   w, h, 0, 1
 //   };

 //   XMFLOAT3 wptemp = XMFLOAT3(wp.x, wp.y, wp.z);
 //   //XMFLOAT3 wptemp = XMFLOAT3(wp.x, wp.z, wp.y);
 //   XMVECTOR World_Pos = XMLoadFloat3(&wptemp);


 //   //XMFLOAT3 cinfopos = cinfo.CameraPosRaw;
 //   ////pos.x *= -1.0f;
 //   //cinfopos.y *= -1.0f;
 //   ////pos.z *= -1.0f;
 //   //XMFLOAT3 cinfotarget = cinfo.CameraTargetRaw;
 //   //XMFLOAT3 cinfoup = cinfo.CameraUpRaw;
 //   //cinfoup = { 0.0f, 0.0f, 1.0f };


 //   // ビュー変換とプロジェクション変換
 //   World_Pos = XMVector3Transform(World_Pos, cinfo.CameraScaling);
 //   World_Pos = XMVector3Transform(World_Pos, cinfo.CameraView);
 //   World_Pos = XMVector3Transform(World_Pos, cinfo.CameraPos);

 //   XMFLOAT3 temp;
 //   DirectX::XMStoreFloat3(&temp, World_Pos);

 //   // zで割って-1~1の範囲に収める
 //   // スクリーン変換
 //   const XMVECTOR view_vec = XMVectorSet(
 //       temp.x / temp.z,
 //       temp.y / temp.z, 
 //       1.0f, 1.0f);
 //   //const XMVECTOR view_vec = XMVectorSet(
 //   //    temp.x / temp.y,
 //   //    temp.z / temp.y,
 //   //    1.0f, 1.0f);
 //   const XMVECTOR r_vec = XMVector3Transform(view_vec, viewport);
 //   XMFLOAT3 rtemp;
 //   DirectX::XMStoreFloat3(&rtemp, r_vec);

 //   //cposBase.x = rtemp.x - w;
 //   //cposBase.y = rtemp.y - h;
 //   cposBase.x = rtemp.x - w;
 //   cposBase.y = rtemp.y - h;

 //   //cposBase.x /= CAMERA_3D_MOD;
 //   //cposBase.y /= CAMERA_3D_MOD;
 //   //cposBase.y *= -1.0f;
 //   //cposBase.x /= camera.trans.zoom;
 //   //cposBase.y /= camera.trans.zoom;
 //   //cposBase.x /= CAMERA_3D_MOD / 2;
 //   //cposBase.y /= CAMERA_3D_MOD / 2;

 //   //cposBase.x *= camera.trans.zoom;
 //   //cposBase.y *= camera.trans.zoom;
	//return cposBase;

    // ワールド座標
    //XMVECTOR worldPos = XMVectorSet(
    //    wp.x,
    //    wp.y,
    //    wp.z, 
    //    1.0f);
    XMFLOAT3 wptemp = XMFLOAT3(wp.x, wp.y, wp.z);
    XMVECTOR worldPos = XMLoadFloat3(&wptemp);

    // ビュー行列と投影行列の設定
    XMMATRIX viewMatrix = cinfo.CameraView;
    XMMATRIX projMatrix = cinfo.CameraPos;

    // ワールド座標をビュー座標に変換
    XMVECTOR viewPos = XMVector3TransformCoord(worldPos, viewMatrix);

    // ビュー座標をクリップ座標に変換
    XMVECTOR clipPos = XMVector3TransformCoord(viewPos, projMatrix);

    // クリップ座標を正規化デバイス座標に変換
    XMVECTOR ndcPos = XMVectorDivide(clipPos, XMVectorSplatW(clipPos));

    // 正規化デバイス座標をスクリーン座標に変換
    cposBase.x = (ndcPos.m128_f32[0] + 1.0f) * 0.5f * -SCREEN_WIDTH + SCREEN_WIDTH / 2;
    //cposBase.y = (1.0f - ndcPos.m128_f32[1]) * 0.5f * SCREEN_HEIGHT + SCREEN_HEIGHT / 2;
    cposBase.y = (1.0f - ndcPos.m128_f32[1]) * 0.5f * SCREEN_HEIGHT - SCREEN_HEIGHT / 2;
    /// cinfo.CameraVector.x
    //    / cinfo.CameraVector.y
    //    / cinfo.CameraVector.z
    

    return cposBase;
}

Float2 CameraPosToWorldPos(int index, Float2 cp)
{ //現状角度は未考慮
    CAMERA& camera = g_Camera[index];

	double x = (double)cp.x;
	double y = (double)cp.y;
	//{
	//	float rotRad = camera.trans.rot * 3.14159f / 180;
	//	float si = sinf(rotRad),
	//		co = cosf(rotRad);
	//	//x = (x * co - y * si);
	//	//y = (x * si + y * co);
	//}
	x *= (1.0 / camera.trans.scale.x) * (1.0 / camera.trans.zoom);
	y *= (1.0 / camera.trans.scale.y) * (1.0 / camera.trans.zoom);
	x += camera.trans.pos.x;
	y += camera.trans.pos.y;

	return MkF2((float)x, (float)y);
}

Float2 GetCameraSize(int index)
{
    CAMERA& camera = g_Camera[index];

	return
		MkF2((float)SCREEN_WIDTH * (1.0f / camera.trans.scale.x) * (1.0f / camera.trans.zoom),
			(float)SCREEN_HEIGHT * (1.0f / camera.trans.scale.y) * (1.0f / camera.trans.zoom));
}

Float3 GetCameraPos(int index)
{
    CAMERA& camera = g_Camera[index];
	return camera.trans.pos;
}
Float3 GetCameraPCPPos(int index)
{
    CAMERA& camera = g_Camera[index];
    return camera.playerChase.cameraPosProvision;
}
Float3 GetCameraPCPTarget(int index)
{
    CAMERA& camera = g_Camera[index];
    return camera.playerChase.cameraTargetProvision;
}
float GetCameraPCPZoom(int index)
{
    CAMERA& camera = g_Camera[index];
    return camera.playerChase.cameraZoomProvision;
}



void CameraShakeRequestAll(int frame, float power,
    float powerY,
    bool isPriorityHighPower
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraShakeRequest(i, frame, power, powerY, isPriorityHighPower);
    }
}
void CameraShakeRequest(int index, int frame, float power,
    float powerY, 
    bool isPriorityHighPower
)
{
    CAMERA& camera = g_Camera[index];
    if (!isPriorityHighPower
        || camera.shakeFrame <= 0
        || power >= camera.shakePower || camera.shakePowerY >= powerY
    )
    {
        camera.shakePower = power;
        if (powerY < 0)
        {
            camera.shakePowerY = camera.shakePower;
        }
        else
        {
            camera.shakePowerY = powerY;
        }

        camera.shakeFrameMax = frame;
        camera.shakeFrame = camera.shakeFrameMax;
    }
}


void cameraMoveSetEndTrans(int index)
{
    CAMERA& camera = g_Camera[index];
	//camera.moveRequ.frame = 1;
	camera.moveRequ.frame = 0;
	camera.trans = camera.moveRequ.endTrans;
}

void CameraMoveRequestAll(
    int frame, 
    Float2 pos, 
    float rot,
    Float2 scale, 
    float zoom,
    std::function<float(float)> easing, 
    CAMERA_MOVE_TYPE moveType
)
{
    CameraMoveRequestAll(
        frame,
        Float2ToFloat3(pos),
        rot,
        scale,
        zoom,
        easing,
        moveType
    );
}

//移動リクエスト
void CameraMoveRequestAll(
    int frame,
    Float3 pos,
    float rot,
    Float2 scale,
    float zoom,
    std::function<float(float)> easing,
    CAMERA_MOVE_TYPE moveType
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraMoveRequest(
            i,
            frame,
            pos,
            rot,
            scale,
            zoom,
            easing,
            moveType
        );
    }
}
void CameraMoveRequest(
    int index,
	int frame,
	Float3 pos,
	float rot,
	Float2 scale,
	float zoom,
	std::function<float(float)> easing,
	CAMERA_MOVE_TYPE moveType
)
{
    CAMERA& camera = g_Camera[index];
	if (frame > 0)
	{
		CAMERA_MOVE_REQUEST* mr = &camera.moveRequ;
		(*mr) = {};
		mr->enable = true;

		mr->frameMax = frame;
		mr->frame = mr->frameMax;

		mr->startTrans.pos = camera.trans.pos;
		mr->endTrans.pos = pos;
		//mr->oldPos = mr->startPos;

		mr->startTrans.rot = camera.trans.rot;
		mr->endTrans.rot = rot;

		mr->startTrans.scale = camera.trans.scale;
		mr->endTrans.scale = scale;

		mr->startTrans.zoom = camera.trans.zoom;
		mr->endTrans.zoom = zoom;

		mr->easing = easing;

		mr->moveType = moveType;
	}
}

void CameraMoveRequestAll(
    int frame,
    Float3 pos,
    std::function<float(float)> easing,
    CAMERA_MOVE_TYPE moveType
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraMoveRequest(
            i,
            frame,
            pos,
            easing,
            moveType
        );
    }
}
void CameraMoveRequest(
    int index,
    int frame,
    Float3 pos,
    std::function<float(float)> easing,
    CAMERA_MOVE_TYPE moveType
)
{
    CAMERA& camera = g_Camera[index];
    if (frame > 0)
    {
        CAMERA_MOVE_REQUEST* mr = &camera.moveRequ;
        (*mr) = {};
        mr->enable = true;

        mr->frameMax = frame;
        mr->frame = mr->frameMax;

        mr->startTrans.pos = camera.trans.pos;
        mr->endTrans.pos = pos;
        //mr->oldPos = mr->startPos;

        mr->startTrans.rot = camera.trans.rot;
        mr->endTrans.rot = camera.trans.rot;

        mr->startTrans.scale = camera.trans.scale;
        mr->endTrans.scale = camera.trans.scale;

        mr->startTrans.zoom = camera.trans.zoom;
        mr->endTrans.zoom = camera.trans.zoom;

        mr->easing = easing;

        mr->moveType = moveType;
    }
}

void CameraMoveCancelRequestAll()
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraMoveCancelRequest(i);
    }
}
void CameraMoveCancelRequest(int index)
{
    CAMERA& camera = g_Camera[index];
    {
        CAMERA_MOVE_REQUEST* mr = &camera.moveRequ;
        (*mr) = {};
        mr->enable = false;
    }
}


void CameraTargetMoveRequestAll(
    int frame,
    Float3 pos,
    std::function<float(float)> easing,
    CAMERA_MOVE_TYPE moveType
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraTargetMoveRequest(
            i,
            frame,
            pos,
            easing,
            moveType
        );
    }
}
void CameraTargetMoveRequest(
    int index,
    int frame,
    Float3 pos,
    std::function<float(float)> easing,
    CAMERA_MOVE_TYPE moveType
)
{
    CAMERA& camera = g_Camera[index];
    if (frame > 0)
    {
        CAMERA_MOVE_REQUEST* mr = &camera.moveRequTarget;
        (*mr) = {};
        mr->enable = true;

        mr->frameMax = frame;
        mr->frame = mr->frameMax;

        mr->startTrans.pos = camera.trans.target;
        mr->endTrans.pos = pos;

        mr->startTrans.rot = 0.0f;
        mr->endTrans.rot = 0.0f;

        mr->startTrans.scale = MkF2(1.0f, 1.0f);
        mr->endTrans.scale = MkF2(1.0f, 1.0f);

        mr->startTrans.zoom = 1.0f;
        mr->endTrans.zoom = 1.0f;

        mr->easing = easing;

        mr->moveType = moveType;
    }
}

void CameraTargetMoveCancelRequestAll()
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraTargetMoveCancelRequest(i);
    }
}
void CameraTargetMoveCancelRequest(int index)
{
    CAMERA& camera = g_Camera[index];
    {
        CAMERA_MOVE_REQUEST* mr = &camera.moveRequTarget;
        (*mr) = {};
        mr->enable = false;
    }
}
void CameraTargetPositionRegister(int index, Float3* pos)
{
    CAMERA& camera = g_Camera[index];
    camera.targetRef = pos;
}

void update_moveRequ(CAMERA_MOVE_REQUEST& moveRequ,
    Float3* posRef, float* rotRef, Float2* scaleRef, float* zoomRef
)
{
    if (moveRequ.enable)
    {
        float mrModTmp = (float)moveRequ.frame / moveRequ.frameMax;

        float mrModRvTmp = (1.0f - mrModTmp);

        float mrModEasTmp = mrModRvTmp;
        if (moveRequ.easing != nullptr)
        {
            mrModEasTmp = moveRequ.easing(mrModRvTmp);
        }


        float sXtmp = (moveRequ.endTrans.pos.x - moveRequ.startTrans.pos.x);
        float sYtmp = (moveRequ.endTrans.pos.y - moveRequ.startTrans.pos.y);
        float sZtmp = (moveRequ.endTrans.pos.z - moveRequ.startTrans.pos.z);

        float sRottmp = (moveRequ.endTrans.rot - moveRequ.startTrans.rot);

        float sScaXtmp = (moveRequ.endTrans.scale.x - moveRequ.startTrans.scale.x);
        float sScaYtmp = (moveRequ.endTrans.scale.y - moveRequ.startTrans.scale.y);

        float sZoomtmp = (moveRequ.endTrans.zoom - moveRequ.startTrans.zoom);

        if (moveRequ.frame >= moveRequ.frameMax
            && (
                moveRequ.moveType == CAMERA_MOVE_TYPE_FADE ||
                (moveRequ.moveType == CAMERA_MOVE_TYPE_NORMAL &&
                    (abs(sXtmp) > SCREEN_WIDTH || abs(sYtmp) > SCREEN_HEIGHT
                        || abs(sScaXtmp) > 0.9f || abs(sScaXtmp) > 0.9f
                        || abs(sZoomtmp) > 0.9f
                        )
                    )
                )
            )
        {
            //SetFade(FADE_TYPE_COLOR_BLACK,
            //    cameraMoveSetEndTrans,
            //    EasingInCubic,
            //    camera.moveRequ.frameMax / 4,
            //    true
            //);
        }

        float setXtmp = moveRequ.startTrans.pos.x + sXtmp * mrModEasTmp;
        float setYtmp = moveRequ.startTrans.pos.y + sYtmp * mrModEasTmp;
        float setZtmp = moveRequ.startTrans.pos.z + sZtmp * mrModEasTmp;

        float setRottmp = moveRequ.startTrans.rot + sRottmp * mrModEasTmp;

        float setScaXtmp = 1.0f;
        float setScaYtmp = 1.0f;
        if (scaleRef != nullptr)
        {
            setScaXtmp = moveRequ.startTrans.scale.x + sScaXtmp * mrModEasTmp * scaleRef->x;
            setScaYtmp = moveRequ.startTrans.scale.y + sScaYtmp * mrModEasTmp * scaleRef->y;
        }

        //float setZoomtmp = camera.moveRequ.startTrans.zoom + sZoomtmp * mrModEasTmp * camera.trans.zoom;
        float setZoomtmp = moveRequ.startTrans.zoom + sZoomtmp * mrModEasTmp;

        if (posRef != nullptr)
        {
            posRef->x = setXtmp;
            posRef->y = setYtmp;
            posRef->z = setZtmp;
        }
        if (rotRef != nullptr)
        {
            (*rotRef) = setRottmp;
        }
        if (scaleRef != nullptr)
        {
            scaleRef->x = setScaXtmp;
            scaleRef->y = setScaYtmp;
        }
        if (zoomRef != nullptr)
        {
            (*zoomRef) = setZoomtmp;
        }

        //camera.moveRequ.oldPos = { setXtmp, setYtmp };

        if (moveRequ.frame <= 0)
        {
            moveRequ.enable = false;
        }
        else
        {
            moveRequ.frame--;
        }
    }
}


//フラッシュリクエスト
void CameraFlashRequestAll(
    int frame,
    std::function<float(float)> easing,
    Float4 color,
    DPD_SHADER_OPT_BLEND_STATE colorBlendState
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraFlashRequest(
            i,
            frame,
            easing,
            color,
            colorBlendState
        );
    }
}
void CameraFlashRequest(
    int index,
	int frame,
	std::function<float(float)> easing,
	Float4 color,
    DPD_SHADER_OPT_BLEND_STATE colorBlendState
)
{
    CAMERA& camera = g_Camera[index];
	if (frame > 0)
	{
		CAMERA_FLASH_REQUEST* fr = &camera.flashRequ;
		(*fr) = {};
		fr->frameMax = frame;
		fr->frame = fr->frameMax;

		//fr->color = color;

		fr->easing = easing;

        fr->opt = {};
        fr->opt.color = color;
        fr->opt.texNo = 0;
        fr->opt.texUV = MkF2(0.0f, 0.0f);
        fr->opt.texWH = MkF2(1.0f, 1.0f);
        fr->opt.loadTexType = LOADTEXTURETYPE_MAIN;
        fr->opt.fixed = true;
        fr->opt.priority = DRAW_POLYGON_PRIORITY_FORCE_NORMAL;
        fr->opt.shaderOpt.blendState = colorBlendState;
	}
}
//ズームリクエスト
void CameraZoomRequestAll(
    float speed,
    int objIndex,
    std::function<float(float)> easing,
    float zoomScale
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraZoomRequest(
            i,
            speed,
            objIndex,
            easing,
            zoomScale
        );
    }
}
void CameraZoomRequest(
    int index,
	float speed,
	int objIndex,
	std::function<float(float)> easing,
	float zoomScale
)
{
    CAMERA& camera = g_Camera[index];
	if (speed > 0)
	{
		CAMERA_GAME_OBJECT_ZOOM_REQUEST* gr = &camera.goZoomRequ;
		//(*gr) = {};
		gr->isUse = true;

		gr->speed = speed;

		gr->objIndex = objIndex;

		{
			GAME_OBJECT* obj = GetGameObject(gr->objIndex);
			if (IsActiveGameObject(obj))
			{
				gr->objCreateFrame = obj->createFrame;
			}
			else
			{
				gr->objCreateFrame = 0;
			}
		}

		gr->easing = easing;
		gr->zoomScale = zoomScale;
	}
}
//ズームリクエスト
void CameraZoomRequestAll(
    float speed,
    Float3 pos,
    std::function<float(float)> easing,
    float zoomScale
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraZoomRequest(
            i,
            speed,
            pos,
            easing,
            zoomScale
        );
    }
}
void CameraZoomRequest(
    int index,
	float speed,
	Float3 pos,
	std::function<float(float)> easing,
	float zoomScale
)
{
    CAMERA& camera = g_Camera[index];
	if (speed > 0)
	{
		CAMERA_GAME_OBJECT_ZOOM_REQUEST* gr = &camera.goZoomRequ;
		//(*gr) = {};
		gr->isUse = true;

		gr->speed = speed;

		gr->objIndex = -1;

		gr->pos = pos;

		gr->easing = easing;
		gr->zoomScale = zoomScale;
	}
}

void CameraZoomRequestAll(float speed, Float2 pos, std::function<float(float)> easing, float zoomScale)
{
    CameraZoomRequestAll(speed, Float2ToFloat3(pos), easing, zoomScale);
}

void CameraZoomRequest(int index, float speed, Float2 pos, std::function<float(float)> easing, float zoomScale)
{
    CameraZoomRequest(index, speed, Float2ToFloat3(pos), easing, zoomScale);
}

void CameraZoomResetRequestAll(
    float speed,
    std::function<float(float)> easing, int delay
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraZoomResetRequest(
            i,
            speed,
            easing,
            delay
        );
    }
}
void CameraZoomResetRequest(
    int index,
	float speed,
	std::function<float(float)> easing, int delay
)
{
    CAMERA& camera = g_Camera[index];
	{
		if (delay > 0)
		{
			camera.goZoomDelayRequ.frameMax = delay;
			camera.goZoomDelayRequ.frame = camera.goZoomDelayRequ.frameMax;

			camera.goZoomDelayRequ.dat.speed = speed;
			camera.goZoomDelayRequ.dat.easing = easing;
		}
		else
		{
			CAMERA_GAME_OBJECT_ZOOM_REQUEST* gr = &camera.goZoomRequ;
			//(*gr) = {};

			gr->isUse = false;
			if (speed > 0)
			{
				gr->speed = speed;
			}
			if (easing != nullptr)
			{
				gr->easing = easing;
			}
		}
	}
}


void CameraRevRequestAll(
    float speed,
    std::function<float(float)> easing
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraRevRequest(
            i,
            speed,
            easing
        );
    }
}
void CameraRevRequest(
    int index,
	float speed,
	std::function<float(float)> easing
)
{
    CAMERA& camera = g_Camera[index];
	if (speed > 0)
	{
		CAMERA_REVERSE_REQUEST* rr = &camera.revRequ;
		//(*gr) = {};
		rr->isUse = true;

		rr->speed = speed;

		rr->easing = easing;
	}
}

void CameraRevResetRequestAll(
    float speed,
    std::function<float(float)> easing
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraRevResetRequest(
            i,
            speed,
            easing
        );
    }
}
void CameraRevResetRequest(
    int index,
	float speed,
	std::function<float(float)> easing
)
{
    CAMERA& camera = g_Camera[index];
	{
		CAMERA_REVERSE_REQUEST* rr = &camera.revRequ;
		//(*gr) = {};

		rr->isUse = false;
		if (speed > 0)
		{
			rr->speed = speed;
		}
		if (easing != nullptr)
		{
			rr->easing = easing;
		}
	}
}
bool IsCameraRev(int index)
{
    CAMERA& camera = g_Camera[index];
	return camera.revRequ.mod > 0;
}


CAMERA GetCameraInfo(int index)
{
    CAMERA& camera = g_Camera[index];
	return camera;
}
void SetCameraInfo(int index, CAMERA info)
{
    if (index < 0 || index >= CAMERA_NUM)
    {
        return;
    }
    g_Camera[index] = info;
}

void SetCameraZoomAll(float n)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        SetCameraZoom(i, n);
    }
}
void SetCameraZoom(int index, float n)
{
    CAMERA& camera = g_Camera[index];
	camera.trans.zoom = n;
}

void SetCameraRotAll(float n)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        SetCameraRot(i, n);
    }
}
void SetCameraRot(int index, float n)
{
    CAMERA& camera = g_Camera[index];
    camera.trans.rot = n;
}

void SetCameraPosAll(Float3 pos)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        SetCameraPos(i, pos);
    }
}
void SetCameraPos(int index, Float3 pos)
{
    CAMERA& camera = g_Camera[index];
    camera.trans.pos = pos;
}
void SetCameraTargetPosAll(Float3 pos)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        SetCameraTargetPos(i, pos);
    }
}
void SetCameraTargetPos(int index, Float3 pos)
{
    CAMERA& camera = g_Camera[index];
    camera.trans.target = pos;
}

void SetCameraModeAll(CAMERA_MODE mode, int delay)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        SetCameraMode(i, mode, delay);
    }
}

void SetCameraMode(int index, CAMERA_MODE mode, int delay)
{
    CAMERA& camera = g_Camera[index];
	if (delay > 0)
	{
		camera.modeSetRequ.mode = mode;
		camera.modeSetRequ.frameMax = delay;
		camera.modeSetRequ.frame = camera.modeSetRequ.frameMax;
	}
	else
	{
		camera.mode = mode;
        camera.modeChangeLerpFrame.y = 30;
        camera.modeChangeLerpFrame.x = camera.modeChangeLerpFrame.y;
	}
}