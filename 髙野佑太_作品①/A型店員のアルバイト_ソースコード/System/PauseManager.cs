// =============================================================
// 概要   : 一時停止状態とポーズUIを管理する
// 制作者 : Takano Yuta
// 制作日 : 2026/06/26
// =============================================================

using System;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.InputSystem;

public class PauseManager : MonoBehaviour
{
    public static PauseManager Instance { get; private set; }

    [Header("ポーズ用Canvasのプレハブをセット")]
    [SerializeField] private GameObject pauseCanvasPrefab;
    private GameObject pauseCanvasInstance; // 生成したCanvas全体
    public bool IsPaused { get; private set; } = false;

    private PlayerControls controls;

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(this.gameObject);
        }
        else
        {
            Destroy(this.gameObject);
            return;
        }

        // ゲーム起動時にCanvasを1回だけ生成して引き継ぐ
        if (pauseCanvasPrefab != null && pauseCanvasInstance == null)
        {
            pauseCanvasInstance = Instantiate(pauseCanvasPrefab);
            DontDestroyOnLoad(pauseCanvasInstance);
            pauseCanvasInstance.SetActive(false);
        }

        controls = new PlayerControls();
        controls.Player.Pause.started += OnPauseStarted;
    }

    private void OnEnable()
    {
        controls?.Enable();
    }
    private void OnDisable()
    {
        controls?.Disable();
    }

    private void OnDestroy()
    {
        if (Instance == this)
        {
            controls.Player.Pause.started -= OnPauseStarted;
            controls.Dispose();
            Instance = null;
        }
    }

    private void OnPauseStarted(InputAction.CallbackContext context)
    {
        TogglePause();
    }

    public void TogglePause()
    {
        if (SceneManager.GetActiveScene().name == "Title")
        {
            return;
        }
        SetPause(!IsPaused);
    }

    public void SetPause(bool pause)
    {
        if (SceneManager.GetActiveScene().name == "Title")
        {
            return;
        }
        if (IsPaused == pause)
        {
            return;
        }

        IsPaused = pause;
        Time.timeScale = IsPaused ? 0f : 1f;

        if (pauseCanvasInstance != null)
        {
            pauseCanvasInstance.SetActive(IsPaused);
        }

        Hands hands = FindFirstObjectByType<Hands>();
        if (hands != null)
        {
            hands.enabled = !IsPaused;
        }
    }
}
