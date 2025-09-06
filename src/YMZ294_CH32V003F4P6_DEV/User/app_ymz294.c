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
#include <stdio.h>

void app_ymz294_init()
{
    drv_ymz294_mixser_config(MIXSER_CONFIG_TONE_NOISE, 0);
    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 15);
    drv_ymz294_set_tone_freq_midi_notenum(YMZ294_TONE_CH_A, 0);
}

void app_ymz294_main()
{
    printf("YMZ294 Develop by CH32V003!\r\n");
}