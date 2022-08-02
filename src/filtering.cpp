#include  "../headers/filtering.h"

void Explorer::search(){
    std::string current_path = std::filesystem::current_path();
    std::string media_path = current_path + "/media";

    if (fs::exists(media_path)){
        for (const auto &entry : fs::recursive_directory_iterator(media_path)){
            if (entry.path().extension() == ".png"){
                png_paths.insert({entry.path().filename(), entry});
            }
        }
    }
    moveselected(0); //hack to load previews
}

void Explorer::draw(olc::PixelGameEngine &pge){
    if (visible){
        if (layer == -1){
            layer = pge.CreateLayer();
        }
        pge.SetDrawTarget(uint8_t(layer));
        pge.Clear(olc::BLANK);
        drawframe(pge); 
        drawentries(pge);
        drawpreview(pge);
        pge.EnableLayer(layer, true);
        pge.SetDrawTarget(nullptr);
    } else {
        pge.EnableLayer(layer, false);
    }
}

void Explorer::drawframe(olc::PixelGameEngine &pge){
    pge.FillRect(pos, size, olc::DARK_BLUE);
    pge.FillRect(listpos, listsize, olc::BLUE);
    pge.FillRect(previewpos, previewsize, olc::BLUE);
}

void Explorer::drawentries(olc::PixelGameEngine &pge){
    olc::vi2d next_pos = listpos + olc::vi2d(3, 3);

    int current = 0;
    for (auto &png : png_paths){
        olc::Pixel colour = current == selected ? olc::BLACK : olc::WHITE;
        pge.DrawString(next_pos, png.first, colour);
        next_pos += olc::vi2d{0, 9};
        current++;
        if (next_pos.y > listpos.y+listsize.y-8) break;
    }
}

void Explorer::drawpreview(olc::PixelGameEngine &pge){
    float sf = float(previewsize.x) / 1200.f;
    pge.DrawDecal(previewpos, preview_d, {sf, sf});
}

void Explorer::loadnew(Texture &t, olc::PixelGameEngine &pge){
    if(!visible) return;
    int ind = 0;
    for(auto & png : png_paths){
        if (ind == selected){
            t.load(png.second, pge);
            break;
        }
        ind++;
    }
    visible = false;
}
void Explorer::moveselected(int i){moveselected(i,0.0f);}
void Explorer::moveselected(int i, float time){
    int new_selected = selected + i;
    if (!visible || new_selected >= png_paths.size() || new_selected < 0) return;
    selected = new_selected;

    std::string path = "";
    int ind = 0;
    for(auto & png : png_paths){
        if (ind == selected){
            path = png.second;
            break;
        }
        ind++;
    }

    preview.LoadFromFile(path);
    preview_d = new olc::Decal(&preview);
}

void Texture::load(std::string path, olc::PixelGameEngine &pge){
    BaseImage.LoadFromFile(path);
    generateMMs(pge);

    update_decal();
}

void Texture::update_decal(){
    delete ToDraw;
    ToDraw = new olc::Decal(MipMaps[int(lod)]);
}

void Texture::draw(olc::PixelGameEngine &pge, olc::TransformedView &tv){
    if (layer == -1){
        layer = pge.CreateLayer();
    }
    pge.SetDrawTarget(uint8_t(layer));
    
    float sf = 1200.0f/ToDraw->sprite->width;

    tv.DrawDecal({0.0f, 0.0f}, ToDraw, {sf, sf});
    pge.EnableLayer(layer, true);
    pge.SetDrawTarget(nullptr);
}

void Texture::generateMMs(olc::PixelGameEngine &pge){
    MipMaps.clear();
    MipMaps.push_back(&BaseImage);

    while (MipMaps.back()->width >= 2 && MipMaps.back()->height >=2){
        olc::Sprite* nsp = new olc::Sprite(MipMaps.back()->width/2, MipMaps.back()->height/2);
        pge.SetDrawTarget(nsp);
        for (int v = 0; v < nsp->height; v++){
            for (int u = 0; u < nsp->width; u++){
                olc::Pixel new_colour = MipMaps.back()->GetPixel({u*2, v*2}) * 0.25f;
                new_colour += MipMaps.back()->GetPixel({u*2+1, v*2  }) * 0.25f;
                new_colour += MipMaps.back()->GetPixel({u*2,   v*2+1}) * 0.25f;
                new_colour += MipMaps.back()->GetPixel({u*2+1, v*2+1}) * 0.25f;
                pge.Draw({u, v}, new_colour);
            }
        }
        MipMaps.push_back(nsp);
    }

    pge.SetDrawTarget(nullptr);
}

void Texture::movemipmap(int i){
    int new_lod = lod + i;
    if (new_lod >= MipMaps.size() || new_lod < 0) return;
    lod = new_lod;
    update_decal();
}

Filtering::Filtering(){
}

bool Filtering::OnUserCreate(){
    //todo move away from here
    texture.load("media/SAMPLING/8BIT/RGB/1200x1200/B01C00/img_1200x1200_3x8bit_B01C00_RGB_ducks.png", *this);
    tv.Initialise({ScreenWidth(), ScreenHeight()}, {1.0, 1.0});
    
    explorer.search();

    return true;
}

bool Filtering::OnUserUpdate(float fElapsedTime){
    Clear(olc::BLANK);
    if(GetKey(olc::Key::Q).bPressed) 
        return false;

    if(GetKey(olc::Key::SPACE).bPressed)
        explorer.visible = !explorer.visible;
    if(GetKey(olc::Key::UP).bHeld)
        explorer.moveselected(-1, fElapsedTime);
    if(GetKey(olc::Key::DOWN).bHeld)
        explorer.moveselected(1, fElapsedTime);
    if(GetKey(olc::Key::ENTER).bPressed)
        explorer.loadnew(texture, *this);

    if(GetKey(olc::Key::M).bPressed)
        texture.movemipmap(1);
    if(GetKey(olc::Key::N).bPressed)
        texture.movemipmap(-1);

    
    tv.HandlePanAndZoom(0);

    explorer.draw(*this);
    texture.draw(*this, tv);

    return true;
}




