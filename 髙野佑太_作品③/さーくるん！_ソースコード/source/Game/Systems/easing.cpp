// easing.cpp
// 
// sougo hara  2024/09/03
// 
// Œ³ƒR[ƒh https://easings.net/ja
//---------------------------------------------------------------------------

#include "easing.h"

float EasingInOutLinear(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f: xraw);
	return x == 0.0f
		? 0.0f
		: x == 1.0f
		? 1.0f
		: x < 0.5f ? (float)pow(3, 30 * x - 15) / 3
		: (2.0f - (float)pow(3, -30 * x + 15)) / 3;
}
float EasingReverse(float xraw)
{
    float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
    return 1.0f - x;
}

float EasingInSine(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return 1.0f - (float)cos((x * EASING_PI) / 2);
}

float EasingOutSine(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return (float)sin((x * EASING_PI) / 2);
}

float EasingInOutSine(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return -((float)cos(EASING_PI * x) - 1.0f) / 2;
}

float EasingInQuad(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x * x;
}

float EasingOutQuad(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return 1.0f - (1.0f - x) * (1.0f - x);
}

float EasingInOutQuad(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x < 0.5f ? 2.0f * x * x : 1.0f - (float)pow(-2.0f * x + 2.0f, 2.0f) / 2;
}

float EasingInCubic(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x * x * x;
}

float EasingOutCubic(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return 1.0f - (float)pow(1.0f - x, 3.0f);
}

float EasingInOutCubic(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x < 0.5f ? 4.0f * x * x * x : 1.0f - (float)pow(-2.0f * x + 2.0f, 3.0f) / 2;
}

float EasingInQuart(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x * x * x * x;
}

float EasingOutQuart(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return 1.0f - (float)pow(1.0f - x, 4.0f);
}

float EasingInOutQuart(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - (float)pow(-2.0f * x + 2.0f, 4.0f) / 2;
}

float EasingInQuint(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x * x * x * x * x;
}

float EasingOutQuint(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return 1.0f - (float)pow(1.0f - x, 5.0f);
}

float EasingInOutQuint(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - (float)pow(-2.0f * x + 2.0f, 5.0f) / 2;
}

float EasingInExpo(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x == 0.0f ? 0.0f : (float)pow(2.0f, 10.0f * x - 10.0f);
}

float EasingOutExpo(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x == 1.0f ? 1.0f : 1.0f - (float)pow(2.0f, -10.0f * x);
}

float EasingInOutExpo(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x == 0.0f
		? 0.0f
		: x == 1.0f
		? 1.0f
		: x < 0.5f ? (float)pow(2.0f, 20.0f * x - 10.0f) / 2
		: (2.0f - (float)pow(2.0f, -20.0f * x + 10.0f)) / 2;
}

float EasingInCirc(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return 1.0f - (float)sqrt(1.0f - pow(x, 2.0f));
}

float EasingOutCirc(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return (float)sqrt(1.0f - pow(x - 1.0f, 2.0f));
}

float EasingInOutCirc(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x < 0.5f
		? (1.0f - (float)sqrt(1.0f - pow(2.0f * x, 2.0f))) / 2
		: ((float)sqrt(1.0f - pow(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2;
}

float EasingInBack(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * x * x * x - c1 * x * x;
}

float EasingOutBack(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return 1.0f + c3 * (float)pow(x - 1.0f, 3.0f) + c1 * (float)pow(x - 1.0f, 2.0f);
}

float EasingInOutBack(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return x < 0.5f
		? ((float)pow(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2
		: ((float)pow(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2;
}

float EasingInElastic(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	const float c4 = (float)(2.0f * EASING_PI) / 3;

	return x == 0.0f
		? 0.0f
		: x == 1.0f
		? 1.0f
		: -(float)pow(2.0f, 10.0f * x - 10.0f) * (float)sin((x * 10.0f - 10.75f) * c4);
}

float EasingOutElastic(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	const float c4 = (float)(2.0f * EASING_PI) / 3;

	return x == 0.0f
		? 0.0f
		: x == 1.0f
		? 1.0f
		: (float)pow(2.0f, -10.0f * x) * (float)sin((x * 10.0f - 0.75f) * c4) + 1.0f;
}

float EasingInOutElastic(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	const float c5 = (float)(2.0f * EASING_PI) / 4.5f;

	return x == 0.0f
		? 0.0f
		: x == 1.0f
		? 1.0f
		: x < 0.5f
		? -((float)pow(2.0f, 20.0f * x - 10.0f) * (float)sin((20.0f * x - 11.125f) * c5)) / 2
		: ((float)pow(2.0f, -20.0f * x + 10.0f) * (float)sin((20.0f * x - 11.125f) * c5)) / 2 + 1.0f;
}

float EasingInBounce(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return 1.0f - EasingOutBounce(1.0f - x);
}

float EasingOutBounce(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1.0f / d1)
	{
		return n1 * x * x;
	}
	else if (x < 2.0f / d1)
	{
		return n1 * (x -= 1.5f / d1) * x + 0.75f;
	}
	else if (x < 2.5f / d1)
	{
		return n1 * (x -= 2.25f / d1) * x + 0.9375f;
	}
	return n1 * (x -= 2.625f / d1) * x + 0.984375f;
}

float EasingInOutBounce(float xraw)
{
	float x = xraw < 0.0f ? 0.0f : (xraw > 1.0f ? 1.0f : xraw);
	return x < 0.5f
		? (1.0f - EasingOutBounce(1.0f - 2.0f * x)) / 2.0f
		: (1.0f + EasingOutBounce(2.0f * x - 1.0f)) / 2.0f;
}



float EasingSideRange(
    float value, float minf, float maxf,
    float inRangeMod, float outRangeMod,
    std::function<float(float)> easeIn,
    std::function<float(float)> easeOut
) {
    if (minf > maxf) std::swap(minf, maxf);

    // ƒNƒ‰ƒ“ƒv–h~‚Ì‚½‚ß‚ÉˆÀ‘S‚È”{—¦‚ğİ’è
    inRangeMod = std::max(inRangeMod, 0.0001f);
    outRangeMod = std::max(outRangeMod, 0.0001f);

    // ‘S‘Ì‚Ì’·‚³‚ğ1.0f‚Æ‚µ‚½‚Æ‚«‚Ì³‹K‰»
    float inLen = minf * inRangeMod;
    float outLen = (1.0f - maxf) * outRangeMod;
    float centerLen = 1.0f - inLen - outLen;

    // ’†‰›”ÍˆÍ‚ª’×‚ê‚é‚Ì‚ğ–h‚®iÅ’á’·‚³Šm•Ûj
    if (centerLen < 0.0001f) centerLen = 0.0001f;

    // V‚µ‚¢’†‰›”ÍˆÍ‚Ìmin/max‚ğÄŒvZ
    float minEdge = inLen;
    float maxEdge = inLen + centerLen;

    if (value < minEdge) {
        // 0.0`minEdge ‚ğ easeIn
        float t = value / minEdge;
        t = std::max(0.0f, std::min(t, 1.0f));
        return easeIn(t) * minf;
    }
    else if (value > maxEdge) {
        // maxEdge`1.0 ‚ğ easeOut
        float t = (value - maxEdge) / (1.0f - maxEdge);
        t = std::max(0.0f, std::min(t, 1.0f));
        return maxf + easeOut(t) * (1.0f - maxf);
    }
    else {
        // ’†‰›”ÍˆÍFminfŒÅ’è‚ğ•Ô‚·
        return minf;
    }
}