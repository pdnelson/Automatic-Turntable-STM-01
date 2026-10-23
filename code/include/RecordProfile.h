#include <Arduino.h>

#ifndef RECORDPROFILE_H
#define RECORDPROFILE_H

#define PROFILE_MAX_SIZE 29
#define PLAYLIST_MAX_SIZE 100
#define AUTO_PICKUP_THRESHOLD 0
#define AUTO_PICKUP_TIME_THRESHOLD_MICROS 300000

class TurntableState;

class RecordProfile {
    public:
        RecordProfile(TurntableState* state);

        void clearPlaylist();
        void clearProfile();
        bool playlistEmpty();
        void addProfileTrackAtPosition();
        void addProfileToPlaylist();
        void addProfileTrack(uint16_t position);
        void addToPlaylist(uint8_t profileIndex);
        void removePlaylistTrack(uint8_t playlistIndex);
        void setProfilePosition(uint8_t profileIndex, uint16_t position);

        /**
         * Return's the current track's progress, in encoder ticks. This does not consider the playlist at all, only the positions around the current track.
         */
        uint16_t getCurrentTrackProgress();
        uint16_t getCurrentTrackProgressMax();

        /**
         * The current track that the tonearm is sitting on. This does not consider the playlist at all, only the positions around the current track.
         * -1 if not on any track.
         */
        int8_t getCurrentPlayingTrackIndex();

        /**
         * The playlist's progress.
         */
        uint32_t getPlaylistProgress();
        uint32_t getPlaylistProgressMax();

        void playAtTrackIndex(uint8_t index);

        /**
         * Return `true` if we are running a custom profile. `false` if it was built by the IR sensor.
         */
        bool customProfile();

        /**
         * This method monitors the auto-pickup routine. If there is a profile present
         */
        void monitorAutoPickup();

        /**
         * Skip forward to the next playlist track.
         * If currentPlaylistIndex is -1, play the first playlist track
         * If currentPlaylistIndex indicates the last track on the playlist, then return home.
         * If the profile is NOT custom, clear it before returning home.
         */
        void skipForward();

        /**
         * Skip backward to the previous playlist track.
         * If the current track is the first item in the playlist, then return home.
         * If the profile is NOT custom, clear it before returning home.
         */
        void skipBackward();

        int8_t currentPlaylistIndex = -1;

        /** The normalized record profile, AFTER the raw data has been separated out into individual track starts.
         * These will be horizontal tonearm start positions. Each start position (except for the first one) can also be
         * used as a pickup position, if the next track isn't on the playlist.
         * The last value (before a 0) is always a tonearm pickup position.
         */
        uint16_t normalizedProfile[PROFILE_MAX_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        uint8_t normalizedProfileSize = 0;

        /**
         * This is the playlist that is traversed through, sequentially, by the turntable.
         * This list consists of normalizedProfile indexes.
         * The playlist is automatically cleared if the tonearm is moved outside the bounds
         */
        int8_t playlist[PLAYLIST_MAX_SIZE] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        uint8_t playlistSize = 0;
    
    private:
        void playAtCurrentIndex();
        void advancePlaylistIndex();
        uint16_t nextLocation();
        uint16_t trackRangeByIndex(uint8_t index);

        unsigned long lastTimeNotInThreshold = 0;

        TurntableState* state;
};

#endif