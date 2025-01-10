#include "snake.h"
#include "lcd.h"
#include "key.h"
#include <stdlib.h>

static GameState game;

// ������ͼ��
void Snake_DrawLogo(void)
{
    const uint16_t logo_x = GAME_AREA_X0 + 60;
    const uint16_t logo_y = GAME_AREA_Y0 + 60;
    
    // ��������S�Σ�
    uint16_t points[][2] = {
        {0, 0},   {1, 0},   {2, 0},    // ������
        {2, 1},   {2, 2},               //     ����
        {1, 2},   {0, 2},               // ������
        {0, 3},   {0, 4},               // ��
        {1, 4},   {2, 4},   {3, 4}      // ��������
    };
    
    // �Ŵ���
    const uint16_t scale = 20;
    
    // ����ÿ���㣨����
    for(int i = 0; i < sizeof(points)/sizeof(points[0])-1; i++) {
        LCD_Fill(logo_x + points[i][0] * scale,
                logo_y + points[i][1] * scale,
                logo_x + (points[i][0] + 1) * scale - 2,
                logo_y + (points[i][1] + 1) * scale - 2,
                GREEN);
    }
    
    // ������ͷ�����һ���㣩
    uint16_t head_x = logo_x + points[sizeof(points)/sizeof(points[0])-1][0] * scale;
    uint16_t head_y = logo_y + points[sizeof(points)/sizeof(points[0])-1][1] * scale;
    
    // ��ͷ����
    LCD_Fill(head_x, head_y,
            head_x + scale - 2,
            head_y + scale - 2,
            BLUE);
    
    // ����۾�������С�ڵ㣩
    LCD_Fill(head_x + scale/4, head_y + scale/4,
            head_x + scale/4 + 2, head_y + scale/4 + 2,
            BLACK);
    LCD_Fill(head_x + scale*3/4 - 2, head_y + scale/4,
            head_x + scale*3/4, head_y + scale/4 + 2,
            BLACK);
    
    // �����ͷ����ɫ�����Σ�
    LCD_Fill(head_x + scale/2 - 1, head_y + scale - 2,
            head_x + scale/2 + 1, head_y + scale + 4,
            RED);
}

// ��ʾ���˵�
void Snake_ShowMainMenu(void)
{
    // ����
    LCD_Clear(WHITE);
    
    // ���Ʊ߿�
    LCD_DrawRectangle(GAME_AREA_X0, GAME_AREA_Y0,
                     GAME_AREA_X0 + GAME_AREA_WIDTH,
                     GAME_AREA_Y0 + GAME_AREA_HEIGHT);
    
    // ��ʾ��Ϸ����
    LCD_ShowString(GAME_AREA_X0 + 10, GAME_AREA_Y0 + 20, 200, 32, 32, (uint8_t*)"GREEDY SNAKE");
    // ������ͼ��
    Snake_DrawLogo();
    
    // ��ʾ��ʼ��ʾ
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 180, 200, 16, 16, (uint8_t*)"Press KEY0 to Start");
    
    // ��ʾ����˵��
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 210, 200, 16, 16, (uint8_t*)"Controls:");
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 230, 200, 16, 16, (uint8_t*)"KEY_UP: Up");
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 250, 200, 16, 16, (uint8_t*)"KEY1: Down");
    LCD_ShowString(GAME_AREA_X0 + 120, GAME_AREA_Y0 + 230, 200, 16, 16, (uint8_t*)"KEY2: Left");
    LCD_ShowString(GAME_AREA_X0 + 120, GAME_AREA_Y0 + 250, 200, 16, 16, (uint8_t*)"KEY0: Right");
}

// �޸�Snake_Init����
void Snake_Init(void)
{
    // ��ʾ���˵�
    Snake_ShowMainMenu();
    game.gameState = GAME_STATE_MENU;
}

// ������Ϸ����
void Snake_DrawGameArea(void)
{
    // ���Ʊ߿�ʹ��˫�ߣ�
    LCD_DrawRectangle(GAME_AREA_X0, GAME_AREA_Y0, 
                     GAME_AREA_X0 + GAME_AREA_WIDTH, 
                     GAME_AREA_Y0 + GAME_AREA_HEIGHT);
    LCD_DrawRectangle(GAME_AREA_X0+1, GAME_AREA_Y0+1, 
                     GAME_AREA_X0 + GAME_AREA_WIDTH-1, 
                     GAME_AREA_Y0 + GAME_AREA_HEIGHT-1);
    
    // ��ʾ����������Ϸ�����Ϸ���
    LCD_ShowString(GAME_AREA_X0, GAME_AREA_Y0 - 20, 200, 16, 16, (uint8_t*)"Score:");
    LCD_ShowNum(GAME_AREA_X0 + 15, GAME_AREA_Y0 - 20, game.score, 4, 20);
}
// ��������ڵ�
void DrawSnakeNode(uint16_t x, uint16_t y, uint16_t color)
{
    // ����Բ�Ƿ���
    uint16_t x0 = GAME_AREA_X0 + x * GRID_SIZE;
    uint16_t y0 = GAME_AREA_Y0 + y * GRID_SIZE;
    uint16_t size = GRID_SIZE - 1;  // ����1���ؼ��
    
    // �������
    LCD_Fill(x0+1, y0+1, x0+size-1, y0+size-1, color);
    
    // ���Ʊ߿�
    LCD_DrawRectangle(x0+1, y0+1, x0+size-1, y0+size-1);
}

// ����ʳ��
void DrawFood(uint16_t x, uint16_t y)
{
    uint16_t x0 = GAME_AREA_X0 + x * GRID_SIZE;
    uint16_t y0 = GAME_AREA_Y0 + y * GRID_SIZE;
    uint16_t size = GRID_SIZE - 2;  // ʳ�������Сһ��
    
    // ����ƻ����״
    LCD_Fill(x0+2, y0+2, x0+size-1, y0+size-1, RED);  // ����
    LCD_Fill(x0+3, y0+1, x0+4, y0+2, BLACK);      // Ҷ��
    
//    // ����һ��С����Ϊ��
//    LCD_Fill(x0+4, y0+1, x0+5, y0+2, BLACK);  // Ҷ�Ӿ�
}

// �޸�Snake_CreateFood
void Snake_CreateFood(void)
{
    uint8_t isValid;
    do {
        isValid = 1;
        game.food.x = rand() % (GAME_AREA_WIDTH / GRID_SIZE);
        game.food.y = rand() % (GAME_AREA_HEIGHT / GRID_SIZE);
        
        for(int i = 0; i < game.length; i++) {
            if(game.food.x == game.snake[i].x && 
               game.food.y == game.snake[i].y) {
                isValid = 0;
                break;
            }
        }
    } while(!isValid);
    
    DrawFood(game.food.x, game.food.y);
}

uint8_t Snake_CheckCollision(uint16_t next_x, uint16_t next_y)
{
    // ����Ƿ�ײǽ
    if(next_x >= GAME_AREA_WIDTH / GRID_SIZE ||
       next_y >= GAME_AREA_HEIGHT / GRID_SIZE)
        return 1;
    
    // ����Ƿ�ײ���Լ�
    for(int i = 1; i < game.length; i++) {
        if(next_x == game.snake[i].x &&
           next_y == game.snake[i].y)
            return 1;
    }
    
    return 0;
}

void Snake_Move(void)
{
    // ����β��λ��(���ڲ���)
    uint16_t tail_x = game.snake[game.length-1].x;
    uint16_t tail_y = game.snake[game.length-1].y;
    
    // ����ԭ����ͷ��λ��(���ں������Ϊ����)
    uint16_t old_head_x = game.snake[0].x;
    uint16_t old_head_y = game.snake[0].y;
    
    // ������һ��λ��
    uint16_t next_x = game.snake[0].x;
    uint16_t next_y = game.snake[0].y;
    
    switch(game.direction) {
        case DIR_UP:    next_y--; break;
        case DIR_DOWN:  next_y++; break;
        case DIR_LEFT:  next_x--; break;
        case DIR_RIGHT: next_x++; break;
    }
    
    // �����һ��λ���Ƿ�ᷢ����ײ
    if(Snake_CheckCollision(next_x, next_y)) {
        // ���ƶ��ߣ�ֱ����ʾ��Ϸ����
        game.isGameOver = 1;
        
        // ��ʾ��Ϸ������Ϣ
        LCD_Fill(GAME_AREA_X0 + 45, GAME_AREA_Y0 + 100, 
                GAME_AREA_X0 + 45 + 120, GAME_AREA_Y0 + 124, WHITE);
        LCD_Fill(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 130,
                GAME_AREA_X0 + 20 + 160, GAME_AREA_Y0 + 146, WHITE);
                
        LCD_ShowString(GAME_AREA_X0 + 45, GAME_AREA_Y0 + 100, 200, 24, 24, (uint8_t*)"Game Over!");
        LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 130, 200, 16, 16, (uint8_t*)"Press KEY0 to restart");
        
        // �ػ���ܱ����ָ��ǵı߿�
        Snake_DrawGameArea();
        return;
    }
    
    // �����ƶ�(��β����ʼ,ÿ���ڵ㵽ǰһ���ڵ��λ��)
    for(int i = game.length-1; i > 0; i--) {
        game.snake[i].x = game.snake[i-1].x;
        game.snake[i].y = game.snake[i-1].y;
    }
    
    // �ƶ���ͷ����λ��
    game.snake[0].x = next_x;
    game.snake[0].y = next_y;
    
    // ����Ƿ�Ե�ʳ��
    if(game.snake[0].x == game.food.x && game.snake[0].y == game.food.y) {
        // �����ߵĳ���
        game.snake[game.length].x = tail_x;
        game.snake[game.length].y = tail_y;
        game.length++;
        game.score += 10;
        LCD_ShowNum(GAME_AREA_X0 + 50, GAME_AREA_Y0 - 20, game.score, 4, 16);
        Snake_CreateFood();
    } else {
        // ����β��
        LCD_Fill(GAME_AREA_X0 + tail_x * GRID_SIZE,
                GAME_AREA_Y0 + tail_y * GRID_SIZE,
                GAME_AREA_X0 + (tail_x + 1) * GRID_SIZE - 1,
                GAME_AREA_Y0 + (tail_y + 1) * GRID_SIZE - 1,
                WHITE);
                
        // �ػ���ܱ������ı߿�
        if(tail_x == 0 || tail_x == (GAME_AREA_WIDTH / GRID_SIZE - 1) ||
           tail_y == 0 || tail_y == (GAME_AREA_HEIGHT / GRID_SIZE - 1)) {
            Snake_DrawGameArea();
        }
    }
    
    // ��ԭ������ͷλ�ø�Ϊ��ɫ(������ɫ)
    LCD_Fill(GAME_AREA_X0 + old_head_x * GRID_SIZE,
             GAME_AREA_Y0 + old_head_y * GRID_SIZE,
             GAME_AREA_X0 + (old_head_x + 1) * GRID_SIZE - 1,
             GAME_AREA_Y0 + (old_head_y + 1) * GRID_SIZE - 1,
             GREEN);
             
    // �ػ���ܱ����ǵı߿�
    if(old_head_x == 0 || old_head_x == (GAME_AREA_WIDTH / GRID_SIZE - 1) ||
       old_head_y == 0 || old_head_y == (GAME_AREA_HEIGHT / GRID_SIZE - 1)) {
        Snake_DrawGameArea();
    }
    
    // �����µ���ͷ
    LCD_Fill(GAME_AREA_X0 + game.snake[0].x * GRID_SIZE,
             GAME_AREA_Y0 + game.snake[0].y * GRID_SIZE,
             GAME_AREA_X0 + (game.snake[0].x + 1) * GRID_SIZE - 1,
             GAME_AREA_Y0 + (game.snake[0].y + 1) * GRID_SIZE - 1,
             BLUE);
             
    // �ػ���ܱ����ǵı߿�
    if(game.snake[0].x == 0 || game.snake[0].x == (GAME_AREA_WIDTH / GRID_SIZE - 1) ||
       game.snake[0].y == 0 || game.snake[0].y == (GAME_AREA_HEIGHT / GRID_SIZE - 1)) {
        Snake_DrawGameArea();
    }
}

// �޸���Ϸ��������
void Snake_GameOver(void)
{
    game.isGameOver = 1;
    
    // �����Ϸ����
    LCD_Fill(GAME_AREA_X0, GAME_AREA_Y0,
             GAME_AREA_X0 + GAME_AREA_WIDTH,
             GAME_AREA_Y0 + GAME_AREA_HEIGHT,
             WHITE);
    
    // ��ʾ��Ϸ������Ϣ����Ư���Ĳ��֣�
    LCD_ShowString(GAME_AREA_X0 + 35, GAME_AREA_Y0 + 80, 200, 32, 32, (uint8_t*)"Game Over!");
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 130+50, 200, 16, 16, (uint8_t*)"Final Score:");
    LCD_ShowNum(GAME_AREA_X0 + 120, GAME_AREA_Y0 + 130+50, game.score, 4, 16);
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 160+50, 200, 16, 16, (uint8_t*)"Press KEY0 to MainMenu");
    
    // �ػ�߿�
    Snake_DrawGameArea();
}

// �޸�Snake_Run����
void Snake_Run(void)
{
    switch(game.gameState) {
        case GAME_STATE_MENU:
            if(Key0_GetNum() == 1) {
                // ��ʼ����Ϸ
                game.gameState = GAME_STATE_PLAYING;
                game.length = 3;
                game.direction = DIR_RIGHT;
                game.score = 0;
                game.isGameOver = 0;
                
                // ��������ʼ����Ϸ����
                LCD_Clear(WHITE);
                Snake_DrawGameArea();
                
                // ��ʼ���ߵ�λ��
                game.snake[0].x = GAME_AREA_WIDTH / 2 / GRID_SIZE;
                game.snake[0].y = GAME_AREA_HEIGHT / 2 / GRID_SIZE;
                
                for(int i = 1; i < game.length; i++) {
                    game.snake[i].x = game.snake[0].x - i;
                    game.snake[i].y = game.snake[0].y;
                    DrawSnakeNode(game.snake[i].x, game.snake[i].y, GREEN);
                }
                
                DrawSnakeNode(game.snake[0].x, game.snake[0].y, BLUE);
                Snake_CreateFood();
            }
            break;
            
        case GAME_STATE_PLAYING:
            // ԭ������Ϸ�߼�
            if(game.isGameOver) {
                game.gameState = GAME_STATE_OVER;
                Snake_GameOver();
                break;
            }
            
            // ����������
            if(Key_up_GetNum() == 1 && game.direction != DIR_DOWN) {
                game.direction = DIR_UP;
            }
            else if(Key1_GetNum() == 1 && game.direction != DIR_UP) {
                game.direction = DIR_DOWN;
            }
            else if(Key2_GetNum() == 1 && game.direction != DIR_RIGHT) {
                game.direction = DIR_LEFT;
            }
            else if(Key0_GetNum() == 1 && game.direction != DIR_LEFT) {
                game.direction = DIR_RIGHT;
            }
            
            Snake_Move();
            HAL_Delay(200);
            break;
            
        case GAME_STATE_OVER:
            if(Key0_GetNum() == 1) {
                // �������˵�
                Snake_ShowMainMenu();
                game.gameState = GAME_STATE_MENU;
            }
            break;
    }
}
