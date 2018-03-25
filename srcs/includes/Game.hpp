//
//  Game.hpp 
//  IslandDefense
//
//  Created by Mathieu Corti on 3/12/18.
//

#pragma once

#include "../helpers/Glut.hpp"
#include "../helpers/OpenGL.hpp"

#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory>

#include "../helpers/Displayable.hpp"
#include "../helpers/Movable.hpp"
#include "Boat.hpp"

class Game {

// TYPEDEFS
private:
  typedef std::map<unsigned char, std::function<void(int x, int y)>> KeyboardMap;
  typedef std::map<std::string, Displayable::Ptr> EntityList;

public:
  static Game &getInstance() {
    static Game instance;
    return instance;
  }

  int start(int argc, char **argv);

  void draw();

  void keyboard(unsigned char key, int x, int y) const;

  float getTime() const;

  float getFrameRate() const;

private:
  KeyboardMap _keyboardMap;
  EntityList _entities;
  float _time, _lastTime, _deltaTime = 0.0;
  float _lastFrameRateT, _frameRateInterval, _frameRate, _frames;

  void initDrawCallback() const;

  void initKeyboardCallback() const;

  void initKeyboardMap();

  void initGlut();

  void initEntities();

  void update();

  void updateTime();

  static void idleFunc();

  // Helpers

  void move(std::string entityName, Direction direction) {
    std::dynamic_pointer_cast<Movable>(_entities[entityName])->move(direction);
  }

  void changeCannonPower(std::string entityName, float delta) {
    std::dynamic_pointer_cast<Boat>(_entities[entityName])->getCannon()->speed(delta);
  }

  void changeCannonDirection(std::string entityName, float delta) {
    std::dynamic_pointer_cast<Boat>(_entities[entityName])->getCannon()->rotation(delta);
  }

  void fire(std::string entityName) {
    std::string projectileName(entityName + "_projectile");
    if (_entities.find(projectileName) == _entities.end()) {
      _entities.insert(std::make_pair(projectileName,
                                      std::dynamic_pointer_cast<Boat>(
                                        _entities[entityName])->getCannon()->blast()));
    }
  }

  // Singleton
  Game() = default;

  ~Game() = default;

  Game(const Game &) = delete;

  Game &operator=(const Game &) = delete;
};

// Extern C
extern "C" {
static void drawCallback();
static void keyboardCallback(unsigned char key, int x, int y);
}