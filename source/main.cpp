#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include<string>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image\stb_image.h"

#include "GmOb/tiles.h"
#include "Renderer/Texture.h"
#include "Renderer/ShaderClass.h"

//global variables
unsigned int gm_tiles_count = 50;
int scr_WIDTH = 800;
int scr_HEIGHT = 650;
float gm_tiles_step_x = static_cast<float>(scr_WIDTH)/ static_cast<float>(gm_tiles_count);
float gm_tiles_step_y = static_cast<float>(scr_HEIGHT)/ static_cast<float>(gm_tiles_count);
float gm_speed = 0.1f;
double ms_xpos = 0;
double ms_ypos = 0;
unsigned int textures[16]{ 0 };//0-start, 1-opt, 2-qte, 3-name, 4-speed, 5-rules, 6-back
bool ScreenResize = false;
bool GameStarted = false;
bool Menu = true;
bool Options = false;
bool start = false;
int speed = 1;
//
tiles GameTiles(gm_tiles_count);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    scr_HEIGHT = height;
    scr_WIDTH = width;
    gm_tiles_step_x = static_cast<float>(scr_WIDTH) / static_cast<float>(gm_tiles_count);
    gm_tiles_step_y = static_cast<float>(scr_HEIGHT) / static_cast<float>(gm_tiles_count);

}

void DrawVertical(GLFWwindow* window, unsigned int VBO)
{

    glBindTexture(GL_TEXTURE_2D, 0);
    float vertices[4] = {
         -1.f, -1.f,
         -1.f,  1.f
    };

    for (int i = 0; i < gm_tiles_count; ++i)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glDrawArrays(GL_LINE_LOOP, 0, 2);
        vertices[0] += (float)2 / gm_tiles_count;
        vertices[2] += (float)2 / gm_tiles_count;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void DrawHorizontal(GLFWwindow* window, unsigned int VBO)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    float vertices[4] = {
         -1.f, -1.f,
         1.f,  -1.f
    };

    for (int i = 0; i < gm_tiles_count; ++i)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glDrawArrays(GL_LINE_LOOP, 0, 2);
        vertices[1] += (float)2 / gm_tiles_count;
        vertices[3] += (float)2 / gm_tiles_count;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset < 0)
    {
        gm_tiles_count++;
    }
    else if (yoffset > 0)
    {
        if (gm_tiles_count > 2)
        {
            gm_tiles_count--;
        }
    }
    gm_tiles_step_x = static_cast<float>(scr_WIDTH) / static_cast<float>(gm_tiles_count);
    gm_tiles_step_y = static_cast<float>(scr_HEIGHT) / static_cast<float>(gm_tiles_count);
    GameTiles.resize(gm_tiles_count);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            Menu = true;
            Options = false;
        }
        if (key == GLFW_KEY_S)
        {
            ScreenResize = !ScreenResize;
        }
        if (key == GLFW_KEY_ENTER)
        {
            GameStarted = !GameStarted;
        }
    }
}

void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            glfwGetCursorPos(window, &ms_xpos, &ms_ypos);
            if (ms_xpos * 2 / scr_WIDTH > 0.6f && ms_xpos * 2 / scr_WIDTH < 1.4f && ms_ypos * 2 / scr_HEIGHT > 1.f && ms_ypos * 2 / scr_HEIGHT < 1.23f && !Options)
            {
                Menu = false;
            }
            else if (ms_xpos * 2 / scr_WIDTH > 0.6f && ms_xpos * 2 / scr_WIDTH < 1.4f && ms_ypos * 2 / scr_HEIGHT > 1.3f && ms_ypos * 2 / scr_HEIGHT < 1.53f && Menu)
            {
                Options = !Options;
                Menu = !Menu;
            }
            else if (ms_xpos * 2 / scr_WIDTH > 0.6f && ms_xpos * 2 / scr_WIDTH < 1.4f && ms_ypos * 2 / scr_HEIGHT > 1.6f && ms_ypos * 2 / scr_HEIGHT < 1.83f && Menu)
            {
                glfwSetWindowShouldClose(window, true);
            }
            else if (ms_xpos * 2 / scr_WIDTH > 0.55f && ms_xpos * 2 / scr_WIDTH < 1.45f && ms_ypos * 2 / scr_HEIGHT < 1.f && ms_ypos * 2 / scr_HEIGHT > 0.75f && Options)
            {
                speed == 9 ? speed = 1 : speed++;
            }
            else if (ms_xpos * 2 / scr_WIDTH > 0.55f && ms_xpos * 2 / scr_WIDTH < 1.45f && ms_ypos * 2 / scr_HEIGHT > 1.35f && ms_ypos * 2 / scr_HEIGHT < 1.6f && Options)
            {
                Options = !Options;
                Menu = !Menu;
            }
        }
    }
    
}

void processInput(GLFWwindow* window)
{
    if (!ScreenResize)
    {
        glfwSetScrollCallback(window, nullptr);
    }
    else if(ScreenResize && !GameStarted)
    {
        glfwSetScrollCallback(window, scroll_callback);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &ms_xpos, &ms_ypos);
        GameTiles.SetLive(1, (unsigned int)ms_xpos / gm_tiles_step_x, (unsigned int)ms_ypos / gm_tiles_step_y);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &ms_xpos, &ms_ypos);
        GameTiles.SetLive(0, (unsigned int)ms_xpos / gm_tiles_step_x, (unsigned int)ms_ypos / gm_tiles_step_y);
    }
}

void GameRenderer(GLFWwindow* window, unsigned int VBO, unsigned int EBO)
{
    //DrawVertical(window, VBO);
    //DrawHorizontal(window, VBO);

    if (GameTiles.GetLiveCount() > 0)
    {
        GameTiles.DrawLiveTiles(window, VBO, EBO);
    }

    if (GameStarted)
    {
        if (glfwGetTime() >= (1.f - 0.1f * speed))
        {
            GameTiles.newGen();
            glfwSetTime(0);
        }
    }
}

unsigned int loadTexture(const char* exePath, const std::string texName)
{
    std::string executablePath = std::string(exePath);
    size_t found = executablePath.find_last_of("/\\");
    std::string m_path = executablePath.substr(0, found);
    std::string filePath = m_path + "/res/textures/" + texName;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;

    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

void loadTextures(const char* exePath)
{
    textures[0] = loadTexture(exePath, "start.png");
    textures[1] = loadTexture(exePath, "fon3.png");
    textures[2] = loadTexture(exePath, "options.png");
    textures[3] = loadTexture(exePath, "name4.png");
    textures[4] = loadTexture(exePath, "quite.png");
    textures[5] = loadTexture(exePath, "speed.png");
    textures[6] = loadTexture(exePath, "buttonrules1.png");
    textures[7] = loadTexture(exePath, "numbers.png");
}

void speedNumbersRender(GLFWwindow* window, unsigned int VBO, unsigned int EBO)
{

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindTexture(GL_TEXTURE_2D, textures[7]);

    float vertices[] = //speed vertices
    {
         0.7375f,  0.231f,  0.0f,   1.0f, 0.0f, 0.0f,   0.f, 1.0f,    //ïâ
         0.7375f,  0.f,     0.0f,   0.0f, 1.0f, 0.0f,   0.f, 0.0f,    //ïí
         0.55f,    0.f,     0.0f,   0.0f, 0.0f, 1.0f,   0.f, 0.0f,    //ëí
         0.55f,    0.231f,  0.0f,   1.0f, 1.0f, 0.0f,   0.f, 1.0f     //ëâ
    };
   
    vertices[22] = 0 + 0.1 * (speed - 1);
    vertices[30] = 0 + 0.1 * (speed - 1);
    vertices[6] = 0.1 * speed;
    vertices[14] = 0.1 * speed;
     
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void optionsRenderer(GLFWwindow* window, unsigned int VBO, unsigned int EBO)
{
    unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    float vertices0[] = //back image vertices
    {
         1.f,   1.f,   0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //ïâ
         1.f,  -1.f,   0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //ïí
        -1.f,  -1.f,   0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //ëí
        -1.f,   1.f,   0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //ëâ
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, textures[5]);

    float vertices[] = //speed vertices
    {
         0.45f,   0.231f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //ïâ
         0.45f,   0.f,    0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //ïí
        -0.45f,   0.f,    0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //ëí
        -0.45f,   0.231f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //ëâ
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    speedNumbersRender(window, VBO, EBO);

    glBindTexture(GL_TEXTURE_2D, textures[4]);

    float vertices2[] = //go back image vertices
    {
         0.45f,   -0.35f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //ïâ
         0.45f,   -0.60f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //ïí
        -0.45f,   -0.60f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //ëí
        -0.45f,   -0.35f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //ëâ
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void MenuRenderer(GLFWwindow* window, unsigned int VBO, unsigned int EBO)
{
    unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    float vertices0[] = //back image vertices
    {
         1.f,   1.f,   0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //ïâ
         1.f,  -1.f,   0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //ïí
        -1.f,  -1.f,   0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //ëí
        -1.f,   1.f,   0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //ëâ
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    float vertices[] = //start button image vertices
    {
         0.4f,   0.f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //ïâ
         0.4f,  -0.23f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //ïí
        -0.4f,  -0.23f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //ëí
        -0.4f,   0.f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //ëâ
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, textures[2]);
    float vertices1[] = //option button image vertices
    {
         0.4f,   -0.3f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //ïâ
         0.4f,  -0.53f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //ï
        -0.4f,  -0.53f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //ëí
        -0.4f,   -0.3f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //ëâ
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, textures[4]);
    float vertices2[] = //quite button image vertices
    {
         0.4f,   -0.6f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //ïâ
         0.4f,  -0.83f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //ïí
        -0.4f,  -0.83f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //ëí
        -0.4f,   -0.6f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //ëâ
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, textures[3]);
    float vertices3[] = //game name image vertices
    {
         1.f,   1.f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //ïâ
         1.f,   0.23f,   0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //ïí
        -1.f,   0.23f,   0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //ëí
        -1.f,   1.f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  //ëâ
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

bool glInit()
{
    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void glBind(unsigned int VAO, unsigned int VAO2, unsigned int VBO, unsigned int VBO2, unsigned int EBO)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main(int argc, char* argv[])
{
    GLFWwindow* window;
    
    glInit();

    window = glfwCreateWindow(scr_WIDTH, scr_HEIGHT, "GameOfLife", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Can't create Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if(!gladLoadGL())
	{
		std::cout << "Can't load GLAD!" << std::endl;
		return -1;
	}
	
	std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
	
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mousebutton_callback);

    glClearColor(0.7f, 0.7f, 0.7f, 1);
	
    ShaderClass shader("shd.vs", "shd.fs", argv[0]);
    ShaderClass TextureShader("textures.vs", "textures.fs", argv[0]);

    unsigned int VBO = 0;
    unsigned int VBO2 = 0;
    unsigned int VAO = 0;
    unsigned int VAO2 = 0;
    unsigned int EBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBind(VAO, VAO2, VBO, VBO2, EBO);
    
    stbi_set_flip_vertically_on_load(true);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    loadTextures(argv[0]);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        if (Menu)
        {
            glBindVertexArray(VAO);
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            TextureShader.use();
            MenuRenderer(window, VBO, EBO);
            glUseProgram(0);
            glBindVertexArray(0);
        }
        else if (Options)
        {
            glBindVertexArray(VAO);
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            TextureShader.use();
            optionsRenderer(window, VBO, EBO);
            glUseProgram(0);
            glBindVertexArray(0);
        }
        else if (Menu == false && Options == false)
        {
            processInput(window);
            glBindVertexArray(VAO2);
            shader.use();
            GameRenderer(window, VBO2, EBO);
            glBindVertexArray(0);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}