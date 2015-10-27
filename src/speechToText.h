#pragma once
#include "ofThread.h"

class speechToText : public ofThread
{
    
public:
    
    ofEvent<int> COMMAND_COMPLETE;
    
    void recordAndConvert(){
        stopThread();
        startThread( false );
    }
    
    // CALL THIS DIRECTLY FOR BLOCKING COMMAND
//    std::string exec(char* cmd) {
//        FILE* pipe = popen(cmd, "r");
//        if (!pipe) return "ERROR";
//        char buffer[128];
//        std::string result = "";
//        while(!feof(pipe)) {
//            if(fgets(buffer, 128, pipe) != NULL)
//                result += buffer;
//        }
//        pclose(pipe);
//        return result;
//    }
    
private:
    void threadedFunction(){
        ofLogWarning("recording");
        string command = string("node speech.js");
        //string command = string("rec -c 1 test.wav trim 0 00:05");
        //string command = string("ls");
        //ofLogNotice("COMMAND:  " + command);
        string result = ofSystem("node speech.js");
      
        ofLogNotice("RESULT:  " + result);
        
        string sttLoc = ofToDataPath("stt.txt").c_str();
        
        int args = 18 ;
        ofNotifyEvent( COMMAND_COMPLETE, args ) ;
        
        stopThread();
        
    }
    
    string cmd;    
};
