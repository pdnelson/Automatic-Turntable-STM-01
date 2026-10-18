#include <RecordProfile.h>
#include <TurntableState.h>
#include <CmdGoToPositionH.h>

RecordProfile::RecordProfile(TurntableState* state) {
    this->state = state;
}

void RecordProfile::clearPlaylist() {
    for(uint8_t i = 0; i < PLAYLIST_MAX_SIZE; i++) {
        playlist[i] = -1;
    }
    playlistSize = 0;
    currentPlaylistIndex = -1;
}

void RecordProfile::clearProfile() {
    for(uint8_t i = 0; i < PROFILE_MAX_SIZE; i++) {
        normalizedProfile[i] = 0;
    }
    normalizedProfileSize = 0;

    clearPlaylist();
}

bool RecordProfile::playlistEmpty() {
    return playlist[0] == -1;
}

void RecordProfile::addProfileTrackAtPosition() {
    addProfileTrack(state->azEncoder.getNormalizedPosition());
}

void RecordProfile::addProfileToPlaylist() {
    // Do nothing if there are not at least two entries in the profile. This indicates
    // the profile is not yet complete.
    if(normalizedProfileSize < 2) {
        return;
    }

    for(uint8_t i = 0; i < normalizedProfileSize - 1; i++) {
        playlist[playlistSize] = i;
        playlistSize++;
    }
}

void RecordProfile::addProfileTrack(uint16_t position) {
    normalizedProfile[normalizedProfileSize] = position;
    normalizedProfileSize++;
}

void RecordProfile::addToPlaylist(uint8_t profileIndex) {
    // Exclude the profile's pickup position. Who would want to add that to their playlist?!
    if(profileIndex < normalizedProfileSize - 1) {
        playlist[playlistSize] = profileIndex;
        playlistSize++;
    }
}

void RecordProfile::removePlaylistTrack(uint8_t playlistIndex) {
    // Don't let them delete the current index
    if(playlistIndex != currentPlaylistIndex && playlistIndex < playlistSize) {
        if(playlistIndex < currentPlaylistIndex) {
            currentPlaylistIndex--;
        }

        playlistSize--;

        for(uint8_t i = playlistIndex; i < playlistSize; i++) {
            playlist[i] = playlist[i + 1];
        }

        if(playlistSize < PLAYLIST_MAX_SIZE) {
            playlist[playlistSize] = -1;
        }
    }
}

void RecordProfile::setProfilePosition(uint8_t profileIndex, uint16_t position) {
    if(normalizedProfile[profileIndex] != 0) {
        normalizedProfile[profileIndex] = position;
    }
}

bool RecordProfile::customProfile() {
    return true; // to do: implement this for real when calculated profiles become available
}

void RecordProfile::monitorAutoPickup() {
    if(currentPlaylistIndex > -1) {
        bool onOrPastThreshold = state->azEncoder.getNormalizedPosition() >= nextLocation() + AUTO_PICKUP_THRESHOLD;

        // If we've reached the next location for AUTO_PICKUP_TIME_THRESHOLD_MICROS microseconds, advance the playlist index.
        if(onOrPastThreshold && state->clockMicros - lastTimeNotInThreshold > AUTO_PICKUP_TIME_THRESHOLD_MICROS) {
            advancePlaylistIndex();
        } else if(!onOrPastThreshold) {
            lastTimeNotInThreshold = state->clockMicros;
        }
    } // to do: add else condition here for non-profile auto pickup
}

void RecordProfile::skipForward() {
    // If the playlist is empty, skip to the next track in the profile
    if(playlistEmpty() && normalizedProfileSize > 1) {
        uint16_t currentPosition = state->azEncoder.getNormalizedPosition();
        uint16_t goToPosition = state->calibration.home;

        // If the current position is before the first profile item, then go there.
        if(currentPosition < normalizedProfile[0]) {
            goToPosition = normalizedProfile[0];
        } 
        
        // Otherwise, find the next track to go to, IF we are not on the last track
        else if(currentPosition < normalizedProfile[normalizedProfileSize - 2]) {
            // Iterate through the list until we get to the position the tonearm is past
            for(uint8_t i = 0; i < normalizedProfileSize; i++) {
                if(currentPosition < normalizedProfile[i]) {
                    goToPosition = normalizedProfile[i];
                    break;
                }
            }
        }

        state->currentCommand = std::make_unique<CmdGoToPositionH>(state, goToPosition, 0, 6);

        return;
    }

    // If the current index is the last one, go home
    if(playlist[currentPlaylistIndex + 1] == -1) {
        state->playOrReturn();

        if(!customProfile()) {
            clearProfile();
        } else {
            currentPlaylistIndex = -1;
        }
    }

    // Otherwise, we are free to just increment the playlist and go there
    else {
        currentPlaylistIndex++;
        playAtCurrentIndex();
    }
}

void RecordProfile::skipBackward() {
    // If the playlist is empty, skip to the previous track in the profile
    if(playlistEmpty() && normalizedProfileSize > 1) {
        uint16_t currentPosition = state->azEncoder.getNormalizedPosition();

        // Search for a track with a previous track, if we are at least at the first track, then there
        // will be a previous track (previous from first track being home)
        if(currentPosition >= normalizedProfile[0]) {
            uint16_t goToPosition = state->calibration.home;

            // If we are past the last track, go back to the last track
            if(currentPosition >= normalizedProfile[normalizedProfileSize - 1]) {
                goToPosition = normalizedProfile[normalizedProfileSize - 2];
            } else {
                // Iterate through the list until we get to the position the tonearm is past
                for(uint8_t i = 1; i < normalizedProfileSize; i++) {
                    if(currentPosition < normalizedProfile[i]) {
                        goToPosition = normalizedProfile[i - 2];
                        break;
                    }
                }
            }

            state->currentCommand = std::make_unique<CmdGoToPositionH>(state, goToPosition, 0, 6);
        }

        return;
    }

    // If the current index is the first one, go home
    if(currentPlaylistIndex == 0) {
        state->playOrReturn();

        if(!customProfile()) {
            clearProfile();
        } else {
            currentPlaylistIndex = -1;
        }
    }

    // Otherwise, we are free to just decrement the playlist and go there
    else {
        currentPlaylistIndex--;
        playAtCurrentIndex();
    }
}

void RecordProfile::playAtCurrentIndex() {
    // to do: go at different speeds depending on the distance between tracks
    uint16_t destination = normalizedProfile[playlist[currentPlaylistIndex]];
    state->currentCommand = std::make_unique<CmdGoToPositionH>(state, destination, 0, 6);
}

void RecordProfile::advancePlaylistIndex() {
    // If the next playlist index's value is +1 from the current one, then do not skip the track,
    // just quietly advance the playlist. This implies that the next playlist item is actually the
    // next track on the record.
    if(state->currentCommand == nullptr) {
        if(playlist[currentPlaylistIndex] + 1 == playlist[currentPlaylistIndex + 1]) {
            currentPlaylistIndex++;
        }

        // Otherwise, skip forward
        else {
            skipForward();
        }
    }
}

uint16_t RecordProfile::nextLocation() {
    // The next location is the start of the track after the current one, on the record profile.
    return normalizedProfile[playlist[currentPlaylistIndex] + 1];
}