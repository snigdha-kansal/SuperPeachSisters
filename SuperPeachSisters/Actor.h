#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor : public GraphObject
{
public:
    Actor(StudentWorld *sw, int imageID, int x, int y, int dir, int depth, double size);

    virtual void doSomething(){};
    virtual void bonk(){};

    StudentWorld *getWorld() { return m_world; };

    void set_alive(bool alive) { m_alive = alive; };
    bool is_alive() { return m_alive; };

    void set_blocks(bool blocks) { m_blocks = blocks; };
    bool blocks() { return m_blocks; };

    void set_damageable(bool damageable) { m_damageable = damageable; };
    bool is_damageable() { return m_damageable; };

    void set_bonkable(bool bonkable) { m_bonkable = bonkable; };
    bool is_bonkable() { return m_bonkable; };

    void set_damaged(bool damaged) { m_damaged = damaged; };
    bool is_damaged() { return m_damaged; };

    virtual ~Actor(){};

private:
    StudentWorld *m_world;
    bool m_alive;
    bool m_blocks;
    bool m_damageable;
    bool m_bonkable;
    bool m_damaged;
};

class Peach : public Actor
{
public:
    Peach(StudentWorld *sw, int x, int y);

    virtual void doSomething();
    virtual void bonk();

    void set_healthpoint(int healthpoint) { m_healthpoint = healthpoint; };
    int get_healthpoint() { return m_healthpoint; };

    void set_temp_invincible(int temp_invincible) { m_temp_invincible = temp_invincible; };
    int is_temp_invincible() { return m_temp_invincible; };

    void set_recharge(bool recharge) { m_recharge = recharge; };
    bool is_recharge() { return m_recharge; };

    void set_jump(bool jump) { m_jump = jump; };
    bool is_jump() { return m_jump; };

    void set_shoot_power(bool shoot)
    {
        shoot_power = shoot;
        m_healthpoint = 2;
    };
    bool get_shoot_power() { return shoot_power; };
    void set_star_power(bool star)
    {
        star_power = star;
        m_invincible = 150;
    };
    bool get_star_power() { return star_power; };
    void set_jump_power(bool jump)
    {
        jump_power = jump;
        m_healthpoint = 2;
    };
    bool get_jump_power() { return jump_power; };

    void set_mario_reached(bool mario) { mario_reached = mario; };
    bool has_mario_reached() { return mario_reached; };

    void set_flag_reached(bool flag) { flag_reached = flag; };
    bool has_flag_reached() { return flag_reached; };

    virtual ~Peach(){};

private:
    int m_healthpoint;

    int m_temp_invincible;

    bool jump_power;

    bool shoot_power;
    int to_recharge_before_next_fire;
    bool m_recharge;

    bool star_power;
    int m_invincible;

    bool m_jump;
    int remain_jump_dist;

    bool flag_reached;
    bool mario_reached;
};

class Obstacle : public Actor
{
public:
    Obstacle(StudentWorld *sw, int imageID, int x, int y);

private:
};

class Block : public Obstacle
{
public:
    Block(StudentWorld *sw, int x, int y);
    virtual void bonk();

    void set_goodie(int goodie) { m_goodie = goodie; };
    int get_goodie() { return m_goodie; };

    void set_released(bool released) { m_released = released; };
    bool is_released() { return m_released; };

    virtual ~Block(){};

private:
    int m_goodie;
    bool m_released;
};

class Pipe : public Obstacle
{
public:
    Pipe(StudentWorld *sw, int x, int y);
    virtual void bonk();

    virtual ~Pipe(){};

private:
};

class LevelEnder : public Actor
{
public:
    LevelEnder(StudentWorld *sw, int imageID, int x, int y);
    virtual void doSomething(){};

    virtual ~LevelEnder(){};

private:
};

class Flag : public LevelEnder
{
public:
    Flag(StudentWorld *sw, int x, int y);
    virtual void doSomething();

    virtual ~Flag(){};
};

class Mario : public LevelEnder
{
public:
    Mario(StudentWorld *sw, int x, int y);
    virtual void doSomething();

    virtual ~Mario(){};
};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld *sw, int imageID, int x, int y);
    virtual void bonk(){};
    virtual void doSomething(){};

    virtual ~Goodie(){};

private:
};

class Flower : public Goodie
{
public:
    Flower(StudentWorld *sw, int x, int y);
    virtual void doSomething();

    virtual ~Flower(){};

private:
};

class Mushroom : public Goodie
{
public:
    Mushroom(StudentWorld *sw, int x, int y);
    virtual void doSomething();

    virtual ~Mushroom(){};

private:
};

class Star : public Goodie
{
public:
    Star(StudentWorld *sw, int x, int y);
    virtual void doSomething();

    virtual ~Star(){};

private:
};

class Projectile : public Actor
{
public:
    Projectile(StudentWorld *sw, int imageID, int x, int y, int dir);

private:
};

class Piranhafb : public Projectile
{
public:
    Piranhafb(StudentWorld *sw, int x, int y, int dir);
    virtual void doSomething();

    virtual ~Piranhafb(){};

private:
};

class Peachfb : public Projectile
{
public:
    Peachfb(StudentWorld *sw, int x, int y, int dir);
    virtual void doSomething();

    virtual ~Peachfb(){};

private:
};

class Shell : public Projectile
{
public:
    Shell(StudentWorld *sw, int x, int y, int dir);
    virtual void doSomething();

    virtual ~Shell(){};

private:
};

class Enemy : public Actor
{
public:
    Enemy(StudentWorld *sw, int imageID, int x, int y);
    virtual void doSomething(){};
    virtual void bonk();

    virtual ~Enemy(){};

private:
};

class Goomba : public Enemy
{
public:
    Goomba(StudentWorld *sw, int x, int y);
    virtual void doSomething();

    virtual ~Goomba(){};

private:
};

class Koopa : public Enemy
{
public:
    Koopa(StudentWorld *sw, int x, int y);
    virtual void doSomething();

    virtual ~Koopa(){};

private:
};

class Piranha : public Enemy
{
public:
    Piranha(StudentWorld *sw, int x, int y);
    virtual void doSomething();

    int firing_delay() { return m_firing_delay; };

    virtual ~Piranha(){};

private:
    int m_firing_delay;
};

#endif // ACTOR_H_
