#include "snake.h"
#include "lcd.h"
#include "key.h"
#include <stdlib.h>

static GameState game;

// 绘制蛇图标
void Snake_DrawLogo(void)
{
    const uint16_t logo_x = GAME_AREA_X0 + 60;
    const uint16_t logo_y = GAME_AREA_Y0 + 60;
    
    // 绘制蛇身（S形）
    uint16_t points[][2] = {
        {0, 0},   {1, 0},   {2, 0},    // ■■■
        {2, 1},   {2, 2},               //     ■■
        {1, 2},   {0, 2},               // ■■■
        {0, 3},   {0, 4},               // ■
        {1, 4},   {2, 4},   {3, 4}      // ■■■■
    };
    
    // 放大倍数
    const uint16_t scale = 20;
    
    // 绘制每个点（蛇身）
    for(int i = 0; i < sizeof(points)/sizeof(points[0])-1; i++) {
        LCD_Fill(logo_x + points[i][0] * scale,
                logo_y + points[i][1] * scale,
                logo_x + (points[i][0] + 1) * scale - 2,
                logo_y + (points[i][1] + 1) * scale - 2,
                GREEN);
    }
    
    // 绘制蛇头（最后一个点）
    uint16_t head_x = logo_x + points[sizeof(points)/sizeof(points[0])-1][0] * scale;
    uint16_t head_y = logo_y + points[sizeof(points)/sizeof(points[0])-1][1] * scale;
    
    // 蛇头主体
    LCD_Fill(head_x, head_y,
            head_x + scale - 2,
            head_y + scale - 2,
            BLUE);
    
    // 添加眼睛（两个小黑点）
    LCD_Fill(head_x + scale/4, head_y + scale/4,
            head_x + scale/4 + 2, head_y + scale/4 + 2,
            BLACK);
    LCD_Fill(head_x + scale*3/4 - 2, head_y + scale/4,
            head_x + scale*3/4, head_y + scale/4 + 2,
            BLACK);
    
    // 添加舌头（红色三角形）
    LCD_Fill(head_x + scale/2 - 1, head_y + scale - 2,
            head_x + scale/2 + 1, head_y + scale + 4,
            RED);
}

// 显示主菜单
void Snake_ShowMainMenu(void)
{
    // 清屏
    LCD_Clear(WHITE);
    
    // 绘制边框
    LCD_DrawRectangle(GAME_AREA_X0, GAME_AREA_Y0,
                     GAME_AREA_X0 + GAME_AREA_WIDTH,
                     GAME_AREA_Y0 + GAME_AREA_HEIGHT);
    
    // 显示游戏标题
    LCD_ShowString(GAME_AREA_X0 + 10, GAME_AREA_Y0 + 20, 200, 32, 32, (uint8_t*)"GREEDY SNAKE");
    // 绘制蛇图标
    Snake_DrawLogo();
    
    // 显示开始提示
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 180, 200, 16, 16, (uint8_t*)"Press KEY0 to Start");
    
    // 显示控制说明
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 210, 200, 16, 16, (uint8_t*)"Controls:");
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 230, 200, 16, 16, (uint8_t*)"KEY_UP: Up");
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 250, 200, 16, 16, (uint8_t*)"KEY1: Down");
    LCD_ShowString(GAME_AREA_X0 + 120, GAME_AREA_Y0 + 230, 200, 16, 16, (uint8_t*)"KEY2: Left");
    LCD_ShowString(GAME_AREA_X0 + 120, GAME_AREA_Y0 + 250, 200, 16, 16, (uint8_t*)"KEY0: Right");
}

// 修改Snake_Init函数
void Snake_Init(void)
{
    // 显示主菜单
    Snake_ShowMainMenu();
    game.gameState = GAME_STATE_MENU;
}

// 绘制游戏区域
void Snake_DrawGameArea(void)
{
    // 绘制边框（使用双线）
    LCD_DrawRectangle(GAME_AREA_X0, GAME_AREA_Y0, 
                     GAME_AREA_X0 + GAME_AREA_WIDTH, 
                     GAME_AREA_Y0 + GAME_AREA_HEIGHT);
    LCD_DrawRectangle(GAME_AREA_X0+1, GAME_AREA_Y0+1, 
                     GAME_AREA_X0 + GAME_AREA_WIDTH-1, 
                     GAME_AREA_Y0 + GAME_AREA_HEIGHT-1);
    
    // 显示分数（在游戏区域上方）
    LCD_ShowString(GAME_AREA_X0, GAME_AREA_Y0 - 20, 200, 16, 16, (uint8_t*)"Score:");
    LCD_ShowNum(GAME_AREA_X0 + 15, GAME_AREA_Y0 - 20, game.score, 4, 20);
}
// 绘制蛇身节点
void DrawSnakeNode(uint16_t x, uint16_t y, uint16_t color)
{
    // 绘制圆角方块
    uint16_t x0 = GAME_AREA_X0 + x * GRID_SIZE;
    uint16_t y0 = GAME_AREA_Y0 + y * GRID_SIZE;
    uint16_t size = GRID_SIZE - 1;  // 留出1像素间隔
    
    // 填充主体
    LCD_Fill(x0+1, y0+1, x0+size-1, y0+size-1, color);
    
    // 绘制边框
    LCD_DrawRectangle(x0+1, y0+1, x0+size-1, y0+size-1);
}

// 绘制食物
void DrawFood(uint16_t x, uint16_t y)
{
    uint16_t x0 = GAME_AREA_X0 + x * GRID_SIZE;
    uint16_t y0 = GAME_AREA_Y0 + y * GRID_SIZE;
    uint16_t size = GRID_SIZE - 2;  // 食物比蛇身小一点
    
    // 绘制苹果形状
    LCD_Fill(x0+2, y0+2, x0+size-1, y0+size-1, RED);  // 主体
    LCD_Fill(x0+3, y0+1, x0+4, y0+2, BLACK);      // 叶子
    
//    // 绘制一个小点作为茎
//    LCD_Fill(x0+4, y0+1, x0+5, y0+2, BLACK);  // 叶子茎
}

// 修改Snake_CreateFood
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
    // 检查是否撞墙
    if(next_x >= GAME_AREA_WIDTH / GRID_SIZE ||
       next_y >= GAME_AREA_HEIGHT / GRID_SIZE)
        return 1;
    
    // 检查是否撞到自己
    for(int i = 1; i < game.length; i++) {
        if(next_x == game.snake[i].x &&
           next_y == game.snake[i].y)
            return 1;
    }
    
    return 0;
}

void Snake_Move(void)
{
    // 保存尾部位置(用于擦除)
    uint16_t tail_x = game.snake[game.length-1].x;
    uint16_t tail_y = game.snake[game.length-1].y;
    
    // 保存原来蛇头的位置(用于后面绘制为蛇身)
    uint16_t old_head_x = game.snake[0].x;
    uint16_t old_head_y = game.snake[0].y;
    
    // 计算下一个位置
    uint16_t next_x = game.snake[0].x;
    uint16_t next_y = game.snake[0].y;
    
    switch(game.direction) {
        case DIR_UP:    next_y--; break;
        case DIR_DOWN:  next_y++; break;
        case DIR_LEFT:  next_x--; break;
        case DIR_RIGHT: next_x++; break;
    }
    
    // 检查下一个位置是否会发生碰撞
    if(Snake_CheckCollision(next_x, next_y)) {
        // 不移动蛇，直接显示游戏结束
        game.isGameOver = 1;
        
        // 显示游戏结束信息
        LCD_Fill(GAME_AREA_X0 + 45, GAME_AREA_Y0 + 100, 
                GAME_AREA_X0 + 45 + 120, GAME_AREA_Y0 + 124, WHITE);
        LCD_Fill(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 130,
                GAME_AREA_X0 + 20 + 160, GAME_AREA_Y0 + 146, WHITE);
                
        LCD_ShowString(GAME_AREA_X0 + 45, GAME_AREA_Y0 + 100, 200, 24, 24, (uint8_t*)"Game Over!");
        LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 130, 200, 16, 16, (uint8_t*)"Press KEY0 to restart");
        
        // 重绘可能被文字覆盖的边框
        Snake_DrawGameArea();
        return;
    }
    
    // 身体移动(从尾部开始,每个节点到前一个节点的位置)
    for(int i = game.length-1; i > 0; i--) {
        game.snake[i].x = game.snake[i-1].x;
        game.snake[i].y = game.snake[i-1].y;
    }
    
    // 移动蛇头到新位置
    game.snake[0].x = next_x;
    game.snake[0].y = next_y;
    
    // 检查是否吃到食物
    if(game.snake[0].x == game.food.x && game.snake[0].y == game.food.y) {
        // 增加蛇的长度
        game.snake[game.length].x = tail_x;
        game.snake[game.length].y = tail_y;
        game.length++;
        game.score += 10;
        LCD_ShowNum(GAME_AREA_X0 + 50, GAME_AREA_Y0 - 20, game.score, 4, 16);
        Snake_CreateFood();
    } else {
        // 擦除尾部
        LCD_Fill(GAME_AREA_X0 + tail_x * GRID_SIZE,
                GAME_AREA_Y0 + tail_y * GRID_SIZE,
                GAME_AREA_X0 + (tail_x + 1) * GRID_SIZE - 1,
                GAME_AREA_Y0 + (tail_y + 1) * GRID_SIZE - 1,
                WHITE);
                
        // 重绘可能被擦除的边框
        if(tail_x == 0 || tail_x == (GAME_AREA_WIDTH / GRID_SIZE - 1) ||
           tail_y == 0 || tail_y == (GAME_AREA_HEIGHT / GRID_SIZE - 1)) {
            Snake_DrawGameArea();
        }
    }
    
    // 将原来的蛇头位置改为绿色(蛇身颜色)
    LCD_Fill(GAME_AREA_X0 + old_head_x * GRID_SIZE,
             GAME_AREA_Y0 + old_head_y * GRID_SIZE,
             GAME_AREA_X0 + (old_head_x + 1) * GRID_SIZE - 1,
             GAME_AREA_Y0 + (old_head_y + 1) * GRID_SIZE - 1,
             GREEN);
             
    // 重绘可能被覆盖的边框
    if(old_head_x == 0 || old_head_x == (GAME_AREA_WIDTH / GRID_SIZE - 1) ||
       old_head_y == 0 || old_head_y == (GAME_AREA_HEIGHT / GRID_SIZE - 1)) {
        Snake_DrawGameArea();
    }
    
    // 绘制新的蛇头
    LCD_Fill(GAME_AREA_X0 + game.snake[0].x * GRID_SIZE,
             GAME_AREA_Y0 + game.snake[0].y * GRID_SIZE,
             GAME_AREA_X0 + (game.snake[0].x + 1) * GRID_SIZE - 1,
             GAME_AREA_Y0 + (game.snake[0].y + 1) * GRID_SIZE - 1,
             BLUE);
             
    // 重绘可能被覆盖的边框
    if(game.snake[0].x == 0 || game.snake[0].x == (GAME_AREA_WIDTH / GRID_SIZE - 1) ||
       game.snake[0].y == 0 || game.snake[0].y == (GAME_AREA_HEIGHT / GRID_SIZE - 1)) {
        Snake_DrawGameArea();
    }
}

// 修改游戏结束界面
void Snake_GameOver(void)
{
    game.isGameOver = 1;
    
    // 清除游戏区域
    LCD_Fill(GAME_AREA_X0, GAME_AREA_Y0,
             GAME_AREA_X0 + GAME_AREA_WIDTH,
             GAME_AREA_Y0 + GAME_AREA_HEIGHT,
             WHITE);
    
    // 显示游戏结束信息（更漂亮的布局）
    LCD_ShowString(GAME_AREA_X0 + 35, GAME_AREA_Y0 + 80, 200, 32, 32, (uint8_t*)"Game Over!");
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 130+50, 200, 16, 16, (uint8_t*)"Final Score:");
    LCD_ShowNum(GAME_AREA_X0 + 120, GAME_AREA_Y0 + 130+50, game.score, 4, 16);
    LCD_ShowString(GAME_AREA_X0 + 20, GAME_AREA_Y0 + 160+50, 200, 16, 16, (uint8_t*)"Press KEY0 to MainMenu");
    
    // 重绘边框
    Snake_DrawGameArea();
}

// 修改Snake_Run函数
void Snake_Run(void)
{
    switch(game.gameState) {
        case GAME_STATE_MENU:
            if(Key0_GetNum() == 1) {
                // 开始新游戏
                game.gameState = GAME_STATE_PLAYING;
                game.length = 3;
                game.direction = DIR_RIGHT;
                game.score = 0;
                game.isGameOver = 0;
                
                // 清屏并初始化游戏界面
                LCD_Clear(WHITE);
                Snake_DrawGameArea();
                
                // 初始化蛇的位置
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
            // 原来的游戏逻辑
            if(game.isGameOver) {
                game.gameState = GAME_STATE_OVER;
                Snake_GameOver();
                break;
            }
            
            // 处理按键输入
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
                // 返回主菜单
                Snake_ShowMainMenu();
                game.gameState = GAME_STATE_MENU;
            }
            break;
    }
}
