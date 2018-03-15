#pragma once

void setVolume(int volume);

void playSound(const char* filename);
void playMusic(const char* filename);

void initAudio();
void endAudio();
void pauseAudio();
void unpauseAudio();