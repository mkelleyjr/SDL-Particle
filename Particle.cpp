/* Michael L. Kelley Jr.
   January 2, 2008
   Particle system using SDL
*/

  // Include files 

    #include <iostream>
    #include <SDL/SDL.h>
    #include "SDL/SDL_image.h"
    #include <string>
    #include <cstdlib>
    
    using namespace std;
    
    
  // Variables
  
    const int Screen_Height = 480;
    const int Screen_Width = 640;
    const int Screen_BPP = 32;
    
    // Frame rate
    
     const int F_P_S = 20;
     
    // Dot
    
     const int Dot_Height = 20;
     const int Dot_Width = 20;
     
    // Particles
    
     const int Total_Particles = 20;
    
    // SDL surface
    
    SDL_Surface *dot = NULL;
    SDL_Surface *red = NULL;
    SDL_Surface *green = NULL;
    SDL_Surface *blue = NULL;
    SDL_Surface *shine = NULL;
    SDL_Surface *screen = NULL; 
    
    // SDL event
    
    SDL_Event event;
    
    // Particle class
    
      class Particle
      { 
            
            private:
                    int x, y;
                    
                    int frame;
                    
                    SDL_Surface *type;
                    
            public:
                   Particle(int x, int y); 
                   
                   void Show();
                   
                   bool is_Dead();
                   
       };
       
     // Dot class 
     
     class Dot
     {
           private:
                   int x, y;
                   
                   int xVelocity, yVelocity;
                   
                   Particle *Particles [Total_Particles];
                   
           public:
                  Dot();
                  
                  ~Dot(); // Cleanup
                  
                  void Handle_Input();
                  
                  void Move();
                  
                  void Show_Particles();
                  
                  void Show();
                  
       };
       
      // Timer class
      
      class Timer
      {
            private:
                    int StartTicks; 
                    
                    int PausedTicks;
                    
                    // Status
                    
                    bool Paused;
                    bool Started;
                    
            public:
                   Timer();
                   
                   // Timer stuff
                   
                   void Start();
                   void Stop();
                   void Pause();
                   void Unpause(); 
                   
                   int Get_Ticks();
                   
                   bool is_Started();
                   bool is_Paused(); 
        };
        
        SDL_Surface *load_image(std::string filename)
        {
                    SDL_Surface* loadedImage = NULL;
                    
                    SDL_Surface* optimizedImage = NULL;
                    
                    loadedImage = IMG_Load(filename.c_str() );
                    
                    // Check to see if image loaded
                    
                    if (loadedImage != NULL)
                    {
                                    optimizedImage = SDL_DisplayFormat (loadedImage);
                                    
                                    SDL_FreeSurface(loadedImage);
                                    
                     if (optimizedImage != NULL)
                     {
                                       SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage -> format, 0, 0xFF, 0xFF ));
                                       
                                       }
                     }
                     
                     // Return optimized surface
                     
                     return optimizedImage;
        }
        
        void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
        {
             SDL_Rect offset;
             
             offset.x = x;
             offset.y = y;
             
             SDL_BlitSurface(source, clip, destination, &offset);
        }
        
        bool Init()
        {
             
             // Start SDL
             
             if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
             {
                      return false;
             }
             
             screen = SDL_SetVideoMode(Screen_Width, Screen_Height, Screen_BPP, SDL_SWSURFACE);
             
             // Check for screen error
             
             if (screen == NULL)
             {
                        return false;
             }
             
             // Window caption
             
             SDL_WM_SetCaption("Particle System", NULL);
             
             // Random
             
             srand (SDL_GetTicks() );
             
             return true;
        }
        
        bool Load_Files()
        {
             // Load dot
             
             dot = load_image("image/dot.bmp");
             
             if (dot == NULL)
             {
                     return false;
             }
             
             // Load particles 
             
            red = load_image("image/red.bmp");
            green = load_image("image/green.bmp");
            blue = load_image("image/blue.bmp");
            shine = load_image("image/shine.bmp");
             
             if ((shine == NULL) || (blue == NULL) || (green == NULL) || (red == NULL))
             {
                    return false;
             }
             
             // Set alpha
             
             SDL_SetAlpha(red, SDL_SRCALPHA | SDL_RLEACCEL, 192);
             SDL_SetAlpha(green, SDL_SRCALPHA | SDL_RLEACCEL, 192);
             SDL_SetAlpha(blue, SDL_SRCALPHA | SDL_RLEACCEL, 192);
             SDL_SetAlpha(shine, SDL_SRCALPHA | SDL_RLEACCEL, 192);
             
             return true;
        }
        
        void Free_Up()
        {
             // Free surfaces
             
             SDL_FreeSurface(dot);
             SDL_FreeSurface(red);
             SDL_FreeSurface(green);
             SDL_FreeSurface(blue);
             SDL_FreeSurface(shine);
             
             // Exit SDL
             
             SDL_Quit();
             
        }
        
        Particle::Particle(int X, int Y)
        {
            x = X - 5 + (rand() % 25);
            y = Y - 5 + (rand() % 25);
            
            // Start animation
            
            frame = rand() % 5;
            
            switch (rand() % 3)
            {
                   case 0: type = red; break;
                   case 1: type = green; break;
                   case 2: type = blue; break;
                   
                   }
            }
            
            void Particle::Show()
            {
                 apply_surface(x, y, type, screen);
                 
                 if (frame % 2 == 0)
                 {
                           apply_surface(x,y, shine, screen);
                 }
                 
                 // Animate
                 
                 frame++;
                 
        }
        
        bool Particle:: is_Dead()
        {
             if (frame > 10)
             {
                       return true;
             }
             
             return false;
        }
        
        Dot::Dot()
        {
                  
                  x = 0;
                  y = 0;
                  
                  xVelocity = 0;
                  yVelocity = 0;
                  
                  for (int p = 0; p <Total_Particles; p++)
                  {
                      Particles[p] = new Particle(x,y);
                      }
                  }
                  
        Dot::~Dot()
        {
                   // Delete particles
                   
                   for (int p = 0; p <Total_Particles; p++)
                   {
                       delete Particles[p];
                   }
                   
        }
           
        void Dot::Handle_Input()
        {
             if(event.type == SDL_KEYDOWN)
             {
                           switch (event.key.keysym.sym)
                           {
                                  case SDLK_UP: yVelocity -= Dot_Height / 2; break;
                                  case SDLK_DOWN: yVelocity += Dot_Height / 2; break;
                                  case SDLK_LEFT: xVelocity -= Dot_Width / 2; break;
                                  case SDLK_RIGHT: xVelocity += Dot_Width /2; break;
                                  
                                  }
                           }
                           
                           // Key released
                           
                          else if (event.type = SDL_KEYUP)
                             {
                                  
                                  switch (event.key.keysym.sym)
                                  {
                                         
                                    case SDLK_UP: yVelocity += Dot_Height / 2; break;
                                    case SDLK_DOWN: yVelocity -= Dot_Height / 2; break;
                                    case SDLK_LEFT: xVelocity += Dot_Width / 2; break;
                                    case SDLK_RIGHT: xVelocity -= Dot_Width / 2; break; 
                                    
                                 }
                        }
            }  
                                
             void Dot::Move()
             {
                  
              // Left or right
                
              x += xVelocity;
             
             if ((x < 0) || (x + Dot_Width > Screen_Width))
             {
                    x -= xVelocity;
             }
             
               // Up or down
               
               y += yVelocity;
               
               if (( y < 0) || (y + Dot_Height > Screen_Height))
               {
                     y -= yVelocity;
               }
               
        }
        
        void Dot::Show_Particles()
        {
             for (int p = 0; p < Total_Particles; p++)
             {
                 // Get rid of dead particles
                 
                 if (Particles[p] -> is_Dead() == true)
                 {
                                  delete Particles[p];
                                  
                                  Particles[p] = new Particle(x,y);
                 }
             }
             
             // Show the particles
             
             for (int p = 0; p < Total_Particles; p++)
             {
                 Particles[p]->Show();
                 }
             }
             
             void Dot::Show()
             {
                  apply_surface(x, y, dot, screen);
                  
                  Show_Particles();
                  
                  }
                  
             Timer::Timer()
             {
                           StartTicks = 0;
                           PausedTicks = 0;
                           Paused = false;
                           Started = false;
             }
             
             void Timer::Start()
             {
                  
                  Started = true;
                  Paused = false;
                  
                  StartTicks = SDL_GetTicks();
             }
             
             void Timer::Stop()
             {
                  
                  Started = false;
                  Paused = false;
             }
             
             void Timer::Pause()
             {
                  if((Started == true) && (Paused == false))
                  {
                              Paused = true;
                              
                              PausedTicks = SDL_GetTicks() - StartTicks;
                              
                              }
               
               }
               
               void Timer::Unpause()
               {
                    if(Paused == true)
                    {
                              Paused = false;
                              
                              StartTicks = SDL_GetTicks() - PausedTicks;
                              
                              PausedTicks = 0;
                              
                              }
                    } 
                    
               int Timer::Get_Ticks()
               {
                   if(Started == true)
                   {
                              if(Paused == true)
                              {
                                        
                                        return PausedTicks;
                                        
                                        }
                              else
                              {
                                  return SDL_GetTicks() - StartTicks;
                                  
                                  }
                                  
                               }
                                  
                   return 0;
                   
               }
               
               bool Timer::is_Started()
               {
                    return Started;   
               }
               
               bool Timer::is_Paused()
               {
                    return Paused;
               }
               
               int main(int argc, char* args[])
               {
                   bool Quit = false;
                   
                   Dot MyDot;
                   
                   Timer fps;
                   
                   if(Init() == false)
                   {
                             return 1;
                   }
                   
                   if(Load_Files() == false)
                   {
                                   return 1;
                   }
                   
                   while (Quit == false)
                   {
                         fps.Start();
                         
                         while(SDL_PollEvent(&event))
                         {
                                                     MyDot.Handle_Input();
                                                     
                                                     if(event.type == SDL_QUIT)
                                                     {
                                                                   Quit = true;
                                                                   
                                                                   }
                                                     }  
                                                     
                         MyDot.Move();
                         
                         SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
                         
                         MyDot.Show();
                         
                         if(SDL_Flip(screen) == -1)
                         {
                                             return 1;
                         }
                         
                         if(fps.Get_Ticks() < 1000 / F_P_S)
                         {
                                              SDL_Delay((1000 / F_P_S) - fps.Get_Ticks() );
                                              
                                              }
                                              
                         }
                         
                         // Clean up everything
                         
                         Free_Up();
                         
                         return 0;
                         
             }
                                               
                   
                   
               
               
                              
             
        
                     
                     
                     
