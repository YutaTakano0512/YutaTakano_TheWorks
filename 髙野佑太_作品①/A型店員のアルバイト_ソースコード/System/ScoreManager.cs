// =============================================================
// 概要   : 商品スコアとゲーム中のゲージを管理する
// 制作者 : Takano Yuta
// 制作日 : 2026/06/27
// =============================================================

using UnityEngine;
using System.Collections.Generic;

public class ScoreManager : MonoBehaviour
{
    public static ScoreManager Instance {  get; private set; }

    [Header("現在のスコア")]
    public int currentScore = 0;

    [Header("最大スコア")]
    public int maxScore = 3000; // ゲージや星評価の基準となる最大値

    [Header("元の最大スコア（ペナルティなしの満点）")]
    public int originalMaxScore = 3000;

    [Header("落とした回数")]
    public int dropCount = 0;

    // リザルト画面に渡す「商品ごとの結果」のリスト
    public List<string> resultLogList = new List<string>();

    // InGameControllerのキャッシュ
    private InGameController cachedInGameUI;

    [Header("アイテムリスト用")]
    public Dictionary<string, int> totalItemCounts = new Dictionary<string, int>();
    private Dictionary<string, int> currentItemCounts = new Dictionary<string, int>();

    private void Awake()
    {
        Instance = this;
    }

    private void Start()
    {
        // シーン内に配置されているすべての商品(ItemGimmick)を探し出す
        ItemGimmick[] items = Object.FindObjectsByType<ItemGimmick>(FindObjectsSortMode.None);

        // 見つかった商品の数 × 1000点 を最大スコアとして自動設定する
        if (items != null && items.Length > 0)
        {
            maxScore = 0;
            // アイテムの個数を種類ごとに集計しつつ最大スコアを計算
            foreach (var item in items)
            {
                maxScore += item.GetItemScore();
                string name = item.GetItemName();
                if (totalItemCounts.ContainsKey(name))
                {
                    totalItemCounts[name]++;
                }
                else
                {
                    totalItemCounts[name] = 1;
                }
            }
            originalMaxScore = maxScore;
        }

        UpdateGaugeUI();
    }

    // 商品を落としたときの処理
    public void AddDropPenalty(ItemGimmick item)
    {
        dropCount++;

        // ペナルティとしてその商品のスコアを半減させる
        item.ReduceScoreHalf();

        UpdateGaugeUI();

    }
    public void AddItemResult(ItemGimmick item)
    {
        int addScore = item.GetItemScore();
        string resultText = "";
        if (item.GetIsSpoiled())
        {
            resultText = $"{item.GetItemName()}(ダメにしてしまった) : +{addScore}点";
        }
        else
        {
            resultText = $"{item.GetItemName()} : +{addScore}点";
        }
        currentScore += addScore;
        resultLogList.Add(resultText);

        UpdateGaugeUI();
    }

    // ゲーム中のゲージを更新する
    private void UpdateGaugeUI()
    {
        if (cachedInGameUI == null)
        {
            cachedInGameUI = Object.FindFirstObjectByType<InGameController>();
        }

        if (cachedInGameUI != null)
        {
            cachedInGameUI.UpdateGauge(currentScore, maxScore);
        }
    }


    // カゴの中身をもとに、ゲーム中のゲージを仮計算して更新する
    public void UpdateTempGauge(List<ItemGimmick> basketItems)
    {
        int tempScore = currentScore; // 落とした減点などは currentScore に反映されている想定
        currentItemCounts.Clear();

        foreach (var item in basketItems)
        {
            if (item.GetHeld())
            {
                continue; // 手に持っている間は仮スコア・カウントから除外
            }

            // リスト表示用にカウント
            string name = item.GetItemName();
            if (currentItemCounts.ContainsKey(name))
            {
                currentItemCounts[name]++;
            }
            else
            {
                currentItemCounts[name] = 1;
            }

            tempScore += item.GetItemScore();
        }

        if (cachedInGameUI == null)
        {
            cachedInGameUI = Object.FindFirstObjectByType<InGameController>();
        }

        if (cachedInGameUI != null)
        {
            cachedInGameUI.UpdateGauge(tempScore, maxScore);
            cachedInGameUI.UpdateItemList(totalItemCounts, currentItemCounts);
        }
    }
}
