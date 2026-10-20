// =============================================================
// 概要   : ゲームカメラの切り替えを管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/06/27
// =============================================================

using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.Serialization;

public class MoveCameraScript : MonoBehaviour
{
    [Header("カメラの基準点")]
    [FormerlySerializedAs("main")]
    [SerializeField] private GameObject mainCameraAnchor;
    [FormerlySerializedAs("upper")]
    [SerializeField] private GameObject upperCameraAnchor;

    private bool isMainCamera = true;
    private bool inputSubscribed;

    private void OnEnable()
    {
        SubscribeInput();
    }

    private void OnDisable()
    {
        if (inputSubscribed && InputManager.Controls != null)
        {
            InputManager.Controls.Player.CameraSwitch.started -= OnCameraSwitchStarted;
        }
        inputSubscribed = false;
    }

    private void Start()
    {
        SubscribeInput();
        if (mainCameraAnchor != null)
        {
            transform.SetPositionAndRotation(mainCameraAnchor.transform.position, mainCameraAnchor.transform.rotation);
        }
    }

    private void SubscribeInput()
    {
        if (inputSubscribed || InputManager.Controls == null)
        {
            return;
        }

        InputManager.Controls.Player.CameraSwitch.started += OnCameraSwitchStarted;
        inputSubscribed = true;
    }

    private void OnCameraSwitchStarted(InputAction.CallbackContext context)
    {
        ToggleCamera();
    }

    // 切替先が未設定なら現在の位置と状態を維持する。
    private void ToggleCamera()
    {
        GameObject targetAnchor = isMainCamera ? upperCameraAnchor : mainCameraAnchor;
        if (targetAnchor == null)
        {
            return;
        }

        transform.SetPositionAndRotation(targetAnchor.transform.position, targetAnchor.transform.rotation);
        isMainCamera = !isMainCamera;
    }
}
