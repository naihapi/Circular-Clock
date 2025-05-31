#pragma once

// 自定义库
#include "LEDBoard.h"
#include "WS2812.h"
#include "Flash.h"

#define STYLE_Pin D6
#define STYLE_DIALMAX 4

#ifndef __cplusplus
extern "C"
{
#endif
    void Style_InitPro(void);
    void Style_ShowDial(void);

#ifndef __cplusplus
}
#endif
