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


// テスト用ドレミファソラシド 単音 音階テーブル
const uint8_t g_test_tone_tbl[] = {
    TONE_C4, TONE_D4, TONE_E4, TONE_F4, TONE_G4, TONE_A4, TONE_B4, TONE_C5
};

// テスト用ドレミファソラシド 3和音 音階テーブル
#define TEST_TONE_CHORD_TBL_SIZE      8
const tone_3_chord_data_t g_test_tone_chord_tbl[] = {
    {TONE_C3, TONE_C4, TONE_C5}, {TONE_D3, TONE_D4, TONE_D5}, {TONE_E3, TONE_E4, TONE_E5},
    {TONE_F4, TONE_F4, TONE_F5}, {TONE_G3, TONE_G4, TONE_G5}, {TONE_A3, TONE_A4, TONE_A5},
    {TONE_B3, TONE_B4, TONE_B5}, {TONE_C4, TONE_C5, TONE_C6},
};

// 某コンビニ入店音の音階テーブル
const uint8_t g_music_tone_tbl[] = {
    TONE_FS4, TONE_D4, TONE_A3, TONE_D4, TONE_E4, TONE_A4,
    TONE_E3, TONE_E4, TONE_FS4, TONE_E4, TONE_A3, TONE_D4
};

void app_ymz294_init()
{
    drv_ymz294_play_music_tone(&g_test_tone_tbl[0], sizeof(g_test_tone_tbl));
    Delay_Ms(500);
    drv_ymz294_play_music_chord_tone(&g_test_tone_chord_tbl[0], TEST_TONE_CHORD_TBL_SIZE);
    Delay_Ms(500);
}

void app_ymz294_main()
{
    drv_ymz294_play_music_tone(&g_music_tone_tbl[0], sizeof(g_music_tone_tbl));
    Delay_Ms(500);
    drv_ymz294_play_music_tone(&g_test_tone_tbl[0], sizeof(g_test_tone_tbl));
    Delay_Ms(500);
    drv_ymz294_play_music_chord_tone(&g_test_tone_chord_tbl[0], TEST_TONE_CHORD_TBL_SIZE);
}