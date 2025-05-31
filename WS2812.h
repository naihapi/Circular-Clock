#pragma once

// 官方库
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// 引脚和灯珠数量
#define WS2812_PIN D2
#define WS2812_NUM 192

// 白色系
#define Snow 0xFFFAFA           // 雪白
#define GhostWhite 0xF8F8FF     // 幽灵白
#define WhiteSmoke 0xF5F5F5     // 烟白
#define Gainsboro 0xDCDCDC      // 庚斯博罗灰
#define FloralWhite 0xFFFAF0    // 花卉白
#define OldLace 0xFDF5E6        // 旧蕾丝
#define Linen 0xFAF0E6          // 亚麻色
#define AntiqueWhite 0xFAEBD7   // 古董白
#define PapayaWhip 0xFFEFD5     // 番木瓜色
#define BlanchedAlmond 0xFFEBCD // 漂白杏仁色
#define Bisque 0xFFE4C4         // 陶坯黄
#define PeachPuff 0xFFDAB9      // 桃肉色
#define NavajoWhite 0xFFDEAD    // 纳瓦霍白
#define Moccasin 0xFFE4B5       // 鹿皮鞋色
#define Cornsilk 0xFFF8DC       // 玉米丝色
#define Ivory 0xFFFFF0          // 象牙白
#define LemonChiffon 0xFFFACD   // 柠檬绸色
#define Seashell 0xFFF5EE       // 贝壳白
#define Honeydew 0xF0FFF0       // 蜜瓜绿
#define MintCream 0xF5FFFA      // 薄荷奶油
#define Azure 0xF0FFFF          // 天蓝色
#define AliceBlue 0xF0F8FF      // 爱丽丝蓝
#define Lavender 0xE6E6FA       // 薰衣草紫
#define LavenderBlush 0xFFF0F5  // 薰衣草红
#define MistyRose 0xFFE4E1      // 雾玫瑰
#define White 0xFFFFFF          // 纯白
#define Black 0x000000          // 纯黑

// 灰色系
#define DarkSlateGray 0x2F4F4F  // 深石板灰
#define DimGray 0x696969        // 暗灰
#define SlateGray 0x708090      // 石板灰
#define LightSlateGray 0x778899 // 亮石板灰
#define Gray 0xBEBEBE           // 灰色
#define LightGray 0xD3D3D3      // 亮灰
#define DarkGrey 0xA9A9A9       // 深灰

// 蓝色系
#define MidnightBlue 0x191970         // 午夜蓝
#define NavyBlue 0x000080             // 海军蓝
#define CornflowerBlue 0x6495ED       // 矢车菊蓝
#define DarkSlateBlue 0x483D8B        // 深岩蓝
#define SlateBlue 0x6A5ACD            // 岩蓝
#define MediumSlateBlue 0x7B68EE      // 中岩蓝
#define LightSlateBlue 0x8470FF       // 亮岩蓝
#define MediumBlue 0x0000CD           // 中蓝
#define RoyalBlue 0x4169E1            // 皇家蓝
#define DodgerBlue 0x1E90FF           // 道奇蓝
#define DeepSkyBlue 0x00BFFF          // 深天蓝
#define SkyBlue 0x87CEEB              // 天蓝
#define LightSkyBlue 0x87CEFA         // 亮天蓝
#define SteelBlue 0x4682B4            // 钢蓝
#define LightSteelBlue 0xB0C4DE       // 亮钢蓝
#define LightBlue 0xADD8E6            // 亮蓝
#define PowderBlue 0xB0E0E6           // 粉蓝
#define PaleTurquoise 0xAFEEEE        // 苍绿松石
#define DarkTurquoise 0x00CED1        // 深绿松石
#define MediumTurquoise 0x48D1CC      // 中绿松石
#define Turquoise 0x40E0D0            // 绿松石
#define Cyan 0x00FFFF                 // 青色
#define LightCyan 0xE0FFFF            // 亮青
#define CadetBlue 0x5F9EA0            // 军服蓝
#define MediumAquamarine 0x66CDAA     // 中海蓝
#define Aquamarine 0x7FFFD4           // 碧绿色
#define DarkGreen 0x006400            // 深绿
#define DarkOliveGreen 0x556B2F       // 深橄榄绿
#define DarkSeaGreen 0x8FBC8F         // 深海绿
#define SeaGreen 0x2E8B57             // 海绿
#define MediumSeaGreen 0x3CB371       // 中海绿
#define LightSeaGreen 0x20B2AA        // 亮海绿
#define PaleGreen 0x98FB98            // 苍绿
#define SpringGreen 0x00FF7F          // 春绿
#define LawnGreen 0x7CFC00            // 草坪绿
#define Chartreuse 0x7FFF00           // 查特酒绿
#define MediumSpringGreen 0x00FA9A    // 中春绿
#define GreenYellow 0xADFF2F          // 绿黄
#define LimeGreen 0x32CD32            // 酸橙绿
#define YellowGreen 0x9ACD32          // 黄绿
#define ForestGreen 0x228B22          // 森林绿
#define OliveDrab 0x6B8E23            // 橄榄褐
#define DarkKhaki 0xBDB76B            // 深卡其
#define PaleGoldenrod 0xEEE8AA        // 苍金菊
#define LightGoldenrodYellow 0xFAFAD2 // 亮金菊黄
#define LightYellow 0xFFFFE0          // 亮黄
#define Yellow 0xFFFF00               // 纯黄
#define Gold 0xFFD700                 // 金色
#define LightGoldenrod 0xEEDD82       // 亮金菊
#define Goldenrod 0xDAA520            // 金菊色
#define DarkGoldenrod 0xB8860B        // 深金菊

// 红色系
#define RosyBrown 0xBC8F8F       // 玫瑰褐
#define IndianRed 0xCD5C5C       // 印度红
#define SaddleBrown 0x8B4513     // 鞍褐
#define Sienna 0xA0522D          // 赭色
#define Peru 0xCD853F            // 秘鲁色
#define Burlywood 0xDEB887       // 原木色
#define Beige 0xF5F5DC           // 米色
#define Wheat 0xF5DEB3           // 小麦色
#define SandyBrown 0xF4A460      // 沙褐
#define Tan 0xD2B48C             // 棕褐色
#define Chocolate 0xD2691E       // 巧克力色
#define Firebrick 0xB22222       // 火砖色
#define Brown 0xA52A2A           // 棕色
#define DarkSalmon 0xE9967A      // 深鲑红
#define Salmon 0xFA8072          // 鲑红
#define LightSalmon 0xFFA07A     // 亮鲑红
#define Orange 0xFFA500          // 橙色
#define DarkOrange 0xFF8C00      // 深橙
#define Coral 0xFF7F50           // 珊瑚色
#define LightCoral 0xF08080      // 亮珊瑚
#define Tomato 0xFF6347          // 番茄红
#define OrangeRed 0xFF4500       // 橙红
#define HotPink 0xFF69B4         // 热粉红
#define DeepPink 0xFF1493        // 深粉红
#define Pink 0xFFC0CB            // 粉红
#define LightPink 0xFFB6C1       // 亮粉红
#define PaleVioletRed 0xDB7093   // 苍紫罗兰红
#define Maroon 0xB03060          // 栗色
#define MediumVioletRed 0xC71585 // 中紫罗兰红
#define VioletRed 0xD02090       // 紫罗兰红
#define Magenta 0xFF00FF         // 品红
#define Violet 0xEE82EE          // 紫罗兰
#define Plum 0xDDA0DD            // 李子色
#define Orchid 0xDA70D6          // 兰花紫
#define MediumOrchid 0xBA55D3    // 中兰花紫
#define DarkOrchid 0x9932CC      // 深兰花紫
#define DarkViolet 0x9400D3      // 深紫罗兰
#define BlueViolet 0x8A2BE2      // 蓝紫罗兰
#define Purple 0xA020F0          // 紫色
#define MediumPurple 0x9370DB    // 中紫色
#define Thistle 0xD8BFD8         // 蓟色

// 其他颜色
#define DarkBlue 0x00008B    // 深蓝
#define DarkCyan 0x008B8B    // 深青
#define DarkMagenta 0x8B008B // 深品红
#define DarkRed 0x8B0000     // 深红
#define LightGreen 0x90EE90  // 亮绿

typedef struct WS2812_t
{
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
} WS2812_t;

// 自定义库
#include "LEDBoard.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void WS2812_InitPro();
    void WS2812_Update(void);
    void WS2812_SetOneLight(uint16_t led_num, WS2812_t color);
    void WS2812_SetAllLight(WS2812_t color);
    void WS2812_SetBrightness(uint8_t brightness);
    WS2812_t WS2812_GetColor(uint32_t hex_color);

#ifdef __cplusplus
}
#endif
