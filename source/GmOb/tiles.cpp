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
    if (GetLive(x, y) != live && indexValid(x, y))
    {
        currentData()[x][y] = live;
        live == 1 ? ++live_count : --live_count;
    }
}

void tiles::resize(int gm_tiles_count)
{
    int n_count = 0;
    gm_tiles_count <= count ? n_count = gm_tiles_count : n_count = count;
    int** new_data = new int* [n_count];
    for (int i = 0; i < n_count; ++i)
    {
        new_data[i] = new int[n_count] {0};
    }
    for (int i = 0; i < n_count; ++i)
    {
        for (int j = 0; j < n_count; ++j)
        {
            new_data[i][j] = data1[i][j];
        }
    }
    for (int i = 0; i < count; ++i)
    {
        delete[] data1[i];
    }
    delete[] data1;
    data1 = new unsigned int* [gm_tiles_count];
    for (int i = 0; i < gm_tiles_count; ++i)
    {
        data1[i] = new unsigned int[gm_tiles_count] {0};
    }
    for (int i = 0; i < n_count; ++i)
    {
        for (int j = 0; j < n_count; ++j)
        {
            data1[i][j] = new_data[i][j];
        }
    }

    for (int i = 0; i < n_count; ++i)
    {
        delete[] new_data[i];
    }
    delete[] new_data;
    new_data = new int* [n_count];
    for (int i = 0; i < n_count; ++i)
    {
        new_data[i] = new int[n_count] {0};
    }


    for (int i = 0; i < n_count; ++i)
    {
        for (int j = 0; j < n_count; ++j)
        {
            new_data[i][j] = data2[i][j];
        }
    }
    for (int i = 0; i < count; ++i)
    {
        delete[] data2[i];
    }
    delete[] data2;
    data2 = new unsigned int* [gm_tiles_count];
    for (int i = 0; i < gm_tiles_count; ++i)
    {
        data2[i] = new unsigned int[gm_tiles_count] {0};
    }
    for (int i = 0; i < n_count; ++i)
    {
        for (int j = 0; j < n_count; ++j)
        {
            data2[i][j] = new_data[i][j];
        }
    }

    for (int i = 0; i < n_count; ++i)
    {
        delete[] new_data[i];
    }
    delete[] new_data;

    count = gm_tiles_count;
}

int tiles::GetLive(unsigned int x, unsigned int y)
{
    if (indexValid(x, y))
    {
        return currentData()[x][y];
    }
    return -1;
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

    glBindTexture(GL_TEXTURE_2D, 0);


    float vertices[12]{ 0.f };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    /*unsigned int texCoords[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };
  
    glBindBuffer(GL_TEXTURE_2D_ARRAY, TBO);
    glBufferData(GL_TEXTURE_2D_ARRAY, sizeof(texCoords), texCoords, GL_STATIC_DRAW);*/

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

bool tiles::indexValid(int x, int y)
{
    return (x >= 0) && (x < count) && (y >= 0) && (y < count);
}

/*tiles* tiles::operator=(tiles& n_tiles)
{
    for (int i = 0; i < count; ++i)
    {
        delete[] data1[i];
        delete[] data2[i];
    }
    delete[] data1;
    delete[] data2;

    unsigned int min_count = 0;
    count < n_tiles.count? count = n_tiles.count, min_count = count: count = count, min_count = n_tiles.count;

    curIndex = n_tiles.curIndex;

    data1 = new unsigned int* [count];
    for (int i = 0; i < count; ++i)
    {
        data1[i] = new unsigned int[count] {0};
        for (int j = 0; j < count; ++j)
        {
            data1[i][j] = n_tiles.data1[i][j];
        }
    }
    for (int i = 0; i < min_count; ++i)
    {
        for (int j = 0; j < min_count; ++j)
        {
            data1[i][j] = n_tiles.data1[i][j];
        }
    }
    data2 = new unsigned int* [count];
    for (int i = 0; i < count; ++i)
    {
        data2[i] = new unsigned int[count] {0};
        for (int j = 0; j < count; ++j)
        {
            data2[i][j] = n_tiles.data2[i][j];
        }
    }
    for (int i = 0; i < min_count; ++i)
    {
        data2[i] = new unsigned int[count] {0};
        for (int j = 0; j < min_count; ++j)
        {
            data2[i][j] = n_tiles.data2[i][j];
        }
    }
    return this;
}*/

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
    /*if (x == 0)
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
    }*/

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (indexValid(x + i, y + j) && (i * i + j * j != 0))
            {
                counter += bufferData()[x + i][y + j];
            }
        }
    }

    return counter;
}