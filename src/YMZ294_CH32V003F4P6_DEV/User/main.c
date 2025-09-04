/**
 * @file main.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  CH32V003 メイン
 * @version 0.1
 * @date 2025-09-05
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "debug.h"
#include "drv_ymz294.h"
#include "app_ymz294.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    // SDIでのprintf()を有効
    SDI_Printf_Enable();

    // YMZ294のH/W初期化
    drv_ymz294_init();

    // YMZ294 アプリ初期化
    app_ymz294_init();

    while(1)
    {
        // YMZ294 アプリメイン
        app_ymz294_main();
        Delay_Ms(1000);
    }
}