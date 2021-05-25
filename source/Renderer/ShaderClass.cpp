#include <glad/glad.h>

#include "ShaderClass.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

ShaderClass::ShaderClass(const std::string& vName, const std::string& fName, const char* exePath)
{
    executablePath = std::string(exePath);
    std::string vertexCode;
    std::string fragmentCode;
    try
    {

        vertexCode = getShaderStr("res/shaders/" + vName);
        fragmentCode = getShaderStr("res/shaders/" + fName);
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int fragment = 0;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragment);
}
   
    void ShaderClass::use()
    {
        glUseProgram(ID);
    }
   
    void ShaderClass::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
   
    void ShaderClass::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
   
    void ShaderClass::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }


   
    std::string ShaderClass::getShaderStr(const std::string& relativeFilePath)
    {
        size_t found = executablePath.find_last_of("/\\");
        std::string m_path = executablePath.substr(0, found);
        std::ifstream f;
        f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
        if (!f.is_open())
        {
            std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
            return std::string{};
        }

        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    void ShaderClass::checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }