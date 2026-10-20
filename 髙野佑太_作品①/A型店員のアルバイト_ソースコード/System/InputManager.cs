// =============================================================
// 概要   : プレイヤー入力の生成と寿命を管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/08/18
// =============================================================

using UnityEngine;

public class InputManager : MonoBehaviour
{
    public static InputManager Instance { get; private set; }
    public static PlayerControls Controls { get; private set; }
    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            Controls = new PlayerControls();
            Controls.Enable();
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }
    private void OnDestroy()
    {
        if (Instance == this && Controls != null)
        {
            // 終了後に破棄済みの InputAction を参照しないようにする。
            Controls.Dispose();
            Controls = null;
            Instance = null;
        }
    }
}
