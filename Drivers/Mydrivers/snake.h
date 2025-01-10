#ifndef __SNAKE_H
#define __SNAKE_H

#include "main.h"

// 修改游戏区域定义
#define GAME_AREA_X0    10   // 增加左上角X坐标，留出更多空间
#define GAME_AREA_Y0    20   // 增加左上角Y坐标，留出更多空间
#define SCORE_OFFSET    0  // 添加分数显示的偏移量
#define GAME_AREA_WIDTH  220  // 游戏区域宽度
#define GAME_AREA_HEIGHT 270  // 游戏区域高度
#define GRID_SIZE       10    // 每个网格大小
// 添加游戏状态
#define GAME_STATE_MENU    0
#define GAME_STATE_PLAYING 1
#define GAME_STATE_OVER    2
// 方向定义
#define DIR_UP    0
#define DIR_DOWN  1
#define DIR_LEFT  2
#define DIR_RIGHT 3

// 蛇的最大长度
#define MAX_SNAKE_LENGTH 100

// 蛇身节点结构体
typedef struct {
    uint16_t x;
    uint16_t y;
} SnakeNode;

// 游戏状态
typedef struct {
    SnakeNode snake[MAX_SNAKE_LENGTH];  // 蛇身数组
    uint16_t length;                    // 蛇的当前长度
    uint8_t direction;                  // 移动方向
    SnakeNode food;                     // 食物位置
    uint8_t isGameOver;                 // 游戏结束标志
    uint16_t score;                     // 分数
    uint8_t gameState;                  // 游戏状态
} GameState;

// 函数声明
void Snake_Init(void);
void Snake_Run(void);
void Snake_DrawGameArea(void);
void Snake_Move(void);
void Snake_CreateFood(void);
// 修改函数声明
uint8_t Snake_CheckCollision(uint16_t next_x, uint16_t next_y);
void Snake_GameOver(void);
// 在函数声明部分添加
void Snake_ShowMainMenu(void);
void Snake_DrawLogo(void);
#endif
