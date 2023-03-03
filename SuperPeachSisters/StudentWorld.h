#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

class Actor;
class Peach;
class Block;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  bool overlap(int x, int y, int dir); // blocks
  bool support(int x, int y);
  bool fall(int x, int y);
  void addobject(Actor *act);
  bool goodie_supported(int x, int y);
  bool goodie_blocked(int x, int y, int dir);
  bool mushroom_overlap(int x, int y, int dir);
  bool flower_overlap(int x, int y, int dir);
  bool star_overlap(int x, int y, int dir);
  // levelender overlap
  bool levelender_overlap(int x, int y);
  bool levelender2_overlap(int x, int y);
  bool piranhafb_overlap(int x, int y, int dir);
  bool peachfb_or_shell_overlap(Actor *act);
  bool enemy_supported(int x, int y, int dir);
  bool within_peach(Actor *act);
  bool within_peach_2(Actor *act);
  ~StudentWorld();

private:
  Peach *m_peach;
  std::vector<Actor *> m_actors;
};

#endif /* temp_2_hpp */
