// =============================================================
// 概要   : ポーズメニューの操作を管理する
// 制作者 : Takano Yuta
// 制作日 : 2026/06/26
// =============================================================

using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class PauseMenuController : MonoBehaviour
{
    [Header("表示を切り替えるボタン")]
    [SerializeField] private Button retryButton;
    [SerializeField] private Button stageSelectButton;
    [Header("常時表示するボタン")]
    [SerializeField] private Button settingsButton;
    [SerializeField] private Button backButton;

    private void Start()
    {
        if (retryButton != null)
        {
            retryButton.onClick.AddListener(RetryStage);
        }
        if (stageSelectButton != null)
        {
            stageSelectButton.onClick.AddListener(GoToStageSelect);
        }
        if (settingsButton != null)
        {
            settingsButton.onClick.AddListener(OpenSettings);
        }
        if (backButton != null)
        {
            backButton.onClick.AddListener(ResumeGame);
        }
    }
    private void OnEnable()
    {
        // タイトルとステージ選択では再挑戦・ステージ選択ボタンを隠す。
        string currentScene = SceneManager.GetActiveScene().name;

        if (currentScene == "StageSelect" || currentScene == "Title")
        {
            if (retryButton != null)
            {
                retryButton.gameObject.SetActive(false);
            }
            if (stageSelectButton != null)
            {
                stageSelectButton.gameObject.SetActive(false);
            }
        }
        else
        {
            if (retryButton != null)
            {
                retryButton.gameObject.SetActive(true);
            }
            if (stageSelectButton != null)
            {
                stageSelectButton.gameObject.SetActive(true);
            }
        }
    }

    private void ResumeGame()
    {
        if (PauseManager.Instance != null)
        {
            PauseManager.Instance.SetPause(false);
        }
    }
    private void RetryStage()
    {
        if (PauseManager.Instance != null)
        {
            PauseManager.Instance.SetPause(false);
        }
        FadeManager.Instance.ReloadSceneWithFade();
    }
    private void GoToStageSelect()
    {
        if (PauseManager.Instance != null)
        {
            PauseManager.Instance.SetPause(false);
        }
        FadeManager.Instance.LoadSceneWithFade("StageSelect");
    }
    private void OpenSettings()
    {
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.OpenSettings();
        }
    }
}
