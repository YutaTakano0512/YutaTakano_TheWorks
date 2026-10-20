// =============================================================
// 概要   : 軽い商品の圧壊判定とアニメーションを管理する
// 制作者 : 髙野佑太
// 制作日 : 2026/08/19
// =============================================================

using UnityEngine;
using UnityEngine.Serialization;

public class GimmickLight : MonoBehaviour
{
    private ItemGimmick itemGimmick;
    private Vector3 originalScale;
    private Vector3 targetScale;

    [Header("軽い商品のギミック設定")]
    [FormerlySerializedAs("scaleMultiple")]
    [FormerlySerializedAs("finalCrushedScale")]
    [SerializeField] private Vector3 scaleMultiplier = new Vector3(1.1f, 0.5f, 1.1f); // 押しつぶされたときのサイズ

    private Vector3 originalPosition;
    private bool isCrushed;
    private bool isAnimationDone;

    [FormerlySerializedAs("crushLightTime")]
    [SerializeField] private float crushDuration = 0.5f;
    private float crushElapsedTime;

    private float aliveTime = 0.0f;

    private void Start()
    {
        itemGimmick = GetComponent<ItemGimmick>();
        if (itemGimmick != null)
        {
            itemGimmick.AddAttributeText("軽い");
        }
        originalScale = transform.localScale;
        targetScale = Vector3.Scale(originalScale, scaleMultiplier);
    }

    private void Update()
    {
        aliveTime += Time.deltaTime;

        if (itemGimmick == null || !isCrushed || isAnimationDone)
        {
            return;
        }

        transform.rotation = Quaternion.identity;

        if (crushElapsedTime < crushDuration)
        {
            crushElapsedTime += Time.deltaTime;
            float t = Mathf.Clamp01(crushElapsedTime / crushDuration);
            Vector3 newScale = Vector3.Lerp(originalScale, targetScale, t);
            transform.localScale = newScale;

            float offsetY = (originalScale.y - newScale.y) * 0.5f;
            transform.position = originalPosition + Vector3.down * offsetY;
        }
        else
        {
            transform.localScale = targetScale;
            float finalOffsetY = (originalScale.y - targetScale.y) * 0.5f;
            transform.position = originalPosition + Vector3.down * finalOffsetY;
            isAnimationDone = true;
            itemGimmick.SpoilItem();
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        CheckCrush(collision);
    }

    private void OnCollisionStay(Collision collision)
    {
        CheckCrush(collision);
    }

    private void CheckCrush(Collision collision)
    {
        if (aliveTime < 1.0f)
        {
            return; // 起動直後の無敵時間
        }

        if (isCrushed)
        {
            return;
        }
        // 相手が重い商品かチェック
        if (!GimmickHeavy.IsOnTopOf(collision, transform))
        {
            return;
        }

        isCrushed = true;
        originalPosition = transform.position;
        crushElapsedTime = 0f;
    }
}
