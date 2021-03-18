#ifndef SOUND_TRACKS_H
#define SOUND_TRACKS_H

#include "include/public/factories/i_sound_tracks.h"

#include <vector>

class ISoundTrack;

class SoundTracks : public ISoundTracks {
public:
    virtual ~SoundTracks();;

    /**
     * @brief create a new sound track
     * @return the newly created sound track
     */
    virtual ISoundTrack *Create() override;

private:
    std::vector<ISoundTrack*> _soundTracks;
};

#endif // SOUND_TRACKS_H
