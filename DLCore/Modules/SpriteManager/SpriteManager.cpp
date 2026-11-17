#include "SpriteManager.h"

#include <cassert>

SpriteManager & SpriteManager::Instance() {
    static SpriteManager instance;
    return instance;
}
void SpriteManager::LoadTextureFromFile(const std::string &path, const std::string &name) {

    assert(!images_.contains(name));

    const auto image = std::shared_ptr<Texture2D>(
    new Texture2D(LoadTexture(path.c_str())),
        [](const Texture2D* img) {
            ::UnloadTexture(*img);
            delete img;
        }
    );

    assert(IsTextureValid(*image));
    images_[name] = image;
}

void SpriteManager::UnloadTexture(const std::string &name) {
    const auto it = images_.find(name);
    assert(it != images_.end());
    images_.erase(it);
}

std::shared_ptr<Texture2D> SpriteManager::GetTexture(const std::string &name) {
    const auto it = images_.find(name);
    assert(it != images_.end());
    return it->second;
}
