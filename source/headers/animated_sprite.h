#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "sprite.h"
#include "globals.h"
#include <string>
#include <map>
#include <vector>
#include <SDL3/SDL.h>
#include <cmath>

using namespace std;

class Graphics; 

class AnimatedSprite : public Sprite {
    // Future implementation for animated sprites
    public:
        AnimatedSprite();
        AnimatedSprite(Graphics& graphics, const string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, float timeToUpdate);
        //~AnimatedSprite();

        /*
        Plays the specified animation. If 'once' is true, the animation will play only once and then stop.
        */
        void playAnimation(string animation, bool once = false);

        void update(float elapsedTime);

        void draw(Graphics& graphics, int x, int y);

    protected:
        double _timeToUpdate; 
        bool _currentAnimationOnce;
        string _currentAnimation; 

        void addAnimation(int frames, int x, int y, string name, int width, int height, Vector2D offset);
        void resetAnimation();
        void stopAnimation();
        void setVisible(bool visible);
        bool isVisible() const;

        void setFlipped(bool flipped);
        bool isFlipped() const;

        /*
        Called when an animation is finished playing.
        */
        virtual void animationDone(string currentAnimation) = 0; // Pure virtual function to be implemented by derived classes
        /*
        Sets up the animations for the sprite. This method should be overridden in derived classes to define specific animations.
        */
        virtual void setUpAnimations() = 0; // Pure virtual function to be implemented by derived classes

    private:
        map<string, vector<SDL_FRect>> _animations;
        map<string, Vector2D> _offsets; // Map of animation names to their offsets
        //string _currentAnimation; // Name of the current animation
        int _currentFrame; // Index of the current frame in the animation
        float _timeElapsed; // Time elapsed since the last frame update
        bool _visible; // Whether the sprite is visible
        bool _flipped; // Whether the sprite is flipped horizontally

        //void addAnimation(int frames, int x, int y, string name, int width, int height, Vector2D offset); // Helper method to add an animation
        //void resetAnimation(); // Resets the current animation to the first frame

};


#endif // ANIMATED_SPRITE_H