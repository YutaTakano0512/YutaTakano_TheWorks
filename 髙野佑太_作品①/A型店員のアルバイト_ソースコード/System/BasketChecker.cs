// =============================================================
// 概要   : かご内の商品とステージクリアを判定する
// 制作者 : 小野田喬
// 制作日 : 2025/10/29
// 追記: 髙野佑太 2026/07/03
// =============================================================

using UnityEngine;
using System.Collections.Generic;

public class BasketChecker : MonoBehaviour
{
    //コリジョン生成用
    [Header("対角の頂点A、B")]
    private Vector3 pointA;
    private Vector3 pointB;

    [Header("生成されるBoxCollider")]
    private BoxCollider boxCollider;

    //参照
    private StageManager cachedStageManager;

    //変数
    private readonly List<ItemGimmick> insideItems = new();
    private int totalItemCount;
    private int lastGaugeSignature;
    private bool hasGaugeSignature;

    // 手に持っている商品との衝突除外にも使うため、かご内の商品を公開する。
    public List<ItemGimmick> GetInsideItems()
    {
        return insideItems;
    }


    public void Initialize(GameObject basket)
    {
        Transform vertexA = basket.transform.Find("VertexLeftBackUp");
        Transform vertexB = basket.transform.Find("VertexRightFrontDown");

        if (vertexA == null || vertexB == null)
        {
            Debug.LogError("BasketChecker: Collider生成用頂点が見つかりません。");
            return;
        }

        pointA = vertexA.position;
        pointB = vertexB.position;

        if (boxCollider == null)
        {
            boxCollider = GetComponent<BoxCollider>();
            if (boxCollider == null)
            {
                boxCollider = gameObject.AddComponent<BoxCollider>();
            }
        }

        Vector3 center = basket.transform.position;
        center += new Vector3(0.0f, 0.5f, 0.0f);

        Vector3 size = new Vector3(
            Mathf.Abs(pointA.x - pointB.x),
            Mathf.Abs(pointA.y - pointB.y),
            Mathf.Abs(pointA.z - pointB.z)
        );

        boxCollider.center = transform.InverseTransformPoint(center);
        boxCollider.size = size;

        boxCollider.isTrigger = true;
    }



    private void Start()
    {
        totalItemCount = GameObject.FindGameObjectsWithTag("Merchandise").Length;
        Debug.Log("Total Item Count: " + totalItemCount);
    }


    private void Update()
    {
        // Destroy 済みの商品を除外し、現在の表示内容を表す値を作る。
        int gaugeSignature = 17;
        for (int i = insideItems.Count - 1; i >= 0; i--)
        {
            ItemGimmick item = insideItems[i];
            if (item == null)
            {
                insideItems.RemoveAt(i);
                continue;
            }

            unchecked
            {
                gaugeSignature = gaugeSignature * 31 + item.GetInstanceID();
                gaugeSignature = gaugeSignature * 31 + item.GetItemScore();
                gaugeSignature = gaugeSignature * 31 + (item.GetHeld() ? 1 : 0);
                gaugeSignature = gaugeSignature * 31 + (item.GetIsInBasket() ? 1 : 0);
            }
        }

        int clearedItemCount = 0;

        foreach (var item in insideItems)
        {
            // クリアのチェック（手に持っている間はクリア対象外にする）
            if (item.GetIsInBasket() && !item.GetHeld())
            {
                clearedItemCount++;
            }
        }

        // ゲーム中のゲージをカゴの中身に合わせてリアルタイム更新
        if (cachedStageManager != null && !cachedStageManager.GetIsCleared())
        {
            if (ScoreManager.Instance != null)
            {
                unchecked
                {
                    gaugeSignature = gaugeSignature * 31 + ScoreManager.Instance.currentScore;
                    gaugeSignature = gaugeSignature * 31 + ScoreManager.Instance.maxScore;
                }

                if (!hasGaugeSignature || gaugeSignature != lastGaugeSignature)
                {
                    ScoreManager.Instance.UpdateTempGauge(insideItems);
                    lastGaugeSignature = gaugeSignature;
                    hasGaugeSignature = true;
                }
            }
        }

        if (totalItemCount > 0 && clearedItemCount == totalItemCount)
        {
            if (cachedStageManager == null)
            {
                Debug.LogError("StageManager is missing.");
                return;
            }

            // 既にクリア済みなら重複して呼ばない
            if (cachedStageManager.GetIsCleared())
            {
                return;
            }

            if (ScoreManager.Instance != null)
            {
                foreach (var item in insideItems)
                {
                    // 商品を渡す
                    ScoreManager.Instance.AddItemResult(item);
                }
            }

            //全ての商品がカゴに入ったのでゲームクリアをStageManagerに伝える
            cachedStageManager.SetGameClear();
        }

    }

    private void OnTriggerEnter(Collider other)
    {
        // HotFieldは反応しないようにする
        if (other.gameObject.layer == LayerMask.NameToLayer("HotField"))
        {
            return;
        }

        //ルートを取ります
        Transform root = other.transform.root;

        if (root.CompareTag("Merchandise"))
        {
            //商品を一時変数にセット
            ItemGimmick item = root.GetComponent<ItemGimmick>();

            //商品がinBasketリストの中に入ってなければ追加、入っていればスルー
            if (item != null && !insideItems.Contains(item))
            {
                insideItems.Add(item);

                Debug.Log("Item added to basket: " + item.name);

                //ここで商品がバスケットに入ったというフラグを立てる
                item.SetInBasket(true);
            }
        }
    }

    private void OnTriggerExit(Collider other)
    {
        // HotFieldは反応しないようにする
        if (other.gameObject.layer == LayerMask.NameToLayer("HotField"))
        {
            return;
        }

        //ルートを取ります
        Transform root = other.transform.root;

        if (root.CompareTag("Merchandise"))
        {
            //商品を一時変数にセット
            ItemGimmick item = root.GetComponent<ItemGimmick>();

            //商品がinBasketリストの中に入っていたものだったらゲームオーバー
            if (item != null && insideItems.Contains(item))
            {
                //ここで商品がバスケットから出たというフラグを立てる
                item.SetInBasket(false);
                //リストから削除
                insideItems.Remove(item);
            }
        }
    }

    public void SetManager(GameObject manager)
    {
        cachedStageManager = manager != null ? manager.GetComponent<StageManager>() : null;
        hasGaugeSignature = false;
    }
}

