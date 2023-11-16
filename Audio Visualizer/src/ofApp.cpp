#include "ofApp.hpp"
#include <filesystem>
#include <string>

using namespace std;
namespace fs = std::filesystem;

//--------------------------------------------------------------
void ofApp::setup()
{
    fs::path p = fs::current_path();
    p += "\\bin\\data";
    for (const auto &dir_entry : fs::directory_iterator{p})
    {
        string filename = dir_entry.path().filename().string();
        string extension = filename.substr(filename.find_last_of("."));
        if (extension == ".wav" || extension == ".mp3")
        {
            songs.push_back(dir_entry.path().filename().string());
        }
    }
    sound.load(songs[0]);               // Loads a sound file (in bin/data/)
    sound.setLoop(loopy);               // Makes the song loop indefinitely
    sound.setVolume(vol);               // Sets the song volume
    ofSetBackgroundColor(255, 170, 80); // Sets the Background Color
    newBackgroundPhoto.load("beachwallpaper.jpg");
}

//--------------------------------------------------------------
void ofApp::update()
{
    /* The update method is called muliple times per second
    It's in charge of updating variables and the logic of our app */
    ofSoundUpdate();               // Updates all sound players
    visualizer.updateAmplitudes(); // Updates Amplitudes for visualizer
    progress = sound.getPosition();
    if (!sound.isPlaying() && playL && playing && !loopy && !pause && !mouseDrag)
    {
        sound.unload();
        selectedSong += (selectedSong < songs.size() - 1) ? 1 : -1 * (songs.size() - 1);
        sound.load(songs[selectedSong]);
        sound.setVolume(vol);
        sound.play();
    }
    if (!loopy && !playL && !sound.isPlaying())
    {
        playing = !playing;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    /* The update method is called muliple times per second
    It's in charge of drawing all figures and text on screen */

    if (backgroundPhoto)
    {
        backgroundPhoto = true;
        ofSetColor(255);
        ofFill();
        newBackgroundPhoto.draw(0, 0, ofGetWidth(), ofGetHeight());
    }

    // Progress Bar
    xPos1 = 0;
    YPos1 = ofGetHeight();
    Width1 = ofGetWidth() * progress;
    Height1 = -50;
    songProgressBar = ofRectangle(xPos1, YPos1, Width1, Height1);
    ofSetColor(255);
    ofFill();
    ofDrawRectangle(songProgressBar);

    string Smode;
    if (loopy)
    {
        Smode = "Song Repeat";
    }
    else if (!loopy && playL)
    {
        Smode = "Playlist Loop";
    }
    else
    {
        Smode = "None";
    }

    // Display Info
    ofSetColor(0, 0, 0, 127);
    ofDrawRectangle(0, 0, 350, 80);
    ofSetColor(255);
    float pos = playing ? progress : lastPos;
    int percent = pos * 100;
    ofDrawBitmapString("Song Progress: " + ofToString(percent) + "%", 0, 30);
    ofDrawBitmapString("Current Song: " + songs[selectedSong], 0, 45);
    ofDrawBitmapString("Volume: " + to_string(int(vol * 100)) + '%', 0, 60);
    ofDrawBitmapString("Current Mode: " + ofToString(Smode), 0, 75);

    // Command Menu display
    if (!commandList)
    {
        ofSetColor(0, 0, 0, 127);
        ofDrawRectangle(ofGetWidth(), 0, 235 * -1, 20);
        ofSetColor(255);
        ofDrawBitmapString("Press M to see key commands", ofGetWidth() - 222, 15);
    }
    if (commandList)
    {
        ofSetColor(0, 0, 0, 127);
        ofDrawRectangle(ofGetWidth(), 0, 235 * -1, 160);
        ofSetColor(255);
        ofDrawBitmapString("Press M to hide key commands", ofGetWidth() - 230, 15);
        ofDrawBitmapString("'p' - Plays/Stops", ofGetWidth() - 210, 30);
        ofDrawBitmapString("'a' - Pauses", ofGetWidth() - 210, 45);
        ofDrawBitmapString("'d' - Plays next song", ofGetWidth() - 210, 60);
        ofDrawBitmapString("'r' - Loops song", ofGetWidth() - 210, 75);
        ofDrawBitmapString("'l' - Loops PLaylist", ofGetWidth() - 210, 90);
        ofDrawBitmapString("'b' - Shuffles song", ofGetWidth() - 210, 105);
        ofDrawBitmapString("'i' - Toggle background", ofGetWidth() - 210, 120);
        ofDrawBitmapString("'=' - Increases volume", ofGetWidth() - 210, 135);
        ofDrawBitmapString("'-' - Decreases volume", ofGetWidth() - 210, 150);
    }

    // Mode Selection
    if (!playing)
    {
        ofDrawBitmapString("Press 'p' to play some music!", ofGetWidth() / 2 - 50, ofGetHeight() / 2);
    }
    if (pause)
    {
        ofDrawBitmapString("Paused", ofGetWidth() / 2 - 50, ofGetHeight() / 2);
    }
    vector<float> amplitudes = visualizer.getAmplitudes();
    if (mode == '1')
    {
        drawMode1(amplitudes);
    }
    else if (mode == '2')
    {
        drawMode2(amplitudes);
    }
    else if (mode == '3')
    {
        drawMode3(amplitudes);
    }

    // ofDrawBitmapString("Current Mouse Position: " + ofToString(cur_x) + ", " + ofToString(cur_y), 0, 30);
}
void ofApp::drawMode1(vector<float> amplitudes)
{
    ofFill();        // Drawn Shapes will be filled in with color
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Rectangle Height Visualizer", 0, 15);
    if (!pause)
    {
        lastColorRect = ofRandom(0, 256);
        ofSetColor(0, 0, lastColorRect);
        for (int i = 0; i <= amplitudes.size(); i++)
        {
            ofDrawRectRounded((ofGetWidth() / 64) * (i), ofGetHeight() - 100, (ofGetWidth() / 64), amplitudes[i], 10);
        }
    }
    else
    {
        ofSetColor(0, 0, lastColorRect);
        for (int i = 0; i <= lastAmp.size(); i++)
        {
            ofDrawRectRounded((ofGetWidth() / 64) * (i), ofGetHeight() - 100, (ofGetWidth() / 64), lastAmp[i], 10);
        }
    }
}
void ofApp::drawMode2(vector<float> amplitudes)
{
    ofSetLineWidth(5); // Sets the line width
    ofNoFill();        // Only the outline of shapes will be drawn
    ofSetColor(256);   // This resets the color of the "brush" to white
    ofDrawBitmapString("Circle Radius Visualizer", 0, 15);
    int bands = amplitudes.size();
    for (int i = 0; i <= bands; i++)
    {
        ofSetColor((bands - i) * 32 % 256, 18, 144); // Color varies between frequencies
        if (!pause)
        {
            ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, amplitudes[0] / (i + 1));
        }
        else
        {
            ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, lastAmp[0] / (i + 1));
        }
    }
}

void ofApp::drawMode3(vector<float> amplitudes)
{
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Lined Visualizer", 0, 15);
    ofSetColor(lastColorRect);
    for (int i = 1; i <= amplitudes.size(); i++)
    {
        ofSetLineWidth(100);
        if (!pause)
        {
            lastColorRect = ofRandom(210, 256);
            ofDrawLine((ofGetWidth() / 64) * (i - 1), (ofGetHeight() - 100 + amplitudes[i - 1]),
                       (ofGetWidth() / 64) * i, (ofGetHeight() - 100 + amplitudes[i]));
        }
        else
        {
            ofDrawLine((ofGetWidth() / 64) * (i - 1), (ofGetHeight() - 100 + lastAmp[i - 1]),
                       (ofGetWidth() / 64) * i, (ofGetHeight() - 100 + lastAmp[i]));
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // This method is called automatically when any key is pressed
    switch (key)
    {
    // Start / End Song Button
    case 'p':
        if (playing)
        {
            sound.stop();
            pause = false;
        }
        else
        {
            sound.play();
        }
        playing = !playing;
        break;
    // Pause button
    case 'a':
        if (playing)
        {
            pause = !pause;
            sound.setPaused(pause);
            lastAmp = visualizer.getAmplitudes();
        }
        break;
    // Next Button
    case 'd':
        sound.unload();
        selectedSong = (selectedSong < songs.size() - 1) ? ++selectedSong : 0;
        sound.load(songs[selectedSong]);
        sound.setLoop(loopy);
        if (playing)
        {
            sound.play();
        }
        pause = false;
        break;
    // Song loop
    case 'r':
        loopy = !loopy;
        sound.setLoop(loopy);
        break;
    // Playlist Loop
    case 'l':
        playL = !playL;
        break;
    // Lower Volume
    case '-':
        vol = (vol > 0.1) ? vol - 0.1 : 0;
        sound.setVolume(vol);
        break;
    // Increase Volume
    case '=':
        vol = (vol < 1) ? vol + 0.1 : 1;
        sound.setVolume(vol);
        break;
    // Set Background
    case 'i':
        backgroundPhoto = !backgroundPhoto;
        break;
    // Song Shuffle
    case 'b':
        sound.unload();
        selectedSong = ofRandom(0, songs.size());
        sound.load(songs[selectedSong]);
        sound.setLoop(loopy);
        if (playing)
        {
            sound.play();
        }
        pause = false;
        break;
    case 'm':
        commandList = !commandList;
        break;
    case '1':
        ofSetBackgroundColor(255, 170, 80);
        mode = '1';
        break;
    case '2':
        ofSetBackgroundColor(180, 120, 150);
        mode = '2';
        break;
    case '3':
        ofSetBackgroundColor(40, 230, 160);
        mode = '3';
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    cur_x = x;
    cur_y = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    if (songProgressBar.inside(x, y))
    {
        mouseDrag = true;
        progress = float(x) / float(ofGetWidth());
        sound.setPosition(progress);
        sound.setPaused(true);
    }
}

// --------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if (mouseDrag)
    {
        progress = float(x) / float(ofGetWidth());
        sound.setPosition(progress);
    }
}

// --------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    if (mouseDrag)
    {
        mouseDrag = false;
        sound.play();
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
//--------------------------------------------------------------