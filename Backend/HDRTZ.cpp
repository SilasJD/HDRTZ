#include <SDL.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <jsoncpp/json/json.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>		//UNIX standard function definitions
#include <fcntl.h>		//File control definitions
#include <errno.h>		//Error number definitions
#include <termios.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <fstream>
using namespace cv;
using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int fps = 120;

SDL_Texture * TexFromCV(const Mat& mat, SDL_Renderer* renderer);

int main(int argc, char* args[]){


  int USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );

  
  
  struct termios tty;
  struct termios tty_old;
  memset (&tty, 0, sizeof tty);
  
  /* Error Handling */
  if ( tcgetattr ( USB, &tty ) != 0 ) {
    std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
  }
  
  /* Save old tty parameters */
  tty_old = tty;
  
  /* Set Baud Rate */
  cfsetospeed (&tty, (speed_t)B9600);
  cfsetispeed (&tty, (speed_t)B9600);
  
  /* Setting other Port Stuff */
  tty.c_cflag     &=  ~PARENB;            // Make 8n1
  tty.c_cflag     &=  ~CSTOPB;
  tty.c_cflag     &=  ~CSIZE;
  tty.c_cflag     |=  CS8;
  
  tty.c_cflag     &=  ~CRTSCTS;           // no flow control
  tty.c_cc[VMIN]   =  1;                  // read doesn't block
  tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
  tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
  
  /* Make raw */
  cfmakeraw(&tty);
  
  /* Flush Port, then applies attributes */
  tcflush( USB, TCIFLUSH );
  if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
    std::cout << "Error " << errno << " from tcsetattr" << std::endl;
  }
  
  int n = 0,
      spot = 0;
  char buf = '\0';

/* Whole response*/
 char response[1024];
 memset(response, '\0', sizeof response);

  
  int lf_count = 0;
  float MAX_INTERVAL = 15;
  float audio = 0;
  float interval = .5;
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  SDL_Renderer* renderer = NULL;
  if(SDL_Init(SDL_INIT_VIDEO) < 0) printf("SDL_ERROR: %s\n", SDL_GetError());
  window = SDL_CreateWindow("HDRTZ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT,  SDL_WINDOW_FULLSCREEN_DESKTOP);

  if(window == NULL) printf("Window not created! SDL_ERROR: %s\n", SDL_GetError());
  else{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) printf("Surface not created! SDL_ERROR: %s\n", SDL_GetError());
    else{
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
  }
  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags)) printf("SDL Image not initialized");
  SDL_Surface* maskSurf = IMG_Load("test.png");
  SDL_Texture* maskTex  = SDL_CreateTextureFromSurface(renderer, maskSurf);
  SDL_Surface* xSurf    = IMG_Load("xhair.png");
  SDL_Texture* xTex     = SDL_CreateTextureFromSurface(renderer, xSurf);

  // VideoCapture cap("nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)NV12, framerate=(fraction)120/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"); 
  VideoCapture cap;
  cap.open(1);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
  cap.set(CV_CAP_PROP_FPS, 60);
  cout << "setting fps supported" << endl;
  if(!cap.isOpened()){
    cout << "error opening stream" << endl;
    return -1;
  }


  SDL_Point p;
  double angle = 0;
  SDL_Event e;
  bool quit = false;

 
  clock_t startTime, currentTime;
  Uint32 start_tick;

  

  while(!quit){
    
     std::ifstream coords("/home/nvidia/HDRTZ/api/output.json", std::ifstream::binary);
    
    ifstream ifs("/home/nvidia/HDRTZ/api/output.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    int crosshair;
    int mask;
    
    SDL_Rect videostream;
    videostream.w = 1920;
    videostream.h = 1080;
    videostream.x =-1 * (obj["x"].asInt());
    p.x = 960+obj["x"].asInt();
    videostream.y =-1 * (obj["y"].asInt());
    p.y = 540+obj["y"].asInt();
    crosshair = obj["crosshair"].asInt();
    mask = obj["mask"].asInt();

    
    
    while( SDL_PollEvent( &e ) != 0 )
        {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {	
                        quit = true;
                    }
		    else if( e.type == SDL_KEYDOWN){
		      switch(e.key.keysym.sym)
			{
			case SDLK_RIGHT:
			  cout << "Hello" << endl;
			  interval -= .5;

			
			  
			  break;
	  
			case SDLK_LEFT:
			  interval += .5;
			  break;
			case SDLK_UP:
			  std::cout << interval << std::endl;
			  break;
			case SDLK_DOWN:
			  quit = true;
			  return 0;
		        default:
			  break;
			}
		    }
		    }   
		    

		
     do{
       n = read( USB, &buf, 1 );
       sprintf( &response[spot], "%c", buf );
       spot += n;
      } while( buf != '\r' && n > 0);
    
    tcflush( USB, TCIFLUSH );
    
    if (n < 0) {
      std::cout << "Error reading: " << strerror(errno) << std::endl;
      //interval = 0;
    }
    else if (n == 0) {
      std::cout << "Read nothing!" << std::endl;
    }

    
    // convert response into interval
    
    /*
     ifstream myfile;
     myfile.open("serial_data");
     int raw_crank_data;
     if (myfile.is_open()){
       while(!myfile.eof()){
	 myfile >> response;
       }
     }
     myfile.close();

    */
    int raw_crank_data;
    raw_crank_data = atoi(response);
    
    int idleLow  = 91,
        idleHigh = 94,
        minVal   = 70,
        maxVal   = 115;




    if (n > 0){
      if(raw_crank_data >= idleLow && raw_crank_data <= idleHigh){
	interval = 0;
      }      
      else if (raw_crank_data > maxVal){
	interval = MAX_INTERVAL;
      }
      else if(raw_crank_data < minVal){
	interval = MAX_INTERVAL * -1;
      }
      else if( raw_crank_data < idleLow){
	interval = (raw_crank_data - idleLow) * (MAX_INTERVAL / (idleLow - minVal));
	
   
      }      
      else if(raw_crank_data > idleHigh){
	interval = (raw_crank_data - idleHigh) * (MAX_INTERVAL / (maxVal - idleHigh));

      }
    }

    
    std::cout << "rawCrankVal = " << raw_crank_data << " interval = " <<interval <<"\n";
    
    
    
    
    spot = 0;
    Mat frame;
    SDL_Texture* tex = NULL;
    
    
    cap >> frame;
    waitKey(1);
    tex = TexFromCV(frame, renderer);
    if(tex != NULL){
    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer, tex, NULL, &videostream, angle, &p, SDL_FLIP_NONE);
    cout << "mask: " << mask << " crosshair: " << crosshair << endl;
    if(mask == 1){
      SDL_RenderCopy(renderer, maskTex, NULL, NULL);
    }
    if(crosshair == 1){
      SDL_RenderCopy(renderer, xTex, NULL, NULL);
    }
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(tex);
    angle += interval;

    audio = interval / (MAX_INTERVAL * 2);
    ofstream outputFile("/tmp/pitch");
    //...
    outputFile << to_string(audio);
    outputFile.close();
    }
    if((1000/fps) > SDL_GetTicks() - start_tick){
	SDL_Delay((1000/fps)-(SDL_GetTicks()-start_tick));
      }
      else if((1000/fps) < SDL_GetTicks() - start_tick){
		  lf_count++;
	  }
    
  }
}


SDL_Texture* TexFromCV(const Mat &mat, SDL_Renderer* renderer){

  IplImage cvFrame = (IplImage)mat;
  IplImage* cvFramePtr = &cvFrame;
  

  SDL_Surface* frameData = SDL_CreateRGBSurfaceFrom(
						    (void*)cvFramePtr -> imageData,
						    cvFramePtr->width, cvFramePtr->height,
						    cvFramePtr->depth * cvFramePtr->nChannels,
						    cvFramePtr->widthStep,
						    0xff0000, 0x00ff00, 0x0000ff, 0);



  if(frameData == NULL){
    printf("Failed to convert Mat to Surface");
    return NULL;
  }

  

  SDL_Texture* frameTex = NULL;
  frameTex = SDL_CreateTextureFromSurface(renderer, frameData);


  
  //  SDL_FreeSurface(frameData);
  if(frameTex == NULL) return NULL;
  else {
    SDL_FreeSurface(frameData);

    //    cvReleaseImage(&cvFramePtr);
    return frameTex;
  }
}
