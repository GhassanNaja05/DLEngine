#include "Audio.h"

#include <assert.h>

#include "../../Debug/Debug.h"

Audio &Audio::Instance() {
    static Audio instance;
    return instance;
}

void Audio::Clear() {
    for (const auto& sound : single_shots_) {
        StopSound(sound.alias_);
        UnloadSoundAlias(sound.alias_);
    }
    single_shots_.clear();
    for (const auto& [name, music] : looping_) {
        StopMusicStream(music);
        UnloadMusicStream(music);
    }
    looping_.clear();
    clips_.clear();
}

void Audio::LoadAudioClip(const std::string &path, const std::string &name) {
    assert(!clips_.contains(name));
    const auto sound = std::shared_ptr<Sound>(new Sound(LoadSound(path.c_str())), [] (Sound* s) {
        if (s) {
            UnloadSound(*s);
           delete s;
        }
    });
    assert(IsSoundValid(*sound));
    clips_[name] = sound;
}

void Audio::LoadLoopingClip(const std::string &path, const std::string &name) {
    assert(!looping_.contains(name));
    const Music music = LoadMusicStream(path.c_str());
    assert(IsMusicValid(music));
    looping_[name] = music;
}

void Audio::PlaySingleShotClip(const std::string &name, float volume) {

    const auto sound = clips_.find(name);
    assert(sound != clips_.end());

    const auto& clip = sound->second;
    const auto alias = LoadSoundAlias(*clip);

    if (volume != -1.0f) {
        if (volume < 0.0f) volume = 0.0f;
        else if (volume > 1.0f) volume = 1.0f;

        SetSoundVolume(alias, volume);
    }

    single_shots_.push_back({ clip, alias});

    PlaySound(single_shots_.back().alias_);
}

void Audio::PlayAudioClip(const std::string &name) {

    const auto sound = clips_.find(name);
    assert(sound != clips_.end());

    PlaySound(*sound->second);
}

void Audio::StopAudioCLip(const std::string &name) {

    const auto sound = clips_.find(name);
    assert(sound != clips_.end());

    StopSound(*sound->second);
}

void Audio::ToggleAudioClip(const std::string &name) {

    const auto sound = clips_.find(name);
    assert(sound != clips_.end());

    if (const auto &clip = *sound->second; IsSoundPlaying(clip))
        PauseSound(*sound->second);
    else
        ResumeSound(*sound->second);
}

void Audio::LoopAudioClip(const std::string &name) {
    const auto sound = looping_.find(name);
    assert(sound != looping_.end());

    SeekMusicStream(sound->second, 0.0f);
    PlayMusicStream(sound->second);
}

void Audio::StopLoop(const std::string &name) {
    const auto loop = looping_.find(name);
    assert(loop != looping_.end());
    StopMusicStream(loop->second);
}

void Audio::ToggleLoop(const std::string &name) {

    const auto loop = looping_.find(name);
    assert(loop != looping_.end());

    if (const auto &clip = loop->second; IsMusicStreamPlaying(clip))
        PauseMusicStream(clip);
    else
        ResumeMusicStream(loop->second);
}

void Audio::SetClipVolume(const std::string& name, float volume) {
    const auto sound = clips_.find(name);
    assert(sound != clips_.end());

    if (volume < 0.0f) volume = 0.0f;
    else if (volume > 1.0f) volume = 1.0f;

    const auto &clip = sound->second;
    SetSoundVolume(*clip, volume);
}

void Audio::SetLoopVolume(const std::string& name, float volume) {
    const auto sound = looping_.find(name);
    assert(sound != looping_.end());

    if (volume < 0.0f) volume = 0.0f;
    else if (volume > 1.0f) volume = 1.0f;

    const auto &loop = sound->second;
    SetMusicVolume(loop, volume);
}

void Audio::UnloadAudioClip(const std::string &name) {

    const auto sound = clips_.find(name);
    assert(sound != clips_.end());

    clips_.erase(sound);
}
void Audio::UnloadLoopingClip(const std::string &name) {

    const auto loop = looping_.find(name);
    assert(loop != looping_.end());

    const auto sound = loop->second;
    if (IsMusicStreamPlaying(sound))
        StopMusicStream(sound);

    UnloadMusicStream(sound);
    looping_.erase(loop);
}

void Audio::Update() {
    for (auto& loop : looping_) {
        UpdateMusicStream(loop.second);
    }

    std::erase_if(single_shots_, [](const PlayingSound & s) {
    if (!IsSoundPlaying(s.alias_)) {
        UnloadSoundAlias(s.alias_);
        return true;
    }
    return false;
});
}
