#include "animated_sprite.h"
#include "graphics.h"
#include "sprite.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>  

using namespace std;

AnimatedSprite::AnimatedSprite() {

}

AnimatedSprite::AnimatedSprite(Graphics& graphics, const string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, float timeToUpdate)
    : Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY) {
    _timeToUpdate = timeToUpdate;
    _visible = true;
    _currentFrame = 0;
    _currentAnimationOnce = false;
    _timeElapsed = 0;
    _currentAnimation = "";
}

void AnimatedSprite::addAnimation(int frames, int x, int y, string name, int width, int height, Vector2D offset) {
    vector<SDL_FRect> rectangles;
    for (int i = 0; i < frames; i++) {
        SDL_FRect newRect = { (float)(x + i * width), (float)y, (float)width, (float)height };
        rectangles.push_back(newRect);
    }
    _animations.insert(pair<string, vector<SDL_FRect>>(name, rectangles));
    _offsets.insert(pair<string, Vector2D>(name, offset));
}

void AnimatedSprite::resetAnimation() {
    _animations.clear(); 
    _offsets.clear();
}

void AnimatedSprite::playAnimation(string animation, bool once) {
    if (_animations.count(animation) == 0) {
        cerr << "No animation found with name: " << animation << endl;
        return;
    }
    
    _currentAnimationOnce = once;
    if (_currentAnimation != animation) {
        _currentAnimation = animation;
        _currentFrame = 0;
        _timeElapsed = 0;
    }
}

void AnimatedSprite::setVisible(bool visible) {
    _visible = visible;
}

void AnimatedSprite::stopAnimation() {
    _currentFrame = 0;
    animationDone(_currentAnimation);
    //_currentAnimation = "";
}

void AnimatedSprite::update(float elapsedTime) {
    Sprite::update();
    _timeElapsed += elapsedTime;
    if (_timeElapsed > _timeToUpdate) {
        _timeElapsed -= _timeToUpdate;
        if (_currentFrame < (int)_animations[_currentAnimation].size() - 1) {
            _currentFrame++;
        } else {
            if (_currentAnimationOnce == true) {
                setVisible(false);
            }
            stopAnimation();
        }
    }
}

void AnimatedSprite::draw(Graphics& graphics, int x, int y) {
    if (_visible) {
        SDL_FRect destinationRect;
        Vector2D offset = _offsets[_currentAnimation];
        destinationRect.x = x + (offset.x * globals::SPRITE_SCALE);
        destinationRect.y = y + (offset.y * globals::SPRITE_SCALE);
        //destinationRect.w = _animations[_currentAnimation][_currentFrame].w * globals::SPRITE_SCALE;
        //destinationRect.h = _animations[_currentAnimation][_currentFrame].h * globals::SPRITE_SCALE;
        destinationRect.w = _sourceRect.w * globals::SPRITE_SCALE;
        destinationRect.h = _sourceRect.h * globals::SPRITE_SCALE;

        SDL_FRect sourceRect = _animations[_currentAnimation][_currentFrame];
        graphics.blitSurface(_spriteSheet, &sourceRect, &destinationRect);
    }
}

/*void AnimatedSprite::setUpAnimations() {
    // Running animation - 3 frames horizontally
    // Assuming each frame is 96x128 pixels (check the actual spritesheet dimensions)
    addAnimation(3, 0, 128, "RunRight", 96, 128, Vector2D(0, 0));  // Second row of sprites
    addAnimation(3, 0, 256, "RunLeft", 96, 128, Vector2D(0, 0));   // Third row (if available)
    addAnimation(1, 0, 0, "Idle", 96, 128, Vector2D(0, 0));        // First frame as idle
}*/ 
