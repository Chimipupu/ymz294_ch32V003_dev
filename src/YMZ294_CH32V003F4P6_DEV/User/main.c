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

static void pin_conf(void);

static void pin_conf(void)
{
    GPIO_InitTypeDef  pin_pd4;
    GPIO_InitTypeDef  pin_pd0;
    GPIO_InitTypeDef  pin_pc0;
    GPIO_InitTypeDef  pin_pd3;
    GPIO_InitTypeDef  pin_pd2;
    GPIO_InitTypeDef  pin_pc7;
    GPIO_InitTypeDef  pin_pc6;
    GPIO_InitTypeDef  pin_pc5;
    GPIO_InitTypeDef  pin_pc4;
    GPIO_InitTypeDef  pin_pc3;
    GPIO_InitTypeDef  pin_pd7;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    pin_pd4.GPIO_Pin = YMZ294_WR_PIN;
    pin_pd4.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pd4.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &pin_pd4);

    pin_pd0.GPIO_Pin = YMZ294_CS_PIN;
    pin_pd0.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pd0.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &pin_pd0);

    pin_pc0.GPIO_Pin = YMZ294_A0_PIN;
    pin_pc0.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pc0.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &pin_pc0);

    pin_pd3.GPIO_Pin = YMZ294_D0_PIN;
    pin_pd3.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pd3.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &pin_pd3);

    pin_pd2.GPIO_Pin = YMZ294_D1_PIN;
    pin_pd2.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pd2.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &pin_pd2);

    pin_pc7.GPIO_Pin = YMZ294_D2_PIN;
    pin_pc7.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pc7.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &pin_pc7);

    pin_pc6.GPIO_Pin = YMZ294_D3_PIN;
    pin_pc6.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pc6.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &pin_pc6);

    pin_pc5.GPIO_Pin = YMZ294_D4_PIN;
    pin_pc5.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pc5.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &pin_pc5);

    pin_pc4.GPIO_Pin = YMZ294_D5_PIN;
    pin_pc4.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pc4.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &pin_pc4);

    pin_pc3.GPIO_Pin = YMZ294_D6_PIN;
    pin_pc3.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pc3.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &pin_pc3);

    pin_pd7.GPIO_Pin = YMZ294_D7_PIN;
    pin_pd7.GPIO_Mode = GPIO_Mode_Out_PP;
    pin_pd7.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &pin_pd7);


    GPIO_WriteBit(GPIOD, YMZ294_WR_PIN, Bit_SET);
    GPIO_WriteBit(GPIOD, YMZ294_CS_PIN, Bit_SET);
    GPIO_WriteBit(GPIOC, YMZ294_A0_PIN, Bit_RESET);

    GPIO_WriteBit(GPIOD, YMZ294_D0_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOD, YMZ294_D1_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOC, YMZ294_D2_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOC, YMZ294_D3_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOC, YMZ294_D4_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOC, YMZ294_D5_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOC, YMZ294_D6_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOD, YMZ294_D7_PIN, Bit_RESET);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    // GPIO初期化
    pin_conf();

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