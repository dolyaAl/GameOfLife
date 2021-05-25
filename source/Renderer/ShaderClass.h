#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class ShaderClass
{
public:
    
    unsigned int ID;
    std::string executablePath;
    
    ShaderClass(const std::string& vName, const std::string& fName, const char* exePath);
    
    void use();
   
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    std::string ShaderClass::getShaderStr(const std::string& relativeFilePath);
    void checkCompileErrors(unsigned int shader, std::string type);
};