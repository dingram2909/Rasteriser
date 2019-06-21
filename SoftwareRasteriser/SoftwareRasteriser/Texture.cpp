#include "Texture.h"

Texture::Texture(void)	{
	width	= 0;
	height	= 0;

	texels = NULL;
}

Texture::~Texture(void)	{
	delete[] texels;
}

Texture* Texture::TextureFromTGA(const string &filename) {
	Texture * t = new Texture();
	std::ifstream file;

	std::cout << "Loading TGA from(" << filename << ")" << std::endl;
	file.open(filename.c_str(), std::ios::binary);
	if(!file.is_open()){
		std::cout << "TextureFromTGA file error" << std::endl;
		return t ;
	}

	unsigned char TGAheader[18];

	std::cout << "sizeof(TGAheader) is " << sizeof(TGAheader) << std::endl;

	file.read((char*)TGAheader,sizeof(TGAheader));

	t->width	= (TGAheader[12] + (TGAheader[13] << 8));
	t->height	= (TGAheader[14] + (TGAheader[15] << 8));

	int size = t->width * t->height * (TGAheader[16] / 8);

	t->texels = new Colour[t->width*t->height];

	file.read( (char*) t->texels ,size);
	file.close();

	return t;
}