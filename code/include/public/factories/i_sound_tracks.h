#ifndef I_SOUND_TRACKS_H
#define I_SOUND_TRACKS_H

#include "include/public/entities/i_sound_track.h"

class ISoundTracks {
public:
    virtual ~ISoundTracks() {};

    /**
     * @brief create a new sound track
     * @return the newly created sound track
     */
    virtual ISoundTrack *Create() = 0;

    // TODO : delete method
};

#endif // I_SOUND_TRACKS_H
