
#include "../../Core/_systemModeSwitch.h"

#include "sys_general.h"

#ifdef SWITCH_MODE
#else
#include <d3d11.h>
#include <algorithm>
#include <cmath>
#endif


Float2 MkF2(float x, float y)
{
	//Float2 vec = { {x, y} };
	Float2 vec = {};
	vec.x = x;
	vec.y = y;
    return vec;
}

Float3 MkF3(float x, float y, float z)
{
	//Float3 vec = { {x, y, z} };
	Float3 vec = {};
	vec.x = x;
	vec.y = y;
	vec.z = z;
    return vec;
}
Float4 MkF4(float x, float y, float z, float w)
{
	//Float4 vec = { {x, y, z, w} };
	Float4 vec = {};
	vec.x = x;
	vec.y = y;
	vec.z = z;
	vec.w = w;
    return vec;
}

Float2 F2AngleShape(Float2 angle)
{
	Float2 antmp = angle;
	if (abs(antmp.x) > abs(antmp.y))
	{
		if (antmp.x < 0)
		{
			antmp.y = antmp.y * (-1.0f / antmp.x);
			antmp.x = -1.0f;
		}
		else
		{
			antmp.y = antmp.y * (1.0f / antmp.x);
			antmp.x = 1.0f;
		}
	}
	else if (abs(antmp.x) < abs(antmp.y))
	{
		if (antmp.y < 0)
		{
			antmp.x = antmp.x * (-1.0f / antmp.y);
			antmp.y = -1.0f;
		}
		else
		{
			antmp.x = antmp.x * (1.0f / antmp.y);
			antmp.y = 1.0f;
		}
	}
	return antmp;
}


float CalculateDistance(Float2 a, Float2 b)
{
	//return (float)(abs(a.x - b.x) + abs(a.y - b.y)) / 2;
	return (float)sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
float CalculateDistance(Float3 a, Float3 b)
{
	//return CalculateDistance(MkF2(a.x, a.y), MkF2(b.x, b.y));
	return (float)sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

float CalculateRadToDeg(float rad)
{
	return (float)(rad * (180.0f / SYS_GENERAL_PI));
}
float CalculateDegToRad(float deg)
{
	return (float)(deg * SYS_GENERAL_PI / 180.0f);
}

Float3 NormalizeVector(Float3 vec)
{
	float distance = sqrt(vec.x * vec.x
		+ vec.y * vec.y
		+ vec.z * vec.z);

	if (distance <= 0.0f)
	{
		return {};
	}

	return MkF3(vec.x / distance, vec.y / distance, vec.z / distance);
}
Float2 NormalizeVector(Float2 vec)
{
	float distance = sqrt(vec.x * vec.x
		+ vec.y * vec.y);

	if (distance <= 0.0f)
	{
		return {};
	}

	return MkF2(vec.x / distance, vec.y / distance);
}



Float2 CalculateCentroid(Float2* points, int pointsNum)
{
	double sumX = 0.0;
	double sumY = 0.0;
	for (int i = 0; i < pointsNum; i++) {
		sumX += points[i].x;
		sumY += points[i].y;
	}
	Float2 centroid = {};
	centroid.x = (float)(sumX / pointsNum);
	centroid.y = (float)(sumY / pointsNum);
	return centroid;
}
Float2 CalculateCentroid(Float2 a, Float2 b)
{
	Float2 arr[2] = { a, b };
	return CalculateCentroid(arr, 2);
}
Float3 CalculateCentroid(const std::vector<Float3>& points) {
    Float3 center = { 0.0f, 0.0f, 0.0f };
    for (const auto& p : points) {
        center.x += p.x;
        center.y += p.y;
    }
    center.x /= points.size();
    center.y /= points.size();
    return center;
}



float calculateCross(Float2 a, Float2 b)
{
	return a.x * b.y - a.y * b.x;
}

CalculateIntersectionResult CalculateIntersection(Float2 a, Float2 b, Float2 c, Float2 d)
{
	CalculateIntersectionResult result = {};

	Float2 denoA = {};
	Float2 denoB = {};
	denoA.x = b.x - a.x;
	denoA.y = b.y - a.y;
	denoB.x = d.x - c.x;
	denoB.y = d.y - c.y;

	float s, t, deno ;
	deno = calculateCross(denoA, denoB);

	Float2 error = MkF2( 0, 0 );

	if (deno == 0.0)
	{ // 線分が平行
		result.enable = false;
		result.pos = error;
		
		return result;
	}

	Float2 sA = {};
	Float2 sB = {};
	Float2 tA = {};
	Float2 tB = {};
	sA.x = c.x - a.x;
	sA.y = c.y - a.y;
	sB.x = d.x - c.x;
	sB.y = d.y - c.y;
	tA.x = b.x - a.x;
	tA.y = b.y - a.y;
	tB.x = a.x - c.x;
	tB.y = a.y - c.y;

	s = calculateCross(sA, sB) / deno;
	t = calculateCross(tA, tB) / deno;

	if (s < 0.0 || 1.0 < s || t < 0.0 || 1.0 < t)
	{ // 線分が交差していない
		result.enable = false;
		result.pos = error;
		return result;
	}
	Float2 rA = {};
	rA.x = b.x - a.x;
	rA.y = b.y - a.y;


	result.enable = true;
	result.pos = MkF2(a.x + s * rA.x, a.y + s * rA.y);
	return result;
}


Float2 Float3ToFloat2(Float3 f3)
{
	return MkF2(f3.x, f3.y);
}
Float3 Float2ToFloat3(Float2 f2, float z)
{
	return MkF3(f2.x, f2.y, z);
}


uint32_t RGBToUINT32(Float3 rgb)
{
	//RGB → uint32
	uint32_t colortmp =
		((uint32_t)(255 * rgb.x) << 16)
		| ((uint32_t)(255 * rgb.y) << 8)
		| (uint32_t)(255 * rgb.z)
		;
	return colortmp;
}
uint32_t RGBToUINT32(Float4 rgba)
{
	return RGBToUINT32(MkF3(rgba.x, rgba.y, rgba.z));
}

float CalculatePolygonArea(Float2* points, int pointsNum)
{
	int n = pointsNum;
	double area = 0.0;

	// ショアの公式を使用して面積を計算
	for (int i = 0; i < n; i++)
	{
		int j = (i + 1) % n;
		area += points[i].x * points[j].y;
		area -= points[j].x * points[i].y;
	}
	area = abs(area) / 2.0;
	return (float)area;
}

float CalculateDistanceBetweenParallelLines(Float2 ab, Float2 c)
{
	// 直線の方程式は ax + by + c = 0 形式
	// 平行な直線の方程式は a1 = a2, b1 = b2 であるため、a と b は同じ
	return std::abs(c.x - c.y) / std::sqrt(ab.x * ab.x + ab.y * ab.y);
}

float DegNormalize(float deg)
{
    float deg2 = deg;
    while (deg2 < 0.0f)
    {
        deg2 += 360.0f;
    }
    while (deg2 >= 360.0f)
    {
        deg2 += -360.0f;
    }
    return deg2;
}


STR_SPRIT StrSplit(std::string str, char del, bool shape)
{
	STR_SPRIT result = {};
	std::string subStr = "";

	for (const char c : str)
	{
		if (c == del)
		{
			result.push_back(subStr);
			subStr.clear();
		}
		else
		{
			subStr += c;
		}
	}

	result.push_back(subStr);

	if (shape)
	{
		for (int i = 0; i < result.size(); i++)
		{
			std::string* c = &result[i];
			c->erase(remove(c->begin(), c->end(), '\r'), c->end());
			c->erase(remove(c->begin(), c->end(), '\n'), c->end());
			//c->erase(remove(c->begin(), c->end(), '\t'), c->end());
			//c->erase(remove(c->begin(), c->end(), ' '), c->end());
		}
	}
	return result;
}


// stringをwstringへ変換する
std::wstring StringToWString(std::string oString)
{
	std::string ostr = oString;
	ostr.erase(remove(ostr.begin(), ostr.end(), '\r'), ostr.end());
	ostr.erase(remove(ostr.begin(), ostr.end(), '\n'), ostr.end());

	size_t sizetmp = ostr.size() + 1;

	wchar_t* wcFileName = new wchar_t[sizetmp];

	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcFileName, sizetmp, ostr.c_str(), _TRUNCATE);

	std::wstring rStr = {};
	if (convertedChars > 0)
	{
		rStr = wcFileName;
	}

	delete[] wcFileName;

	return rStr;
}

std::string NormalizeString(std::string oString)
{
    std::string ostr = oString;
    ostr.erase(remove(ostr.begin(), ostr.end(), '\r'), ostr.end());
    ostr.erase(remove(ostr.begin(), ostr.end(), '\n'), ostr.end());

    return ostr;
}


//位置関係からベクトルを計算
Float3 CalculatePosToVector(Float3 posA, Float3 posB)
{
    return MkF3(
        posB.x - posA.x,
        posB.y - posA.y,
        posB.z - posA.z
    );
}


std::vector<FILE_AUTO_LOAD_DAT> FileScanDirectory(const std::string& dir_name, const std::string& f_name)
{
    HANDLE fHandle;
    WIN32_FIND_DATA win32fd;
    std::vector<FILE_AUTO_LOAD_DAT> file_names;

    std::string search_name = dir_name + "\\*";

    fHandle = FindFirstFile(search_name.c_str(), &win32fd);

    if (fHandle == INVALID_HANDLE_VALUE)
    {
        // "'GetLastError() == 3' is 'file not found'"
        return file_names;
    }

    do {
        if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {//ディレクトリである

            //そのディレクトリそのものを表す場合は処理しない
            if (win32fd.cFileName[0] == '.') continue;

            std::string fullpath = dir_name + "\\" + win32fd.cFileName;

            std::string f_nameTmp = f_name + "/" + win32fd.cFileName;

            //再帰的にファイルを検索
            std::vector<FILE_AUTO_LOAD_DAT> files = FileScanDirectory(fullpath, f_nameTmp);

            file_names.insert(file_names.end(), files.begin(), files.end());
        }
        else
        {//ファイルである
            FILE_AUTO_LOAD_DAT dat = {};
            dat.lPath = win32fd.cFileName;
            dat.fPath = dir_name + "\\" + win32fd.cFileName;

            char* cpytxtContext = nullptr;
            char* token = strtok_s(win32fd.cFileName, ".", &cpytxtContext);

            dat.name = f_name + "/" + token;

            file_names.emplace_back(dat);
        }
    } while (FindNextFile(fHandle, &win32fd));

    FindClose(fHandle);

    return file_names;
}

bool GetFileNames(const std::string& folderPath, std::vector<std::string>& fileNames)
{
    WIN32_FIND_DATA findFileData = {};
    HANDLE hFind = FindFirstFile((folderPath + "\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            fileNames.push_back(findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return true;
}
std::string GetExtension(const std::string& filePath)
{
    size_t dotPos = filePath.find_last_of(".");
    if (dotPos == std::string::npos) return ""; // 拡張子なし
    return filePath.substr(dotPos);
}
std::string GetRemoveExtension(const std::string& filePath) 
{
    size_t dotPos = filePath.find_last_of(".");
    if (dotPos == std::string::npos) return filePath; // 拡張子なしの場合、そのまま返す
    return filePath.substr(0, dotPos);
}





//#include <vector>
//#include <iostream>
//#include <cmath>
//#include <tuple>

// 簡易三角形化（ドロネー風）
void Triangulate(
    std::vector<Float3>& points,
    std::vector<FloatTriangle>& output
)
{
    if (points.size() < 3) {
        return;
    }

    // 単純な三角形化（ペアごとに生成）
    for (size_t i = 0; i < points.size() - 2; i++) {
        FloatTriangle tri = { points[i], points[i + 1], points[i + 2] };
        output.push_back(tri);
    }
}

// Triangle Strip 用にソート
std::vector<Float3> SortForTriangleStrip(std::vector<Float3>& points)
{
    std::sort(points.begin(), points.end(),
        [](const Float3& a, const Float3& b) {
            return atan2(a.y, a.x) < atan2(b.y, b.x);
        }
    );
    return points;
}

std::vector<Float3> SortSpiral(std::vector<Float3>& points)
{
    Float3 center = CalculateCentroid(points);
    std::vector<FloatPointData> sortedPoints;

    for (const auto& p : points) {
        float dx = p.x - center.x;
        float dy = p.y - center.y;
        float angle = atan2(dy, dx);
        float distance = sqrt(dx * dx + dy * dy);
        sortedPoints.push_back({ p, angle, distance });
    }

    // 角度でソートし、距離で補正
    std::sort(sortedPoints.begin(),
        sortedPoints.end(),
        [](const FloatPointData& a, const FloatPointData& b) {
        return a.angle < b.angle 
            || (a.angle == b.angle && a.distance < b.distance);
        });

    std::vector<Float3> result;
    for (const auto& p : sortedPoints) {
        result.push_back(p.point);
    }
    return result;
}

// **1. 三角形分割（Ear Clipping法）**
std::vector<Float3> TriangulatePolygon(const std::vector<Float3>& polygon) 
{
    std::vector<Float3> triangles;
    std::vector<Float3> tempPolygon = polygon;

    while (tempPolygon.size() > 2)
    {
        for (size_t i = 0; i < tempPolygon.size(); i++) 
        {
            size_t prev = (i == 0) ? tempPolygon.size() - 1 : i - 1;
            size_t next = (i + 1) % tempPolygon.size();

            // 三角形を追加
            triangles.push_back(tempPolygon[prev]);
            triangles.push_back(tempPolygon[i]);
            triangles.push_back(tempPolygon[next]);

            // 耳を削除
            tempPolygon.erase(tempPolygon.begin() + i);
            break;
        }
    }

    return triangles;
}
