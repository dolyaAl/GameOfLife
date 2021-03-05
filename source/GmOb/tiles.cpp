#include"tiles.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

tiles::tiles(int gm_tiles_count)
{
    count = gm_tiles_count;
    data = new unsigned int* [count];
    for (int i = 0; i < count; ++i)
    {
        data[i] = new unsigned int[count] {0};
    }
}
tiles::tiles(const tiles& til)
{
    count = til.count;
    live_count = til.live_count;
    data = new unsigned int* [count];
    for (int i = 0; i < count; ++i)
    {
        data[i] = new unsigned int[count] {0};
    }
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            data[i][j] = til.data[i][j];
        }
    }
}
tiles:: ~tiles()
{
    for (int i = 0; i < count; ++i)
    {
        delete[] data[i];
    }
    int count = 0;
    int live_count = 0;
}
void tiles::SetLive(unsigned int live, unsigned int x, unsigned int y)
{
    data[x][y] = live;
    live == 1 ? ++live_count : --live_count;
}
int tiles::GetLive(unsigned int x, unsigned int y)
{
    return data[x][y];
}
int tiles::GetLiveCount()
{
    return live_count;
}
int tiles::GetCount() const
{
    return count;
}

void tiles::DrawLiveTiles(GLFWwindow* window, unsigned int VBO, unsigned int EBO, unsigned int VAO)
{
    float vertices[12]{ 0.f };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            if (data[i][j] == 1)
            {
                vertices[9] = -1.f + (float)(i * 2) / (float)(count);
                vertices[10] = 1.f - (float)(j * 2) / (float)(count);
                vertices[0] = vertices[9] + (float)(2) / (float)(count);
                vertices[1] = vertices[10];
                vertices[6] = vertices[9];
                vertices[7] = vertices[10] - (float)(2) / (float)(count);
                vertices[3] = vertices[9] + (float)(2) / (float)(count);
                vertices[4] = vertices[10] - (float)(2) / (float)(count);

                
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

tiles& tiles:: operator= (const tiles& new_tiles)
{
    if (&new_tiles != this)
    {
        for (int i = 0; i < count; ++i)
        {
            delete[] data[i];
        }
        count = new_tiles.count;
        live_count = new_tiles.live_count;
        data = new unsigned int* [count] {0};
        for (int i = 0; i < count; ++i)
        {
            data[i] = new unsigned int[count] {0};
        }
        for (int i = 0; i < count; ++i)
        {
            for (int j = 0; j < count; ++j)
            {
                data[i][j] = new_tiles.data[i][j];
            }
        }
    }
    return *this;
}

void tiles::FreeMemory()
{
    for (int i = 0; i < count; ++i)
    {
        delete[] data[i];
    }
    for (int i = 0; i < count; ++i)
    {
        data[i] = new unsigned int[count] {0};
    }
}

void tiles::addAll(const tiles& new_tiles)
{
    if (count < new_tiles.count)
    {
        for (int i = 0; i < new_tiles.count; ++i)
        {
            for (int j = 0; j < new_tiles.count; ++j)
            {
                data[i][j] = new_tiles.data[i][j];
            }
        }
    }
}

int tiles::getLiveNeighborsCount(int x, int y) const
{
    int counter = 0;
    if (x == 0)
    {
        if (y == 0)
        {
            counter = data[x + 1][y] + data[x][y + 1] + data[x + 1][y + 1];
        }
        else if (y == count - 1)
        {
            counter = data[x + 1][y] + data[x][y - 1] + data[x + 1][y - 1];
        }
        else
        {
            counter = data[x + 1][y] + data[x][y - 1] + data[x][y + 1] + data[x + 1][y - 1] + data[x + 1][y + 1];
        }
    }
    else if (x == count - 1)
    {
        if (y == 0)
        {
            counter = data[x - 1][y] + data[x][y + 1] + data[x - 1][y + 1];
        }
        else if (y == count - 1)
        {
            counter = data[x - 1][y] + data[x][y - 1] + data[x - 1][y - 1];
        }
        else
        {
            counter = data[x - 1][y] + data[x][y - 1] + data[x][y + 1] + data[x - 1][y - 1] + data[x - 1][y + 1];
        }
    }
    else if (y == 0)
    {

        counter = data[x + 1][y] + data[x - 1][y] + data[x][y + 1] + data[x - 1][y + 1] + data[x + 1][y + 1];

    }
    else if (y == count - 1)
    {
        counter = data[x + 1][y] + data[x - 1][y] + data[x][y - 1] + data[x - 1][y - 1] + data[x + 1][y - 1];
    }
    else
    {
        counter = data[x + 1][y] + data[x - 1][y] + data[x][y - 1] + data[x - 1][y - 1] + data[x + 1][y - 1] + data[x][y + 1] + data[x + 1][y + 1] + data[x - 1][y + 1];
    }
    return counter;
}