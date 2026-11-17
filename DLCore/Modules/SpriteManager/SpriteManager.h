#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H
#include <map>
#include <memory>
#include <string>

#include "raylib.h"


class SpriteManager {

public:
    static SpriteManager& Instance();
    void Clear() { images_.clear(); }

    void LoadTextureFromFile(const std::string& path, const std::string& name);
    void UnloadTexture(const std::string& name);

private:
    SpriteManager() = default;
    ~SpriteManager() = default;

    std::map<std::string, std::shared_ptr<Texture2D>> images_ = std::map<std::string, std::shared_ptr<Texture2D>>();

    std::shared_ptr<Texture2D> GetTexture(const std::string& name);

    friend class Sprite;
    friend class TiledSprite;
};



#endif
