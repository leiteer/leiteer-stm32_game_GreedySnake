#ifndef __SNAKE_H
#define __SNAKE_H

#include "main.h"

// �޸���Ϸ������
#define GAME_AREA_X0    10   // �������Ͻ�X���꣬��������ռ�
#define GAME_AREA_Y0    20   // �������Ͻ�Y���꣬��������ռ�
#define SCORE_OFFSET    0  // ��ӷ�����ʾ��ƫ����
#define GAME_AREA_WIDTH  220  // ��Ϸ������
#define GAME_AREA_HEIGHT 270  // ��Ϸ����߶�
#define GRID_SIZE       10    // ÿ�������С
// �����Ϸ״̬
#define GAME_STATE_MENU    0
#define GAME_STATE_PLAYING 1
#define GAME_STATE_OVER    2
// ������
#define DIR_UP    0
#define DIR_DOWN  1
#define DIR_LEFT  2
#define DIR_RIGHT 3

// �ߵ���󳤶�
#define MAX_SNAKE_LENGTH 100

// ����ڵ�ṹ��
typedef struct {
    uint16_t x;
    uint16_t y;
} SnakeNode;

// ��Ϸ״̬
typedef struct {
    SnakeNode snake[MAX_SNAKE_LENGTH];  // ��������
    uint16_t length;                    // �ߵĵ�ǰ����
    uint8_t direction;                  // �ƶ�����
    SnakeNode food;                     // ʳ��λ��
    uint8_t isGameOver;                 // ��Ϸ������־
    uint16_t score;                     // ����
    uint8_t gameState;                  // ��Ϸ״̬
} GameState;

// ��������
void Snake_Init(void);
void Snake_Run(void);
void Snake_DrawGameArea(void);
void Snake_Move(void);
void Snake_CreateFood(void);
// �޸ĺ�������
uint8_t Snake_CheckCollision(uint16_t next_x, uint16_t next_y);
void Snake_GameOver(void);
// �ں��������������
void Snake_ShowMainMenu(void);
void Snake_DrawLogo(void);
#endif
