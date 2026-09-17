// qiu_qiu.cpp : 只改路径版，不改其他内容
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable:4996)

#include <winsock2.h>
#include <iphlpapi.h>
#include <graphics.h>
#include <cmath>
#include <algorithm>
#include <functional>
#include <vector>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#pragma comment (lib,"winmm.lib")
#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

COLORREF color_coco = RGB(190, 40, 220);
COLORREF color_yp = RGB(138, 43, 226);
int view_width = 1200;
int view_height = 800;
const int VIEW_WIDTH = 1200;
const int VIEW_HEIGHT = 800;
const int BALL_COUNT = 20;
const int FOOD_COUNT = 90;
const int TARGET_FPS = 144;

const int MAX_LEADERBOARD_ENTRIES = 10;
std::vector<int> g_leaderboardScores;

void addScoreToLeaderboard(int newScore);

struct Ball {
    int x;
    int y;
    int r;
    int vx;
    int vy;
    COLORREF color;
};

const int BALL_COUNT_TOTAL = 30;
std::vector<Ball> g_balls;
bool g_balls_inited = false;

bool is_target_area(int x, int y) {
    return (x < 300 || x > 900 || y < 100 || y > 650);
}

void init_balls(std::vector<Ball>& balls) {
    srand((unsigned int)time(NULL));

    for (int a = 0; a < BALL_COUNT_TOTAL; a++) {
        Ball ball;

        do {
            ball.x = rand() % 1200;
            ball.y = rand() % 800;
        } while (!is_target_area(ball.x, ball.y));

        ball.r = rand() % 10 + 10;
        ball.vx = rand() % 10 - 5;
        if (ball.vx == 0) ball.vx = 1;
        ball.vy = rand() % 10 - 5;
        if (ball.vy == 0) ball.vy = 1;
        ball.color = RGB(rand() % 256, rand() % 256, rand() % 256);

        balls.push_back(ball);
    }
}

void update_balls(std::vector<Ball>& balls) {
    for (auto& ball : balls) {
        int oldX = ball.x;
        int oldY = ball.y;

        ball.x += ball.vx;
        ball.y += ball.vy;

        if (ball.x + ball.r >= 300 && ball.x - ball.r < 300) {
            ball.x = 300 - ball.r - 1;
            ball.vx = -ball.vx;
        }
        else if (ball.x - ball.r <= 900 && ball.x + ball.r > 900) {
            ball.x = 900 + ball.r + 1;
            ball.vx = -ball.vx;
        }

        if (ball.y + ball.r >= 100 && ball.y - ball.r < 100) {
            ball.y = 100 - ball.r - 1;
            ball.vy = -ball.vy;
        }
        else if (ball.y - ball.r <= 650 && ball.y + ball.r > 650) {
            ball.y = 650 + ball.r + 1;
            ball.vy = -ball.vy;
        }

        if (ball.x - ball.r < 0) {
            ball.x = ball.r;
            ball.vx = -ball.vx;
        }
        if (ball.x + ball.r > 1200) {
            ball.x = 1200 - ball.r;
            ball.vx = -ball.vx;
        }
        if (ball.y - ball.r < 0) {
            ball.y = ball.r;
            ball.vy = -ball.vy;
        }
        if (ball.y + ball.r > 800) {
            ball.y = 800 - ball.r;
            ball.vy = -ball.vy;
        }
    }
}

void draw_scene(const std::vector<Ball>& balls) {
    for (const auto& ball : balls) {
        setfillcolor(ball.color);
        solidcircle(ball.x, ball.y, ball.r);
    }
}

void rings_20() {
    if (!g_balls_inited) {
        init_balls(g_balls);
        g_balls_inited = true;
    }
    update_balls(g_balls);
    draw_scene(g_balls);
}

void handleLeftButtonDown(int x, int y)
{
    if (y > 30 && y < 90)
    {
        if (x > 220 && x < 260) { color_coco = RGB(190, 40, 220); color_yp = RGB(138, 43, 226); }
        else if (x > 430 && x < 470) { color_coco = RGB(106, 90, 205); color_yp = RGB(72, 61, 139); }
        else if (x > 640 && x < 680) { color_coco = RGB(70, 130, 180); color_yp = RGB(90, 150, 200); }
        else if (x > 850 && x < 890) { color_coco = RGB(255, 255, 0); color_yp = RGB(204, 153, 51); }
    }
    else if (y > 240 && y < 300)
    {
        if (x > 220 && x < 260) { color_coco = RGB(64, 224, 208); color_yp = RGB(0, 128, 128); }
        else if (x > 430 && x < 470) { color_coco = RGB(50, 205, 50); color_yp = RGB(34, 139, 34); }
        else if (x > 640 && x < 680) { color_coco = RGB(154, 205, 50); color_yp = RGB(174, 215, 70); }
        else if (x > 850 && x < 890) { color_coco = RGB(255, 200, 0); color_yp = RGB(205, 133, 63); }
    }
    else if (y > 450 && y < 510)
    {
        if (x > 220 && x < 260) { color_coco = RGB(255, 165, 0); color_yp = RGB(204, 85, 0); }
        else if (x > 430 && x < 470) { color_coco = RGB(235, 80, 50); color_yp = RGB(160, 60, 45); }
        else if (x > 640 && x < 680) { color_coco = RGB(160, 30, 40); color_yp = RGB(128, 0, 32); }
    }
}

bool draw_rings = false;
int base_radius = 0;
clock_t ring_start = 0;

void rings_effect() {
    if (!draw_rings) return;

    int current_radius = base_radius;
    bool is_black = true;
    ExMessage msg;

    while (current_radius > 0) {
        if (is_black) setfillcolor(RGB(0, 0, 0));
        else setfillcolor(RGB(255, 255, 255));
        solidcircle(1200 / 2, 800 / 2, current_radius);
        is_black = !is_black;
        current_radius -= 20;

        setfillcolor(color_coco);
        solidcircle(1000, 450, 100);
        setfillcolor(RGB(220, 255, 250));
        solidcircle(1000 - 40, 450 - 25, 32);
        solidcircle(1000 + 40, 450 - 25, 32);
        settextcolor(RGB(0, 2, 2));
        outtextxy(900 + 16, 550, _T("Coco"));

        setfillcolor(RGB(0, 2, 2));
        solidcircle(1000 - 40, 450 - 25, 19);
        solidcircle(1000 + 40, 450 - 25, 19);
    }

    if (clock() - ring_start < 20000 && base_radius < 5000) base_radius += 5;
    else { draw_rings = false; base_radius = 0; }
}

float grayProgress = 0.0f;
DWORD startTime = 0;
const int GRADIENT_DURATION = 8000;

void convertToGrayscale(IMAGE* src, IMAGE* dst) {
    if (startTime == 0) startTime = GetTickCount();

    DWORD currentTime = GetTickCount();
    DWORD elapsedTime = currentTime - startTime;
    grayProgress = (elapsedTime < GRADIENT_DURATION) ?
        (float)elapsedTime / GRADIENT_DURATION : 1.0f;

    DWORD* pSrc = GetImageBuffer(src);
    DWORD* pDst = GetImageBuffer(dst);
    int w = src->getwidth();
    int h = src->getheight();

    for (int i = 0; i < w * h; i++) {
        COLORREF c = pSrc[i];
        BYTE r = GetRValue(c);
        BYTE g = GetGValue(c);
        BYTE b = GetBValue(c);
        BYTE targetGray = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b);
        BYTE newR = (BYTE)(r * (1 - grayProgress) + targetGray * grayProgress);
        BYTE newG = (BYTE)(g * (1 - grayProgress) + targetGray * grayProgress);
        BYTE newB = (BYTE)(b * (1 - grayProgress) + targetGray * grayProgress);
        pDst[i] = RGB(newR, newG, newB);
    }
}

bool musicEnabled = true;
int musicVolume = 500;

void InitMusic() {
    mciSendStringA("close mymusic", NULL, 0, NULL);
}

void PlayBackgroundMusic() {
    if (!musicEnabled) return;
    mciSendStringA("open \".\\meitu\\aigei_com1.mp3\" alias mymusic", NULL, 0, NULL);
    mciSendStringA("play mymusic repeat", NULL, 0, NULL);
}

void StopBackgroundMusic() {
    mciSendStringA("stop mymusic", NULL, 0, NULL);
    mciSendStringA("close mymusic", NULL, 0, NULL);
}

void PauseBackgroundMusic() {
    mciSendStringA("pause mymusic", NULL, 0, NULL);
}

void ResumeBackgroundMusic() {
    if (!musicEnabled) return;
    mciSendStringA("resume mymusic", NULL, 0, NULL);
}

void PlayEatSound() {
    if (!musicEnabled) return;
    Beep(800, 100);
    Beep(1000, 100);
}

void PlayGameOverSound() {
    if (!musicEnabled) return;
    Beep(400, 500);
    Beep(300, 500);
    Beep(200, 500);
}

void PlayButtonSound() {
    if (!musicEnabled) return;
    Beep(600, 50);
}

struct Food {
    float x, y, r;
    COLORREF color;
    int exist;
};

struct AIBall {
    int x, y, r, vx, vy;
    COLORREF color;
};

struct PlayerBall {
    float x, y, r;
    COLORREF color;
    int score;
} player;

std::vector<AIBall> g_aiBalls;
Food g_foods[FOOD_COUNT];
bool g_ai_balls_inited = false;
bool g_game_over = false;
int g_game_score = 0;

void initPlayer() {
    player.x = VIEW_WIDTH / 2.0f;
    player.y = VIEW_HEIGHT / 2.0f;
    player.r = 15.0f;
    player.color = color_coco;
    player.score = 0;
}

bool isTargetArea(int x, int y) {
    return (x < 1600 && x > 0 && y > 0 && y < 1000);
}

void initAIBalls() {
    g_aiBalls.clear();
    srand((unsigned int)time(NULL));
    for (int i = 0; i < BALL_COUNT; i++) {
        AIBall b;
        do {
            b.x = rand() % VIEW_WIDTH;
            b.y = rand() % VIEW_HEIGHT;
        } while (!isTargetArea(b.x, b.y));

        b.r = rand() % 10 + 10;
        b.vx = rand() % 16 - 8;
        if (b.vx == 0) b.vx = rand() % 2 ? 1 : -1;
        b.vy = rand() % 16 - 8;
        if (b.vy == 0) b.vy = rand() % 2 ? 1 : -1;
        b.color = RGB(rand() % 256, rand() % 256, rand() % 256);
        g_aiBalls.push_back(b);
    }
}

int i = 2;

void generateAIBall() {
    if (g_game_over) return;
    AIBall newBall;
    bool valid = false;

    while (!valid) {
        do {
            newBall.x = rand() % VIEW_WIDTH;
            newBall.y = rand() % VIEW_HEIGHT;
        } while (!isTargetArea(newBall.x, newBall.y));

        i = i + 2;
        if (i > 100) i = 2;
        newBall.r = rand() % i + 20;
        newBall.vx = rand() % 12 - 8;
        if (newBall.vx == 0) newBall.vx = rand() % 2 ? 1 : -1;
        newBall.vy = rand() % 12 - 8;
        if (newBall.vy == 0) newBall.vy = rand() % 2 ? 1 : -1;
        newBall.color = RGB(rand() % 256, rand() % 256, rand() % 256);

        float dx = newBall.x - player.x;
        float dy = newBall.y - player.y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist > newBall.r + player.r + 10) valid = true;
    }
    g_aiBalls.push_back(newBall);
}

void initFoods() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < FOOD_COUNT; i++) {
        g_foods[i].x = rand() % VIEW_WIDTH;
        g_foods[i].y = rand() % VIEW_HEIGHT;
        g_foods[i].r = 4.0f;
        g_foods[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
        g_foods[i].exist = 1;
    }
}

void generateFood(int index) {
    if (index < 0 || index >= FOOD_COUNT) return;
    float newX, newY, d;
    do {
        newX = rand() % VIEW_WIDTH;
        newY = rand() % VIEW_HEIGHT;
        d = pow(newX - player.x, 2) + pow(newY - player.y, 2) - pow(g_foods[index].r + player.r, 2);
    } while (d <= 0);

    g_foods[index].x = newX;
    g_foods[index].y = newY;
    g_foods[index].color = RGB(rand() % 256, rand() % 256, rand() % 256);
    g_foods[index].exist = 1;
}

void movePlayerBall() {
    float baseSpeed = 8.0f;
    float speed = baseSpeed;
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) speed = baseSpeed * 1.5f;

    if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000) player.y -= speed;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000) player.y += speed;
    if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000) player.x -= speed;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000) player.x += speed;

    if (player.x - player.r < 0) player.x = player.r;
    if (player.x + player.r > VIEW_WIDTH) player.x = VIEW_WIDTH - player.r;
    if (player.y - player.r < 0) player.y = player.r;
    if (player.y + player.r > VIEW_HEIGHT) player.y = VIEW_HEIGHT - player.r;
}

void updateAIBalls() {
    for (auto& b : g_aiBalls) {
        int oldX = b.x, oldY = b.y;
        b.x += b.vx;
        b.y += b.vy;

        if (b.x - b.r < 0) { b.x = b.r; b.vx = -b.vx; }
        if (b.x + b.r > VIEW_WIDTH) { b.x = VIEW_WIDTH - b.r; b.vx = -b.vx; }
        if (b.y - b.r < 0) { b.y = b.r; b.vy = -b.vy; }
        if (b.y + b.r > VIEW_HEIGHT) { b.y = VIEW_HEIGHT - b.r; b.vy = -b.vy; }

        if (!isTargetArea(b.x, b.y)) {
            b.x = oldX;
            b.y = oldY;
            b.vx = -b.vx;
            b.vy = -b.vy;
        }
    }
}

void eatFood() {
    for (int i = 0; i < FOOD_COUNT; i++) {
        if (g_foods[i].exist == 0) continue;
        float dx = g_foods[i].x - player.x;
        float dy = g_foods[i].y - player.y;
        float dist = sqrt(dx * dx + dy * dy);

        if (dist < player.r + g_foods[i].r) {
            player.r += 0.3f;
            player.score++;
            g_foods[i].exist = 0;
            PlayEatSound();
            generateFood(i);
        }
    }
}

void checkAICollision() {
    int eatenCount = 0;
    bool wasGameOverBefore = g_game_over;
    for (auto it = g_aiBalls.begin(); it != g_aiBalls.end();) {
        float dx = it->x - player.x;
        float dy = it->y - player.y;
        float dist = sqrt(dx * dx + dy * dy);

        if (dist < player.r + it->r) {
            if (player.r > it->r) {
                player.r += it->r / 8.0f;
                player.score += it->r;
                PlayEatSound();
                it = g_aiBalls.erase(it);
                eatenCount++;
            }
            else {
                g_game_over = true;
                PlayGameOverSound();
                return;
            }
        }
        else ++it;
    }
    if (!wasGameOverBefore && g_game_over) addScoreToLeaderboard(player.score);
    for (int i = 0; i < eatenCount; i++) generateAIBall();
}

void drawPlayerBall() {
    setfillcolor(player.color);
    solidcircle((int)player.x, (int)player.y, (int)player.r);
}

void drawAIBalls() {
    for (const auto& b : g_aiBalls) {
        setfillcolor(b.color);
        solidcircle(b.x, b.y, b.r);
    }
}

void drawFoods() {
    for (int i = 0; i < FOOD_COUNT; i++) {
        if (g_foods[i].exist) {
            setfillcolor(g_foods[i].color);
            solidcircle((int)g_foods[i].x, (int)g_foods[i].y, (int)g_foods[i].r);
        }
    }
}

void drawGameUI() {
    settextcolor(RGB(255, 0, 0));
    settextstyle(30, 0, _T("微软雅黑"));
    TCHAR scoreStr[50], sizeStr[50], aiCountStr[50];
    wsprintf(scoreStr, _T("分数：%d"), player.score);
    wsprintf(sizeStr, _T("大小：%.0f"), player.r);
    wsprintf(aiCountStr, _T("剩余AI：%d"), g_aiBalls.size());
    outtextxy(10, 12, scoreStr);
    outtextxy(10, 52, sizeStr);
    outtextxy(10, 92, aiCountStr);

    settextcolor(RGB(100, 100, 100));
    settextstyle(20, 0, _T("微软雅黑"));
    outtextxy(10, 140, _T("按住Shift加速"));
}

void drawGameOver() {
    settextcolor(RGB(255, 0, 0));
    settextstyle(48, 0, _T("微软雅黑"));
    outtextxy(VIEW_WIDTH / 2 - 100, VIEW_HEIGHT / 2 - 50, _T("游戏结束"));

    TCHAR scoreStr[50];
    wsprintf(scoreStr, _T("最终分数：%d"), player.score);
    settextstyle(36, 0, _T("微软雅黑"));
    outtextxy(VIEW_WIDTH / 2 - 100, VIEW_HEIGHT / 2 + 20, scoreStr);

    settextstyle(24, 0, _T("微软雅黑"));
    outtextxy(VIEW_WIDTH / 2 - 80, VIEW_HEIGHT / 2 + 80, _T("按空格键返回菜单"));
}

void resetGame() {
    initPlayer();
    initAIBalls();
    initFoods();
    g_game_over = false;
}

void drawGridBackground() {
    setlinecolor(RGB(245, 230, 250));
    setlinestyle(PS_DASHDOT, 1);
    for (int x = 0; x <= VIEW_WIDTH; x += 24) line(x, 0, x, VIEW_HEIGHT);
    for (int y = 4; y <= VIEW_HEIGHT; y += 24) line(0, y, VIEW_WIDTH, y);
}

enum GameStateType {
    menu,
    game,
    shop,
    pause,
    pregame,
    leaderboard,
    netmenu,
    nethost,
    netinput,
    netconnect,
    netgame,
    netresult
};

enum EyeStateType {
    eye_open,
    eye_wink1,
    eye_wink2
};

enum HoverStateType {
    normal,
    hover1,
    hover2
};

GameStateType current_state = menu;
EyeStateType eye_state = eye_open;
HoverStateType hover_state = normal;

float grayProgressMain = 0.0f;
DWORD startTimeMain = 0;
void resetGradient() { startTimeMain = 0; grayProgressMain = 0.0f; }

void loadLeaderboard() {
    g_leaderboardScores.clear();
    std::ifstream fin("leaderboard.dat", std::ios::binary);
    if (!fin) return;
    int score;
    while (fin.read((char*)&score, sizeof(score))) {
        g_leaderboardScores.push_back(score);
    }
    fin.close();
    std::sort(g_leaderboardScores.begin(), g_leaderboardScores.end(), std::greater<int>());
    if (g_leaderboardScores.size() > MAX_LEADERBOARD_ENTRIES)
        g_leaderboardScores.resize(MAX_LEADERBOARD_ENTRIES);
}

void saveLeaderboard() {
    std::ofstream fout("leaderboard.dat", std::ios::binary);
    for (int s : g_leaderboardScores) {
        fout.write((char*)&s, sizeof(s));
    }
    fout.close();
}

void addScoreToLeaderboard(int newScore) {
    g_leaderboardScores.push_back(newScore);
    std::sort(g_leaderboardScores.begin(), g_leaderboardScores.end(), std::greater<int>());
    if (g_leaderboardScores.size() > MAX_LEADERBOARD_ENTRIES)
        g_leaderboardScores.resize(MAX_LEADERBOARD_ENTRIES);
    saveLeaderboard();
}

SOCKET netSock = INVALID_SOCKET;
SOCKET netListenSock = INVALID_SOCKET;
HANDLE hNetRecvThread = NULL;
CRITICAL_SECTION g_netCS;
bool g_netCS_inited = false;
bool g_netWSA = false;
DWORD netConnectT0 = 0;
int netRole = 0;
bool isNetGame = false;
bool netConnected = false;
bool netMatchOver = false;
bool netOppDisconnected = false;
bool netOppReady = false;
int netResult = 0;
DWORD netOverTime = 0;
int netConnPhase = 0;

TCHAR netClientIP[16] = _T("");
int netIPLen = 0;

struct NetOppState {
    float x, y, r;
    int score;
    int alive;
    int cmd;
};
NetOppState netOpp;

struct NetBallPacket {
    int len;
    float x, y, r;
    int score;
    int alive;
    int cmd;
};

void netDrawGridBg() {
    setlinecolor(RGB(245, 230, 250));
    setlinestyle(PS_DASHDOT, 1);
    for (int x = 0; x <= VIEW_WIDTH; x += 24) line(x, 0, x, VIEW_HEIGHT);
    for (int y = 4; y <= VIEW_HEIGHT; y += 24) line(0, y, VIEW_WIDTH, y);
}

void netEnsureWSA() {
    if (!g_netWSA) {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
        g_netWSA = true;
    }
}

void netSendState() {
    if (netSock == INVALID_SOCKET) return;
    NetBallPacket p;
    p.len = sizeof(NetBallPacket);
    p.x = player.x;
    p.y = player.y;
    p.r = player.r;
    p.score = player.score;
    p.alive = (g_game_over || netMatchOver) ? 0 : 1;
    p.cmd = 0;
    if (netMatchOver) {
        if (netResult == 1) p.cmd = 2;
        else if (netResult == 2) p.cmd = 1;
    }
    send(netSock, (char*)&p, sizeof(p), 0);
}

DWORD WINAPI netRecvThread(LPVOID) {
    char buf[2048];
    int off = 0;
    while (1) {
        int n = recv(netSock, buf + off, (int)(sizeof(buf) - off), 0);
        if (n <= 0) break;
        off += n;
        while (off >= (int)sizeof(NetBallPacket)) {
            NetBallPacket* pk = (NetBallPacket*)buf;
            if (pk->len != (int)sizeof(NetBallPacket)) { off = 0; break; }
            if (g_netCS_inited) {
                EnterCriticalSection(&g_netCS);
                netOpp.x = pk->x;
                netOpp.y = pk->y;
                netOpp.r = pk->r;
                netOpp.score = pk->score;
                netOpp.alive = pk->alive;
                netOpp.cmd = pk->cmd;
                netOppReady = true;
                LeaveCriticalSection(&g_netCS);
            }
            int rem = off - (int)sizeof(NetBallPacket);
            if (rem > 0) memmove(buf, buf + (int)sizeof(NetBallPacket), rem);
            off = rem;
        }
    }
    netOppDisconnected = true;
    return 0;
}

void netBeginMatch() {
    netConnected = true;
    isNetGame = true;
    netMatchOver = false;
    netOppDisconnected = false;
    netOppReady = false;
    netResult = 0;
    netOverTime = 0;
    memset(&netOpp, 0, sizeof(netOpp));

    srand((unsigned int)time(NULL) ^ GetTickCount());
    resetGame();
    g_game_over = false;

    if (netRole == 1) {
        player.x = 260.0f + (float)(rand() % 200);
        player.y = 150.0f + (float)(rand() % 200);
    }
    else {
        player.x = 740.0f + (float)(rand() % 200);
        player.y = 150.0f + (float)(rand() % 200);
    }

    if (!g_netCS_inited) {
        InitializeCriticalSection(&g_netCS);
        g_netCS_inited = true;
    }
    if (hNetRecvThread == NULL)
        hNetRecvThread = CreateThread(NULL, 0, netRecvThread, NULL, 0, NULL);
}

void netCleanup() {
    if (netSock != INVALID_SOCKET) { closesocket(netSock); netSock = INVALID_SOCKET; }
    if (hNetRecvThread) {
        WaitForSingleObject(hNetRecvThread, 1000);
        CloseHandle(hNetRecvThread);
        hNetRecvThread = NULL;
    }
    if (netListenSock != INVALID_SOCKET) { closesocket(netListenSock); netListenSock = INVALID_SOCKET; }
    if (g_netCS_inited) { DeleteCriticalSection(&g_netCS); g_netCS_inited = false; }
    if (g_netWSA) { WSACleanup(); g_netWSA = false; }
    netRole = 0;
    isNetGame = false;
    netConnected = false;
    netMatchOver = false;
    netOppDisconnected = false;
    netOppReady = false;
    netResult = 0;
    netConnPhase = 0;
    memset(&netOpp, 0, sizeof(netOpp));
}

bool netHostStart() {
    netEnsureWSA();
    if (netListenSock != INVALID_SOCKET) closesocket(netListenSock);
    netListenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (netListenSock == INVALID_SOCKET) return false;
    sockaddr_in a;
    a.sin_family = AF_INET;
    a.sin_port = htons(8888);
    a.sin_addr.s_addr = INADDR_ANY;
    if (bind(netListenSock, (sockaddr*)&a, sizeof(a)) == SOCKET_ERROR) {
        closesocket(netListenSock);
        netListenSock = INVALID_SOCKET;
        return false;
    }
    listen(netListenSock, 5);
    u_long m = 1;
    ioctlsocket(netListenSock, FIONBIO, &m);
    return true;
}

bool netClientConnectStart() {
    netEnsureWSA();
    if (netSock != INVALID_SOCKET) { closesocket(netSock); netSock = INVALID_SOCKET; }
    netSock = socket(AF_INET, SOCK_STREAM, 0);
    if (netSock == INVALID_SOCKET) return false;
    char ipA[16];
#ifdef UNICODE
    wcstombs(ipA, netClientIP, 15);
#else
    strncpy(ipA, netClientIP, 15);
#endif
    ipA[15] = '\0';
    sockaddr_in a;
    a.sin_family = AF_INET;
    a.sin_port = htons(8888);
    a.sin_addr.s_addr = inet_addr(ipA);
    u_long m = 1;
    ioctlsocket(netSock, FIONBIO, &m);
    connect(netSock, (sockaddr*)&a, sizeof(a));
    netConnPhase = 0;
    netConnectT0 = GetTickCount();
    return true;
}

void drawNetBtn(int x, int y, int w, int h, const TCHAR* t1, const TCHAR* t2) {
    setfillcolor(RGB(64, 224, 208));
    solidroundrect(x, y, x + w, y + h, 40, 40);
    settextcolor(RGB(0, 0, 0));
    settextstyle(32, 0, _T("微软雅黑"));
    int tw = textwidth(t1);
    outtextxy(x + (w - tw) / 2, y + 16, t1);
    if (t2 && t2[0] != _T('\0')) {
        settextcolor(RGB(60, 60, 60));
        settextstyle(18, 0, _T("微软雅黑"));
        tw = textwidth(t2);
        outtextxy(x + (w - tw) / 2, y + 90, t2);
    }
}

void netHostWaitScreen(int frame) {
    settextcolor(RGB(60, 60, 120));
    settextstyle(44, 0, _T("微软雅黑"));
    int d = (frame / 25) % 4;
    TCHAR line[80];
    _stprintf(line, _T("等待对方加入%s%s%s"),
        d >= 1 ? _T(".") : _T(""),
        d >= 2 ? _T(".") : _T(""),
        d >= 3 ? _T(".") : _T(""));
    int tw = textwidth(line);
    outtextxy((VIEW_WIDTH - tw) / 2, 70, line);

    settextcolor(RGB(0, 0, 0));
    settextstyle(24, 0, _T("微软雅黑"));
    outtextxy(60, 160, _T("让对方在自己的“加入房间”里输入下面任意一个IP，然后按 Enter："));
    outtextxy(60, 210, _T("本机 IP："));

    ULONG sz = 0;
    GetAdaptersInfo(NULL, &sz);
    if (sz > 0) {
        PIP_ADAPTER_INFO p = (PIP_ADAPTER_INFO)malloc(sz);
        if (GetAdaptersInfo(p, &sz) == NO_ERROR) {
            PIP_ADAPTER_INFO it = p;
            int y = 245;
            int shown = 0;
            while (it) {
                char* ip = it->IpAddressList.IpAddress.String;
                if (it->Type != MIB_IF_TYPE_LOOPBACK &&
                    strcmp(ip, "0.0.0.0") != 0 &&
                    strncmp(ip, "127.", 4) != 0) {
                    TCHAR s[80];
                    _stprintf(s, _T("    %hs"), ip);
                    settextcolor(RGB(210, 30, 30));
                    outtextxy(60, y, s);
                    y += 38;
                    shown++;
                }
                it = it->Next;
            }
            if (shown == 0) {
                settextcolor(RGB(150, 60, 60));
                outtextxy(60, 245, _T("未检测到有效 IP，请检查网络连接"));
                y += 38;
            }
            settextcolor(RGB(0, 0, 0));
            outtextxy(60, y + 10, _T("端口: 8888"));
        }
        free(p);
    }

    settextcolor(RGB(120, 120, 120));
    settextstyle(20, 0, _T("微软雅黑"));
    outtextxy(60, 470, _T("对方连接成功后将自动开始对战。"));
    outtextxy(60, 505, _T("按 ESC 取消并返回主菜单。"));
}

int main(void)
{
    bool goto_exit = false;
    int map_height = 6000;
    int map_width = 8000;
    float mouse_x = 0.0;
    float mouse_y = 0.0;
    clock_t startime_1 = 0;
    clock_t freamtime_1 = 0;
    clock_t startime = 0;
    clock_t freamtime = 0;
    const clock_t FPS = 1000 / 80;
    int score = 0;
    TCHAR str_score[20] = { 0 };
    int x = 0;
    int y = 0;
    int i = 1;

    initgraph(VIEW_WIDTH, VIEW_HEIGHT, TRUE);
    setbkcolor(RGB(220, 255, 250));
    cleardevice();
    setbkmode(TRANSPARENT);

    ExMessage msg = { 0 };
    IMAGE img_bk_1;
    loadimage(&img_bk_1, _T(".\\meitu\\qiuqiu_bk_1.jpg"), 600, 520);
    IMAGE img_bk_2;
    loadimage(&img_bk_2, _T(".\\meitu\\qiuqiu_bk_2.jpg"), 600, 520);
    IMAGE img_shengchen1;
    IMAGE img_shengchen2;
    loadimage(&img_shengchen1, _T(".\\meitu\\qiuqiu_shengchen_1.jpg"), 1200, 800);
    IMAGE img_gray(600, 520);

    InitMusic();
    PlayBackgroundMusic();
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    printf("球球大作战 防闪屏优化版 + 音乐已集成 + 排行榜功能 + 局域网双人对战\n");

    loadLeaderboard();

    while (!goto_exit) {
        startime = clock();

        BeginBatchDraw();
        cleardevice();

        switch (current_state)
        {
        case menu:
        {
            startTime = 0;
            grayProgress = 0.0f;

            setlinecolor(RGB(245, 230, 250));
            setlinestyle(PS_DASHDOT, 1);
            for (int x = 0; x <= VIEW_WIDTH; x += 24) line(x, 0, x, VIEW_HEIGHT);
            for (int y = 4; y <= VIEW_HEIGHT; y += 24) line(0, y, VIEW_WIDTH, y);

            rings_20();

            putimage(300, 0, &img_bk_2, NOTSRCERASE);
            putimage(300, 0, &img_bk_1, SRCINVERT);

            bool hoverStart = (mouse_x >= 410 && mouse_x <= 790 && mouse_y >= 380 && mouse_y <= 510);
            bool hoverShop = (mouse_x >= 410 && mouse_x <= 790 && mouse_y >= 520 && mouse_y <= 650);
            bool hoverLeader = (mouse_x >= 410 && mouse_x <= 790 && mouse_y >= 660 && mouse_y <= 790);
            bool hoverNetBtn = (mouse_x >= 30 && mouse_x <= 290 && mouse_y >= 700 && mouse_y <= 780);

            if (hoverStart) setfillcolor(RGB(100, 240, 220));
            else setfillcolor(RGB(64, 224, 208));
            solidroundrect(410, 380, 790, 510, 50, 50);
            settextstyle(64, 36, _T("微软雅黑"));
            settextcolor(RGB(0, 0, 0));
            outtextxy(440, 420, _T("开始游戏"));

            if (hoverShop) setfillcolor(RGB(100, 240, 220));
            else setfillcolor(RGB(64, 224, 208));
            solidroundrect(410, 520, 790, 650, 50, 50);
            settextstyle(64, 36, _T("微软雅黑"));
            settextcolor(RGB(0, 0, 0));
            outtextxy(440, 550, _T("皮肤商城"));

            if (hoverLeader) setfillcolor(RGB(100, 240, 220));
            else setfillcolor(RGB(64, 224, 208));
            solidroundrect(410, 660, 790, 790, 50, 50);
            settextstyle(64, 36, _T("微软雅黑"));
            settextcolor(RGB(0, 0, 0));
            outtextxy(440, 690, _T("排行榜"));

            if (hoverNetBtn) setfillcolor(RGB(100, 240, 220));
            else setfillcolor(RGB(64, 224, 208));
            solidroundrect(30, 700, 290, 780, 40, 40);
            settextcolor(RGB(0, 0, 0));
            settextstyle(32, 0, _T("微软雅黑"));
            outtextxy(48, 706, _T("双人对战"));
            settextcolor(RGB(50, 50, 50));
            settextstyle(16, 0, _T("微软雅黑"));
            outtextxy(45, 750, _T("局域网联机"));

            setfillcolor(color_coco);
            solidcircle(1000, 450, 100);
            setfillcolor(RGB(220, 255, 250));
            solidcircle(1000 - 40, 450 - 25, 32);
            solidcircle(1000 + 40, 450 - 25, 32);
            settextcolor(RGB(70, 80, 120));
            outtextxy(900 + 16, 550, _T("Coco"));

            while (peekmessage(&msg, EX_MOUSE))
            {
                if (msg.message == WM_LBUTTONDOWN)
                {
                    if (msg.x >= 410 && msg.x <= 790 && msg.y >= 380 && msg.y <= 510)
                    {
                        freamtime_1 = clock();
                        current_state = pregame;
                    }
                    else if (msg.x >= 410 && msg.x <= 790 && msg.y >= 520 && msg.y <= 650)
                    {
                        current_state = shop;
                    }
                    else if (msg.x >= 410 && msg.x <= 790 && msg.y >= 660 && msg.y <= 790)
                    {
                        current_state = leaderboard;
                        loadLeaderboard();
                    }
                    else if (msg.x >= 30 && msg.x <= 290 && msg.y >= 700 && msg.y <= 780)
                    {
                        PlayButtonSound();
                        netRole = 0;
                        isNetGame = false;
                        current_state = netmenu;
                    }
                    else if (msg.x >= 13 * 24 && msg.x <= 16 * 24 && msg.y >= 10 * 24 && msg.y <= 13 * 24)
                    {
                        eye_state = eye_wink1;
                    }
                    else if (msg.x >= 37 * 24 && msg.x <= 46 * 24 && msg.y >= 14 * 24 && msg.y <= 24 * 24)
                    {
                        eye_state = eye_wink2;
                    }

                    if (msg.x >= 1100 && msg.x <= 1180 && msg.y >= 20 && msg.y <= 60) {
                        PlayButtonSound();
                        musicEnabled = !musicEnabled;
                        if (musicEnabled) ResumeBackgroundMusic();
                        else PauseBackgroundMusic();
                    }
                }
                else if (msg.message == WM_MOUSEMOVE)
                {
                    mouse_x = msg.x;
                    mouse_y = msg.y;

                    if (eye_state == eye_wink1)
                    {
                        if (msg.x <= 12 * 24 || msg.x >= 18 * 24 || msg.y <= 10 * 24 || msg.y >= 13 * 24) eye_state = eye_open;
                    }
                    else if (eye_state == eye_wink2)
                    {
                        if (msg.x <= 37 * 24 || msg.x >= 46 * 24 || msg.y <= 14 * 24 || msg.y >= 24 * 24) eye_state = eye_open;
                    }

                    if (msg.x <= 790 && msg.x >= 410 && msg.y <= 510 && msg.y >= 380) hover_state = hover1;
                    if (msg.x <= 790 && msg.x >= 410 && msg.y <= 650 && msg.y >= 520) hover_state = hover2;
                    if (msg.x >= 790 || msg.x <= 410 || msg.y >= 510 || msg.y <= 380) hover_state = normal;
                    if (msg.x >= 790 || msg.x <= 410 || msg.y >= 650 || msg.y <= 520) hover_state = normal;
                }
            }

            if (eye_state == eye_wink1)
            {
                setfillcolor(RGB(128, 0, 128));
                solidellipse(311, 267, 341, 287);
                solidellipse(361, 267, 391, 287);
                setfillcolor(RGB(0, 2, 2));
                solidcircle(1000 - 40 + (mouse_x - 1000 + 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);
                solidcircle(1000 + 40 + (mouse_x - 1000 - 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);
            }
            else if (eye_state == eye_wink2)
            {
                setfillcolor(color_yp);
                solidcircle(1000 - 40, 450 - 25, 32);
                solidcircle(1000 + 40, 450 - 25, 32);
                setfillcolor(RGB(0, 2, 2));
                solidcircle(327 + (mouse_x - 327) / 70, 278 + (mouse_y - 278) / 70, 6);
                solidcircle(377 + (mouse_x - 377) / 70, 278 + (mouse_y - 278) / 70, 6);
            }
            else if (eye_state == eye_open)
            {
                setfillcolor(RGB(0, 2, 2));
                solidcircle(327 + (mouse_x - 327) / 70, 278 + (mouse_y - 278) / 70, 6);
                solidcircle(377 + (mouse_x - 377) / 70, 278 + (mouse_y - 278) / 70, 6);
                solidcircle(1000 - 40 + (mouse_x - 1000 + 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);
                solidcircle(1000 + 40 + (mouse_x - 1000 - 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);
            }

            if (hover_state == hover1) {
                setfillcolor(RGB(180, 180, 180));
                solidroundrect(412, 382, 792, 512, 50, 50);
                setfillcolor(RGB(64, 224, 208));
                solidroundrect(410, 380, 790, 510, 50, 50);
                setcolor(RGB(255, 255, 255));
                setlinestyle(PS_SOLID, 2);
                line(415, 380, 785, 380);
                line(410, 385, 410, 505);
                solidroundrect(410, 520, 790, 650, 50, 50);
                settextstyle(64, 36, _T("微软雅黑"));
                settextcolor(RGB(0, 0, 0));
                outtextxy(440, 550, _T("皮肤商城"));
            }
            break;
        }

        case pregame:
        {
            setlinecolor(RGB(195, 195, 200));
            setlinestyle(PS_DASHDOT, 1);
            for (int x = 0; x <= VIEW_WIDTH; x += 24) line(x, 0, x, VIEW_HEIGHT);
            for (int y = 4; y <= VIEW_HEIGHT; y += 24) line(0, y, VIEW_WIDTH, y);

            rings_20();

            convertToGrayscale(&img_bk_1, &img_gray);
            putimage(300, 0, &img_bk_2, NOTSRCERASE);
            putimage(300, 0, &img_gray, SRCINVERT);

            setfillcolor(color_coco);
            solidcircle(1000, 450, 100);
            setfillcolor(RGB(220, 255, 250));
            solidcircle(1000 - 40, 450 - 25, 32);
            solidcircle(1000 + 40, 450 - 25, 32);
            settextcolor(RGB(0, 2, 2));

            setfillcolor(RGB(0, 2, 2));
            solidcircle(327 + (mouse_x - 327) / 70, 278 + (mouse_y - 278) / 70, 6);
            solidcircle(377 + (mouse_x - 377) / 70, 278 + (mouse_y - 278) / 70, 6);
            solidcircle(1000 - 40 + (mouse_x - 1000 + 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);
            solidcircle(1000 + 40 + (mouse_x - 1000 - 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);

            setfillcolor(RGB(230, 230, 240));
            solidroundrect(410, 380, 790, 510, 50, 50);
            settextstyle(64, 36, _T("微软雅黑"));
            settextcolor(RGB(0, 0, 0));
            outtextxy(440, 420, _T("开始游戏"));

            solidroundrect(410, 520, 790, 650, 50, 50);
            settextstyle(64, 36, _T("微软雅黑"));
            settextcolor(RGB(0, 0, 0));
            outtextxy(440, 550, _T("皮肤商城"));

            startime_1 = clock();
            if ((startime_1 - freamtime_1) > 9000) {
                current_state = pause;
                draw_rings = true;
                ring_start = clock();
                base_radius = 0;
            }

            while (peekmessage(&msg, EX_MOUSE))
            {
                if (msg.message == WM_MBUTTONDOWN)
                {
                    current_state = game;
                    resetGame();
                }
                if (msg.message == WM_LBUTTONDOWN && msg.x >= 1100 && msg.x <= 1180 && msg.y >= 20 && msg.y <= 60) {
                    PlayButtonSound();
                    musicEnabled = !musicEnabled;
                    if (musicEnabled) ResumeBackgroundMusic();
                    else PauseBackgroundMusic();
                }
            }
            break;
        }

        case pause:
        {
            setlinecolor(RGB(195, 195, 200));
            setlinestyle(PS_DASHDOT, 1);
            for (int x = 0; x <= VIEW_WIDTH; x += 24) line(x, 0, x, VIEW_HEIGHT);
            for (int y = 4; y <= VIEW_HEIGHT; y += 24) line(0, y, VIEW_WIDTH, y);

            rings_20();

            convertToGrayscale(&img_bk_1, &img_gray);
            putimage(300, 0, &img_bk_2, NOTSRCERASE);
            putimage(300, 0, &img_gray, SRCINVERT);

            setfillcolor(color_coco);
            solidcircle(1000, 450, 100);
            setfillcolor(RGB(220, 255, 250));
            solidcircle(1000 - 40, 450 - 25, 32);
            solidcircle(1000 + 40, 450 - 25, 32);
            settextcolor(RGB(0, 2, 2));

            setfillcolor(RGB(0, 2, 2));
            solidcircle(327 + (mouse_x - 327) / 70, 278 + (mouse_y - 278) / 70, 6);
            solidcircle(377 + (mouse_x - 377) / 70, 278 + (mouse_y - 278) / 70, 6);
            solidcircle(1000 - 40 + (mouse_x - 1000 + 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);
            solidcircle(1000 + 40 + (mouse_x - 1000 - 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);

            setfillcolor(RGB(230, 230, 240));
            solidroundrect(410, 380, 790, 510, 50, 50);
            settextstyle(64, 36, _T("微软雅黑"));
            settextcolor(RGB(0, 0, 0));
            outtextxy(440, 420, _T("开始游戏"));

            solidroundrect(410, 520, 790, 650, 50, 50);
            settextstyle(64, 36, _T("微软雅黑"));
            settextcolor(RGB(0, 0, 0));
            outtextxy(440, 550, _T("皮肤商城"));

            while (peekmessage(&msg, EX_MOUSE))
            {
                if (msg.message == WM_MBUTTONDOWN)
                {
                    current_state = game;
                    resetGame();
                }
                if (msg.message == WM_LBUTTONDOWN && msg.x >= 1100 && msg.x <= 1180 && msg.y >= 20 && msg.y <= 60) {
                    PlayButtonSound();
                    musicEnabled = !musicEnabled;
                    if (musicEnabled) ResumeBackgroundMusic();
                    else PauseBackgroundMusic();
                }
            }

            rings_effect();

            startime_1 = clock();
            if ((startime_1 - freamtime_1) > 17000) {
                current_state = game;
                resetGame();
            }
            break;
        }

        case game:
        {
            setlinecolor(RGB(245, 230, 250));
            setlinestyle(PS_DASHDOT, 1);
            for (int x = 0; x <= VIEW_WIDTH; x += 24) line(x, 0, x, VIEW_HEIGHT);
            for (int y = 4; y <= VIEW_HEIGHT; y += 24) line(0, y, VIEW_WIDTH, y);

            if (!g_game_over) {
                drawGridBackground(); movePlayerBall(); updateAIBalls(); eatFood(); checkAICollision();
                drawFoods(); drawAIBalls(); drawPlayerBall(); drawGameUI();
            }
            else {
                drawGridBackground(); drawFoods(); drawAIBalls(); drawGameOver();
            }

            if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                if (g_game_over) {
                    current_state = menu;
                    resetGame();
                    startTime = 0;
                    grayProgress = 0.0f;
                }
            }
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                current_state = menu;
                resetGame();
                startTime = 0;
                grayProgress = 0.0f;
            }

            while (peekmessage(&msg, EX_MOUSE))
            {
                if (msg.message == WM_MBUTTONDOWN)
                {
                    current_state = menu;
                    resetGame();
                    startTime = 0;
                    grayProgress = 0.0f;
                }
                if (msg.message == WM_LBUTTONDOWN && msg.x >= 1100 && msg.x <= 1180 && msg.y >= 20 && msg.y <= 60) {
                    PlayButtonSound();
                    musicEnabled = !musicEnabled;
                    if (musicEnabled) ResumeBackgroundMusic();
                    else PauseBackgroundMusic();
                }
            }
            break;
        }

        case shop:
        {
            setlinecolor(RGB(245, 230, 250));
            setlinestyle(PS_DASHDOT, 1);
            for (int x = 0; x <= VIEW_WIDTH; x += 24) line(x, 0, x, VIEW_HEIGHT);
            for (int y = 4; y <= VIEW_HEIGHT; y += 24) line(0, y, VIEW_WIDTH, y);
            putimage(0, 0, &img_shengchen1);

            setfillcolor(color_coco);
            solidcircle(1000, 450, 100);
            setfillcolor(RGB(220, 255, 250));
            solidcircle(1000 - 40, 450 - 25, 32);
            solidcircle(1000 + 40, 450 - 25, 32);

            settextcolor(RGB(190, 40, 220));
            settextstyle(40, 20, _T("微软雅黑"));
            outtextxy(230, 40, _T("原颜色"));
            settextcolor(RGB(106, 90, 205));
            outtextxy(440, 40, _T("薰衣草色"));
            settextcolor(RGB(70, 130, 180));
            outtextxy(650, 40, _T("钢蓝色"));
            settextcolor(RGB(255, 255, 0));
            outtextxy(860, 40, _T("柠檬黄色"));
            settextcolor(RGB(64, 224, 208));
            outtextxy(230, 250, _T("青绿色"));
            settextcolor(RGB(50, 205, 50));
            outtextxy(440, 250, _T("松石绿色"));
            settextcolor(RGB(154, 205, 50));
            outtextxy(650, 250, _T("黄绿色"));
            settextcolor(RGB(255, 200, 0));
            outtextxy(860, 250, _T("橙黄色"));
            settextcolor(RGB(255, 165, 0));
            outtextxy(230, 460, _T("橘黄色"));
            settextcolor(RGB(235, 80, 50));
            outtextxy(440, 460, _T("朱红色"));
            settextcolor(RGB(160, 30, 40));
            outtextxy(650, 460, _T("深红色"));

            while (peekmessage(&msg, EX_MOUSE))
            {
                switch (msg.message)
                {
                case WM_MBUTTONDOWN:
                    current_state = menu;
                    startTime = 0;
                    grayProgress = 0.0f;
                    break;
                case WM_MOUSEMOVE:
                    mouse_x = msg.x;
                    mouse_y = msg.y;
                    if (msg.x <= 37 * 24 || msg.x >= 46 * 24 || msg.y <= 14 * 24 || msg.y >= 24 * 24) eye_state = eye_open;
                    break;
                case WM_LBUTTONDOWN:
                    if (msg.x >= 37 * 24 && msg.x <= 46 * 24 && msg.y >= 14 * 24 && msg.y <= 24 * 24) eye_state = eye_wink2;
                    if (msg.x >= 1100 && msg.x <= 1180 && msg.y >= 20 && msg.y <= 60) {
                        PlayButtonSound();
                        musicEnabled = !musicEnabled;
                        if (musicEnabled) ResumeBackgroundMusic();
                        else PauseBackgroundMusic();
                    }
                    handleLeftButtonDown(msg.x, msg.y);
                    break;
                default:
                    break;
                }
            }

            if (eye_state == eye_wink2)
            {
                setfillcolor(color_yp);
                solidcircle(1000 - 40, 450 - 25, 32);
                solidcircle(1000 + 40, 450 - 25, 32);
            }
            else if (eye_state == eye_open)
            {
                setfillcolor(RGB(0, 2, 2));
                solidcircle(1000 - 40 + (mouse_x - 1000 + 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);
                solidcircle(1000 + 40 + (mouse_x - 1000 - 40) / 60, 450 - 25 + (mouse_y - 450 + 25) / 60, 19);
            }

            settextcolor(color_coco);
            settextstyle(100, 40, _T("微软雅黑"));
            outtextxy(80, 640, _T("点击色牌第一个字以改变颜色"));
            break;
        }

        case leaderboard:
        {
            setlinecolor(RGB(245, 230, 250));
            setlinestyle(PS_DASHDOT, 1);
            for (int x = 0; x <= VIEW_WIDTH; x += 24) line(x, 0, x, VIEW_HEIGHT);
            for (int y = 4; y <= VIEW_HEIGHT; y += 24) line(0, y, VIEW_WIDTH, y);

            settextcolor(RGB(138, 43, 226));
            settextstyle(64, 36, _T("微软雅黑"));
            outtextxy(450, 40, _T("排行榜"));

            settextstyle(36, 18, _T("微软雅黑"));
            if (g_leaderboardScores.empty()) {
                settextcolor(RGB(100, 100, 100));
                outtextxy(400, 200, _T("暂无记录"));
            }
            else {
                for (size_t ii = 0; ii < g_leaderboardScores.size(); ii++) {
                    TCHAR str[100];
                    wsprintf(str, _T("第 %d 名: %d"), ii + 1, g_leaderboardScores[ii]);
                    settextcolor(RGB(50, 50, 50));
                    outtextxy(400, 140 + ii * 50, str);
                }
            }

            settextstyle(28, 14, _T("微软雅黑"));
            settextcolor(RGB(100, 100, 100));
            outtextxy(350, 700, _T("按鼠标中键 或 ESC 返回菜单"));

            while (peekmessage(&msg, EX_MOUSE)) {
                if (msg.message == WM_MBUTTONDOWN) {
                    current_state = menu;
                    startTime = 0;
                    grayProgress = 0.0f;
                }
                if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                    current_state = menu;
                    startTime = 0;
                    grayProgress = 0.0f;
                }
                if (msg.message == WM_LBUTTONDOWN && msg.x >= 1100 && msg.x <= 1180 && msg.y >= 20 && msg.y <= 60) {
                    PlayButtonSound();
                    musicEnabled = !musicEnabled;
                    if (musicEnabled) ResumeBackgroundMusic();
                    else PauseBackgroundMusic();
                }
            }

            if (mouse_x >= 1100 && mouse_x <= 1180 && mouse_y >= 20 && mouse_y <= 60) setfillcolor(RGB(230, 230, 240));
            else setfillcolor(RGB(199, 200, 201));
            solidroundrect(1100, 20, 1180, 60, 20, 20);
            settextcolor(color_yp);
            settextstyle(20, 10, _T("微软雅黑"));
            outtextxy(1114, 31, musicEnabled ? _T("音乐开") : _T("音乐关"));
            break;
        }

        case netmenu:
        {
            settextcolor(RGB(60, 60, 120));
            settextstyle(48, 0, _T("微软雅黑"));
            int tw = textwidth(_T("双人对战（局域网）"));
            outtextxy((VIEW_WIDTH - tw) / 2, 70, _T("双人对战（局域网）"));
            settextcolor(RGB(130, 130, 130));
            settextstyle(20, 0, _T("微软雅黑"));
            tw = textwidth(_T("一方选“创建房间”，另一方在同一局域网输入它的IP加入"));
            outtextxy((VIEW_WIDTH - tw) / 2, 150, _T("一方选“创建房间”，另一方在同一局域网输入它的IP加入"));

            drawNetBtn(410, 220, 380, 160, _T("创建房间"), _T("我是主机，等待对方加入"));
            drawNetBtn(410, 420, 380, 160, _T("加入房间"), _T("我是客户端，输入主机IP"));
            drawNetBtn(410, 640, 380, 120, _T("返回主菜单"), _T(""));

            settextcolor(RGB(150, 150, 150));
            settextstyle(18, 0, _T("微软雅黑"));
            outtextxy(20, VIEW_HEIGHT - 40, _T("端口:8888   提示: 连不上请关闭防火墙或放行本程序"));

            while (peekmessage(&msg, EX_MOUSE)) {
                if (msg.message == WM_LBUTTONDOWN) {
                    if (msg.x >= 410 && msg.x <= 790 && msg.y >= 220 && msg.y <= 380) {
                        if (netHostStart()) { netRole = 1; current_state = nethost; }
                    }
                    else if (msg.x >= 410 && msg.x <= 790 && msg.y >= 420 && msg.y <= 580) {
                        netRole = 2;
                        netClientIP[0] = _T('\0');
                        netIPLen = 0;
                        current_state = netinput;
                    }
                    else if (msg.x >= 410 && msg.x <= 790 && msg.y >= 640 && msg.y <= 760) {
                        current_state = menu;
                    }
                }
            }
            break;
        }

        case nethost:
        {
            if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
                netCleanup();
                current_state = menu;
                break;
            }

            SOCKET c = accept(netListenSock, NULL, NULL);
            if (c != INVALID_SOCKET) {
                closesocket(netListenSock);
                netListenSock = INVALID_SOCKET;
                u_long m0 = 0;
                ioctlsocket(c, FIONBIO, &m0);
                netSock = c;
                netBeginMatch();
                current_state = netgame;
                break;
            }

            static int waitFrame = 0;
            waitFrame++;
            netHostWaitScreen(waitFrame);
            break;
        }

        case netinput:
        {
            static bool prevKey[256] = { false };

            settextcolor(RGB(60, 60, 120));
            settextstyle(40, 0, _T("微软雅黑"));
            int tw = textwidth(_T("输入主机 IP"));
            outtextxy((VIEW_WIDTH - tw) / 2, 130, _T("输入主机 IP"));
            settextcolor(RGB(130, 130, 130));
            settextstyle(20, 0, _T("微软雅黑"));
            tw = textwidth(_T("在主机界面上查看 IP，格式如 192.168.1.100"));
            outtextxy((VIEW_WIDTH - tw) / 2, 200, _T("在主机界面上查看 IP，格式如 192.168.1.100"));

            int boxX = (VIEW_WIDTH - 500) / 2, boxY = 290, boxW = 500, boxH = 50;
            setfillcolor(RGB(255, 255, 255));
            solidroundrect(boxX, boxY, boxX + boxW, boxY + boxH, 15, 15);
            settextcolor(RGB(20, 20, 20));
            settextstyle(30, 0, _T("微软雅黑"));
            outtextxy(boxX + 12, boxY + 10, netClientIP);

            bool changed = false;
            for (int k = 0; k < 256; k++) {
                bool down = (GetAsyncKeyState(k) & 0x8000) != 0;
                if (down && !prevKey[k]) {
                    prevKey[k] = true;
                    if (k == VK_RETURN) {
                        if (netIPLen > 0) { netClientConnectStart(); current_state = netconnect; changed = true; }
                    }
                    else if (k == VK_ESCAPE) { current_state = netmenu; changed = true; }
                    else if (k == VK_BACK) {
                        if (netIPLen > 0) { netIPLen--; netClientIP[netIPLen] = _T('\0'); }
                    }
                    else if (k >= '0' && k <= '9') {
                        if (netIPLen < 14) { netClientIP[netIPLen++] = (TCHAR)k; netClientIP[netIPLen] = _T('\0'); }
                    }
                    else if (k >= VK_NUMPAD0 && k <= VK_NUMPAD9) {
                        if (netIPLen < 14) { netClientIP[netIPLen++] = (TCHAR)('0' + (k - VK_NUMPAD0)); netClientIP[netIPLen] = _T('\0'); }
                    }
                    else if (k == 190) {
                        if (netIPLen < 14 && netIPLen > 0) { netClientIP[netIPLen++] = _T('.'); netClientIP[netIPLen] = _T('\0'); }
                    }
                    else if (k == 110 || k == VK_DECIMAL) {
                        if (netIPLen < 14 && netIPLen > 0) { netClientIP[netIPLen++] = _T('.'); netClientIP[netIPLen] = _T('\0'); }
                    }
                    if (changed) break;
                }
                else if (!down) prevKey[k] = false;
            }

            settextcolor(RGB(150, 150, 150));
            settextstyle(20, 0, _T("微软雅黑"));
            outtextxy(boxX + 12, boxY + 70, _T("Enter:连接    Backspace:删除    Esc:返回"));
            if ((GetTickCount() / 400) % 2 == 0) {
                int cx = boxX + 12 + textwidth(netClientIP);
                setcolor(RGB(20, 20, 20));
                line(cx, boxY + 10, cx, boxY + boxH - 10);
            }
            break;
        }

        case netconnect:
        {
            if (netConnPhase == 0) {
                fd_set wf;
                FD_ZERO(&wf);
                FD_SET(netSock, &wf);
                timeval tv = { 0, 0 };
                int r = select(0, NULL, &wf, NULL, &tv);
                bool done = false, ok = false;
                if (r > 0) {
                    int err = 0, elen = sizeof(err);
                    getsockopt(netSock, SOL_SOCKET, SO_ERROR, (char*)&err, &elen);
                    if (err == 0) ok = true;
                    done = true;
                }
                else if (GetTickCount() - netConnectT0 > 8000) done = true;

                if (done) {
                    if (ok) {
                        u_long m0 = 0;
                        ioctlsocket(netSock, FIONBIO, &m0);
                        netBeginMatch();
                        current_state = netgame;
                        break;
                    }
                    closesocket(netSock);
                    netSock = INVALID_SOCKET;
                    netConnPhase = 1;
                }

                TCHAR s[80];
                _stprintf(s, _T("正在连接 %s ..."), netClientIP);
                settextcolor(RGB(60, 60, 120));
                settextstyle(40, 0, _T("微软雅黑"));
                int tw = textwidth(s);
                outtextxy((VIEW_WIDTH - tw) / 2, 250, s);
                settextcolor(RGB(150, 150, 150));
                settextstyle(20, 0, _T("微软雅黑"));
                tw = textwidth(_T("请确认主机已选择“创建房间”并处于等待界面"));
                outtextxy((VIEW_WIDTH - tw) / 2, 330, _T("请确认主机已选择“创建房间”并处于等待界面"));
            }
            else {
                settextcolor(RGB(200, 50, 50));
                settextstyle(40, 0, _T("微软雅黑"));
                int tw = textwidth(_T("连接失败"));
                outtextxy((VIEW_WIDTH - tw) / 2, 200, _T("连接失败"));
                settextcolor(RGB(0, 0, 0));
                settextstyle(24, 0, _T("微软雅黑"));
                outtextxy(150, 300, _T("请检查："));
                outtextxy(180, 350, _T("1. 主机是否已启动并停在等待界面"));
                outtextxy(180, 390, _T("2. IP 地址是否输入正确"));
                outtextxy(180, 430, _T("3. 两台电脑是否同一局域网，防火墙是否放行"));
                settextcolor(RGB(150, 150, 150));
                settextstyle(22, 0, _T("微软雅黑"));
                outtextxy(150, 520, _T("按 R 重试输入   按 ESC 返回主菜单"));
                if ((GetAsyncKeyState('R') & 0x8000) || (GetAsyncKeyState('r') & 0x8000)) {
                    netConnPhase = 0;
                    current_state = netinput;
                }
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    netCleanup();
                    current_state = menu;
                }
            }
            break;
        }

        case netgame:
        {
            if (!netMatchOver) {
                movePlayerBall();
                updateAIBalls();
                eatFood();
                checkAICollision();
                if (g_game_over && !netMatchOver) {
                    netResult = 2;
                    netMatchOver = true;
                    netOverTime = GetTickCount();
                }
            }

            NetOppState snap;
            bool hasOpp = false;
            if (g_netCS_inited) {
                EnterCriticalSection(&g_netCS);
                if (netOppReady) { snap = netOpp; netOpp.cmd = 0; hasOpp = true; }
                LeaveCriticalSection(&g_netCS);
            }

            if (!netMatchOver) {
                if (netOppDisconnected) {
                    netResult = 3;
                    netMatchOver = true;
                    netOverTime = GetTickCount();
                }
                else if (hasOpp) {
                    if (snap.cmd == 1) {
                        netResult = 1;
                        netMatchOver = true;
                        netOverTime = GetTickCount();
                    }
                    else if (snap.cmd == 2) {
                        netResult = 2;
                        netMatchOver = true;
                        netOverTime = GetTickCount();
                    }
                    else {
                        float dx = player.x - snap.x, dy = player.y - snap.y;
                        float rr = player.r + snap.r;
                        if (dx * dx + dy * dy < rr * rr) {
                            if (player.r > snap.r + 1.0f) {
                                player.r += snap.r / 6.0f;
                                player.score += (int)snap.r;
                                netResult = 1;
                                netMatchOver = true;
                                netOverTime = GetTickCount();
                                PlayEatSound();
                            }
                            else if (snap.r > player.r + 1.0f) {
                                netResult = 2;
                                netMatchOver = true;
                                netOverTime = GetTickCount();
                            }
                        }
                    }
                }
            }

            netDrawGridBg();
            if (hasOpp && snap.alive) {
                setfillcolor(color_yp);
                solidcircle((int)snap.x, (int)snap.y, (int)snap.r);
                setfillcolor(RGB(220, 220, 255));
                solidcircle((int)(snap.x - snap.r * 0.25f), (int)(snap.y - snap.r * 0.25f),
                    max(2, (int)(snap.r * 0.15f)));
            }
            drawFoods();
            drawAIBalls();
            drawPlayerBall();

            TCHAR line[128];
            settextstyle(24, 0, _T("微软雅黑"));
            settextcolor(RGB(200, 20, 20));
            _stprintf(line, _T("我方 分数:%d  大小:%.0f"), player.score, player.r);
            outtextxy(20, 20, line);
            settextcolor(RGB(20, 60, 200));
            if (hasOpp) {
                _stprintf(line, _T("对方 分数:%d  大小:%.0f"), snap.score, snap.r);
                outtextxy(20, 55, line);
            }
            else outtextxy(20, 55, _T("等待对方数据..."));
            settextcolor(RGB(120, 120, 120));
            settextstyle(18, 0, _T("微软雅黑"));
            outtextxy(20, 95, _T("吃掉对方球即可获胜   |   ESC:中途退出"));

            if (netMatchOver) {
                setfillcolor(RGB(30, 30, 30));
                solidroundrect(VIEW_WIDTH / 2 - 160, VIEW_HEIGHT / 2 - 70, VIEW_WIDTH / 2 + 160, VIEW_HEIGHT / 2 - 10, 20, 20);
                settextcolor(WHITE);
                settextstyle(32, 0, _T("微软雅黑"));
                outtextxy(VIEW_WIDTH / 2 - 100, VIEW_HEIGHT / 2 - 60, _T("本局结束"));
                if (GetTickCount() - netOverTime > 1500) current_state = netresult;
            }

            netSendState();

            if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
                netCleanup();
                resetGame();
                current_state = menu;
                break;
            }
            break;
        }

        case netresult:
        {
            netDrawGridBg();
            TCHAR line[128];
            settextstyle(64, 0, _T("微软雅黑"));
            if (netResult == 1) { settextcolor(RGB(255, 180, 0)); _stprintf(line, _T("你赢了！")); }
            else if (netResult == 2) { settextcolor(RGB(60, 60, 220)); _stprintf(line, _T("你输了！")); }
            else { settextcolor(RGB(200, 50, 50)); _stprintf(line, _T("对方已断开连接")); }
            int tw = textwidth(line);
            outtextxy((VIEW_WIDTH - tw) / 2, 180, line);

            settextcolor(RGB(0, 0, 0));
            settextstyle(30, 0, _T("微软雅黑"));
            _stprintf(line, _T("我方分数: %d      对方分数: %d"), player.score, netOpp.score);
            tw = textwidth(line);
            outtextxy((VIEW_WIDTH - tw) / 2, 340, line);
            _stprintf(line, _T("我方大小: %.0f      对方大小: %.0f"), player.r, netOpp.r);
            tw = textwidth(line);
            outtextxy((VIEW_WIDTH - tw) / 2, 395, line);

            settextcolor(RGB(130, 130, 130));
            settextstyle(22, 0, _T("微软雅黑"));
            _stprintf(line, _T("按 空格 / ESC 返回主菜单"));
            tw = textwidth(line);
            outtextxy((VIEW_WIDTH - tw) / 2, 560, line);

            bool quit = false;
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) quit = true;
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) quit = true;
            while (peekmessage(&msg, EX_MOUSE)) {
                if (msg.message == WM_MBUTTONDOWN || msg.message == WM_LBUTTONDOWN) quit = true;
            }
            if (quit) {
                netCleanup();
                resetGame();
                current_state = menu;
            }
            break;
        }
        }

        EndBatchDraw();

        freamtime = clock() - startime;
        if (freamtime < FPS) Sleep(FPS - freamtime);
    }

    closegraph();
    return 0;
}