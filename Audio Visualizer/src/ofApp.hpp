#pragma once

#include "AudioVisualizer.hpp"
#include "ofMain.h"
#include <vector>

class ofApp : public ofBaseApp {

  private:
    ofSoundPlayer sound;
    AudioVisualizer visualizer;
    ofImage newBackgroundPhoto;
    bool backgroundPhoto = false;

    bool playing = false;
    bool pause = false;
    int lastColorRect;
    vector<float> lastAmp;
    char mode = '1';
    int selectedSong = 0;
    vector<string> songs;
    float vol = 1;
    bool loopy = false;
    bool playL = true;

    int cur_x, cur_y = 0;
    float sound_length;

    float progress = 0;
    float lastPos = 0;
    bool mouseDrag = false;
    bool commandList = false;
    float xPos1;
    float YPos1;
    float Width1;
    float Height1;
    ofRectangle songProgressBar;

  public:
    void setup();
    void update();
    void draw();

    void drawMode1(vector<float> amplitudes);
    void drawMode2(vector<float> amplitudes);
    void drawMode3(vector<float> amplitudes);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
};