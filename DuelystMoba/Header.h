#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <filesystem>
#include "include/raylib.h"

namespace fs = std::filesystem;

struct IVector2 {
    int x;
    int y;
};
struct animation_frame_t {
    IVector2 position;
    IVector2 size;
};
std::map<std::string, std::map<std::string, std::vector<animation_frame_t>>> UnitAnimations;
std::map<std::string, Texture> UnitSpriteSheets;
std::vector<std::string> unit_names;
std::vector<std::string> paths;

void initAnimations(const char * exe_path) {
    std::string path(exe_path);
    std::string ext(".txt");
    const size_t last_slash_idx = path.rfind('\\');
    if (std::string::npos != last_slash_idx)
    {
        path = path.substr(0, last_slash_idx);
    }
    for (auto& p : fs::recursive_directory_iterator(path))
    {
        if (p.path().extension() == ext)
        {
            auto name = p.path().filename().string();
            name = name.substr(0, name.find_last_of("."));
            unit_names.push_back(p.path().stem().string());
            paths.push_back(p.path().string());
            UnitAnimations[name] = std::map<std::string, std::vector<animation_frame_t>>();
            //Texture tex = LoadTexture((path+"\\"+name+".png").c_str());
            //SetTextureFilter(tex, TEXTURE_FILTER_POINT);
            UnitSpriteSheets[name] = Texture();

        }
    }
    for (int i = 0; i < unit_names.size(); i++) {
        std::string name = unit_names[i];
        std::string path = paths[i];
        std::ifstream file(path);
        std::string line;
        std::string animation_name;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            std::vector<animation_frame_t> frames;
            std::getline(iss, token, ',');
            animation_name = token;
            while (std::getline(iss, token, ',')) {
                IVector2 position, size;
                position.x = std::stoi(token);
                std::getline(iss, token, ',');
                position.y = std::stoi(token);
                std::getline(iss, token, ',');
                size.x = std::stoi(token);
                std::getline(iss, token, ',');
                size.y = std::stoi(token);
                animation_frame_t frame;
                frame.position = position;
                frame.size = size;
                frames.push_back(frame);
            }
            UnitAnimations[name][animation_name] = frames;
            
        }
    }
}