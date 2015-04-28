//
//  ofApp.cpp
//  Frequency by Andreas Müller
//
//  Comments by Glenna Xie on 4/28/15.
//
//

#include <ofApp.h>

void ofApp::setup(){
    
    //load a new font with a font size of 8
    fontSmall.loadFont("Fonts/DIN.otf", 8 );
    
    //turn on alpha blending
    ofEnableAlphaBlending();

}

void ofApp::draw(){
    //render a circular gradient background of color black and gray
    ofBackgroundGradient( ofColor(55), ofColor(0), OF_GRADIENT_CIRCULAR );
    
    // As we are feeding values measured in pixels, the frequency value will be pretty small
    
    //read the current x value of mouse location and map it to the value of frequency, which is between .0001 and .05
    float frequency = ofMap( ofGetMouseX(),  0, ofGetWidth(),  0.0001, 0.05 );
    
    
    // Draw ticks at each unit edge based on the frequency
    //the ceil method rounds up to the smallest integral value
    // ceil of 2.3 is 3.0
    // ceil of 3.8 is 4.0
    // ceil of -2.3 is -2.0
    // ceil of -3.8 is -3.0
    
    int numTicks = ceil(ofGetWidth() * frequency);
    
    //store the numTicks integer to a float
    float areaCovered = numTicks;
    
    //when there are less or equal to 5 ticks, numTicks = numTicks * 10;
    if( numTicks <= 5 ) { numTicks *= 10; }
    
    
    for( int i = 0; i < numTicks; i++ )
    {
        //loop through each numTicks
        //and get noisePos which is a value mapped from numTicks to areaCovered
        float noisePos = ofMap( i, 0, numTicks, 0, areaCovered);
        
        //if frequancy = .0001, which means mouseX is on the very left of the screen
        // numTicks = ceil(1024 * .0001) = 1;
        // because numTicks < 5, numTicks = 10;
        // areaCovered = 1.0;
        // noisePos = .1;
        // screenX = .1 * (1/0.0001) = 1000;
        // therefore one tick is drawn at x position of 1000
        float screenX = noisePos * (1.0/frequency);
        
        //create the two ofVec for the ticks
        ofVec2f top( screenX, 0);
        ofVec2f bot( screenX, ofGetHeight());
        
        // Brighter white lines at unit edges (0,1,2) and less bright at fractionals (0.1,0.2, etc), but only if we are zoomed in enough
        
        //the fmodf() method returns the remainder(float) of dividing the arguments
        //fmodf of 5.3 / 2 is 1.300000
        if( (abs(fmodf(noisePos, 1.0)) <= 0) && areaCovered < 18 )  {
            ofSetColor( ofColor::white, 90 );
        } else {
            ofSetColor( ofColor::white, 30 );
        }
        //draw the ticks
        ofLine( top, bot);
        
        //draw the text that indicates the noisePos
        ofVec2f textPos = top + ofVec2f(4,20);
        fontSmall.drawString( ofToString(noisePos), textPos.x, textPos.y );
    }
    
    // Compute a noise position for each pixels x position, taking a frequency value into account
    // the noiseMagnitude is basically the peak amplitude of the wave
    float noiseMagnitude = 300;
    
    // create a mesh
    ofMesh mesh;
    mesh.setMode( OF_PRIMITIVE_LINE_STRIP );
    
    int res = ofGetWidth();
    for( int i = 0; i < res; i++ )
    {
        //loop through each pixels x position
        //y position is the middle of the window height
        ofVec2f pos = ofVec2f( ofMap( i, 0, res, 0, ofGetWidth() ), ofGetHeight() * 0.5f );
        
        //calculates the noisePos based on the frequency
        //if pos.x = 1024, frequency  = .05
        //noisePos = 51.2
        //the higher the frequency is, the more the wave oscillates
        float noisePos = (pos.x * frequency);
        
        // ofSignedNoise gives us a value between -1 and 1
        float noiseValue = ofSignedNoise( noisePos );
        
        //the drawPos ofvec is created based on the caculated noiseValue, which is normalized between -1 and 1, multiplies noiseMagnitude, which is the peak amplitude of the wave
        ofVec2f drawPos = pos + ofVec2f(0,noiseValue * noiseMagnitude);
        
        //add the drawPos the mesh
        mesh.addVertex( drawPos );
    }
    
    ofSetColor( ofColor::white );
    mesh.draw();

}