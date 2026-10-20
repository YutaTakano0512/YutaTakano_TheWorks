// =============================================================
// 概要   : タイトルメニューの操作を管理する
// 制作者 : Takano Yuta
// 制作日 : 2026/6/25
// =============================================================

using DG.Tweening;
using UnityEngine;
using UnityEngine.UI;

public class TitleMenuController : MonoBehaviour
{
    [Header("メインメニューボタン")]
    [SerializeField] private Button playButton;
    [SerializeField] private Button settingButton;
    [SerializeField] private Button exitButton;

    [Header("設定メニューボタン")]
    [SerializeField] private Button settingsBackButton;

    private void Start()
    {
        // 画面遷移は FadeManager を通して演出と入力を揃える。
        playButton.onClick.AddListener(() =>
        {
            if (FadeManager.Instance != null)
            {
                FadeManager.Instance.LoadSceneWithFade("StageSelect");
            }
        });

        settingButton.onClick.AddListener(() =>
        {
            if (AudioManager.Instance != null)
            {
                AudioManager.Instance.OpenSettings();
            }
        });

        exitButton.onClick.AddListener(() =>
        {
            if (FadeManager.Instance != null)
            {
                FadeManager.Instance.ExitGameWithFade();
            }
        });
    }
}
