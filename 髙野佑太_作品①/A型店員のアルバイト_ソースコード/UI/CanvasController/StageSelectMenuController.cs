// =============================================================
// 概要   : ステージ選択メニューの操作を管理する
// 制作者 : Takano Yuta
// 制作日 : 2026/6/28
// =============================================================

using UnityEngine;
using UnityEngine.UI;

public class StageSelectMenuController : MonoBehaviour
{
    [Header("ステージボタン")]
    [SerializeField] private Button[] stageButtons;

    [Header("戻るボタン")]
    [SerializeField] private Button backButton;

    [Header("ステージのシーン名の前置き")]
    [SerializeField] private string sceneNamePrefix = "Stage";

    private void Start()
    {
        // 各ボタンに対応するシーン名をループごとに固定する。
        for (int i = 0; i < stageButtons.Length; i++)
        {
            int stageNumber = i + 1;
            string sceneName = sceneNamePrefix + stageNumber;

            stageButtons[i].onClick.AddListener(() =>
            {
                if (FadeManager.Instance != null)
                {
                    FadeManager.Instance.LoadSceneWithFade(sceneName);
                }
            });
        }

        backButton.onClick.AddListener(() =>
        {
            if (FadeManager.Instance != null)
            {
                FadeManager.Instance.LoadSceneWithFade("Title");
            }
        } );
    }
}
