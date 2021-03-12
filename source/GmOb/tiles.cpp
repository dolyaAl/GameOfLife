#include"tiles.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

tiles::tiles(int gm_tiles_count)
{
    count = gm_tiles_count;
    curIndex = 1;
    data1 = new unsigned int* [count];
    for (int i = 0; i < count; ++i)
    {
        data1[i] = new unsigned int[count] {0};
    }
    data2 = new unsigned int* [count];
    for (int i = 0; i < count; ++i)
    {
        data2[i] = new unsigned int[count] {0};
    }
}

tiles::tiles(const tiles& til)
{
    count = til.count;
    live_count = til.live_count;
    curIndex = til.curIndex;
    data1 = new unsigned int* [count];
    for (int i = 0; i < count; ++i)
    {
        data1[i] = new unsigned int[count] {0};
    }
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            data1[i][j] = til.data1[i][j];
        }
    }
    data2 = new unsigned int* [count];
    for (int i = 0; i < count; ++i)
    {
        data2[i] = new unsigned int[count] {0};
    }
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            data2[i][j] = til.data2[i][j];
        }
    }
}

tiles:: ~tiles()
{
    for (int i = 0; i < count; ++i)
    {
        delete[] data1[i];
    }
    for (int i = 0; i < count; ++i)
    {
        delete[] data2[i];
    }
    count = 0;
    live_count = 0;
    curIndex = 0;
}

unsigned int** tiles::currentData()
{
    return curIndex == 1?data1:data2;
}

unsigned int** tiles::bufferData()
{
    return curIndex == 1? data2: data1;
}

void tiles::SetLive(unsigned int live, unsigned int x, unsigned int y)
{
    currentData()[x][y] = live;
    live == 1 ? ++live_count : --live_count;
}

int tiles::GetLive(unsigned int x, unsigned int y)
{
    return currentData()[x][y];
}

int tiles::GetLiveCount()
{
    return live_count;
}

int tiles::GetCount() const
{
    return count;
}

void tiles::DrawLiveTiles(GLFWwindow* window, unsigned int VBO, unsigned int EBO)
{
    float vertices[12]{ 0.f };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            if (currentData()[i][j] == 1)
            {
                vertices[6] = -1.f + (float)(i * 2) / (float)(count);
                vertices[7] = 1.f - (float)(j * 2) / (float)(count);
                vertices[0] = vertices[6] + (float)(2) / (float)(count);
                vertices[1] = vertices[7];
                vertices[4] = vertices[6];
                vertices[5] = vertices[7] - (float)(2) / (float)(count);
                vertices[2] = vertices[6] + (float)(2) / (float)(count);
                vertices[3] = vertices[7] - (float)(2) / (float)(count);

                
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void tiles::newGen()
{
    curIndex == 1 ? curIndex = 2 : curIndex = 1;
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            if (bufferData()[i][j] == 1 && getLiveNeighborsCount(i, j) >= 2 && getLiveNeighborsCount(i, j) <= 3)
            {
                SetLive(1, i, j);
            }
            else if (getLiveNeighborsCount(i, j) == 3)
            {
                SetLive(1, i, j);
            }
        }
    }
    FreeMemory();
}

void tiles::FreeMemory()
{
    for (int i = 0; i < count; ++i)
    {
        delete[] bufferData()[i];
    }
    for (int i = 0; i < count; ++i)
    {
        bufferData()[i] = new unsigned int[count] {0};
    }
    
}

int tiles::getLiveNeighborsCount(int x, int y)
{
    int counter = 0;
    if (x == 0)
    {
        if (y == 0)
        {
            counter = bufferData()[x + 1][y] + bufferData()[x][y + 1] + bufferData()[x + 1][y + 1];
        }
        else if (y == count - 1)
        {
            counter = bufferData()[x + 1][y] + bufferData()[x][y - 1] + bufferData()[x + 1][y - 1];
        }
        else
        {
            counter = bufferData()[x + 1][y] + bufferData()[x][y - 1] + bufferData()[x][y + 1] + bufferData()[x + 1][y - 1] + bufferData()[x + 1][y + 1];
        }
    }
    else if (x == count - 1)
    {
        if (y == 0)
        {
            counter = bufferData()[x - 1][y] + bufferData()[x][y + 1] + bufferData()[x - 1][y + 1];
        }
        else if (y == count - 1)
        {
            counter = bufferData()[x - 1][y] + bufferData()[x][y - 1] + bufferData()[x - 1][y - 1];
        }
        else
        {
            counter = bufferData()[x - 1][y] + bufferData()[x][y - 1] + bufferData()[x][y + 1] + bufferData()[x - 1][y - 1] + bufferData()[x - 1][y + 1];
        }
    }
    else if (y == 0)
    {

        counter = bufferData()[x + 1][y] + bufferData()[x - 1][y] + bufferData()[x][y + 1] + bufferData()[x - 1][y + 1] + bufferData()[x + 1][y + 1];

    }
    else if (y == count - 1)
    {
        counter = bufferData()[x + 1][y] + bufferData()[x - 1][y] + bufferData()[x][y - 1] + bufferData()[x - 1][y - 1] + bufferData()[x + 1][y - 1];
    }
    else
    {
        counter = bufferData()[x + 1][y] + bufferData()[x - 1][y] + bufferData()[x][y - 1] + bufferData()[x - 1][y - 1] + bufferData()[x + 1][y - 1] + bufferData()[x][y + 1] + bufferData()[x + 1][y + 1] + bufferData()[x - 1][y + 1];
    }
    return counter;
}