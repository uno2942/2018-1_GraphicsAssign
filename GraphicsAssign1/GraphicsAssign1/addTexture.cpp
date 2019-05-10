#include"bmpddsloader.h"
#include"Draw.h"

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
	GLuint tex1 = loadBMP_custom("Cat_diffuse.bmp");
	ObjData->tex.push_back(tex1);
}