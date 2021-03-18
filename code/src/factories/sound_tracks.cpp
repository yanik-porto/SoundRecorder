#include "include/private/factories/sound_tracks.h"

#include "include/private/sound_track.h"

SoundTracks::~SoundTracks(){
    for (auto soundTrack : _soundTracks) {
        if (soundTrack) {
            delete soundTrack;
        }
    }
    _soundTracks.clear();
}

ISoundTrack *SoundTracks::Create() {
    ISoundTrack* soundTrackCreated{ new SoundTrack() };
    _soundTracks.push_back(soundTrackCreated);
    return soundTrackCreated;
}
