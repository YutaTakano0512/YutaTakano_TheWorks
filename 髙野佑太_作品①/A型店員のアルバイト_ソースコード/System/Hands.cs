// =============================================================
// 概要   : 商品を掴み、移動・回転・解放する
// 制作者 : 桐ケ谷 健浩
// 制作日 : 要確認
// 追記: 髙野佑太 2026/06/27
// =============================================================

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.Rendering;
using UnityEngine.Serialization;

public class Hands : MonoBehaviour
{
    [Header("掴んでいるかどうかの判定")]
    [FormerlySerializedAs("Held")]
    [SerializeField] private bool isHeld;
    [Header("パラメータ")]
    [FormerlySerializedAs("HOLD_HEIGHT")]
    [SerializeField] private float holdHeight = 1.0f;
    [Header("保持中の見た目")]
    [Range(0.1f, 1.0f)]
    [SerializeField] private float heldItemOpacity = 0.5f;
    private const float AngleAcceleration = 10.0f;
    private const float CameraRayLength = 100.0f;

    private ItemGimmick heldItemGimmick;
    private Rigidbody heldRigidbody;
    private Transform heldTransform;
    private Camera gameplayCamera;
    private bool inputSubscribed;
    private readonly Dictionary<Renderer, Material[]> originalMaterials = new Dictionary<Renderer, Material[]>();
    private readonly List<Material> transparentMaterials = new List<Material>();

    public bool GetHeld() { return isHeld; }
    public void SetHeld(bool held) { isHeld = held; }
    private void OnEnable()
    {
        SubscribeInput();
    }

    private void Start()
    {
        isHeld = false;
        gameplayCamera = Camera.main;
        SubscribeInput();
    }
    private void OnDisable()
    {
        if (inputSubscribed && InputManager.Controls != null)
        {
            InputManager.Controls.Player.Grab.started -= OnGrabStarted;
        }
        inputSubscribed = false;
        RestoreHeldItemMaterials();

        if (GameCursorUI.Instance != null)
        {
            GameCursorUI.Instance.HideItemInfo();
        }
        if (isHeld)
        {
            // ポーズなどで無効化された場合も、かご内商品との衝突を戻す。
            SetIgnoreCollisionWithBasketItems(false);
            if (heldItemGimmick != null)
            {
                heldItemGimmick.SetHeld(false);
            }
            if (heldRigidbody != null)
            {
                heldRigidbody.isKinematic = false;
            }

            ClearCache();
            isHeld = false;
        }
    }

    private void OnGrabStarted(InputAction.CallbackContext context)
    {
        OnClickGrab();
    }

    private void SubscribeInput()
    {
        if (inputSubscribed || InputManager.Controls == null)
        {
            return;
        }

        InputManager.Controls.Player.Grab.started += OnGrabStarted;
        inputSubscribed = true;
    }

    // クリックした瞬間にだけ呼ばれる（掴む / 離す）
    private void OnClickGrab()
    {
        if (gameplayCamera == null)
        {
            gameplayCamera = Camera.main;
            if (gameplayCamera == null)
            {
                return;
            }
        }

        Vector2 mousePos = InputManager.Controls.Player.Pointer.ReadValue<Vector2>();
        Ray ray = gameplayCamera.ScreenPointToRay(mousePos);
        if (!isHeld) // 何も持っていない時（掴む処理）
        {
            float fixedY = holdHeight;
            // 視線が水平に近いときは平面との交点を計算しない。
            Vector3 groundPos = ray.origin;
            if (Mathf.Abs(ray.direction.y) > 0.0001f)
            {
                float t = (fixedY - ray.origin.y) / ray.direction.y;
                groundPos = ray.GetPoint(t);
            }
            RaycastHit hit;
            bool isHit = false;
            if (Physics.Raycast(ray, out hit, CameraRayLength))
            {
                isHit = true;
            }
            else if (Physics.Raycast(groundPos + Vector3.up * 0.5f, Vector3.down, out hit, CameraRayLength))
            {
                isHit = true;
            }

            if (isHit)
            {
                ItemGimmick itemGimmick = hit.collider.GetComponentInParent<ItemGimmick>();
                if (itemGimmick != null && itemGimmick.CompareTag("Merchandise"))
                {
                    // かごに入っている場合は掴めない
                    if (itemGimmick.GetInBasket() || itemGimmick.GetIsInBasket())
                    {
                        return;
                    }

                    heldItemGimmick = itemGimmick;
                    heldTransform = hit.collider.GetComponentInParent<ItemGimmick>()?.transform ?? hit.collider.transform.root;
                    heldRigidbody = heldTransform.GetComponent<Rigidbody>();
                    heldItemGimmick.SetHeld(true);
                    isHeld = true;
                    MakeHeldItemTransparent();

                    if (heldRigidbody != null)
                    {
                        heldRigidbody.isKinematic = true;
                    }

                    // かごの仲のアイテムとの当たり判定を無視する
                    SetIgnoreCollisionWithBasketItems(true);
                }
            }
        }
        else // すべて持っている時（離す処理）
        {
            RestoreHeldItemMaterials();
            if (heldItemGimmick != null)
            {
                heldItemGimmick.SetHeld(false);
            }

            if (heldRigidbody != null)
            {
                heldRigidbody.isKinematic = false;
            }

            SetIgnoreCollisionWithBasketItems(false);

            ClearCache();
            isHeld = false;
        }
    }
    private void Update()
    {
        if (gameplayCamera == null)
        {
            gameplayCamera = Camera.main;
            if (gameplayCamera == null)
            {
                return;
            }
        }

        Vector2 mousePos = InputManager.Controls.Player.Pointer.ReadValue<Vector2>();
        Ray ray = gameplayCamera.ScreenPointToRay(mousePos);
        Debug.DrawRay(ray.origin, ray.direction * CameraRayLength, Color.red, 0.2f);

        // アイテム回転処理（ActionAssetから状態を取得）
        if (isHeld && heldTransform != null)
        {
            bool isQ = InputManager.Controls.Player.RotateX.IsPressed();
            bool isW = InputManager.Controls.Player.RotateY.IsPressed();
            bool isE = InputManager.Controls.Player.RotateZ.IsPressed();

            float scroll = InputManager.Controls.Player.Scroll.ReadValue<Vector2>().y;
            if (isQ)
            {
                if (scroll > 0)
                {
                    heldTransform.Rotate(AngleAcceleration, 0.0f, 0.0f, Space.World);
                }
                if (scroll < 0)
                {
                    heldTransform.Rotate(-AngleAcceleration, 0.0f, 0.0f, Space.World);
                }
            }
            if (isW)
            {
                if (scroll > 0)
                {
                    heldTransform.Rotate(0.0f, AngleAcceleration, 0.0f, Space.World);
                }
                if (scroll < 0)
                {
                    heldTransform.Rotate(0.0f, -AngleAcceleration, 0.0f, Space.World);
                }
            }
            if (isE)
            {
                if (scroll > 0)
                {
                    heldTransform.Rotate(0.0f, 0.0f, AngleAcceleration, Space.World);
                }
                if (scroll < 0)
                {
                    heldTransform.Rotate(0.0f, 0.0f, -AngleAcceleration, Space.World);
                }
            }
        }
        // アイテム移動処理
        if (isHeld && heldTransform != null && heldItemGimmick != null)
        {
            if (heldItemGimmick.GetHeld())
            {
                float fixedY = holdHeight;

                if (Mathf.Abs(ray.direction.y) > 0.0001f)
                {
                    float t = (fixedY - ray.origin.y) / ray.direction.y;
                    Vector3 worldPos = ray.origin + ray.direction * t;
                    worldPos.y = fixedY;
                    heldTransform.position = worldPos;
                }

                if (heldRigidbody != null)
                {
                    heldRigidbody.isKinematic = true;
                }

                if (GameCursorUI.Instance != null)
                {
                    float remain = heldItemGimmick.GetRemainingHoldTime();
                    GameCursorUI.Instance.SetHoldingCursor(remain);
                }
            }
            else
            {
                RestoreHeldItemMaterials();
                if (heldRigidbody != null)
                {
                    heldRigidbody.isKinematic = false;
                }
                ClearCache();
                isHeld = false;
            }
        }
        else
        {
            if (GameCursorUI.Instance != null)
            {
                GameCursorUI.Instance.SetNormalCursor();
            }
        }

        // レーザーを飛ばして何かに当たったか
        if (isHeld)
        {
            if (GameCursorUI.Instance != null)
            {
                GameCursorUI.Instance.HideItemInfo();
            }
        }
        else if (Physics.Raycast(ray, out RaycastHit hit, CameraRayLength))
        {
            // 当たったものがアイテム(ItemGimmick)を持っているかチェック
            ItemGimmick lookingItem = hit.collider.GetComponentInParent<ItemGimmick>();

            if (lookingItem != null && lookingItem.CompareTag("Merchandise"))
            {
                // アイテムを見ていたら情報を表示
                if (GameCursorUI.Instance != null)
                {
                    GameCursorUI.Instance.ShowItemInfo(lookingItem.GetItemName(), lookingItem.GetAttributeText(), lookingItem.GetHoldLimitTime());
                }
            }
            else
            {
                // アイテム以外を見ていたら隠す
                if (GameCursorUI.Instance != null)
                {
                    GameCursorUI.Instance.HideItemInfo();
                }
            }
        }
        else
        {
            // 何も見ていなければ隠す
            if (GameCursorUI.Instance != null)
            {
                GameCursorUI.Instance.HideItemInfo();
            }
        }
    }
    private void ClearCache()
    {
        RestoreHeldItemMaterials();
        heldItemGimmick = null;
        heldTransform = null;
        heldRigidbody = null;
    }

    private void MakeHeldItemTransparent()
    {
        RestoreHeldItemMaterials();
        if (heldTransform == null) return;

        foreach (Renderer itemRenderer in heldTransform.GetComponentsInChildren<Renderer>())
        {
            // ParticleSystem などの商品演出には触れず、商品本体のメッシュだけを透過する。
            if (!(itemRenderer is MeshRenderer) && !(itemRenderer is SkinnedMeshRenderer)) continue;

            Material[] sources = itemRenderer.sharedMaterials;
            Material[] replacements = (Material[])sources.Clone();
            bool changed = false;
            for (int i = 0; i < sources.Length; i++)
            {
                Material source = sources[i];
                if (source == null) continue;

                Material transparent;
                if (source.HasProperty("_Surface") && source.HasProperty("_BaseColor"))
                {
                    transparent = new Material(source);
                }
                else
                {
                    // FBX 内蔵マテリアルなど、透過設定を持たない商品も対象にする。
                    Shader litShader = Shader.Find("Universal Render Pipeline/Lit");
                    if (litShader == null) continue;
                    transparent = new Material(litShader);
                    if (source.mainTexture != null)
                    {
                        transparent.SetTexture("_BaseMap", source.mainTexture);
                        transparent.SetTextureScale("_BaseMap", source.mainTextureScale);
                        transparent.SetTextureOffset("_BaseMap", source.mainTextureOffset);
                    }
                    if (source.HasProperty("_Color")) transparent.SetColor("_BaseColor", source.GetColor("_Color"));
                    else if (source.HasProperty("_BaseColor")) transparent.SetColor("_BaseColor", source.GetColor("_BaseColor"));
                }
                transparent.name = source.name + " (Held)";
                transparent.SetFloat("_Surface", 1f);
                transparent.SetFloat("_Blend", 0f);
                transparent.SetFloat("_SrcBlend", (float)BlendMode.SrcAlpha);
                transparent.SetFloat("_DstBlend", (float)BlendMode.OneMinusSrcAlpha);
                transparent.SetFloat("_ZWrite", 0f);
                transparent.SetOverrideTag("RenderType", "Transparent");
                transparent.renderQueue = (int)RenderQueue.Transparent;
                transparent.EnableKeyword("_SURFACE_TYPE_TRANSPARENT");
                Color color = transparent.GetColor("_BaseColor");
                color.a *= heldItemOpacity;
                transparent.SetColor("_BaseColor", color);
                replacements[i] = transparent;
                transparentMaterials.Add(transparent);
                changed = true;
            }

            if (!changed) continue;
            originalMaterials.Add(itemRenderer, sources);
            itemRenderer.sharedMaterials = replacements;
        }
    }

    private void RestoreHeldItemMaterials()
    {
        foreach (KeyValuePair<Renderer, Material[]> pair in originalMaterials)
        {
            if (pair.Key != null) pair.Key.sharedMaterials = pair.Value;
        }
        originalMaterials.Clear();
        foreach (Material material in transparentMaterials)
        {
            if (material != null) Destroy(material);
        }
        transparentMaterials.Clear();
    }

    // 持っているアイテムと、かごの中のアイテムの当たり判定をオン・オフするメソッド
    private void SetIgnoreCollisionWithBasketItems(bool ignore)
    {
        if (heldTransform == null || heldItemGimmick == null)
        {
            return;
        }

        Collider[] heldColliders = heldTransform.GetComponentsInChildren<Collider>();
        BasketChecker basket = Object.FindFirstObjectByType<BasketChecker>();

        if (basket != null)
        {
            List<ItemGimmick> basketItems = basket.GetInsideItems();
            if (basketItems != null)
            {
                foreach (var item in basketItems)
                {
                    if (item == null || item == heldItemGimmick)
                    {
                        continue;
                    }
                    Collider[] itemColliders = item.GetComponentsInChildren<Collider>();
                    foreach (var hc in heldColliders) // 持っている商品のすべての当たり判定
                    {
                        foreach (var ic in itemColliders) // かごの中の商品のすべての当たり判定
                        {
                            // ignoreがtrueなら無視、falseなら元に戻す
                            Physics.IgnoreCollision(hc, ic, ignore);
                        }
                    }
                }
            }
        }
    }

}

