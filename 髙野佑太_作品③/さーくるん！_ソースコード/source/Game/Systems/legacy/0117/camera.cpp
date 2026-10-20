// ===================================================
// camera.cpp �J��������
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
#include "../Entities/player.h"


// ===================================================
// �}�N����`
// ===================================================

// ===================================================
// �\���̐錾
// ===================================================

// ===================================================
// �v���g�^�C�v�錾
// ===================================================
void cameraMoveSetEndTrans(int index);

// ===================================================
// �O���[�o���ϐ�
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
// �J�����̏�����
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
        camera.trans.pos = MkF2(0.0f, 0.0f);
        camera.trans.rot = 0.0f;
        camera.trans.scale = MkF2(1.0f, 1.0f);
        camera.trans.zoom = 1.0f;

        camera.shakePower = 1.0f;
        camera.shakeFrame = 0;
        camera.shakeFrameMax = 1;

        camera.goZoomRequ.objIndex = -1;


        //3D�p�̐ݒ�
        //camera.camera3D.fov = 1.0f;
        camera.camera3D.fov = 1.0f;
        camera.camera3D.pos = MkF3(0.0f, 6.0f, -10.0f);
        //camera.camera3D.pos = MkF3(0.0f, 8.0f, -8.0f);
        //camera.camera3D.pos = MkF3(0.0f, 8.0f, -0.01f);
        camera.camera3D.target = MkF3(0.0f, 0.0f, 0.0f);
        camera.camera3D.up = MkF3(0.0f, -1.0f, 0.0f);
        camera.camera3D.scale = 0.01f;

        //���ۂ̐ݒ�̓��[�v���ł��

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
// �J�����̍X�V
// ===================================================
void UpdateCamera(void)
{
#if defined(_DEBUG) || defined(DEBUG)
    //�f�o�b�O

    //if(!WinSockClientIsEnable())
    {
        static bool kp = false;
        if (GetAsyncKeyState('C'))
        {
            if (!kp)
            {
                g_CameraCurrentIndex++;
                if (g_CameraCurrentIndex >= CAMERA_NUM)
                {
                    g_CameraCurrentIndex = 0;
                }

                kp = true;
            }
        }
        else
        {
            kp = false;
        }
    }
#endif

    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CAMERA& camera = g_Camera[i];


        //�J�����V�F�C�N�̍X�V
        if (camera.shakeFrame > 0)
        {
            camera.shakeFrame--;
        }


        //���u���N����

        if (camera.mode == CAMERA_MODE_PLAYER_CHASE)
        {
            PLAYER* pl = GetPlayer(i);
            if (pl != nullptr)
            {
                GAME_OBJECT* plOb = pl->obj;
                if (plOb != nullptr)
                {

                    float m = 1.0f / 36.0f;
                    //Float2 ang = MkF2(pl->angle.x, -pl->angle.y);
                    Float2 ang = {};

                    camera.playerChase.angle.x += (ang.x - camera.playerChase.angle.x) / 50.0f + plOb->vel.x / 60.0f;
                    camera.playerChase.angle.y += (ang.y - camera.playerChase.angle.y) / 50.0f + plOb->vel.y / 60.0f;
                    if (camera.playerChase.angle.x > 1.0f)
                    {
                        camera.playerChase.angle.x = 1.0f;
                    }
                    if (camera.playerChase.angle.x < -1.0f)
                    {
                        camera.playerChase.angle.x = -1.0f;
                    }
                    if (camera.playerChase.angle.y > 1.0f)
                    {
                        camera.playerChase.angle.y = 1.0f;
                    }
                    if (camera.playerChase.angle.y < -1.0f)
                    {
                        camera.playerChase.angle.y = -1.0f;
                    }

                    float ax = (float)(
                        (plOb->pos.x - CAMERA_PLAYER_CHASE_POS_X
                            + (ang.x * CAMERA_PLAYER_CHASE_ANGLE_POS_X) + ((CAMERA_PLAYER_CHASE_MOVE_POS_X + plOb->vel.x) * camera.playerChase.angle.x)
                            ) - camera.trans.pos.x);

                    float ay = (float)(
                        (plOb->pos.y - (plOb->isGround ? CAMERA_PLAYER_CHASE_POS_Y : 0.0f)
                            + (ang.y * CAMERA_PLAYER_CHASE_ANGLE_POS_Y) + ((CAMERA_PLAYER_CHASE_MOVE_POS_Y + plOb->vel.y) * camera.playerChase.angle.y)
                            ) - camera.trans.pos.y);

                    camera.trans.pos.x += (float)(ax * m);
                    camera.trans.pos.y += (float)(ay * m);

                    camera.trans.zoom += (float)(CAMERA_PLAYER_CHASE_ZOOM - camera.trans.zoom) * m;
                }
            }
        }

        if (camera.moveRequ.enable)
        {
            float mrModTmp = (float)camera.moveRequ.frame / camera.moveRequ.frameMax;

            float mrModRvTmp = (1.0f - mrModTmp);

            float mrModEasTmp = mrModRvTmp;
            if (camera.moveRequ.easing != nullptr)
            {
                mrModEasTmp = camera.moveRequ.easing(mrModRvTmp);
            }


            float sXtmp = (camera.moveRequ.endTrans.pos.x - camera.moveRequ.startTrans.pos.x);
            float sYtmp = (camera.moveRequ.endTrans.pos.y - camera.moveRequ.startTrans.pos.y);

            float sRottmp = (camera.moveRequ.endTrans.rot - camera.moveRequ.startTrans.rot);

            float sScaXtmp = (camera.moveRequ.endTrans.scale.x - camera.moveRequ.startTrans.scale.x);
            float sScaYtmp = (camera.moveRequ.endTrans.scale.y - camera.moveRequ.startTrans.scale.y);

            float sZoomtmp = (camera.moveRequ.endTrans.zoom - camera.moveRequ.startTrans.zoom);

            if (camera.moveRequ.frame >= camera.moveRequ.frameMax
                && (
                    camera.moveRequ.moveType == CAMERA_MOVE_TYPE_FADE ||
                    (camera.moveRequ.moveType == CAMERA_MOVE_TYPE_NORMAL &&
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

            float setXtmp = camera.moveRequ.startTrans.pos.x + sXtmp * mrModEasTmp;
            float setYtmp = camera.moveRequ.startTrans.pos.y + sYtmp * mrModEasTmp;

            float setRottmp = camera.moveRequ.startTrans.rot + sRottmp * mrModEasTmp;

            float setScaXtmp = camera.moveRequ.startTrans.scale.x + sScaXtmp * mrModEasTmp * camera.trans.scale.x;
            float setScaYtmp = camera.moveRequ.startTrans.scale.y + sScaYtmp * mrModEasTmp * camera.trans.scale.y;

            //float setZoomtmp = camera.moveRequ.startTrans.zoom + sZoomtmp * mrModEasTmp * camera.trans.zoom;
            float setZoomtmp = camera.moveRequ.startTrans.zoom + sZoomtmp * mrModEasTmp;


            camera.trans.pos.x = setXtmp;
            camera.trans.pos.y = setYtmp;
            camera.trans.rot = setRottmp;
            camera.trans.scale.x = setScaXtmp;
            camera.trans.scale.y = setScaYtmp;
            camera.trans.zoom = setZoomtmp;

            //camera.moveRequ.oldPos = { setXtmp, setYtmp };

            if (camera.moveRequ.frame <= 0)
            {
                camera.moveRequ.enable = false;
            }
            else
            {
                camera.moveRequ.frame--;
            }
        }

        //�ӂ�肭����
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

        //���[�h�ύX���N�G�X�g
        if (camera.modeSetRequ.frame > 0)
        {
            camera.modeSetRequ.frame--;
            if (camera.modeSetRequ.frame <= 0)
            {
                camera.mode = camera.modeSetRequ.mode;
            }
        }

        {
            {
                double sptmp = (double)camera.shakeFrame / (camera.shakeFrameMax < 1.0f ? 1.0f : camera.shakeFrameMax);
                double xtmp = sin(7.6f * (double)camera.shakeFrame) * sptmp * camera.shakePower;
                double ytmp = cos(5.6f * (double)camera.shakeFrame) * sptmp * camera.shakePower;

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

        //�[�x�ݒ��ێ�
        bool deSave = DXDepthStencilStateDepthEnable();
        if (deSave)
        {

            DX_SetCameraInfo(
                i,
                camera.camera3D.fov,
                {
                    camera.camera3D.pos.x,
                    camera.camera3D.pos.y,
                    camera.camera3D.pos.z
                },
                {
                    camera.camera3D.target.x,
                    camera.camera3D.target.y,
                    camera.camera3D.target.z
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

	//�e�X�g
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
// �J�����̏I������
// ===================================================
void UninitCamera(void)
{
}

// ===================================================
// �J�����̕`��
// ===================================================
void DrawCamera(void)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CAMERA& camera = g_Camera[i];
        if (camera.flashRequ.frame > 0)
        {
            Float4 color = camera.flashRequ.color;
            float alpha = (float)camera.flashRequ.frame / camera.flashRequ.frameMax;
            if (camera.flashRequ.easing != nullptr)
            {
                color.w *= camera.flashRequ.easing(alpha);
            }
            else
            {
                color.w *= alpha;
            }

            DRAW_SPRITE_BUFFER dsb = {};
            SpriteSetUp(&dsb);
            SpriteShape(&dsb,
                MkF2(-1.0f, -1.0f),
                0.0f,
                MkF2((float)SCREEN_WIDTH + 2.0f, (float)SCREEN_HEIGHT + 2.0f)
            );
            SpriteRelease(&dsb,
                color,
                0,
                MkF2(0.0f, 0.0f),
                MkF2(1.0f, 1.0f),
                LOADTEXTURETYPE_MAIN,
                true,
                DRAW_POLYGON_PRIORITY_FORCE_ALWAYS
            );
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

        if (i == g_CameraCurrentIndex)
        {
            //for (int j = 0; j < g_CameraDrawBuf[i].size(); j++)
            //{
            //    PolygonRelease(
            //        &g_CameraDrawBuf[i][j].dpb,
            //        g_CameraDrawBuf[i][j].pro
            //    );
            //}
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

        for (int j = 0; j < CAMERA_DRAW_BUF_MAX; j++)
        {
            g_CameraDrawBuf[i][j].enable = false;
        }
        //g_CameraDrawBuf[i].clear();
    }
}


void DrawSpriteInCameraSingle (
    int index,
    float x, float y, float w, float h,
    float rot,
    Float4 color,
    unsigned int texNo,
    Float2 texUV, Float2 texWH,
    DRAW_POLYGON_PRIORITY priority
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
    DrawPolygonInCameraSingle(index, drawDatTmp, opt);
}

void DrawSpriteInCamera(
    float x, float y, float w, float h,
    float rot,
    Float4 color,
    unsigned int texNo,
    Float2 texUV, Float2 texWH,
    DRAW_POLYGON_PRIORITY priority
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

void DrawPolygonInCameraSingle(int index,
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

    DRAW_POLYGON_BUFFER dpb = {};
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

    //�J�����̔ԍ���n��
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

    //		// �x�N�g���̒������v�Z
    //		float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    //		// �s�b�`�ix������̉�]�p�x�j���v�Z
    //		float pitch = std::asin(vec.y / length) * 180.0f / SYS_GENERAL_PI;

    //		// ���[�iy������̉�]�p�x�j���v�Z
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

    //		// �x�N�g���̒������v�Z
    //		float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    //		// �s�b�`�ix������̉�]�p�x�j���v�Z
    //		float pitch = std::asin(vec.y / length) * 180.0f / SYS_GENERAL_PI;

    //		// ���[�iy������̉�]�p�x�j���v�Z
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

    ////2D����
    //PolygonShape(&dpb,
    //	MkF3(drawDat.pos.x, drawDat.pos.y, 0.0f),
    //	MkF3(0.0f, 0.0f, drawDat.rot),
    //	MkF3(drawDat.size.x, drawDat.size.y, 1.0f)
    //);
    ////3D����
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
        MkF3(drawDat.size.x, drawDat.size.y, drawDat.size.z)
    );

    ////2D����
    //PolygonShape(&dpb,
    //	Float2ToFloat3(drawDat.startPos),
    //	MkF3(0.0f, 0.0f, drawDat.startRot),
    //	Float2ToFloat3(drawDat.startScale, 1.0f)
    //);
    ////3D����
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
        )
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

        //�[�x�ݒ��ێ�
        bool deSave = DXDepthStencilStateDepthEnable();

        //�J�����̏�Ԃɉ����ĕό`
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
    }


    //SpriteRelease(&dpb,
    //	drawDat.color,
    //	drawDat.texNo,
    //	drawDat.texUV,
    //	drawDat.texWH,
    //	drawDat.loadTexType,
    //	fixed
    //);

    POLYGON_RELEASE_OPTION ropt = {};
    ropt.color = drawDat.color;
    ropt.texNo = drawDat.texNo;
    ropt.texUV = drawDat.texUV;
    ropt.texWH = drawDat.texWH;
    ropt.loadTexType = drawDat.loadTexType;
    ropt.fixed = opt.fixed;
    ropt.priority = opt.priority;
    ropt.depthBuffIgnore = opt.depthBuffIgnore;

    ropt.enableShade = opt.enableShade;
    //ropt.billboard = opt.billboard;

    ropt.shaderOpt = drawDat.shaderOpt;

    //PolygonRelease(&dpb, ropt);

    XMMATRIX polMat = PolygonReleaseMat(&dpb, ropt);

    //��ʊO�͏������Ȃ�
    if (
        polMat.r[3].m128_f32[0] < -SCREEN_WIDTH / 2 || polMat.r[3].m128_f32[0] > SCREEN_WIDTH / 2
        || polMat.r[3].m128_f32[1] < -SCREEN_HEIGHT || polMat.r[3].m128_f32[1] > SCREEN_HEIGHT / 2)
    {
    }
    else
    {
        //�v�Z���ʂ��i�[
        dpb.pro = polMat;

        {
            CAMERA_DRAW_BUF bufTmp = {};
            bufTmp.dpb = dpb;
            bufTmp.pro = ropt;
            CameraAddDrawBuf(ccount, bufTmp);
        }
    }
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

	Float2 cposBase = {};
	
	//bool deSave = DXDepthStencilStateDepthEnable();

	//if (deSave)
	//{
	//	DX_CAMERA_INFO dxCinfo = DX_GetCameraInfo();

	//	cposBase.x = wp.x - camera.trans.pos.x - camera.cameraDrawDat.zoomAddPos.x;
	//	cposBase.y = wp.y - camera.trans.pos.y - camera.cameraDrawDat.zoomAddPos.y;

	//	cposBase.x *= (camera.trans.zoom * camera.trans.scale.x) + camera.cameraDrawDat.zoomAddScale;
	//	cposBase.y *= (camera.trans.zoom * camera.trans.scale.y) + camera.cameraDrawDat.zoomAddScale;

	//	Float3 cposBase2 = Float2ToFloat3(cposBase);
	//	{
	//		float rotRad = atan2(dxCinfo.CameraVector.y, dxCinfo.CameraVector.z) + CalculateDegToRad(45);
	//		float si = sinf(rotRad),
	//			co = cosf(rotRad);

	//		{
	//			float tmpX, tmpY;
	//			//x = (xcos�� - ysin��) + posX
	//			//y = (xsin�� + ycos��) + posY
	//			tmpX = (cposBase2.y * co - cposBase2.z * si);
	//			tmpY = (cposBase2.y * si + cposBase2.z * co);

	//			cposBase2.y = tmpX;
	//			cposBase2.z = tmpY;
	//		}
	//	}
	//	cposBase.x = cposBase2.x;
	//	cposBase.y = cposBase2.y;

	//	//cposBase.x = wp.x - camera.trans.pos.x;
	//	//cposBase.y = wp.y - camera.trans.pos.y;

	//	//XMMATRIX mWorld = {};
	//	//mWorld *= XMMatrixTranslation(cposBase.x, cposBase.y, 0.0f); //�{���̈ʒu
	//	////mWorld *= dxCinfo.CameraScaling;

	//	//mWorld *= dxCinfo.CameraView;
	//	//mWorld *= dxCinfo.CameraPos;

	//	//cposBase.x = mWorld.r[3].m128_f32[0];
	//	//cposBase.y = mWorld.r[3].m128_f32[1];
	//	 
	//	//cposBase.x += -SCREEN_WIDTH / 2 * (1.0f - dxCinfo.CameraVector.z) * (1.0f - dxCinfo.CameraVector.y);
	//			
	//	//cposBase.y += -SCREEN_HEIGHT / 2 * (1.0f - dxCinfo.CameraVector.x) * (1.0f - dxCinfo.CameraVector.z);
	//	



	//	//cposBase.x = wp.x;
	//	//cposBase.y = wp.y;


	//	//cposBase.x += -camera.trans.pos.x * (1.0f - dxCinfo.CameraVector.y);
	//	//cposBase.x += -camera.trans.pos.x * (1.0f - dxCinfo.CameraVector.z);
	//	//		
	//	//cposBase.y += -camera.trans.pos.y * (1.0f - dxCinfo.CameraVector.x);
	//	//cposBase.y += -camera.trans.pos.y * (1.0f - dxCinfo.CameraVector.z);

	//	//cposBase.x *= (1.0f - dxCinfo.CameraVector.x);
	//	//cposBase.y *= (1.0f - dxCinfo.CameraVector.y);
	//	//{
	//	//	float rotRad = CalculateDegToRad(opt.startRot.x);
	//	//	float si = sinf(rotRad),
	//	//		co = cosf(rotRad);

	//	//	{
	//	//		float tmpX, tmpY;
	//	//		//x = (xcos�� - ysin��) + posX
	//	//		//y = (xsin�� + ycos��) + posY
	//	//		tmpX = (cposBase.y * co - cposBase.z * si);
	//	//		tmpY = (cposBase.y * si + cposBase.z * co);

	//	//		posTmp.y = tmpX;
	//	//		posTmp.z = tmpY;
	//	//	}
	//	//}
	//}
	//else
	//{
	//	cposBase.x = wp.x - camera.trans.pos.x - camera.cameraDrawDat.zoomAddPos.x;
	//	cposBase.y = wp.y - camera.trans.pos.y - camera.cameraDrawDat.zoomAddPos.y;

	//	cposBase.x *= (camera.trans.zoom * camera.trans.scale.x) + camera.cameraDrawDat.zoomAddScale;
	//	cposBase.y *= (camera.trans.zoom * camera.trans.scale.y) + camera.cameraDrawDat.zoomAddScale;
	//}

	cposBase.x = wp.x - camera.trans.pos.x - camera.cameraDrawDat.zoomAddPos.x;
	cposBase.y = wp.y - camera.trans.pos.y - camera.cameraDrawDat.zoomAddPos.y;

	cposBase.x *= (camera.trans.zoom * camera.trans.scale.x) + camera.cameraDrawDat.zoomAddScale;
	cposBase.y *= (camera.trans.zoom * camera.trans.scale.y) + camera.cameraDrawDat.zoomAddScale;

	return cposBase;
}

Float2 CameraPosToWorldPos(int index, Float2 cp)
{ //����p�x�͖��l��
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
Float2 GetCameraPos(int index)
{
    CAMERA& camera = g_Camera[index];
	return camera.trans.pos;
}


void CameraShakeRequestAll(int frame, float power)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraShakeRequest(i, frame, power);
    }
}
void CameraShakeRequest(int index, int frame, float power)
{
    CAMERA& camera = g_Camera[index];
    camera.shakePower = power;
    camera.shakeFrameMax = frame;
    camera.shakeFrame = camera.shakeFrameMax;
}


void cameraMoveSetEndTrans(int index)
{
    CAMERA& camera = g_Camera[index];
	//camera.moveRequ.frame = 1;
	camera.moveRequ.frame = 0;
	camera.trans = camera.moveRequ.endTrans;
}

//�ړ����N�G�X�g
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
	Float2 pos,
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
//�t���b�V�����N�G�X�g
void CameraFlashRequestAll(
    int frame,
    std::function<float(float)> easing,
    Float4 color
)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        CameraFlashRequest(
            i,
            frame,
            easing,
            color
        );
    }
}
void CameraFlashRequest(
    int index,
	int frame,
	std::function<float(float)> easing,
	Float4 color
)
{
    CAMERA& camera = g_Camera[index];
	if (frame > 0)
	{
		CAMERA_FLASH_REQUEST* fr = &camera.flashRequ;
		(*fr) = {};
		fr->frameMax = frame;
		fr->frame = fr->frameMax;

		fr->color = color;

		fr->easing = easing;
	}
}
//�Y�[�����N�G�X�g
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
//�Y�[�����N�G�X�g
void CameraZoomRequestAll(
    float speed,
    Float2 pos,
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
	Float2 pos,
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

		gr->pos = MkF3(pos.x, pos.y, 0.0f);

		gr->easing = easing;
		gr->zoomScale = zoomScale;
	}
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

void SetCameraPosAll(Float2 pos)
{
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        SetCameraPos(i, pos);
    }
}
void SetCameraPos(int index, Float2 pos)
{
    CAMERA& camera = g_Camera[index];
	camera.trans.pos = pos;
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
	}
}