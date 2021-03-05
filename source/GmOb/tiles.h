#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class tiles
{
public:
    int count = 0;
    int live_count = 0;
    unsigned int** data;

    tiles(int);
    tiles(const tiles& til);
    ~tiles();

    void SetLive(unsigned int live, unsigned int x, unsigned int y);
    int GetLive(unsigned int x, unsigned int y);
    int GetLiveCount();
    int GetCount() const;
    void FreeMemory();
    void addAll(const tiles& new_tiles);
    int getLiveNeighborsCount(int x, int y) const;
    void DrawLiveTiles(GLFWwindow* window, unsigned int, unsigned int, unsigned int);

    tiles& operator= (const tiles& new_tiles);
};