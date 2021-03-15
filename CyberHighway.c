 #define SDL_MAIN_HANDLED
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>
 #include <stdbool.h>
 #include <string.h>
 #include <ctype.h>
 #include <time.h>
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_timer.h>
 #include <SDL2/SDL_image.h>
 #include <SDL2/SDL_ttf.h>
 #include <SDL2/SDL_mixer.h>

 #define WINDOW_WIDTH (640)
 #define WINDOW_HEIGHT (480)

 #define SPEED (300)

 #define NOW (30)



 

 bool has_collided(SDL_Rect Obj_A, SDL_Rect Obj_B)
 {
   int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;
   leftA = Obj_A.x;
   rightA = Obj_A.x + Obj_A.w;
   topA = Obj_A.y;
   bottomA = Obj_A.y + Obj_A.h;

   leftB = Obj_B.x;
   rightB = Obj_B.x + Obj_B.w;
   topB = Obj_B.y;
   bottomB = Obj_B.y + Obj_B.h;

   if(bottomA <= topB)
   {
     return false;
   }
   if(topA >= bottomB)
   {
     return false;
   }
   if(rightA <= leftB)
   {
     return false;
   }
   if (leftA >= rightB)
   {
     return false;
   }

   return true;

 }

 void get_and_update_hud(int number, SDL_Renderer* rend, TTF_Font* font, SDL_Color Colour, int type, int xpos, int ypos, int width, int height)
 {
    

    char str[255];
    
    switch (type)
    {
      case 1:
        snprintf(str, 255, "Score: %d", number);
      break;
      case 2:
        snprintf(str, 255, "Health: %d", number);
      break;
      case 3:
        snprintf(str, 255, "Double Bullets: %d", number);
      break;
      case 4:
        snprintf(str, 255, "Rocket: %d", number);
      break;
      case 5:
        snprintf(str, 255, "Highscore: %d", number);
      break;

    }
    


    SDL_Surface* surfHud = TTF_RenderText_Solid(font, str, Colour);

    SDL_Texture* texHud = SDL_CreateTextureFromSurface(rend, surfHud);

    SDL_Rect destHud;

    destHud.w = width;
    destHud.h = height;

    destHud.x = xpos;
    destHud.y = ypos;

    SDL_RenderCopy(rend, texHud, NULL, &destHud);
    SDL_FreeSurface(surfHud);
    SDL_DestroyTexture(texHud);
    
  
 }

 int main(int argc, char *argv[])
 {
  
   
   //initialize
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
      printf("Error initiatiating sdl: %s\n", SDL_GetError());
      return 1;
    }
    //create window
    SDL_Window* win = SDL_CreateWindow("CyberHighway", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    

    if(!win)
    {
      printf("error creating window: %s\n", SDL_GetError());
    }

    //create seed for rand
    srand(time(0));

    //create render
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if(!rend)
    {
      printf("Error creating render: %s\n", SDL_GetError());
      SDL_DestroyWindow(win);
      SDL_Quit();
      return 1;
    }
    SDL_RenderSetLogicalSize(rend, 640, 480);

    //Initalize fonts
    TTF_Init();

    //set up music
    Mix_Music *bgMusic = NULL;

    //set up sound effects
    Mix_Chunk *sndBulletCollide = NULL;
    Mix_Chunk *sndPlayerBullet = NULL;
    Mix_Chunk *sndDoubleBullet = NULL;
    Mix_Chunk *sndDrone = NULL;
    Mix_Chunk *sndPoliceShoot = NULL;
    Mix_Chunk *sndPlayerRocketCollide = NULL;
    Mix_Chunk *sndPlayerRocketShoot = NULL;
    Mix_Chunk *sndEnemyRocketShoot = NULL;
    Mix_Chunk *sndEnemyRocketCollide = NULL;
    Mix_Chunk *sndPlayerHit = NULL;
    Mix_Chunk *sndPlayerCrash = NULL;
    Mix_Chunk *sndVanCrash = NULL;
    Mix_Chunk *sndPoliceCrash = NULL;
    Mix_Chunk *sndDroneCrash = NULL;
    Mix_Chunk *sndPickup = NULL;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
      printf("Error creating Music: %s\n", Mix_GetError());
      SDL_DestroyRenderer(rend);
      SDL_DestroyWindow (win);
      SDL_Quit();
      Mix_Quit();
      return 1;
    }
    //set up font
    TTF_Font* font = TTF_OpenFont("Content/PressStart2P-Regular.ttf", 24);

    //set up colour for fonts
    SDL_Color White = {255, 255, 255};
    SDL_Color Red = {255, 0, 0};
    SDL_Color Green = {0, 255, 0};

    //create strings for Options Menu
    char OnStr[3];
    snprintf(OnStr, 3, "On");

    char OffStr[4];
    snprintf(OffStr, 4, "Off");

    char screenStr[14];
    snprintf(screenStr, 14, "[F]ullscreen:");

    char musicStr[9];
    snprintf(musicStr, 9, "[M]usic:");

    char audioStr[9];
    snprintf(audioStr, 9, "[A]udio:");

    //version string
    char version[13];
    snprintf(version, 13, "Version: 1.1");

    
    
    

    //load images into memory
    SDL_Surface* surfCar = IMG_Load("Content/Textures/carDemo.png");
    SDL_Surface* surfCarWreck = IMG_Load("Content/Textures/carWrecked.png");
    SDL_Surface* surfWall = IMG_Load("Content/Textures/Wall.png");
    SDL_Surface* surfRoad = IMG_Load("Content/Textures/road.png");
    SDL_Surface* surfRoadLine = IMG_Load("Content/Textures/roadLine.png");
    SDL_Surface* surfBlock = IMG_Load("Content/Textures/block.png");
    SDL_Surface* surfTruck = IMG_Load("Content/Textures/truck.png");
    SDL_Surface* surfTruckFire = IMG_Load("Content/Textures/truckFire.png");
    SDL_Surface* surfRocket = IMG_Load("Content/Textures/rocket.png");
    SDL_Surface* surfBullet = IMG_Load("Content/Textures/bullet.png");
    SDL_Surface* surfPowerup = IMG_Load("Content/Textures/powerup.png");
    SDL_Surface* surfDoubleBullet = IMG_Load("Content/Textures/doubleBullet.png");
    SDL_Surface* surfRocket2 = IMG_Load("Content/Textures/rocket2.png");
    SDL_Surface* surfStreet = IMG_Load("Content/Textures/street.png");
    SDL_Surface* surfStreet2 = IMG_Load("Content/Textures/street2.png");
    SDL_Surface* surfStreet3 = IMG_Load("Content/Textures/street3.png");
    SDL_Surface* surfStreet4 = IMG_Load("Content/Textures/street4.png");
    SDL_Surface* surfStreet5 = IMG_Load("Content/Textures/street5.png");
    SDL_Surface* surfPolice = IMG_Load("Content/Textures/carPolice.png");
    SDL_Surface* surfPoliceShoot = IMG_Load("Content/Textures/carPoliceShoot.png");
    SDL_Surface* surfPoliceBullet = IMG_Load("Content/Textures/policeBullet.png");
    SDL_Surface* surfDrone = IMG_Load("Content/Textures/drone.png");
    SDL_Surface* surfDroneShoot = IMG_Load("Content/Textures/droneShoot.png");
    SDL_Surface* surfDroneBullet = IMG_Load("Content/Textures/droneBullet.png");
    SDL_Surface* surfTruckWreck = IMG_Load("Content/Textures/truckWrecked.png");
    SDL_Surface* surfPoliceWreck = IMG_Load("Content/Textures/carPoliceWrecked.png");
    SDL_Surface* surfDroneWreck = IMG_Load("Content/Textures/droneWrecked.png");
    SDL_Surface* surfHit = IMG_Load("Content/Textures/hit.png");
    SDL_Surface* surfExplode = IMG_Load("Content/Textures/explode.png");
    SDL_Surface* surfOverlay = IMG_Load("Content/Textures/overlay.png");
    SDL_Surface* surfPause = IMG_Load("Content/Textures/Pause.png");
    SDL_Surface* surfGameOver = IMG_Load("Content/Textures/GameOver.png");
    SDL_Surface* surfMenu = IMG_Load("Content/Textures/menu.png");
    SDL_Surface* surfOptions = IMG_Load("Content/Textures/Options.png");
    SDL_Surface* surfControls = IMG_Load("Content/Textures/controls.png");
    SDL_Surface* surfFullscreen = TTF_RenderText_Solid(font, screenStr, Red);
    SDL_Surface* surfMusic = TTF_RenderText_Solid(font, musicStr, Red);
    SDL_Surface* surfAudio = TTF_RenderText_Solid(font, audioStr, Red);
    SDL_Surface* surfOnText = TTF_RenderText_Solid(font, OnStr, Red);
    SDL_Surface* surfOffText = TTF_RenderText_Solid(font, OffStr, Red);
    SDL_Surface* surfVersion = TTF_RenderText_Solid(font, version, Red);
    

    

    //load image into gpu memory
    SDL_Texture* texCar = SDL_CreateTextureFromSurface(rend, surfCar);
    SDL_Texture* texCarWreck = SDL_CreateTextureFromSurface(rend, surfCarWreck);
    SDL_Texture* texWall = SDL_CreateTextureFromSurface(rend, surfWall);
    SDL_Texture* texRoad = SDL_CreateTextureFromSurface(rend, surfRoad);
    SDL_Texture* texRoadLine = SDL_CreateTextureFromSurface(rend, surfRoadLine);
    SDL_Texture* texBlock = SDL_CreateTextureFromSurface(rend, surfBlock);
    SDL_Texture* texTruck = SDL_CreateTextureFromSurface(rend, surfTruck);
    SDL_Texture* texTruckFire = SDL_CreateTextureFromSurface(rend, surfTruckFire);
    SDL_Texture* texRocket = SDL_CreateTextureFromSurface(rend, surfRocket);
    SDL_Texture* texBullet = SDL_CreateTextureFromSurface(rend, surfBullet);
    SDL_Texture* texPowerup = SDL_CreateTextureFromSurface(rend, surfPowerup);
    SDL_Texture* texDoubleBullet = SDL_CreateTextureFromSurface(rend, surfDoubleBullet);
    SDL_Texture* texRocket2 = SDL_CreateTextureFromSurface(rend, surfRocket2);
    SDL_Texture* texStreet = SDL_CreateTextureFromSurface(rend, surfStreet);
    SDL_Texture* texStreet2 = SDL_CreateTextureFromSurface(rend, surfStreet2);
    SDL_Texture* texStreet3 = SDL_CreateTextureFromSurface(rend, surfStreet3);
    SDL_Texture* texStreet4 = SDL_CreateTextureFromSurface(rend, surfStreet4);
    SDL_Texture* texStreet5 = SDL_CreateTextureFromSurface(rend, surfStreet5);
    SDL_Texture* texPolice = SDL_CreateTextureFromSurface(rend, surfPolice);
    SDL_Texture* texPoliceShoot = SDL_CreateTextureFromSurface(rend, surfPoliceShoot);
    SDL_Texture* texPoliceBullet = SDL_CreateTextureFromSurface(rend, surfPoliceBullet);
    SDL_Texture* texDrone = SDL_CreateTextureFromSurface(rend, surfDrone);
    SDL_Texture* texDroneShoot = SDL_CreateTextureFromSurface(rend, surfDroneShoot);
    SDL_Texture* texDroneBullet = SDL_CreateTextureFromSurface(rend, surfDroneBullet);
    SDL_Texture* texTruckWreck = SDL_CreateTextureFromSurface(rend, surfTruckWreck);
    SDL_Texture* texPoliceWreck = SDL_CreateTextureFromSurface(rend, surfPoliceWreck);
    SDL_Texture* texDroneWreck = SDL_CreateTextureFromSurface(rend, surfDroneWreck);
    SDL_Texture* texHit = SDL_CreateTextureFromSurface(rend, surfHit);
    SDL_Texture* texExplode = SDL_CreateTextureFromSurface(rend, surfExplode);
    SDL_Texture* texOverlay = SDL_CreateTextureFromSurface(rend, surfOverlay);
    SDL_Texture* texPause = SDL_CreateTextureFromSurface(rend, surfPause);
    SDL_Texture* texGameOver = SDL_CreateTextureFromSurface(rend, surfGameOver);
    SDL_Texture* texMenu = SDL_CreateTextureFromSurface(rend, surfMenu);
    SDL_Texture* texOptions = SDL_CreateTextureFromSurface(rend, surfOptions);
    SDL_Texture* texControls = SDL_CreateTextureFromSurface(rend, surfControls);
    SDL_Texture* texFullscreen = SDL_CreateTextureFromSurface(rend, surfFullscreen);
    SDL_Texture* texMusic = SDL_CreateTextureFromSurface(rend, surfMusic);
    SDL_Texture* texAudio = SDL_CreateTextureFromSurface(rend, surfAudio);
    SDL_Texture* texOnText = SDL_CreateTextureFromSurface(rend, surfOnText);
    SDL_Texture* texOffText = SDL_CreateTextureFromSurface(rend, surfOffText);
    SDL_Texture* texVersion = SDL_CreateTextureFromSurface(rend, surfVersion);

    //free Surfaces
    SDL_FreeSurface(surfCar);
    SDL_FreeSurface(surfCarWreck);
    SDL_FreeSurface(surfWall);
    SDL_FreeSurface(surfRoad);
    SDL_FreeSurface(surfRoadLine);
    SDL_FreeSurface(surfBlock);
    SDL_FreeSurface(surfTruck);
    SDL_FreeSurface(surfTruckFire);
    SDL_FreeSurface(surfRocket);
    SDL_FreeSurface(surfBullet);
    SDL_FreeSurface(surfPowerup);
    SDL_FreeSurface(surfDoubleBullet);
    SDL_FreeSurface(surfRocket2);
    SDL_FreeSurface(surfStreet);
    SDL_FreeSurface(surfStreet2);
    SDL_FreeSurface(surfStreet3);
    SDL_FreeSurface(surfStreet4);
    SDL_FreeSurface(surfStreet5);
    SDL_FreeSurface(surfPolice);
    SDL_FreeSurface(surfPoliceBullet);
    SDL_FreeSurface(surfDrone);
    SDL_FreeSurface(surfDroneShoot);
    SDL_FreeSurface(surfDroneBullet);
    SDL_FreeSurface(surfTruckWreck);
    SDL_FreeSurface(surfPoliceWreck);
    SDL_FreeSurface(surfDroneWreck);
    SDL_FreeSurface(surfHit);
    SDL_FreeSurface(surfExplode);
    SDL_FreeSurface(surfOverlay);
    SDL_FreeSurface(surfPause);
    SDL_FreeSurface(surfGameOver);
    SDL_FreeSurface(surfMenu);
    SDL_FreeSurface(surfOptions);
    SDL_FreeSurface(surfControls);
    SDL_FreeSurface(surfFullscreen);
    SDL_FreeSurface(surfMusic);
    SDL_FreeSurface(surfAudio);
    SDL_FreeSurface(surfOnText);
    SDL_FreeSurface(surfOffText);
    SDL_FreeSurface(surfVersion);
    
    

    
    //rects to hold positions and size of sprites
    SDL_Rect destCar;
    SDL_Rect destWall[NOW];
    SDL_Rect destRoad[NOW/2];
    SDL_Rect destTruck;

    SDL_Rect destBlock;
    SDL_Rect destPowerup;
    SDL_Rect destRocket;

    SDL_Rect destBullets;
    SDL_Rect destRocket2;

    SDL_Rect destDoubleBullet[2];

    SDL_Rect Background[2];

    SDL_Rect destSmashVehicle;

    SDL_Rect destHit;

    SDL_Rect destEnmyHit;

    SDL_Rect destOverlay;

    SDL_Rect destTextOverlay;

    SDL_Rect destFullscreen;
    SDL_Rect destFullscreenStatus;

    SDL_Rect destMusic;
    SDL_Rect destMusicStatus;

    SDL_Rect destAudio;
    SDL_Rect destAudioStatus;

    SDL_Rect destVersion;

    

    //get dimensions of Texture

     SDL_QueryTexture(texOverlay, NULL, NULL, &destOverlay.w, &destOverlay.h);
     destOverlay.x = 0;
     destOverlay.y = 0;
     SDL_QueryTexture(texPause, NULL, NULL, &destTextOverlay.w, &destTextOverlay.h);
     destTextOverlay.x = 0;
     destTextOverlay.y = 0;
    SDL_QueryTexture(texCar, NULL, NULL, &destCar.w, &destCar.h);

    for(int i = 0; i < NOW; i++)
    {
      SDL_QueryTexture(texWall, NULL, NULL, &destWall[i].w, &destWall[i].h);

      destWall[i].h *= 8;
      destWall[i].w *= 8;

    }
    for(int i =0; i < NOW/2; i++)
    {
      if(i%2==0)
      {
        SDL_QueryTexture(texRoad, NULL, NULL, &destRoad[i].w, &destRoad[i].h);
      }
      else
      {
        SDL_QueryTexture(texRoadLine, NULL, NULL, &destRoad[i].w, &destRoad[i].h);
      }
      destRoad[i].h *= 8;
    }


    SDL_QueryTexture(texTruck, NULL, NULL, &destTruck.w, &destTruck.h);
    SDL_QueryTexture(texBlock, NULL, NULL, &destBlock.w, &destBlock.h);
    SDL_QueryTexture(texPowerup, NULL, NULL, &destPowerup.w, &destPowerup.h);
    SDL_QueryTexture(texRocket, NULL, NULL, &destRocket.w, &destRocket.h);
    SDL_QueryTexture(texTruck, NULL, NULL, &destSmashVehicle.w, &destSmashVehicle.h);
    SDL_QueryTexture(texHit, NULL, NULL, &destHit.w, &destHit.h);
    SDL_QueryTexture(texHit, NULL, NULL, &destEnmyHit.w, &destEnmyHit.h);

    //set dimentions of rects
    destCar.h *= 4;
    destCar.w *= 4;

    

    destTruck.h *= 5;
    destTruck.w *= 5;

    destSmashVehicle.h *= 5;
    destSmashVehicle.w *= 5;

    


    
    float x_pos = (WINDOW_WIDTH - destCar.w) / 1.5;
    float y_pos = (WINDOW_HEIGHT - destCar.h) / 1.25;
    float yadder = 0;
    for(int i = 0; i < NOW; i++)
    {

      if(i%2==0)
      {
        destWall[i].x = (WINDOW_WIDTH / 6) * 1.5;
        destWall[i].y = 0 + yadder;
        destRoad[i/2].x = (WINDOW_WIDTH / 6) * 2 + destWall[i].w;
        destRoad[i/2].y = 0 + yadder;
      }
      else
      {
        destWall[i].x = (WINDOW_WIDTH / 6) * 4.5;
        destWall[i].y = 0 + yadder;

        yadder += destWall[i].h;
      }


    }

    destBlock.w *= 4;
    destBlock.h *= 4;

    destBlock.x = destRoad[0].x + (rand() % (destRoad[0].w - destBlock.w));
    destBlock.y = WINDOW_HEIGHT;

    destPowerup.w *= 4;
    destPowerup.h *= 4;

    destPowerup.x = destRoad[0].x + (rand() % (destRoad[0].w - destBlock.w));
    destPowerup.y = WINDOW_HEIGHT;

    destRocket.x = 0;
    destRocket.y = WINDOW_HEIGHT;

    destTruck.y = WINDOW_HEIGHT;
    
    destTruck.x = destWall[0].x + 30;

    destSmashVehicle.x=destWall[0].x + 30;
    destSmashVehicle.y = WINDOW_HEIGHT;

    


    destRocket.w *= 4;
    destRocket.h *= 4;

    destHit.w *= 4;
    destHit.h *= 4;

    destEnmyHit.w *= 4;
    destEnmyHit.h *= 4;

    destEnmyHit.x = -128;
    destEnmyHit.y = -128;


      SDL_QueryTexture(texBullet, NULL, NULL, &destBullets.w, &destBullets.h);
      destBullets.h *= 4;
      destBullets.w *= 4;
      destBullets.x = destCar.x + (destCar.w/2);
      destBullets.y = destCar.y;

      for(int i =0; i < 2; i++)
      {
        SDL_QueryTexture(texDoubleBullet, NULL, NULL, &destDoubleBullet[i].w, &destDoubleBullet[i].h);
        SDL_QueryTexture(texStreet, NULL, NULL, &Background[i].w, &Background[i].h);
        destDoubleBullet[i].h *= 4;
        destDoubleBullet[i].w *= 4;
        destDoubleBullet[i].y = destCar.y;
        Background[i].x = 0;
      }

    destDoubleBullet[0].x = destCar.x;
    destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);

    Background[0].y = 0;
    Background[1].y = Background[0].y - Background[1].h;

    SDL_QueryTexture(texRocket, NULL, NULL, &destRocket2.w, &destRocket2.h);
    destRocket2.h *= 4;
    destRocket2.w *= 4;
    destRocket2.x = destCar.x + (destCar.w/2);
    destRocket2.y = destCar.y;

    destFullscreen.w = 250;
    destFullscreen.h = 75;

    destFullscreen.x = 320-(destFullscreen.w/2);
    destFullscreen.y = 160-(destFullscreen.h/2);

    destFullscreenStatus.w = 50;
    destFullscreenStatus.h = 25;

    destFullscreenStatus.x = destFullscreen.x + (destFullscreen.w + 30);
    destFullscreenStatus.y = destFullscreen.y+(destFullscreen.h/2);

    destMusic.w = 250;
    destMusic.h = 75;

    destMusic.x = 320-(destMusic.w/2);
    destMusic.y = 240-(destMusic.h/2);

    destMusicStatus.w = 50;
    destMusicStatus.h = 25;

    destMusicStatus.x = destMusic.x + (destMusic.w + 30);
    destMusicStatus.y = destMusic.y+(destMusic.h/2);

    destAudio.w = 250;
    destAudio.h = 75;

    destAudio.x = 320-(destAudio.w/2);
    destAudio.y = 320-(destAudio.h/2);

    destAudioStatus.w = 50;
    destAudioStatus.h = 25;

    destAudioStatus.x = destAudio.x + (destAudio.w + 30);
    destAudioStatus.y = destAudio.y+(destAudio.h/2);

    destVersion.w = 60;
    destVersion.h = 20;

    destVersion.x = (WINDOW_WIDTH/2) - (destVersion.w/2);
    destVersion.y = WINDOW_HEIGHT - destVersion.h;

    //set up music for menu
    bgMusic = Mix_LoadMUS("Content/Audio/MenuMusic.mp3");
    Mix_PlayMusic(bgMusic, -1);

    //set up soundeffects
    sndBulletCollide = Mix_LoadWAV("Content/Audio/BulletCollide.ogg");
    sndPlayerBullet = Mix_LoadWAV("Content/Audio/playerBullet.ogg");
    sndDoubleBullet = Mix_LoadWAV("Content/Audio/doubleBullet.ogg");
    sndDrone = Mix_LoadWAV("Content/Audio/drone.ogg");
    sndPoliceShoot = Mix_LoadWAV("Content/Audio/policeShoot.ogg");
    sndPlayerRocketCollide = Mix_LoadWAV("Content/Audio/playerRocketCollide.ogg");
    sndPlayerRocketShoot = Mix_LoadWAV("Content/Audio/playerRocketShoot.ogg");
    sndEnemyRocketShoot = Mix_LoadWAV("Content/Audio/enemyRocketShoot.ogg");
    sndEnemyRocketCollide = Mix_LoadWAV("Content/Audio/enemyRocketCollide.ogg");
    sndPlayerHit = Mix_LoadWAV("Content/Audio/playerHit.ogg");
    sndPlayerCrash = Mix_LoadWAV("Content/Audio/playerCrash.ogg");
    sndVanCrash = Mix_LoadWAV("Content/Audio/vanCrash.ogg");
    sndPoliceCrash = Mix_LoadWAV("Content/Audio/policeCrash.ogg");
    sndDroneCrash = Mix_LoadWAV("Content/Audio/droneCrash.ogg");
    sndPickup = Mix_LoadWAV("Content/Audio/pickup.ogg");

    //give sprite velocity
    float x_vel = 0;
    float y_vel = 0;

    //inputs
    int left = 0;
    int right = 0;

    int shoot = 0;

    //create and set varibles for game rules
    bool close_requested = false;
    int countdown = 5;
    int roadturn = 0;
    int old_x[NOW];
    int direction;
    bool turnLeft = false;
    bool turnRight = false;
    int moveRoad = 3;
    bool changeRoad = false;
    int blockOffset = destBlock.x - destRoad[0].x;
    int itemOffset = destBlock.x - destRoad[0].x;
    int spawnRoad = 0;
    int truckFire = 16;
    bool bulletLoaded = true;
    int ranRoadBlock = 0;
    int ranRoadItem = 0;
    bool truckOnRoad = false;
    int randTruck = 0;
    bool doubleBulletActivated = false;
    bool playerRocketActivated = false;
    int powerupCooldown = 1800;
    int truckHealth = 5;
    int blockHealth = 2;
    int score = 0;
    
    int playerRotate = 0;
    int enemyRotate = 0;

    int lastVehicle = 0;

    int hitCountdown = 0;
    int enmyHitCountdown = 0;

    int lastBullet = 0;

    int randBack1 = 0;
    int randBack2 = 0;

    int playerHealth = 3;

    int enmyDamage = 0;

    int escDown = 0;
    int enterDown = 0;

    int qDown = 0;
    int SDown = 0;
    int oDown = 0;
    int cDown = 0;
    int fDown = 0;
    int mDown = 0;
    int aDown = 0;

    bool canPause = true;
    bool pause = false;
    bool playerAlive = true;

    int gameMode = 1;
    int subMenu = 1;
    int changeScreen = true;

    bool fullScreen = false;
    bool playMusic = true;
    bool playAudio = true;

    int diffAdder = 1;

    bool damagedFile = false;

    int highScore = 0;
    int scoreI = 0;

    //get configuration
    FILE *fp;
   char buff[255];

  //strings for holding file data
   char curFul[255];
   char curMus[255];
   char curAud[255];

   if ((fp = fopen("Content/config", "r")))
   {
   
   
    fgets(curFul, 255, (FILE*)fp);
    if(strcmp(curFul, "Fullscreen: On\n")==0)
    {
        SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
        fullScreen = true;
        
    }
    else if(strcmp(curFul, "Fullscreen: Off\n")!=0)
    {
        damagedFile = true;
    }
    fgets(curMus, 255, (FILE*)fp);
    if(strcmp(curMus, "Music: Off\n")==0)
    {
        Mix_HaltMusic();
        playMusic = false;
    }
    else if(strcmp(curMus, "Music: On\n")!=0)
    {
        damagedFile = true;
    }
    fgets(curAud, 255, (FILE*)fp);
    if(strcmp(curAud, "Audio: Off\n")==0)
    {
        playAudio = false;
    }
    else if(strcmp(curAud, "Audio: On\n")!=0)
    {
        damagedFile = true;
    }
    fgets(buff, 255, (FILE*)fp);
    
    while(buff[scoreI]!='\0')
    {
      if(!isdigit(buff[scoreI]))
      {
          damagedFile = true;
      }
      scoreI++;
    }
    if(!damagedFile)
    {
      highScore = atoi(buff);
    }
    fclose(fp);
   }
   else
   {
     damagedFile = true;
   }
   //if file data dosen't match the standard set a default config
   if(damagedFile)
   {
        FILE *fp;

        fp = fopen("Content/config", "w+");
        fputs("Fullscreen: Off\n", fp);
        fputs("Music: On\n", fp);
        fputs("Audio: On\n", fp);
        fputs("0\n", fp);
        fclose(fp);

        fullScreen = false;
        playMusic = true;
        playAudio = true;
        highScore = 0;
        
        Mix_HaltMusic();
        bgMusic = Mix_LoadMUS("Content/Audio/MenuMusic.mp3");
        Mix_PlayMusic(bgMusic, -1);

        SDL_SetWindowFullscreen(win, 0);
        sprintf(curFul, "Fullscreen: Off\n");
        sprintf(curMus, "Music: On\n");
        sprintf(curAud, "Audio: On\n");
   }
    
  


    

    

    

   
    

    //Game loop
    while (!close_requested)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        //check what buttons are down
        switch (event.type) {
          case SDL_QUIT:
            close_requested = true;
            break;
          case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
              case SDL_SCANCODE_LEFT:
                left = 1;
                break;
              case SDL_SCANCODE_RIGHT:
                right = 1;
                break;
              case SDL_SCANCODE_SPACE:
                shoot = 1;
                break;
              case SDL_SCANCODE_Q:
                qDown = 1;
                break;
              case SDL_SCANCODE_S:
                SDown = 1;
                break;
              case SDL_SCANCODE_C:
                cDown = 1;
                break;
              case SDL_SCANCODE_O:
                oDown = 1;
                break;
              case SDL_SCANCODE_F:
                fDown = 1;
                break;
              case SDL_SCANCODE_M:
                mDown = 1;
                break;
              case SDL_SCANCODE_A:
                aDown = 1;
                break;
              case SDL_SCANCODE_ESCAPE:
                escDown = 1;
                break;
              case SDL_SCANCODE_RETURN:
                enterDown = 1;
                break;
              default:
                break;
            }
            break;
          case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
             case SDL_SCANCODE_LEFT:
                left = 0;
                break;
              case SDL_SCANCODE_RIGHT:
                right = 0;
                break;
              case SDL_SCANCODE_SPACE:
                shoot = 0;
                break;
              case SDL_SCANCODE_Q:
                qDown = 0;
                break;
              case SDL_SCANCODE_S:
                SDown = 0;
                break;
              case SDL_SCANCODE_C:
                cDown = 0;
                break;
              case SDL_SCANCODE_O:
                oDown = 0;
                break;
              case SDL_SCANCODE_F:
                fDown = 0;
                break;
              case SDL_SCANCODE_M:
                mDown = 0;
                break;
              case SDL_SCANCODE_A:
                aDown = 0;
                break;
              case SDL_SCANCODE_ESCAPE:
                escDown = 0;
                break;
              case SDL_SCANCODE_RETURN:
                enterDown = 0;
                break;
              default:
                break;
            }
            break;
        }
      }
      //determines the game mode
      switch(gameMode) 
      {
      //menu
      case 1:
      //main menu
      if(subMenu == 1)
      {

        //start game
        if(SDown == 1)
        {

          
    


    

    
        x_pos = (WINDOW_WIDTH - destCar.w) / 1.5;
        y_pos = (WINDOW_HEIGHT - destCar.h) / 1.25;
        yadder = 0;
    
    for(int i = 0; i < NOW; i++)
    {

      if(i%2==0)
      {
        destWall[i].x = (WINDOW_WIDTH / 6) * 1.5;
        destWall[i].y = 0 + yadder;
        destRoad[i/2].x = (WINDOW_WIDTH / 6) * 2 + destWall[i].w;
        destRoad[i/2].y = 0 + yadder;
      }
      else
      {
        destWall[i].x = (WINDOW_WIDTH / 6) * 4.5;
        destWall[i].y = 0 + yadder;

        yadder += destWall[i].h;
      }


    }

          destBlock.x = destRoad[0].x + (rand() % (destRoad[0].w - destBlock.w));
          destBlock.y = WINDOW_HEIGHT;

   
          destPowerup.x = destRoad[0].x + (rand() % (destRoad[0].w - destBlock.w));
          destPowerup.y = WINDOW_HEIGHT;

          destRocket.x = 0;
          destRocket.y = WINDOW_HEIGHT;

          destTruck.y = WINDOW_HEIGHT;
    
          destTruck.x = destWall[0].x + 30;

    


          destEnmyHit.x = -128;
          destEnmyHit.y = -128;


      
        destBullets.x = destCar.x + (destCar.w/2);
        destBullets.y = destCar.y;

        for(int i =0; i < 2; i++)
        {
        
          destDoubleBullet[i].y = destCar.y;
          Background[i].x = 0;
        }

      destDoubleBullet[0].x = destCar.x;
      destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);

      Background[0].y = 0;
      Background[1].y = Background[0].y - Background[1].h;

    
      destRocket2.x = destCar.x;
      destRocket2.y = destCar.y;

      destSmashVehicle.x=destWall[0].x + 30;
      destSmashVehicle.y = WINDOW_HEIGHT;
          if(playMusic)
          {
            Mix_HaltMusic();
            bgMusic = Mix_LoadMUS("Content/Audio/MainMusic.mp3");
            Mix_PlayMusic(bgMusic, -1);
          }
          countdown = 5;
          roadturn = 0;
          turnLeft = false;
          turnRight = false;
          moveRoad = 5;
          changeRoad = false;
          blockOffset = destBlock.x - destRoad[0].x;
          itemOffset = destBlock.x - destRoad[0].x;
          spawnRoad = 0;
          truckFire = 16;
          bulletLoaded = true;
          ranRoadBlock = 0;
          ranRoadItem = 0;
          truckOnRoad = false;
          randTruck = 0;
          doubleBulletActivated = false;
          playerRocketActivated = false;
          powerupCooldown = 1800;
          truckHealth = 5;
          blockHealth = 2;
          score = 0;
          diffAdder = 1;
    
          playerRotate = 0;
          enemyRotate = 0;

          lastVehicle = 0;

          hitCountdown = 0;
          enmyHitCountdown = 0;

          lastBullet = 0;

          randBack1 = 0;
          randBack2 = 0;

          playerHealth = 3;

          enmyDamage = 0;

          

        canPause = true;
        pause = false;
        playerAlive = true;

          gameMode = 2;

        }
        //close game
        else if(qDown == 1)
        {
          close_requested = true;
        }
        //switch to controls
        else if(cDown == 1)
        {
            subMenu = 2;
        }
        //switch to options
        else if(oDown == 1)
        {
            subMenu = 3;
        }
      }
      //Controls menu
      else if(subMenu == 2)
      {
         if(escDown)
        {
          subMenu = 1;
        }
      }
      //options menu
      else
      {
         if(escDown)
        {
          subMenu = 1;
        }
        //sets fullscreen
        if((fDown == 1) && (changeScreen))
        {
          if(!fullScreen)
          {
            SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
            fullScreen = true;

            
   
            sprintf(curFul, "Fullscreen: On\n");
          }
          else
          {
            SDL_SetWindowFullscreen(win, 0);
            fullScreen = false;
            sprintf(curFul, "Fullscreen: Off\n");
          }
          FILE *fp;

          fp = fopen("Content/config", "w+");
          fputs(curFul, fp);
          fputs(curMus, fp);
          fputs(curAud, fp);
          fprintf(fp, "%d", highScore);
          fclose(fp);
          changeScreen = false;
        }
        //sets music on or off
        if((mDown == 1) && (changeScreen))
        {
          if(playMusic)
          {
            playMusic = false;
            Mix_HaltMusic();
            sprintf(curMus, "Music: Off\n");
          }
          else
          {
            playMusic = true;
            bgMusic = Mix_LoadMUS("Content/Audio/MenuMusic.mp3");
            Mix_PlayMusic(bgMusic, -1);
            sprintf(curMus, "Music: On\n");
          }
          FILE *fp;

          fp = fopen("Content/config", "w+");
          fputs(curFul, fp);
          fputs(curMus, fp);
          fputs(curAud, fp);
          fprintf(fp, "%d", highScore);
          fclose(fp);
          changeScreen = false;
        }
        //sets audio on or off
        if((aDown == 1) && (changeScreen))
        {
          if(playAudio)
          {
            playAudio = false;
           sprintf(curAud, "Audio: Off\n");
          }
          else
          {
            playAudio = true;
            sprintf(curAud, "Audio: On\n");
          }
          FILE *fp;

          fp = fopen("Content/config", "w+");
          fputs(curFul, fp);
          fputs(curMus, fp);
          fputs(curAud, fp);
          fprintf(fp, "%d", highScore);
          fclose(fp);
          changeScreen = false;
        }
        if((fDown == 0) && (mDown == 0) && (aDown == 0))
        {
          changeScreen = true;
        }
      }
      //background animation
         for(int i = 0; i < NOW; i++)
        {
          old_x[i] = destWall[i].x;
        }
        if(moveRoad%8==0)
        {
          for(int i = 0; i < NOW/2; i++)
          {
            destRoad[i].y += destWall[i].h;


            if(destRoad[i].y >= WINDOW_HEIGHT)
            {
              destRoad[i].y = 0;
            }
          }
          destBlock.y += destWall[0].h;

          destPowerup.y += destWall[0].h;

        }
        if ((roadturn > 0))
        {
            if(moveRoad==0)
            {
              if(turnLeft && !turnRight)
              {
                destWall[0].x--;
                destWall[1].x--;


              }
              else if (!turnLeft && turnRight)
              {
                destWall[0].x++;
                destWall[1].x++;

              }
            }


            
            roadturn--;
            if(roadturn == 0)
            {
              countdown = 1;
            }
            

          }
          for(int i = 2; i < NOW; i++)
          {
            destWall[i].x = old_x[i-2];

          }

          for(int i = 0; i < NOW; i++)
          {

              for(int j = 0; j < NOW/2; j++)
              {
                if(has_collided(destRoad[j], destWall[i]))
                {
                  if(i%2==0)
                  {
                    destRoad[j].x = destWall[i].x + destWall[i].w;
                    if(destRoad[j].y == destBlock.y)
                    {
                    
                    }
                  }
                  else
                  {
                    destRoad[j].x = destWall[i].x - destRoad[j].w;
                    if(destRoad[j].y == destBlock.y)
                    {
                      
                    }
                  }
                }
              }
          }
        //scrolls buildings
        for (int i = 0; i < 2; i++)
        {
          
          Background[i].y+=5;
          
          
          
        }
        if(Background[0].y > WINDOW_HEIGHT)
          {
            srand(time(0));
            Background[0].y = Background[1].y - Background[0].h;
            randBack1 = rand() % 5;
          
          }
        if(Background[1].y > WINDOW_HEIGHT)
          {
            srand(time(0));
            Background[1].y = Background[0].y - Background[1].h;
            randBack2 = rand() % 5;
          
          }
          moveRoad--;
        if(moveRoad < 0)
        {
          moveRoad = 4;
        }
        //renders menu content
        SDL_RenderClear(rend);
        switch(randBack1){
              case 0:
              SDL_RenderCopy(rend, texStreet, NULL, &Background[0]);
              break;
              case 1:
              SDL_RenderCopy(rend, texStreet2, NULL, &Background[0]);
              break;
              case 2:
              SDL_RenderCopy(rend, texStreet3, NULL, &Background[0]);
              break;
              case 3:
              SDL_RenderCopy(rend, texStreet4, NULL, &Background[0]);
              break;
              default:
              SDL_RenderCopy(rend, texStreet5, NULL, &Background[0]);
              break;
            }
            switch(randBack2){
              case 0:
              SDL_RenderCopy(rend, texStreet, NULL, &Background[1]);
              break;
              case 1:
              SDL_RenderCopy(rend, texStreet2, NULL, &Background[1]);
              break;
              case 2:
              SDL_RenderCopy(rend, texStreet3, NULL, &Background[1]);
              break;
              case 3:
              SDL_RenderCopy(rend, texStreet4, NULL, &Background[1]);
              break;
              default:
              SDL_RenderCopy(rend, texStreet5, NULL, &Background[1]);
              break;
            }
        for(int i = 0; i < NOW; i++)
        {


          SDL_RenderCopy(rend, texWall, NULL, &destWall[i]);
        }
        for(int i = 0; i < NOW/2; i++)
        {

            if(i%2==0)
            {
              SDL_RenderCopy(rend, texRoad, NULL, &destRoad[i]);
            }
            else
            {
              SDL_RenderCopy(rend, texRoadLine, NULL, &destRoad[i]);
            }

        }
        SDL_RenderCopy(rend, texOverlay, NULL, &destOverlay);
        if(subMenu == 1)
        {
          SDL_RenderCopy(rend, texMenu, NULL, &destTextOverlay);
          if(highScore > 0)
          {
             get_and_update_hud(highScore, rend, font, Red, 5, 220, 190, 200, 50); 
          }
          SDL_RenderCopy(rend, texVersion, NULL, &destVersion);
        }
        else if(subMenu == 2)
        {
          SDL_RenderCopy(rend, texControls, NULL, &destTextOverlay);
        }
        else
        {
          SDL_RenderCopy(rend, texOptions, NULL, &destTextOverlay);
          SDL_RenderCopy(rend, texFullscreen, NULL, &destFullscreen);
          SDL_RenderCopy(rend, texMusic, NULL, &destMusic);
          SDL_RenderCopy(rend, texAudio, NULL, &destAudio);
          if(fullScreen)
          {
            SDL_RenderCopy(rend, texOnText, NULL, &destFullscreenStatus);
          }
          else
          {
            SDL_RenderCopy(rend, texOffText, NULL, &destFullscreenStatus);
          }

          if(playMusic)
          {
            SDL_RenderCopy(rend, texOnText, NULL, &destMusicStatus);
          }
          else
          {
            SDL_RenderCopy(rend, texOffText, NULL, &destMusicStatus);
          }

          if(playAudio)
          {
            SDL_RenderCopy(rend, texOnText, NULL, &destAudioStatus);
          }
          else
          {
            SDL_RenderCopy(rend, texOffText, NULL, &destAudioStatus);
          }
        }
        

        
        
        break;
      //game
      case 2:
      //pauses or closes game
      if(escDown == 1)
      {
        if(playerAlive == true)
        {
          if(canPause == true)
          {
            if(pause == false)
            {
              pause = true;
            }
            else
            {
              pause = false;
            }
            canPause = false;
          }
        }
        else
        {
          close_requested = true;
        }
      }
      else
      {
        canPause = true;
      }
      //takes user back to menu
      if((enterDown == 1) && (playerAlive == false))
      {
        if(playMusic)
        {
            Mix_HaltMusic();
          bgMusic = Mix_LoadMUS("Content/Audio/MenuMusic.mp3");
          Mix_PlayMusic(bgMusic, -1);
        }
        
        gameMode = 1;

        
      }
      //main game actions
      if((pause == false) && (playerAlive == true))
      {
        
        //set velocity and rotation
        x_vel = 0;
        y_vel = 0;
        playerRotate = 0;
        if(left && !right)
        {
          x_vel = -SPEED;

          playerRotate = 330;
        }
        if(!left && right)
        {
          x_vel = SPEED;

          playerRotate = 30;
        }

        //random varibles
        roadturn = rand() % (50);

        direction = rand() % (5);
        //determine road direction and position
        switch (direction)
        {
          case 0:
            turnLeft = true;
            turnRight = false;
            break;
          case 1:
            turnLeft = false;
            turnRight = true;
            break;
          default:
            turnLeft = false;
            turnRight = false;
            break;
        }

        
        
        if(destWall[0].x == (destWall[0].w))
        {
          turnLeft = false;
        }
        if(destWall[1].x == (WINDOW_WIDTH - destWall[1].w))
        {
          turnRight = false;
        }

        for(int i = 0; i < NOW; i++)
        {
          old_x[i] = destWall[i].x;
        }
        if(moveRoad==0)
        {
          for(int i = 0; i < NOW/2; i++)
          {
            destRoad[i].y += destWall[i].h;


            if(destRoad[i].y >= WINDOW_HEIGHT)
            {
              destRoad[i].y = 0;
            }
          }
          destBlock.y += destWall[0].h;

          destPowerup.y += destWall[0].h;

        }
        if ((roadturn > 0))
        {
            if(moveRoad==0)
            {
              if(turnLeft && !turnRight)
              {
                destWall[0].x--;
                destWall[1].x--;


              }
              else if (!turnLeft && turnRight)
              {
                destWall[0].x++;
                destWall[1].x++;

              }
            }


           
            roadturn--;
            if(roadturn == 0)
            {
              countdown = 1;

            }


          }
          for(int i = 2; i < NOW; i++)
          {
            destWall[i].x = old_x[i-2];

          }










        //update car position
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;



        //set car position
        
        destCar.y = (int) y_pos;
        destCar.x = (int) x_pos;
        


      
          //moves road with walls
          for(int i = 0; i < NOW; i++)
          {

            
              for(int j = 0; j < NOW/2; j++)
              {
                if(has_collided(destRoad[j], destWall[i]))
                {
                  if(i%2==0)
                  {
                    destRoad[j].x = destWall[i].x + destWall[i].w;
                    
                  }
                  else
                  {
                    destRoad[j].x = destWall[i].x - destRoad[j].w;
                    
                  }
                }
              }
          }
        //moves enemy weapon varible
        if(enmyHitCountdown <= 0)
        {
          destEnmyHit.y = -128;
        }
        else
        {
          enmyHitCountdown--;
        }
        //sets rules for enemy vehicle position, texture and attributes
        if(truckOnRoad == false)
        {
          if(destRoad[0].y == 0)
          {
            randTruck = rand() % (10);

            if(randTruck == 1)
            {
              randTruck = rand() % (6);
              
              
              switch(randTruck) {
                case 0:
                  destTruck.x = destWall[0].x + 30;
                  destTruck.y = 0;
                  truckHealth = 5;

                  SDL_QueryTexture(texTruck, NULL, NULL, &destTruck.w, &destTruck.h);

                  destTruck.h *= 5;
                  destTruck.w *= 5;

                  SDL_QueryTexture(texRocket, NULL, NULL, &destRocket.w, &destRocket.h);

                  destRocket.w *= 4;
                  destRocket.h *= 4;
                  
                break;
                case 1:
                  destTruck.x = (destWall[1].x - 30) - destTruck.w;
                  destTruck.y = 0;
                  truckHealth = 5;

                  SDL_QueryTexture(texTruck, NULL, NULL, &destTruck.w, &destTruck.h);

                  destTruck.h *= 5;
                  destTruck.w *= 5;

                  SDL_QueryTexture(texRocket, NULL, NULL, &destRocket.w, &destRocket.h);

                  destRocket.w *= 4;
                  destRocket.h *= 4;
                  
                  
                break;
                case 2:
                  destTruck.x = destWall[0].x + 30;
                  destTruck.y = 0;
                  truckHealth = 3;

                  SDL_QueryTexture(texPolice, NULL, NULL, &destTruck.w, &destTruck.h);

                  destTruck.h *= 4;
                  destTruck.w *= 4;

                  SDL_QueryTexture(texPoliceBullet, NULL, NULL, &destRocket.w, &destRocket.h);

                  destRocket.w *= 4;
                  destRocket.h *= 4;
                  
                  
                break;
                case 3:
                  destTruck.x = (destWall[1].x - 30) - destTruck.w;
                  destTruck.y = 0;
                  truckHealth = 3;

                  SDL_QueryTexture(texPolice, NULL, NULL, &destTruck.w, &destTruck.h);

                  destTruck.h *= 4;
                  destTruck.w *= 4;

                  SDL_QueryTexture(texPoliceBullet, NULL, NULL, &destRocket.w, &destRocket.h);

                  destRocket.w *= 4;
                  destRocket.h *= 4;

                  
                break;

                case 4:
                  destTruck.x = destWall[0].x + 30;
                  destTruck.y = 0;
                  truckHealth = 1;

                  SDL_QueryTexture(texDrone, NULL, NULL, &destTruck.w, &destTruck.h);

                  destTruck.h *= 4;
                  destTruck.w *= 4;

                  SDL_QueryTexture(texDroneBullet, NULL, NULL, &destRocket.w, &destRocket.h);

                  destRocket.w *= 4;
                  destRocket.h *= 4;
                  
                break;
                default:
                  destTruck.x = (destWall[1].x - 30) - destTruck.w;
                  destTruck.y = 0;
                  truckHealth = 1;
                  
                  SDL_QueryTexture(texDrone, NULL, NULL, &destTruck.w, &destTruck.h);

                  destTruck.h *= 4;
                  destTruck.w *= 4;

                  SDL_QueryTexture(texDroneBullet, NULL, NULL, &destRocket.w, &destRocket.h);

                  destRocket.w *= 4;
                  destRocket.h *= 4;
                  
                break;
              }
              truckOnRoad = true;

              
            }
          }

        }
        //set position of road block
        ranRoadBlock = rand() % NOW/2;
        if((destRoad[ranRoadBlock].y == 0) && (destBlock.y >= WINDOW_HEIGHT))
        {
          spawnRoad = rand() % 4;
          if(spawnRoad == 2)
          {
            destBlock.y = 0;
            destBlock.x = destRoad[ranRoadBlock].x + (rand() % (destRoad[ranRoadBlock].w - destBlock.w));



            blockOffset = destBlock.x - destRoad[ranRoadBlock].x;
            blockHealth = 2;

          }
        }
        destBlock.x = destRoad[ranRoadBlock].x + blockOffset;
        //set position of powerup
        srand(time(0));
        ranRoadItem = rand() % NOW/2;
        if((destRoad[ranRoadItem].y == 0) && (destPowerup.y >= WINDOW_HEIGHT))
        {
          spawnRoad = rand() % 4;
          if(spawnRoad == 2)
          {

            destPowerup.y = 0;
            destPowerup.x = destRoad[ranRoadItem].x + (rand() % (destRoad[ranRoadItem].w - destPowerup.w));


            itemOffset = destPowerup.x - destRoad[ranRoadItem].x;

          }
        }
        destPowerup.x = destRoad[ranRoadItem].x + itemOffset;
        

        //move background
        
        for (int i = 0; i < 2; i++)
        {
          
          Background[i].y+=(diffAdder + 3);
          
          
          
        }
        if(Background[0].y > WINDOW_HEIGHT)
          {
            srand(time(0));
            Background[0].y = Background[1].y - Background[0].h;//WINDOW_HEIGHT - (Background[0].h*2) + ((Background[0].y - WINDOW_HEIGHT) * 2);
            randBack1 = rand() % 5;
          
          }
        if(Background[1].y > WINDOW_HEIGHT)
          {
            srand(time(0));
            Background[1].y = Background[0].y - Background[1].h;//WINDOW_HEIGHT - (Background[1].h*2) + ((Background[1].y - WINDOW_HEIGHT) * 2);
            randBack2 = rand() % 5;
          
          }

        

        
        

        



        //set road speed
        moveRoad--;
        if(moveRoad < 0)
        {

          moveRoad = 5 - diffAdder;

          if(moveRoad <= 1)
          {
            moveRoad = 1;
          }
        }
        
        
        

      //actions for enemy vehicle on the road
      if(truckOnRoad == true)
      {
        
        
        if(destTruck.y < 100)
        {
          destTruck.y+=diffAdder;
          
          
        }
        else
        {
          if(truckFire == 4)
          {
            
            
            
              destRocket.x = destTruck.x;
              destRocket.y = destTruck.y + (destTruck.h);
              switch(randTruck) {
                case 0:
                case 1:
                  if(playAudio)
                  {
                    Mix_PlayChannel( -1, sndEnemyRocketShoot, 0 );
                  }
                  
                break;
                case 2:
                case 3:
                  if(playAudio)
                  {
                    Mix_PlayChannel( -1, sndPoliceShoot, 0 );
                  }
                break;
                default:
                  if(playAudio)
                  {
                    Mix_PlayChannel( -1, sndDrone, 0 );
                  }
                
                break;
              }
            


          }
          truckFire--;
          

        //set damage of enemy weapon and move it when on road
        if(destRocket.y > 0)
        {
          destRocket.y+= 3 + diffAdder;
        switch(randTruck) {
                case 0:
                case 1:
                  
                  enmyDamage = 3;
                break;
                case 2:
                case 3:
                  
                  enmyDamage = 2;
                break;

                default:
                  
                  enmyDamage = 1;
                break;
              }
        }

          

          if(destRocket.y > WINDOW_HEIGHT)
          {
            if(truckFire <= 0)
            {
              truckFire = 16;
            }
          }
          
        }


        

        enemyRotate = 0;
        if(destTruck.x < destCar.x - 10)
        {
          destTruck.x+=diffAdder;

          enemyRotate = 330;
        }
        else if(destTruck.x > destCar.x + 10)
        {
          destTruck.x-=diffAdder;

          enemyRotate = 30;
        }
        else
        {
          enemyRotate = 0;
        }
        
      }
        
      

        //player shooting code
        if((bulletLoaded == true) && (shoot == 1))
        {
          bulletLoaded = false;
          if(doubleBulletActivated == true)
          {
            if(playAudio)
            {
              Mix_PlayChannel( -1, sndDoubleBullet, 0 );
            }
          }
          else if(playerRocketActivated == true)
          {
            if(playAudio)
            {
              Mix_PlayChannel( -1, sndPlayerRocketShoot, 0 );
            }
          }
          else
          {
            if(playAudio)
            {
              Mix_PlayChannel( -1, sndPlayerBullet, 0 );
            }
          }
        }
        if(bulletLoaded == false)
        {
          if(doubleBulletActivated == true)
          {
            lastBullet = 0;
            SDL_QueryTexture(texHit, NULL, NULL, &destHit.w, &destHit.h);
            destHit.w *= 4;
            destHit.h *= 4;
            for(int k = 0; k < 2; k++)
            {
              destDoubleBullet[k].y -= 6;

          
              if(destDoubleBullet[k].y <= 0)
              {
                destDoubleBullet[0].y = destCar.y;
                destDoubleBullet[1].y = destCar.y;
                destDoubleBullet[0].x = destCar.x;
                destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);
                bulletLoaded = true;
              }
            }

            destRocket2.x = destCar.x;
            destRocket2.y = destCar.y;

            destBullets.x = (destCar.x +(destCar.w/2));
            destBullets.y = destCar.y;
          }
          else if (playerRocketActivated == true)
          {
            lastBullet = 1;
            SDL_QueryTexture(texExplode, NULL, NULL, &destHit.w, &destHit.h);
            destHit.w *= 8;
            destHit.h *= 8;
            destRocket2.y -= 6;
            if(destRocket2.y <= 0)
            {
              destRocket2.x = destCar.x;
              destRocket2.y = destCar.y;
              bulletLoaded = true;
            }
            destDoubleBullet[0].y = destCar.y;
            destDoubleBullet[1].y = destCar.y;
            destDoubleBullet[0].x = destCar.x;
            destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);
            destBullets.x = (destCar.x +(destCar.w/2));
            destBullets.y = destCar.y;
          }
          else
          {

            lastBullet = 0;
            SDL_QueryTexture(texHit, NULL, NULL, &destHit.w, &destHit.h);
            destHit.w *= 4;
            destHit.h *= 4;
            destBullets.y -= 6;
            if(destBullets.y <= 0)
            {
              destBullets.x = (destCar.x +(destCar.w/2));
              destBullets.y = destCar.y;
              bulletLoaded = true;
            }
            destDoubleBullet[0].y = destCar.y;
            destDoubleBullet[1].y = destCar.y;
            destDoubleBullet[0].x = destCar.x;
            destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);
            destRocket2.x = destCar.x;
            destRocket2.y = destCar.y;
          }

        }

      
      



        if((bulletLoaded == true) && (shoot == 0))
        {
          destBullets.x = (destCar.x +(destCar.w/2));
          destBullets.y = destCar.y;


          destDoubleBullet[0].y = destCar.y;
          destDoubleBullet[1].y = destCar.y;
          destDoubleBullet[0].x = destCar.x;
          destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);

          destRocket2.x = destCar.x;
          destRocket2.y = destCar.y;

        }

          
        //move smashed vehicle on road
        if (destSmashVehicle.y <= WINDOW_HEIGHT + destSmashVehicle.h)
        {
            destSmashVehicle.y+=5;
          }

          

        
        
        
        //move hit effect after some time
        if(hitCountdown <= 0)
        {
          destHit.y = WINDOW_HEIGHT + destHit.h;
        }
        //set the hit enemy effect based on enemy type
        switch (randTruck) {
              case 0:
              case 1:
              SDL_QueryTexture(texExplode, NULL, NULL, &destEnmyHit.w, &destEnmyHit.h);
              destEnmyHit.w *= 8;
                destEnmyHit.h *= 8;
                
              break;
              default:
              SDL_QueryTexture(texHit, NULL, NULL, &destEnmyHit.w, &destEnmyHit.h);
              destEnmyHit.w *= 4;
                destEnmyHit.h *= 4;
              break;
              
            }
            
          
        hitCountdown--;

        //collisions

        for(int i = 0; i < NOW; i++)
        {

          if(has_collided(destCar, destWall[i]))
          {
            playerHealth = 0;
            if(playAudio)
            {
              Mix_PlayChannel( -1, sndPlayerHit, 0 );
            }
          }
        }

        if(has_collided(destCar, destBlock))
        {
          
          playerHealth--;
          blockHealth = 0;
          if(playAudio)
          {
            Mix_PlayChannel( -1, sndPlayerHit, 0 );
          }

        }

        if(has_collided(destCar, destSmashVehicle))
        {
          //close_requested = true;
          playerHealth--;
          destSmashVehicle.y = WINDOW_HEIGHT + destSmashVehicle.h;
          if(playAudio)
          {
            Mix_PlayChannel( -1, sndPlayerHit, 0 );
          }
          
        }

        if(has_collided(destCar, destPowerup))
        {
          destPowerup.y = WINDOW_HEIGHT + destPowerup.h;
          if(playAudio)
          {
            Mix_PlayChannel( -1, sndPickup, 0 );
          }
          if((rand() % 2) == 1)
          {
            doubleBulletActivated = true;
          }
          else
          {
            playerRocketActivated = true;
          }
        
          powerupCooldown = 300;
        }
        if(has_collided(destCar, destRocket))
          {
            playerHealth -= enmyDamage;
            destEnmyHit.x = destRocket.x;
            destEnmyHit.y = destRocket.y - (destEnmyHit.h /2);
            
            enmyHitCountdown = 8;
            destRocket.y = -128;
            truckFire = 16;
            if(playAudio)
            {
              Mix_PlayChannel( -1, sndPlayerHit, 0 );
            }
            switch (randTruck) {
              case 0:
              case 1:
                if(playAudio)
                {
                  Mix_PlayChannel( -1, sndEnemyRocketCollide, 0 );
                }
                
              break;
              default:
                if(playAudio)
                {
                  Mix_PlayChannel( -1, sndBulletCollide, 0 );
                }
              break;
              
            }
          }

        if(has_collided(destBullets, destTruck))
        {
          
          destHit.x = destBullets.x;
          destHit.y = destBullets.y;
          hitCountdown = 5;
          truckHealth--;
          destBullets.x = (destCar.x +(destCar.w/2));
          destBullets.y = destCar.y;
          bulletLoaded = true;

          score += 10;
          if(playAudio)
          {
            Mix_PlayChannel( -1, sndBulletCollide, 0 );
          }
        }

        if(has_collided(destBullets, destRocket))
        {
          destHit.x = destBullets.x;
          destHit.y = destBullets.y;
          hitCountdown = 5;
          destEnmyHit.x = destRocket.x;
          destEnmyHit.y = destRocket.y - (destEnmyHit.h /2);  
          enmyHitCountdown = 8;
          destRocket.y = -128;
          destBullets.x = (destCar.x +(destCar.w/2));
          destBullets.y = destCar.y;
          bulletLoaded = true;
          score += 5;
          truckFire = 16;
        }

        if(has_collided(destBullets, destBlock))
        {
          destHit.x = destBullets.x;
          destHit.y = destBullets.y;
          hitCountdown = 5;
          blockHealth--;
          destBullets.x = (destCar.x +(destCar.w/2));
          destBullets.y = destCar.y;
          bulletLoaded = true;
          score += 5;
          if(playAudio)
          {
            Mix_PlayChannel( -1, sndBulletCollide, 0 );
          }
        }


        for(int k = 0; k < 2; k++)
        {
          if(has_collided(destDoubleBullet[k], destTruck))
          {
            destHit.x = destDoubleBullet[k].x;
          destHit.y = destDoubleBullet[k].y;
          hitCountdown = 5;
            truckHealth--;
            destDoubleBullet[0].y = destCar.y;
            destDoubleBullet[1].y = destCar.y;
            destDoubleBullet[0].x = destCar.x;
            destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);
            score += 5;
            bulletLoaded = true;
            if(playAudio)
            {
              Mix_PlayChannel( -1, sndBulletCollide, 0 );
            }

            
          }

          if(has_collided(destDoubleBullet[k], destRocket))
          {
            destHit.x = destDoubleBullet[k].x;
          destHit.y = destDoubleBullet[k].y;
          hitCountdown = 5;
          destEnmyHit.x = destRocket.x;
          destEnmyHit.y = destRocket.y - (destEnmyHit.h /2);  
          enmyHitCountdown = 8;
            destRocket.y = -128;
            destDoubleBullet[0].y = destCar.y;
            destDoubleBullet[1].y = destCar.y;
            destDoubleBullet[0].x = destCar.x;
            destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);
            bulletLoaded = true;
            score += 5;
            truckFire = 16;
          }

          if(has_collided(destDoubleBullet[k], destBlock))
          {
            destHit.x = destDoubleBullet[k].x;
          destHit.y = destDoubleBullet[k].y;
          hitCountdown = 5;
            blockHealth--;
            destDoubleBullet[0].y = destCar.y;
            destDoubleBullet[1].y = destCar.y;
            destDoubleBullet[0].x = destCar.x;
            destDoubleBullet[1].x = destCar.x + (destCar.w - destDoubleBullet[1].w);
            bulletLoaded = true;
            score += 5;
            if(playAudio)
            {
              Mix_PlayChannel( -1, sndBulletCollide, 0 );
            }
          }


        }

        if(has_collided(destRocket2, destTruck))
        {
          destHit.x = destRocket2.x;
          destHit.y = destRocket2.y;
          hitCountdown = 5;
          truckHealth = 0;
          destRocket2.x = destCar.x;
          destRocket2.y = destCar.y;
          bulletLoaded = true;

          score += 10;
          if(playAudio)
          {
            Mix_PlayChannel( -1, sndPlayerRocketCollide, 0 );
          }
        }

        if(has_collided(destRocket2, destRocket))
        {
          destHit.x = destRocket2.x;
          destHit.y = destRocket2.y;
          hitCountdown = 5;
          destEnmyHit.x = destRocket.x;
          destEnmyHit.y = destRocket.y - (destEnmyHit.h /2);  
          enmyHitCountdown = 8;
          destRocket.y = -128;
          destRocket2.x = destCar.x;
          destRocket2.y = destCar.y;
          bulletLoaded = true;
          truckFire = 16;

          score += 5;
        }

        if(has_collided(destRocket2, destBlock))
        {
          destHit.x = destRocket2.x;
          destHit.y = destRocket2.y;
          hitCountdown = 5;
          blockHealth = 0;
          destRocket2.x = destCar.x;
          destRocket2.y = destCar.y;
          bulletLoaded = true;

          score += 5;
          if(playAudio)
          {
            Mix_PlayChannel( -1, sndPlayerRocketCollide, 0 );
          }
        }

        //handle block when health is 0
        if(blockHealth <= 0)
        {
          destBlock.y = WINDOW_HEIGHT + destBlock.h;
        }
        //handle truck when health is 0
        if(truckHealth <= 0)
        {
          if (destTruck.y != WINDOW_HEIGHT + destTruck.h)
          {
            score += 10;
            lastVehicle = randTruck;
            destSmashVehicle.h = destTruck.h;
            destSmashVehicle.w = destTruck.w;
            destSmashVehicle.y = destTruck.y;
            destSmashVehicle.x = destTruck.x;
            destRocket.y =-128;
            truckFire = 16;

            switch(randTruck) {
                case 0:
                case 1:
                  if(playAudio)
                  {
                    Mix_PlayChannel( -1, sndVanCrash, 0 );
                  }
                  
                break;
                case 2:
                case 3:
                  if(playAudio)
                  {
                    Mix_PlayChannel( -1, sndPoliceCrash, 0 );
                  }
                break;
                default:
                  if(playAudio)
                  {
                    Mix_PlayChannel( -1, sndDroneCrash, 0 );
                  }
                
                break;
              }
          }
          destTruck.y = WINDOW_HEIGHT + destTruck.h;
          truckOnRoad = false;
          
          
          
        }
        //manage cool down for weapon power ups
        
        if(doubleBulletActivated == true || playerRocketActivated == true)
        {
          powerupCooldown--;
        }

        if((powerupCooldown <= 0) && (bulletLoaded == true))
        {
          doubleBulletActivated = false;
          playerRocketActivated = false;
        }

        
        
        
        //handle player when health is -
        if(playerHealth <= 0)
        {
          playerHealth = 0;
          if(score > highScore)
          {
            highScore = score;
            fp = fopen("Content/config", "w+");
            fputs(curFul, fp);
            fputs(curMus, fp);
            fputs(curAud, fp);
            fprintf(fp, "%d", highScore);
            fclose(fp);
          }
          if(playAudio)
          {
            Mix_PlayChannel( -1, sndPlayerCrash, 0 );
          }
          playerAlive = false;
          if(playMusic)
          {
            Mix_HaltMusic();
          bgMusic = Mix_LoadMUS("Content/Audio/DeathMusic.mp3");
          Mix_PlayMusic(bgMusic, -1);
          }
        
          
        }
      }
      //increase difficulty with higher score
      if(score >= ((diffAdder*100) + 100))
      {
        diffAdder++;
      }
      
      //rendering
      //clear Window
        SDL_RenderClear(rend);
      switch(randBack1){
              case 0:
              SDL_RenderCopy(rend, texStreet, NULL, &Background[0]);
              break;
              case 1:
              SDL_RenderCopy(rend, texStreet2, NULL, &Background[0]);
              break;
              case 2:
              SDL_RenderCopy(rend, texStreet3, NULL, &Background[0]);
              break;
              case 3:
              SDL_RenderCopy(rend, texStreet4, NULL, &Background[0]);
              break;
              default:
              SDL_RenderCopy(rend, texStreet5, NULL, &Background[0]);
              break;
            }
            switch(randBack2){
              case 0:
              SDL_RenderCopy(rend, texStreet, NULL, &Background[1]);
              break;
              case 1:
              SDL_RenderCopy(rend, texStreet2, NULL, &Background[1]);
              break;
              case 2:
              SDL_RenderCopy(rend, texStreet3, NULL, &Background[1]);
              break;
              case 3:
              SDL_RenderCopy(rend, texStreet4, NULL, &Background[1]);
              break;
              default:
              SDL_RenderCopy(rend, texStreet5, NULL, &Background[1]);
              break;
            }
        for(int i = 0; i < NOW; i++)
        {


          SDL_RenderCopy(rend, texWall, NULL, &destWall[i]);
        }
        for(int i = 0; i < NOW/2; i++)
        {

            if(i%2==0)
            {
              SDL_RenderCopy(rend, texRoad, NULL, &destRoad[i]);
            }
            else
            {
              SDL_RenderCopy(rend, texRoadLine, NULL, &destRoad[i]);
            }

        }

      if(truckOnRoad == true)
      {
        switch(randTruck) {
                case 0:
                case 1:
                  SDL_RenderCopyEx(rend, texTruck, NULL, &destTruck, enemyRotate, NULL, 0);
                  
                break;
                case 2:
                case 3:
                  SDL_RenderCopyEx(rend, texPolice, NULL, &destTruck, enemyRotate, NULL, 0);
                break;
                default:
                  SDL_RenderCopyEx(rend, texDrone, NULL, &destTruck, enemyRotate, NULL, 0);
                
                break;
              }
        
        
        
          if(truckFire > 8)
          {
            
          }
          else if(truckFire > 0)
          {
            switch(randTruck) {
                case 0:
                case 1:
                  SDL_RenderCopyEx(rend, texTruckFire, NULL, &destTruck, enemyRotate, NULL, 0);
                  
                break;
                case 2:
                case 3:
                  SDL_RenderCopyEx(rend, texPoliceShoot, NULL, &destTruck, enemyRotate, NULL, 0);
                break;
                default:
                  SDL_RenderCopyEx(rend, texDroneShoot, NULL, &destTruck, enemyRotate, NULL, 0);
                
                break;
              }
          }
      }
        if(bulletLoaded == false)
        {
          if(doubleBulletActivated == true)
          {
            
            
              SDL_RenderCopy(rend, texDoubleBullet, NULL, &destDoubleBullet[0]);
              SDL_RenderCopy(rend, texDoubleBullet, NULL, &destDoubleBullet[1]);

             
          
              

          
          }
          else if (playerRocketActivated == true)
          {
            
            SDL_RenderCopy(rend, texRocket2, NULL, &destRocket2);
            
            
          }
          else
          {

            
            SDL_RenderCopy(rend, texBullet, NULL, &destBullets);
            

          }
        }
          if(doubleBulletActivated == true)
          {
            
            
             
              get_and_update_hud(((powerupCooldown/60) + 1), rend, font, Green, 3, 0, 430, 100, 50);
          
              

          
          }
          else if (playerRocketActivated == true)
          {
            
           
            get_and_update_hud(((powerupCooldown/60) + 1), rend, font, Green, 4, 0, 430, 100, 50);
            
          }
         switch(lastVehicle) {
                case 0:
                case 1:
                  SDL_RenderCopy(rend, texTruckWreck, NULL, &destSmashVehicle);
                  
                break;
                case 2:
                case 3:
                  SDL_RenderCopy(rend, texPoliceWreck, NULL, &destSmashVehicle);
                break;
                default:
                  SDL_RenderCopy(rend, texDroneWreck, NULL, &destSmashVehicle);
                
                break;
              }
            switch (randTruck) {
              case 0:
              case 1:
                SDL_RenderCopy(rend, texExplode, NULL, &destEnmyHit);
                
              break;
              default:
                SDL_RenderCopy(rend, texHit, NULL, &destEnmyHit);
              break;
              
            }
        
          if(playerHealth > 0)
        {

        
          SDL_RenderCopyEx(rend, texCar, NULL, &destCar, playerRotate, NULL, 0);
        }
        else
        {
          SDL_RenderCopyEx(rend, texCarWreck, NULL, &destCar, playerRotate, NULL, 0);
        }
          SDL_RenderCopy(rend, texBlock, NULL, &destBlock);

        SDL_RenderCopy(rend, texPowerup, NULL, &destPowerup);
        switch (lastBullet) {
          case 0: 
            SDL_RenderCopy(rend, texHit, NULL, &destHit);
          break;
          case 1:
            SDL_RenderCopy(rend, texExplode, NULL, &destHit);
          break;
        }
          switch(randTruck) {
                case 0:
                case 1:
                  SDL_RenderCopy(rend, texRocket, NULL, &destRocket);
                  enmyDamage = 3;
                break;
                case 2:
                case 3:
                  SDL_RenderCopy(rend, texPoliceBullet, NULL, &destRocket);
                  enmyDamage = 2;
                break;

                default:
                  SDL_RenderCopy(rend, texDroneBullet, NULL, &destRocket);
                  enmyDamage = 1;
                break;
              }
          get_and_update_hud(score, rend, font, White, 1, 0, 0, 100, 50);
          get_and_update_hud(playerHealth, rend, font, Red, 2, 540, 0, 100, 50);
          SDL_RenderCopy(rend, texOverlay, NULL, &destOverlay);
      
        if(pause == true)
        {
          SDL_RenderCopy(rend, texPause, NULL, &destTextOverlay);
        }
        if (playerAlive == false)
        {

          
          SDL_RenderCopy(rend, texGameOver, NULL, &destTextOverlay);
          
          
        }
        break;
        
      }
      
      
      
      
      

      SDL_RenderPresent(rend);
      //delay to 60fps
      SDL_Delay(1000/60);
      if(countdown >= 0)
      {
        countdown--;
      }


     
    }

 


    //Clean program
    SDL_DestroyTexture(texCar);
    SDL_DestroyTexture(texWall);
    SDL_DestroyTexture(texRoad);
    SDL_DestroyTexture(texRoadLine);
    SDL_DestroyTexture(texTruck);
    SDL_DestroyTexture(texTruckFire);
    SDL_DestroyTexture(texRocket);
    SDL_DestroyTexture(texPowerup);
    SDL_DestroyTexture(texDoubleBullet);
    SDL_DestroyTexture(texRocket2);
    SDL_DestroyTexture(texStreet);
    SDL_DestroyTexture(texStreet2);
    SDL_DestroyTexture(texStreet3);
    SDL_DestroyTexture(texStreet4);
    SDL_DestroyTexture(texStreet5);
    SDL_DestroyTexture(texPolice);
    SDL_DestroyTexture(texPoliceShoot);
    SDL_DestroyTexture(texPoliceBullet);
    SDL_DestroyTexture(texDrone);
    SDL_DestroyTexture(texDroneShoot);
    SDL_DestroyTexture(texDroneBullet);
    SDL_DestroyTexture(texTruckWreck);
    SDL_DestroyTexture(texPoliceWreck);
    SDL_DestroyTexture(texDroneWreck);
    SDL_DestroyTexture(texHit);
    SDL_DestroyTexture(texOverlay);
    SDL_DestroyTexture(texMenu);
    SDL_DestroyTexture(texPause);
    SDL_DestroyTexture(texGameOver);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_DestroyTexture(texFullscreen);
    SDL_DestroyTexture(texMusic);
    SDL_DestroyTexture(texAudio);
    SDL_DestroyTexture(texOnText);
    SDL_DestroyTexture(texOffText);
    SDL_DestroyTexture(texVersion);
    TTF_CloseFont(font);
    Mix_FreeChunk(sndBulletCollide);
    Mix_FreeChunk(sndPlayerBullet);
    Mix_FreeChunk(sndDoubleBullet);
    Mix_FreeChunk(sndDrone);
    Mix_FreeChunk(sndPoliceShoot);
    Mix_FreeChunk(sndPlayerRocketCollide);
    Mix_FreeChunk(sndPlayerRocketShoot);
    Mix_FreeChunk(sndEnemyRocketShoot);
    Mix_FreeChunk(sndEnemyRocketCollide);
    Mix_FreeChunk(sndPlayerHit);
    Mix_FreeChunk(sndPlayerCrash);
    Mix_FreeChunk(sndVanCrash);
    Mix_FreeChunk(sndPoliceCrash);
    Mix_FreeChunk(sndDroneCrash);
    Mix_FreeChunk(sndPickup);
    SDL_Quit();
    Mix_Quit();
    
 }
