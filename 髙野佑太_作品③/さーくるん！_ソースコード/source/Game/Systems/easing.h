// easing.h
// 
// sougo hara  2024/09/03
//---------------------------------------------------------------------------
#ifndef _EASING_H_
#define _EASING_H_


#include "sys_general.h"

#include <math.h>

//
// É}ÉNÉçíËã`
//

#define EASING_PI	(3.14159)


float EasingInOutLinear(float xraw);

float EasingReverse(float xraw);

float EasingInSine(float xraw);
float EasingOutSine(float xraw);
float EasingInOutSine(float xraw);

float EasingInQuad(float xraw);
float EasingOutQuad(float xraw);
float EasingInOutQuad(float xraw);

float EasingInCubic(float xraw);
float EasingOutCubic(float xraw);
float EasingInOutCubic(float xraw);

float EasingInQuart(float xraw);
float EasingOutQuart(float xraw);
float EasingInOutQuart(float xraw);

float EasingInQuint(float xraw);
float EasingOutQuint(float xraw);
float EasingInOutQuint(float xraw);

float EasingInExpo(float xraw);
float EasingOutExpo(float xraw);
float EasingInOutExpo(float xraw);

float EasingInCirc(float xraw);
float EasingOutCirc(float xraw);
float EasingInOutCirc(float xraw);

float EasingInBack(float xraw);
float EasingOutBack(float xraw);
float EasingInOutBack(float xraw);

float EasingInElastic(float xraw);
float EasingOutElastic(float xraw);
float EasingInOutElastic(float xraw);

float EasingInBounce(float xraw);
float EasingOutBounce(float xraw);
float EasingInOutBounce(float xraw);


float EasingSideRange(
    float value, float minf, float maxf,
    float inRangeMod, float outRangeMod,
    std::function<float(float)> easeIn = EasingInSine,
    std::function<float(float)> easeOut = EasingOutSine
);

#endif // _EASING_H_