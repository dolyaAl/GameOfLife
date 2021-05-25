#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class tiles
{
public:
    int count = 0;
    int live_count = 0;
    unsigned int curIndex = 1;
    unsigned int** data1;
    unsigned int** data2;

    tiles(int);
    tiles(const tiles& til);
    ~tiles();

    unsigned int** currentData();
    unsigned int** bufferData();
    void SetLive(unsigned int live, unsigned int x, unsigned int y);
    void resize(int gm_tiles_count);
    int GetLive(unsigned int x, unsigned int y);
    int GetLiveCount();
    int GetCount() const;
    void FreeMemory();
    int getLiveNeighborsCount(int x, int y);
    void DrawLiveTiles(GLFWwindow* window, unsigned int, unsigned int);
    void newGen();
    bool indexValid(int x, int y);
    tiles* operator= (tiles& n_tiles);
};