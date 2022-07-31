#ifndef FILTERING_H
#define FILTERING_H
#include "olcPGEX_TransformedView.h"
#include "olcPixelGameEngine.h"
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <map>

namespace fs = std::filesystem;

class Texture {
public:
    int size = 1200;
    float lod = 6.0f;
    int8_t layer = -1;
    olc::Sprite BaseImage;
    olc::Decal* ToDraw;
    std::vector<olc::Sprite*> MipMaps;

    Texture(){}
    void load(std::string path, olc::PixelGameEngine &pge);
    void draw(olc::PixelGameEngine &pge, olc::TransformedView &tv);
    void generateMMs(olc::PixelGameEngine &pge);
    void movemipmap(int i);
    void update_decal();
};

class Explorer {
public:
    std::map<std::string,fs::path> png_paths;
    int8_t layer = -1;
    bool visible = true;
        
    olc::Sprite preview;
    olc::Decal* preview_d;
        
    //layout settings
    olc::vi2d border = {10, 10}; 
    olc::vi2d pos;
    olc::vi2d size;
    olc::vi2d listpos;
    olc::vi2d listsize;    
    olc::vi2d previewpos;
    olc::vi2d previewsize;

    int selected = 0;

    Explorer(){
        pos = {100, 100};
        //TODO: hardcoded
        size = {1200-200, 1200-200};
        listpos = pos + border;
        listsize = {600, size.y - 2*border.x};
        previewpos = {listpos.x + listsize.x + border.x, listpos.y};
        previewsize = {size.x - listsize.x - 3*border.x, listsize.y};
    };
    void search(); 
    void draw(olc::PixelGameEngine &pge);
    void drawframe(olc::PixelGameEngine &pge);
    void drawlist(olc::PixelGameEngine &pge);
    void drawentries(olc::PixelGameEngine &pge);
    void drawpreview(olc::PixelGameEngine &pge);
    void moveselected(int i, float time);
    void moveselected(int i);
    void loadnew(Texture &t, olc::PixelGameEngine &pge);
};

class Filtering : public olc::PixelGameEngine {
public:
    olc::TransformedView tv;
    Texture texture;
    Explorer explorer;

    Filtering();
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

#endif
