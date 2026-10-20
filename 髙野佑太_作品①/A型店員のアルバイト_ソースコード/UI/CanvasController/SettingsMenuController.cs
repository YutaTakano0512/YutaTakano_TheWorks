// =============================================================
// 概要   : 設定メニューの操作を管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/06/27
// =============================================================

using UnityEngine;
using UnityEngine.UI;

public class SettingsMenuController : MonoBehaviour
{
    [Header("音量調整スライダー")]
    [SerializeField] private Slider bgmSlider;
    [SerializeField] private Slider seSlider;

    [Header("ボタン")]
    [SerializeField] private Button backButton;

    private void Start()
    {
        if (AudioManager.Instance == null)
        {
            Debug.LogError("AudioManagerが見つかりません。");
            return;
        }

        // 保存済み音量を反映してから変更イベントを登録する。
        if (bgmSlider != null)
        {
            bgmSlider.value = AudioManager.Instance.masterBgmVolume;
            bgmSlider.onValueChanged.AddListener(AudioManager.Instance.SetBGMVolume);
        }

        if (seSlider != null)
        {
            seSlider.value = AudioManager.Instance.masterSeVolume;
            seSlider.onValueChanged.AddListener (AudioManager.Instance.SetSEVolume);
        }

        backButton.onClick.AddListener(() =>
            {
                if (AudioManager.Instance != null)
                {
                    CloseMenu();
                }
            });
    }

    private void CloseMenu()
    {
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.CloseSettings();
        }
    }
}
