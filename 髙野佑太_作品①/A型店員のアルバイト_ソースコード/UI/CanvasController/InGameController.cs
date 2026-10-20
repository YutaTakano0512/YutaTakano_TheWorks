// =============================================================
// 概要   : インゲームのUIを管理するスクリプト
// 制作者 : Takano Yuta
// 制作日 : 2026/6/25
// =============================================================

using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System.Collections.Generic;
using System.Text;
using UnityEngine.Serialization;

public class InGameController : MonoBehaviour
{
    [Header("ポーズボタン")]
    [FormerlySerializedAs("pouseButton")]
    [SerializeField] private Button pauseButton;

    [Header("Gageの設定")]
    [SerializeField] private Image gaugeFillImage;
    [SerializeField] private TextMeshProUGUI scoreText;

    [Header("アイテムリスト")]
    [SerializeField] private TextMeshProUGUI itemListText;
    private readonly StringBuilder itemListBuilder = new StringBuilder();

    private void Start()
    {
        pauseButton.onClick.AddListener(() =>
        {
            if (PauseManager.Instance != null)
            {
                PauseManager.Instance.TogglePause();
            }
        });

    }

    public void UpdateGauge(int currentScore, int maxScore)
    {
        if (gaugeFillImage == null || maxScore <= 0)
        {
            return;
        }

        float scoreRatio = (float)currentScore / maxScore;
        // 念のため0〜1の間に制限する
        gaugeFillImage.fillAmount = Mathf.Clamp01(scoreRatio);

        if (scoreText != null)
        {
            scoreText.text = $"Score：{currentScore}";
        }
    }

    public void UpdateItemList(Dictionary<string, int> totalCounts, Dictionary<string, int> currentCounts)
    {
        if (itemListText == null)
        {
            return;
        }

        itemListBuilder.Clear();
        foreach (var kvp in totalCounts)
        {
            string itemName = kvp.Key;
            int maxCount = kvp.Value;
            currentCounts.TryGetValue(itemName, out int currentCount);

            // 例: りんご (1/2)
            string line = $"{itemName} ({currentCount}/{maxCount})";

            // 全て入っていれば取り消し線を引く
            if (currentCount >= maxCount)
            {
                line = $"<s>{line}</s>";
            }

            itemListBuilder.Append(line).Append('\n');
        }

        itemListText.text = itemListBuilder.ToString();
    }
}
