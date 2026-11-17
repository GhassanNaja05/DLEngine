#ifndef AUDIO_H
#define AUDIO_H
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include <raylib.h>
#include <string>


class Audio final {
public:

    static Audio& Instance();
    void Clear();

    void LoadAudioClip(const std::string& path, const std::string& name);
    void LoadLoopingClip(const std::string& path, const std::string& name);

    void PlaySingleShotClip(const std::string& name, float volume = -1.0f);

    void PlayAudioClip(const std::string& name);
    void StopAudioCLip(const std::string& name);
    void ToggleAudioClip(const std::string& name);
    void SetClipVolume(const std::string& name, float volume);

    void LoopAudioClip(const std::string& name);
    void StopLoop(const std::string& name);
    void ToggleLoop(const std::string& name);
    void SetLoopVolume(const std::string& name, float volume);

    void UnloadAudioClip(const std::string& name);
    void UnloadLoopingClip(const std::string& name);

private:

    Audio() = default;
    ~Audio() = default;

    void Update();

    std::unordered_map<std::string, std::shared_ptr<Sound>> clips_ = std::unordered_map<std::string, std::shared_ptr<Sound>>();
    std::unordered_map<std::string, Music> looping_ = std::unordered_map<std::string, Music>();

    struct PlayingSound {
        std::shared_ptr<Sound> owner_;
        Sound alias_;
    };

    std::vector<PlayingSound> single_shots_ = std::vector<PlayingSound>();

    friend class Engine;
};



#endif
