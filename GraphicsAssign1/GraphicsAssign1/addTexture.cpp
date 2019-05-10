#include"bmpddsloader.h"
#include"Draw.h"
#include"soil.h"
void AddWallTexture(const vector<string> filePaths, MyObjData* ObjData);
void AddPolygonTexture(const vector<string> filePaths, MyObjData* ObjData);

void addTexture(map< string, int > mappingFromStringToInt, map<string, MyObjData*>* ObjData_map, const vector<string> WallTexturePaths, const vector<string> PlayerTexturePaths) { // bind .obj path for each object
	for (int i = 0; i < objectsTreeVectorForDraw.size(); i++) {
		if (mappingFromStringToInt[objectsTreeVectorForDraw[i].name] == BOTTOMWALL)
			AddWallTexture(WallTexturePaths, (*ObjData_map)[objectsTreeVectorForDraw[i].name]);
	}
	for (int i = 0; i < objectsTreeVectorForDraw.size(); i++) {
		switch (mappingFromStringToInt[objectsTreeVectorForDraw[i].name])
		{
		case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL:
			(*ObjData_map)[objectsTreeVectorForDraw[i].name]->tex = (*ObjData_map)["bottomwall"]->tex; break;
			break;
		case BALL:  break;
		case PLAYER: AddPolygonTexture(PlayerTexturePaths, (*ObjData_map)[objectsTreeVectorForDraw[i].name]); break;
		case ENEMY: (*ObjData_map)[objectsTreeVectorForDraw[i].name]->tex = (*ObjData_map)["player"]->tex; break;
		default: break;
		}
	}
}

void AddWallTexture(const vector<string> filePaths, MyObjData* ObjData) {
	GLuint tex1 = loadDDS("diffuse.DDS"); //mipmap ±¦ÂúÀºÁö È®ÀÎ ÇÊ¿ä
	ObjData->tex.push_back(tex1);
	GLuint tex2 = loadDDS("specular.DDS");
	ObjData->tex.push_back(tex2);
	GLuint tex3 = loadBMP_custom("normal.bmp");
	ObjData->tex.push_back(tex3);
}
void AddPolygonTexture(const vector<string> filePaths, MyObjData* ObjData) {
	GLuint tex1 = SOIL_load_OGL_texture
	(
		"Image09C37780_09C26A60.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	ObjData->tex.push_back(tex1);
	GLuint tex2 = SOIL_load_OGL_texture
	(
		"Image09C27700_09C272A0.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS |  SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	ObjData->tex.push_back(tex2);
	GLuint tex3 = SOIL_load_OGL_texture
	(
		"Image09C3F800_09C26E80.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	ObjData->tex.push_back(tex3);
	GLuint tex4 = SOIL_load_OGL_texture
	(
		"Image09CDBA00_09CC3480.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	ObjData->tex.push_back(tex4);
	GLuint tex5 = SOIL_load_OGL_texture
	(
		"Image09CD3980_09CC3060.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	ObjData->tex.push_back(tex5);
}