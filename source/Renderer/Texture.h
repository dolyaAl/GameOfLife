#pragma once

class Texture
{
private:
	unsigned int texture = 0;
	const char* filepath = { 0 };
public:
	Texture();
	unsigned int GetTexture();
	void LoadTexture(const char* path);
};