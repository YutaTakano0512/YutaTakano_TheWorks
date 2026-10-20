// =============================================================
// 概要   : リザルト画面の評価とスコア表示を管理する
// 制作者 : Takano Yuta
// 制作日 : 2026/06/26
// =============================================================

using UnityEngine;
using UnityEngine.UI;
using DG.Tweening;
using TMPro;
using System.Collections.Generic;
public class ResultCanvasManager : MonoBehaviour
{
    public static ResultCanvasManager Instance { get; private set; }

    [Header("星の評価設定")]
    [SerializeField] private GameObject[] starObjects; // 明るい星のオブジェクト（Star1, Star2, Star3）

    [Header("評価の基準（割合：0.0〜1.0）")]
    [SerializeField] private float oneStarThreshold = 0.33f; // 星1つの基準
    [SerializeField] private float twoStarThreshold = 0.66f; // 星2つの基準
    [SerializeField] private float threeStarThreshold = 1.0f; // 星3つの基準

    [Header("UI要素")]
    [SerializeField] private GameObject resultBG; // "すべて入れることができた！"の画像
    [SerializeField] private TextMeshProUGUI scoreText; // スコアを表示するテキスト
    [SerializeField] private Button nextButton; // ステージセレクトへのボタン

    [Header("SE")]
    [SerializeField] private AudioClip clearSE;

    private void Awake()
    {
        Instance = this;
    }
    private void Start()
    {
        // 最初はUIを非表示にしておく
        resultBG.SetActive(false);
        nextButton.gameObject.SetActive(false);
        if (scoreText != null)
        {
            scoreText.gameObject.SetActive(false);
        }

        // 星を最初はすべて非表示にしておく
        if (starObjects != null)
        {
            foreach (var star in starObjects)
            {
                if (star != null)
                {
                    star.SetActive(false);
                }
            }
        }

        // ボタンが押されたときの処理
        nextButton.onClick.AddListener(() =>
        {
            if (FadeManager.Instance != null)
            {
                FadeManager.Instance.LoadSceneWithFade("StageSelect");
            }
        });
    }
    // StageManagerからクリア時に呼ばれる処理
    public void ShowResultUI(int totalScore, int dropCount, List<string> logs)
    {
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.PlaySE(clearSE);
        }

        // 1. ResultBG（画像）を中央からもわっと出す
        resultBG.SetActive(true);
        resultBG.transform.localScale = Vector3.zero; // 最初は大きさを0に

        // 大きさを1に戻すアニメーション（OutBackで少し跳ねるような動き）
        resultBG.transform.DOScale(Vector3.one, 0.5f).SetEase(Ease.OutBack).SetUpdate(true).OnComplete(() =>
        {
            // 順番にアニメーションを実行するためのSequenceを作成
            Sequence seq = DOTween.Sequence();
            seq.SetUpdate(true); // ポーズ中でも動くようにする

            // 星評価のアニメーション
            int starCount = GetStarCount(totalScore);
            if (starObjects != null)
            {
                for (int i = 0; i < starObjects.Length; i++)
                {
                    if (starObjects[i] == null)
                    {
                        continue;
                    }

                    if (i < starCount)
                    {
                        GameObject star = starObjects[i];
                        // 順番に出すためにAppendCallbackとAppendを使用
                        seq.AppendCallback(() =>
                        {
                            star.transform.localScale = Vector3.zero;
                            star.SetActive(true);
                        });
                        seq.Append(star.transform.DOScale(Vector3.one, 0.3f).SetEase(Ease.OutBack));
                    }
                }
            }

            // 星が出終わってから少し待つ
            seq.AppendInterval(0.3f);

            // スコアテキストのアニメーション表示
            if (scoreText != null)
            {
                seq.AppendCallback(() =>
                {
                    int displayScore = totalScore;
                    if (ScoreManager.Instance != null && ScoreManager.Instance.maxScore > 0)
                    {
                        float ratio = (float)totalScore / ScoreManager.Instance.maxScore;
                        displayScore = Mathf.RoundToInt(ScoreManager.Instance.originalMaxScore * ratio);
                    }

                    scoreText.gameObject.SetActive(true);
                    scoreText.text = $"スコア : {displayScore}";
                    // 透明にしておく
                    Color c = scoreText.color;
                    c.a = 0;
                    scoreText.color = c;
                });
                seq.Append(scoreText.DOFade(1f, 0.5f));
            }

            // スコアが出終わってから少し待つ
            seq.AppendInterval(0.3f);

            // ステージセレクトボタンのアニメーション表示
            seq.AppendCallback(() =>
            {
                nextButton.gameObject.SetActive(true);
                nextButton.transform.localScale = Vector3.zero;
            });
            seq.Append(nextButton.transform.DOScale(Vector3.one, 0.3f).SetEase(Ease.OutBack));
        });
    }

    /// <summary>
    /// リザルト画面で星の評価を計算して星の数を返す
    /// </summary>
    private int GetStarCount(int totalScore)
    {
        // ScoreManagerから最大スコアを取得（念のため1以上にする）
        int maxScore = 3000;
        if (ScoreManager.Instance != null && ScoreManager.Instance.maxScore > 0)
        {
            maxScore = ScoreManager.Instance.maxScore;
        }

        // スコア割合を計算 (0.0 〜 1.0 に収める)
        float scoreRatio = Mathf.Clamp01((float)totalScore / maxScore);
        int starCount = 0;

        if (scoreRatio >= threeStarThreshold)
        {
            starCount = 3;
        }
        else if (scoreRatio >= twoStarThreshold)
        {
            starCount = 2;
        }
        else if (scoreRatio >= oneStarThreshold)
        {
            starCount = 1;
        }

        return starCount;
    }
}
