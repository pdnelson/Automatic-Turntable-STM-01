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
}

void RecordProfile::clearProfile() {
    for(uint8_t i = 0; i < PROFILE_MAX_SIZE; i++) {
        normalizedProfile[i] = 0;
    }

    clearPlaylist();
}

bool RecordProfile::playlistEmpty() {
    return playlist[0] == -1;
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
    // If the playlist is empty, do nothing
    if(playlistEmpty()) {
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
    if(playlistEmpty() || currentPlaylistIndex == -1) {
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
    if(playlist[currentPlaylistIndex] + 1 == playlist[currentPlaylistIndex + 1]) {
        currentPlaylistIndex++;
    }

    // Otherwise, skip forward
    else {
        if(state->currentCommand == nullptr) {
            skipForward();
        }
    }
}

uint16_t RecordProfile::nextLocation() {
    // The next location is the start of the track after the current one, on the record profile.
    return normalizedProfile[playlist[currentPlaylistIndex] + 1];
}