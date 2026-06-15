// ===================================================
// sys_general.h 汎用的なインクルードとか
// 
// hara sougo  2024/07/26
// ===================================================
#ifndef _SYS_GENERAL_H_
#define _SYS_GENERAL_H_

#include "../../Core/_systemModeSwitch.h"
#include <vector>
#include <map>

//constexpr int SYS_PLAYER_MAX = (4);
constexpr int SYS_PLAYER_MAX = (1);

#define SYS_GENERAL_PI (3.1415)
//#define SYS_GENERAL_PI (3.15)

enum AXIS_2D {
    AXIS_2D_N = 0,
    AXIS_2D_LEFT,
    AXIS_2D_UP,
    AXIS_2D_RIGHT,
    AXIS_2D_DOWN,

    AXIS_2D_MAX,
};

enum AXIS_2D_EX {
    AXIS_2D_EX_N = 0,
    AXIS_2D_EX_D,
    AXIS_2D_EX_LD,
    AXIS_2D_EX_L,
    AXIS_2D_EX_LU,
    AXIS_2D_EX_U,
    AXIS_2D_EX_RU,
    AXIS_2D_EX_R,
    AXIS_2D_EX_RD,

    AXIS_2D_EX_MAX,
};


#ifdef SWITCH_MODE

#include "system.h"

#else

#include <iostream>
#include <functional>

// ===================================================
// 構造体宣言
// ===================================================
struct Int2 {
    union
    {
        int v[2];     //!< 要素配列
        struct
        {
            int x;
            int y;
        };
    };
};
struct Int3 {
    union
    {
        int v[3];     //!< 要素配列
        struct
        {
            int x;
            int y;
            int z;
        };
    };
};
struct Int4 {
    union
    {
        int v[4];     //!< 要素配列
        struct
        {
            int x;
            int y;
            int z;
            int w;
        };
    };
};

struct Float2 {
    union
    {
        float v[2];     //!< 要素配列
        struct
        {
            float x;
            float y;
        };
    };
};
struct Float3 {
    union
    {
        float v[3];     //!< 要素配列
        struct
        {
            float x;
            float y;
            float z;
        };
    };
};

//内積を求める関数
static float Float3Dot(const Float3& a, const Float3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
// ベクトルの引き算
static Float3 operator-(const Float3& a, const Float3& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

// ベクトルの加算
static Float3 operator+(const Float3& a, const Float3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

// ベクトルとスカラーの乗算
static Float3 operator*(const Float3& v, float t) {
    return { v.x * t, v.y * t, v.z * t };
}

struct Float4 {
    union
    {
        float v[4];     //!< 要素配列
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
    };
};


// 三角形型
struct FloatTriangle {
    Float3 v0, v1, v2;
};
struct FloatPointData {
    Float3 point;
    float angle;
    float distance;
};


struct FloatCapsule {
    Float3 pointA; // 端点A（座標）
    Float3 pointB; // 端点B（座標）
    float radius;  // 半径
};



Float3 NormalizeVector(Float3 vec);

struct Quaternion {
    double w = 0, x = 0, y = 0, z = 0;

    Quaternion() = default;
    Quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}

    // 正規化
    void normalize() {
        double mag = std::sqrt(w * w + x * x + y * y + z * z);
        w /= mag;
        x /= mag;
        y /= mag;
        z /= mag;
    }

    // クォータニオンの共役を計算
    Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }
    // クォータニオンの積を計算
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }

    static Quaternion LookRotation(Float3 forward)
    {
        Float3 norm = { 0, 0, 1 };

        //内積
        float dot = (norm.x * forward.x + norm.y * forward.y + norm.z * forward.z);

        float theta = acos(dot); //角度の算出

        //外積
        Float3 cross = {
            norm.y * forward.z - norm.z * forward.y,
            norm.z * forward.x - norm.x * forward.z,
            norm.x * forward.y - norm.y * forward.x
        };
        Quaternion rot = {};
        cross = NormalizeVector(cross);
        theta = theta / 2;
        rot.x = cross.x * sin(theta);
        rot.y = cross.y * sin(theta);
        rot.z = cross.z * sin(theta);
        rot.w = cos(theta);

        return rot;
    }

    // クォータニオンを使用してベクトルを回転
    static Float3 rotateVector(const Quaternion& q, const Float3& v) {
        Quaternion qv(0, v.x, v.y, v.z);
        Quaternion q_conjugate = q.conjugate();
        Quaternion q_result = q * qv * q_conjugate;

        return { (float)q_result.x, (float)q_result.y, (float)q_result.z };
    }

    static Quaternion EulerToQuaternion(Float3 e) {
        auto cx = std::cos(0.5f * e.x);
        auto sx = std::sin(0.5f * e.x);
        auto cy = std::cos(0.5f * e.y);
        auto sy = std::sin(0.5f * e.y);
        auto cz = std::cos(0.5f * e.z);
        auto sz = std::sin(0.5f * e.z);

        return Quaternion(
            cx * sy * sz + sx * cy * cz,
            -sx * cy * sz + cx * sy * cz,
            cx * cy * sz + sx * sy * cz,
            -sx * sy * sz + cx * cy * cz
        );

        throw "conversion of euler angle to quaterion is failed.";
    }

    // 内積
    double dot(const Quaternion& q) const {
        return w * q.w + x * q.x + y * q.y + z * q.z;
    }

    // 球面線形補間 (Slerp)
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, double t) {
        // 内積を計算
        double dot = q1.dot(q2);

        // クォータニオンが逆向きの場合、内積を反転
        Quaternion q2_copy = q2;
        if (dot < 0.0) {
            q2_copy.w = -q2_copy.w;
            q2_copy.x = -q2_copy.x;
            q2_copy.y = -q2_copy.y;
            q2_copy.z = -q2_copy.z;
            dot = -dot;
        }

        const double DOT_THRESHOLD = 0.9995;
        if (dot > DOT_THRESHOLD) {
            // 線形補間を使用
            Quaternion result(
                q1.w + t * (q2_copy.w - q1.w),
                q1.x + t * (q2_copy.x - q1.x),
                q1.y + t * (q2_copy.y - q1.y),
                q1.z + t * (q2_copy.z - q1.z)
            );
            result.normalize();
            return result;
        }

        // 角度を計算
        double theta_0 = std::acos(dot);
        double theta = theta_0 * t;

        // q2を基準にしたq1の直交ベクトル
        Quaternion q3(
            q2_copy.w - q1.w * dot,
            q2_copy.x - q1.x * dot,
            q2_copy.y - q1.y * dot,
            q2_copy.z - q1.z * dot
        );
        q3.normalize();

        // Slerpの計算
        return Quaternion(
            q1.w * std::cos(theta) + q3.w * std::sin(theta),
            q1.x * std::cos(theta) + q3.x * std::sin(theta),
            q1.y * std::cos(theta) + q3.y * std::sin(theta),
            q1.z * std::cos(theta) + q3.z * std::sin(theta)
        );
    }
};

#endif

typedef std::vector<std::string> STR_SPRIT;

struct FILE_AUTO_LOAD_DAT {
    std::string lPath;
    std::string fPath;

    std::string name;
};

struct CalculateIntersectionResult {
    bool enable;
    Float2 pos;
};

// ===================================================
// プロトタイプ宣言
// ===================================================

// Make Float2
Float2 MkF2(float x, float y);
// Make Float3
Float3 MkF3(float x, float y, float z); 
// Make Float4
Float4 MkF4(float x, float y, float z, float w); 

//Float3をFloat2に変換
Float2 Float3ToFloat2(Float3 f3);
//Float2をFloat3に変換
Float3 Float2ToFloat3(Float2 f2,
    float z=0.0f);

//NormalizeVectorを知る前に作った産廃
//一応残すけど使わないでください
Float2 F2AngleShape(Float2 angle);

//2点から距離を計算
float CalculateDistance(Float2 a, Float2 b);
float CalculateDistance(Float3 a, Float3 b);

//ラジアンをディグリーに変換
float CalculateRadToDeg(float rad);
//ディグリーをラジアンに変換
float CalculateDegToRad(float deg);

//ベクトルの正規化
Float3 NormalizeVector(Float3 vec);
Float2 NormalizeVector(Float2 vec);

//複数の頂点の中心部を計算
Float2 CalculateCentroid(Float2* points, int pointsNum);
Float2 CalculateCentroid(Float2 a, Float2 b);
Float3 CalculateCentroid(const std::vector<Float3>& points);

//2つの線の交点を計算
CalculateIntersectionResult CalculateIntersection(Float2 a, Float2 b, Float2 c, Float2 d);

//RGBをUINT32に変換する
uint32_t RGBToUINT32(Float3);
uint32_t RGBToUINT32(Float4);

//複数の頂点から多角形の面積を計算する
float CalculatePolygonArea(Float2* points, int pointsNum);

//平行な線の距離
float CalculateDistanceBetweenParallelLines(Float2 ab, Float2 c);

//角度を0～360の値にする
float DegNormalize(float deg);

//簡易的な文字列の分割
STR_SPRIT StrSplit(std::string str, char del, bool shape=true);

// stringをwstringへ変換する
std::wstring StringToWString(std::string oString);

std::string NormalizeString(std::string oString);

//位置関係からベクトルを計算
Float3 CalculatePosToVector(Float3 posA, Float3 posB);

//フォルダをスキャン
std::vector<FILE_AUTO_LOAD_DAT> FileScanDirectory(const std::string& dir_name, const std::string& f_name);

//ファイル名取得関数
bool GetFileNames(const std::string& folderPath, std::vector<std::string>& fileNames);
std::string GetExtension(const std::string& filePath);
std::string GetRemoveExtension(const std::string& filePath);

void Triangulate(
    std::vector<Float3>& points,
    std::vector<FloatTriangle>& output
);

std::vector<Float3> SortForTriangleStrip(std::vector<Float3>& points);
std::vector<Float3> SortSpiral(std::vector<Float3>& points);

std::vector<Float3> TriangulatePolygon(const std::vector<Float3>& polygon);

#endif