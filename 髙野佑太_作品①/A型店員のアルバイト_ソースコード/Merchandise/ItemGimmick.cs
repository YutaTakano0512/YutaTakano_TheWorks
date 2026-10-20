// =============================================================
// 概要   : 商品の共通状態とスコアを管理する
// 制作者 : 成岡孝耶
// 制作日 : 2025/10/24
// 加筆: 小野田喬 2025/10/30
// 加筆: 髙野佑太 2026/07/03
// =============================================================

using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Serialization;

public class ItemGimmick : MonoBehaviour
{
    [Header("商品の設定")]
    [SerializeField] private string itemName = "商品名";
    [SerializeField] private int itemScore = 1000;                  // 商品のスコア
    [SerializeField] private float heldLimitTime = 10.0f;           // 手に持たれる限界時間

    [Header("商品のパラメータ")]
    private bool isHeld;    // 手に持たれるかどうか
    private float heldTime; // 手に持たれている時間

    [Header("かご判定")]
    [FormerlySerializedAs("inBasket")]
    [SerializeField] private bool isInBasket;                       // かごに入っているかどうか
    [SerializeField] private float inBasketTime;                    // かごに入っている時間
    [SerializeField] private bool isVelocityZero;                   // ベロシティがゼロかどうか
    [FormerlySerializedAs("isOK")]
    [SerializeField] private bool isSettledInBasket;                // かごに入り切っているかどうか
    [SerializeField] private float inBasketUntilTime = 2.0f;        // かごに入りきるまでの時間

    [Header("状態")]
    private bool isSpoiled;                                         // 商品が悪くなっているかどうか
    private bool inHotField;                                        // 熱いフィールドに入っているかどうか
    private Vector3 deskPosition;
    private Rigidbody itemRigidbody;
    private bool popupShown = false;

    // 外部から追加された属性テキスト（「熱い」「とげとげ」など）を保持するリスト
    private List<string> attributeList = new List<string>();

    private void Awake()
    {
        itemRigidbody = GetComponent<Rigidbody>();
    }

    private void Start()
    {
        // 初期化
        isHeld = false;
        isInBasket = false;
        isSettledInBasket = false;
        isVelocityZero = false;
        isSpoiled = false;

        heldTime = 0.0f;
        inBasketTime = 0.0f;

        deskPosition = transform.position;
    }

    private void Update()
    {
        // Rigidbodyのベロシティがゼロかどうか判定
        if (itemRigidbody != null && itemRigidbody.linearVelocity.sqrMagnitude <= 0.01f &&
            itemRigidbody.angularVelocity.sqrMagnitude <= 0.01f)
        {
            isVelocityZero = true;
        }
        else
        {
            isVelocityZero = false;
        }

        // 手に持っている処理
        if (isHeld)
        {
            heldTime += Time.deltaTime;

            if (heldTime >= heldLimitTime)
            {
                heldTime = 0.0f;
                isHeld = false;
            }
        }

        // かごに入っているかの時間計測
        if (isInBasket)
        {
            if (isVelocityZero)
            {
                inBasketTime += Time.deltaTime;

                if (inBasketTime >= inBasketUntilTime)
                {
                    inBasketTime = 0.0f;
                    isInBasket = false;
                    isSettledInBasket = true; // かごに入り切ったと判定
                }
            }
            else
            {
                inBasketTime = 0.0f;
            }
        }

        // 悪くなったらポップアップを出す
        if (isSpoiled && !popupShown)
        {
            popupShown = true;
            if (PopupManager.Instance != null)
            {
                PopupManager.Instance.ShowPopup(transform.position, "Bad...", Color.red);
            }
        }
    }

    // 落下した後机の上に戻す処理
    public void ReturnToDesk()
    {
        transform.position = deskPosition;

        if (itemRigidbody != null)
        {
            itemRigidbody.linearVelocity = Vector3.zero;
            itemRigidbody.angularVelocity = Vector3.zero;
        }
        SetHeld(false);
    }

    // Gimmick側から商品をダメにする処理
    public void SpoilItem()
    {
        if (!isSpoiled)
        {
            Debug.Log("商品が悪くなりました: " + itemName);
            isSpoiled = true;
            ReduceScoreHalf();
        }
    }

    // =======================================================
    // ゲッター・セッター・便利関数
    // =======================================================

    public bool GetHeld()
    {
        return isHeld;
    }
    public void SetHeld(bool value)
    {
        isHeld = value;
    }

    public bool GetInBasket()
    {
        return isInBasket;
    }
    public void SetInBasket(bool value)
    {
        isInBasket = value;
        if (!value)
        {
            isSettledInBasket = false;
            inBasketTime = 0.0f;
        }
    }

    public bool GetIsInBasket()
    {
        return isSettledInBasket;
    }

    public void SetHotField(bool value)
    {
        inHotField = value;
    }
    public bool GetInHotField()
    {
        return inHotField;
    }

    public bool GetIsSpoiled()
    {
        return isSpoiled;
    }

    public float GetRemainingHoldTime()
    {
        return Mathf.Max(0, heldLimitTime - heldTime);
    }

    public string GetItemName()
    {
        return itemName;
    }

    public float GetHoldLimitTime()
    {
        return heldLimitTime;
    }
    public void SetHoldLimitTime(float newLimitTime)
    {
        heldLimitTime = newLimitTime;
    }

    public int GetItemScore()
    {
        return itemScore;
    }
    public void ReduceScoreHalf()
    {
        itemScore /= 2;
    }

    // 属性テキスト関連
    public void AddAttributeText(string text)
    {
        if (!attributeList.Contains(text))
        {
            attributeList.Add(text);
        }
    }

    public string GetAttributeText()
    {
        if (attributeList.Count == 0)
        {
            return "普通";
        }
        return string.Join("、", attributeList);
    }
}
