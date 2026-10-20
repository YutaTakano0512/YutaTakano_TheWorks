// =============================================================
// 概要   : ステージのかご生成とクリア状態を管理する
// 制作者 : 小野田喬
// 制作日 : 2025/10/30
// =============================================================

using UnityEngine;

public class StageManager : MonoBehaviour
{
    [Header("バスケット生成用")]
    [SerializeField] private GameObject basketPrefab;
    [SerializeField] private GameObject basketCheckerPrefab;

    [Header("デバッグ用:バスケットのスポーン位置")]
    [SerializeField] private GameObject debugBasketSpawnPoint;
    private GameObject curBasket;
    private GameObject curBasketChecker;

    [Header("ゲーム状態")]
    [SerializeField] private bool isCleared = false;

    [Header("経過時間")]
    private float elapsedTime;

    private void Start()
    {
        SpawnBasket();
        SpawnBasketChecker();
    }
    private void Update()
    {
        elapsedTime += Time.deltaTime;
    }
    private void SpawnBasket()
    {
        Quaternion q = Quaternion.identity;
        if (transform.name == "BasketBoxX")
        {
            q = Quaternion.Euler(0, 90, 0);
        }
        curBasket = Instantiate(basketPrefab, debugBasketSpawnPoint.transform.position, q);
    }

    private void SpawnBasketChecker()
    {
        Quaternion q = Quaternion.identity;
        curBasketChecker = Instantiate(basketCheckerPrefab, curBasket.transform.position, q);
        BasketChecker checker = curBasketChecker.GetComponent<BasketChecker>();
        checker.SetManager(this.gameObject);
        checker.Initialize(curBasket);
    }

    // ゲームクリア（すべてのかごに入った）時に呼ばれる
    public void SetGameClear()
    {
        if (isCleared)
        {
            return;
        }

        isCleared = true;
        Debug.Log("Game Cleared!");
        // ScoreManager から結果を取得して UI に渡す
        if (ScoreManager.Instance != null && ResultCanvasManager.Instance != null)
        {
            int totalScore = ScoreManager.Instance.currentScore;
            int dropCount = ScoreManager.Instance.dropCount;
            var logs = ScoreManager.Instance.resultLogList;

            ResultCanvasManager.Instance.ShowResultUI(totalScore, dropCount, logs);
        }
    }

    public bool GetIsCleared()
    {
        return isCleared;
    }

    public float GetElapsedTime()
    {
        return elapsedTime;
    }
}
