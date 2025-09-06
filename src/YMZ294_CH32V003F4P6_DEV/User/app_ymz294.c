/**
 * @file app_ymz294.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  YMZ294アプリ
 * @version 0.1
 * @date 2025-09-05
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "app_ymz294.h"
#include "common.h"


// 某コンビニ入店音の音階テーブル
const uint8_t g_music_tone_tbl[12] = {
    // ファミリーマート入店音 ドレミ音階例
    TONE_FS4, TONE_D4, TONE_A3, TONE_D4, TONE_E4, TONE_A5,
    TONE_E3, TONE_E4, TONE_FS4, TONE_E4, TONE_A3, TONE_D4
};

void app_ymz294_init()
{
    drv_ymz294_play_music_tone(&g_music_tone_tbl[0], sizeof(g_music_tone_tbl));
}

void app_ymz294_main()
{
    // TODO
}