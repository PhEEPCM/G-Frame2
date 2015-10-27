#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup()
{
    
    myfont.loadFont(OF_TTF_SANS, 32);
    mybiggerfont.loadFont(OF_TTF_SANS, 33);
    
    searchCriteria = "funny+cat";
    getImageForText(searchCriteria);
    
    lastUpdate = ofGetElapsedTimef();
    
    screenHeight = 1080;//ofGetScreenHeight();
    screenWidth = 1920;//ofGetScreenWidth();

    timer.setup( 3000 );
    timer.start( true ) ;
    
//    randomizerTimer.setup( 15000 );
//    randomizerTimer.start( true ) ;
    
    ofAddListener( timer.TIMER_COMPLETE , this, &ofApp::timerCompleteHandler ) ;
    //ofAddListener( randomizerTimer.TIMER_COMPLETE , this, &ofApp::randomizerTimerCompleteHandler ) ;

    
    gifPlayer.setLoopState(OF_LOOP_NORMAL);
    gifPlayer.setUseTexture(true);
    
    //string actionUrl = "https://speech.platform.bing.com/recognize";
    
    //stt.recordAndConvert();
}

void ofApp::getImageForText (string query)
{
    string url = "http://api.giphy.com/v1/gifs/search?q=";
    
    url += query;
    
    url += "&api_key=dc6zaTOxFJmzC";

    ofxJSONElement response;
    
    if (!response.open(url))
    {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
    }

    int numImages = response["data"].size();
    
    //string jsonresponse = response["data"];
    
    if (numImages > 0)
    {
        int selection = ofRandom(0, numImages-1);
        
        //string imageUrl = response["data"][selection]["images"]["original"]["mp4"].asString();
        string imageUrl = response["data"][selection]["images"]["original"]["url"].asString();
        
        if (imageUrl.empty())
            return;
        
        gifPlayer.closeMovie();
        
        gifPlayer.setUseTexture(true);
        gifPlayer.loadMovie(imageUrl);
        
        gifPlayer.play();
        
        
        
    }
    //stt.recordAndConvert();
}

//--------------------------------------------------------------
void ofApp::update()
{
    
    gifPlayer.update();
    
    timer.update( );
    
    randomizerTimer.update();
    
}

/*void ofApp::bingResponseHandler(ofxHttpResponse & response)
{
    responseMessage = ofToString(response.status) + ": " + (string)response.responseBody;
    
    int i=0;
}*/

//--------------------------------------------------------------
void ofApp::draw()
{
    int strwdth = myfont.stringWidth(searchCriteria);
    gifPlayer.draw(0, 0, screenWidth, screenHeight);
    ofSetColor(44, 44, 44);
    ofDrawPlane(100+(strwdth/2), 90, strwdth+40, myfont.stringHeight(searchCriteria)+20);
    ofSetColor(255, 255, 255);
    myfont.drawString(searchCriteria, 100, 100);
    
    
}

//void ofApp::randomizerTimerCompleteHandler( int &args )
//{
//    getImageForText(searchCriteria);
//}

void ofApp::timerCompleteHandler( int &args )
{
    ofLogNotice("Starting Record and Convert");

    ifstream fin;
    
    ofFile file;
    
    file.open("stt/stt.txt");
    
    if (!file.exists()) {
        ofLogNotice("FILE DOESN'T EXIST");
        return;
    }
    
    
    ofBuffer buffer = file.readToBuffer();
    
    searchCriteria = buffer.getFirstLine();
        
    ofStringReplace(searchCriteria, " ", "+");
    
    getImageForText(searchCriteria);
    
//    file.close();
    
    //file.remove();
    
}

void ofApp::sttCompleteHandler( int &args )
{
    ifstream fin;
    
    ofLogNotice("OPENING STT");
    
    fin.open( ofToDataPath("stt.txt").c_str() );
    
    //recognizedWords.clear();
    
    while(fin!=NULL)
    {
        string str;
        getline(fin, str);
        
        //recognizedWords.push_back(str);
        
        remove(ofToDataPath("stt.txt").c_str());
    }
    
    //stt.recordAndConvert();
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    PostRequest request ("https://speech.platform.bing.com/recognize/query", "HTTP/1.1");
    
    BaseResponse response;

    SessionSettings settings;
 
    request.set ("version", "3.0");
    request.set("requestid", "todo");
    request.set("appID", "aec8856c-cf33-4502-9ed4-c9b0e1c1c1e4");
    request.set("format", "json");
    request.set("locale", "en-US");
    request.set("device.os", "raspberry pi");
    request.set("scenarios", "ulm");
    request.set("instanceid", "13231648-9e71-4348-a3b3-e1807781efde");
    
    ofFile audioFile;
    
    audioFile.open("test.wav");
    
    ofBuffer bodyBuffer = audioFile.readToBuffer();
    
    request.addFormBuffer("audio", bodyBuffer);
    
    audioFile.close();
    
    request.setContentType("audio/wav; samplerate=8000");
    
    try
    {
        // Execute the request and get the response stream.
        std::istream& responseStream = sttClient.execute(request,
                                                      response,
                                                      sttContext);
        
        // Request and response headers can be examined here.
        
        
        std::cout << "============" << endl;
        // Copy the output to the terminal.
        Poco::StreamCopier::copyStream(responseStream, std::cout);
        
        // Flush the input stream.
        std::cout << std::endl;
        
        std::cout << "============" << endl;
        
        
    }
    catch(const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
    }
    catch(...)
    {
        ofLogError("ofApp::setup") << "Got unknown exception.";
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
