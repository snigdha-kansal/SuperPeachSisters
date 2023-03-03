#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

Actor::Actor(StudentWorld *sw, int imageID, int x, int y, int dir, int depth, double size)
    : GraphObject(imageID, x, y, dir, depth, size), m_world(sw)
{
    set_alive(true);
    set_damaged(false);
}

Peach::Peach(StudentWorld *sw, int x, int y)
    : Actor(sw, IID_PEACH, x, y, 0, 0, 1.0)
{
    set_healthpoint(1);
    set_blocks(false);
    set_bonkable(true);
    set_damageable(true);
    set_jump(false);
    set_recharge(false);
    remain_jump_dist = 0;
    jump_power = false;
    star_power = false;
    shoot_power = false;
    m_temp_invincible = 0;
    m_invincible = 0;
    to_recharge_before_next_fire = 0;
    set_flag_reached(false);
    set_mario_reached(false);
}

Obstacle::Obstacle(StudentWorld *sw, int imageID, int x, int y)
    : Actor(sw, imageID, x, y, 0, 2, 1)
{
    set_blocks(true);
    set_bonkable(true);
    set_damageable(false);
}

Block::Block(StudentWorld *sw, int x, int y)
    : Obstacle(sw, IID_BLOCK, x, y)
{
    set_released(false);
    set_goodie(0);
}

Pipe::Pipe(StudentWorld *sw, int x, int y)
    : Obstacle(sw, IID_PIPE, x, y)
{
}

LevelEnder::LevelEnder(StudentWorld *sw, int imageID, int x, int y)
    : Actor(sw, imageID, x, y, 0, 1, 1)
{
    set_blocks(false);
    set_bonkable(false);
    set_damaged(false);
}

Flag::Flag(StudentWorld *sw, int x, int y)
    : LevelEnder(sw, IID_FLAG, x, y)
{
}

Mario::Mario(StudentWorld *sw, int x, int y)
    : LevelEnder(sw, IID_MARIO, x, y)
{
}

Goodie::Goodie(StudentWorld *sw, int imageID, int x, int y)
    : Actor(sw, imageID, x, y, 0, 1, 1)
{
    set_blocks(false);
    set_bonkable(true);
    set_damageable(false);
}

Flower::Flower(StudentWorld *sw, int x, int y)
    : Goodie(sw, IID_FLOWER, x, y)
{
}

Mushroom::Mushroom(StudentWorld *sw, int x, int y)
    : Goodie(sw, IID_MUSHROOM, x, y)
{
}

Star::Star(StudentWorld *sw, int x, int y)
    : Goodie(sw, IID_STAR, x, y)
{
}

Projectile::Projectile(StudentWorld *sw, int imageID, int x, int y, int dir)
    : Actor(sw, imageID, x, y, dir, 1, 1)
{
    set_blocks(false);
    set_bonkable(true);
    set_damageable(false);
}

Piranhafb::Piranhafb(StudentWorld *sw, int x, int y, int dir)
    : Projectile(sw, IID_PIRANHA_FIRE, x, y, dir)
{
}

Peachfb::Peachfb(StudentWorld *sw, int x, int y, int dir)
    : Projectile(sw, IID_PEACH_FIRE, x, y, dir)
{
}

Shell::Shell(StudentWorld *sw, int x, int y, int dir)
    : Projectile(sw, IID_SHELL, x, y, dir)
{
}

Enemy::Enemy(StudentWorld *sw, int imageID, int x, int y)
    : Actor(sw, imageID, x, y, (randInt(0, 1)) * 180, 1, 0)
{
    set_blocks(false);
    set_bonkable(true);
    set_damageable(true);
}

Goomba::Goomba(StudentWorld *sw, int x, int y)
    : Enemy(sw, IID_GOOMBA, x, y)
{
}

Koopa::Koopa(StudentWorld *sw, int x, int y)
    : Enemy(sw, IID_KOOPA, x, y)
{
}

Piranha::Piranha(StudentWorld *sw, int x, int y)
    : Enemy(sw, IID_PIRANHA, x, y)
{
    m_firing_delay = 0;
}

void Peach::doSomething()
{
    if (!(is_alive()))
    {
        return;
    }
    if (star_power == true)
    {
        m_invincible--;
        if (m_invincible == 0)
        {
            star_power = false;
        }
        // Decrement the number of remaining game ticks before she loses this invincibility power. If this tick count reaches zero, Peach must set her invincibility status to off.
    }
    if (is_temp_invincible() > 0)
    {
        m_temp_invincible--;
        // decrement the number of remaining game ticks before she loses temporary invincibility. If this tick count reaches zero, Peach must set her temporary invincibility status to false. (Peach gains temporary invincibility if she overlaps with an enemy while she has Jump Power or Fire Power.)
    }
    if (to_recharge_before_next_fire > 0)
    {
        to_recharge_before_next_fire--;
        // If the number of time_to_recharge_before_next_fire ticks is greater than zero, she must decrement this tick count by one. If the tick count reaches zero, then Peach may again shoot a fireball (if she has Shoot Power).
    }

    // overlap in getworld and bonk in specific actor
    getWorld()->overlap(getX(), getY(), 0);

    if (is_jump() && remain_jump_dist > 0)
    {
        // x, y+4
        if (getWorld()->overlap(getX(), getY() + 4, 0) == true)
        {
            remain_jump_dist = 0;
            set_jump(false);
        }
        else
        {
            moveTo(getX(), getY() + 4);
            remain_jump_dist--;
        }
        // if block, bonk in specific actor
        // and remain_jump_dist = 0;
        // else, moveto
        // and remain_jump_dist--;
    }
    else
    {
        if (getWorld()->fall(getX(), getY()) == false)
        {
            moveTo(getX(), getY() - 4);
        }
        // check block between 0 and 3 (inclusive) pixels directly below her
        // if not, y = y-4 using moveto
    }

    int key = 0;
    if (getWorld()->getKey(key))
    {
        switch (key)
        {
        case KEY_PRESS_LEFT:
            setDirection(180);
            if (getWorld()->overlap(getX() - 4, getY(), getDirection()) == true)
            {
                return;
            }
            moveTo(getX() - 4, getY());
            break;

        case KEY_PRESS_RIGHT:
            setDirection(0);
            if (getWorld()->overlap(getX() + 4, getY(), getDirection()) == true)
            {
                return;
            }
            moveTo(getX() + 4, getY());
            break;

        case KEY_PRESS_UP:
            // check support one pixel below her
            if (getWorld()->support(getX(), getY()) == false)
            {
                return;
            }
            // if yes then proceed else return
            if (jump_power == true)
            {
                remain_jump_dist = 12;
            }
            else
            {
                remain_jump_dist = 8;
            }
            getWorld()->playSound(SOUND_PLAYER_JUMP);
            set_jump(true);
            break;
        case KEY_PRESS_SPACE:
            if (shoot_power == false)
            {
                return;
            }
            if (to_recharge_before_next_fire > 0)
            {
                return;
            }
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            to_recharge_before_next_fire = 8;
            // x,y position in front of peach 4 pixels away in the direction she is facing
            // add fireball there in the same direction as peach
            if (getDirection() == 0)
            {
                Peachfb *temp = new Peachfb(getWorld(), getX() + 4, getY(), getDirection());
                getWorld()->addobject(temp);
            }
            else
            {
                Peachfb *temp = new Peachfb(getWorld(), getX() - 4, getY(), getDirection());
                getWorld()->addobject(temp);
            }
        }
    }
}

void Peach::bonk()
{
    if (star_power == true || is_temp_invincible() > 0)
    {
        return;
    }
    m_healthpoint--;
    set_temp_invincible(10);
    if (shoot_power == true)
    {
        shoot_power = false;
    }
    if (jump_power == true)
    {
        jump_power = false;
    }
    if (get_healthpoint() >= 1)
    {
        getWorld()->playSound(SOUND_PLAYER_HURT);
    }
    if (get_healthpoint() <= 0)
    {
        set_alive(false);
        getWorld()->decLives();
    }
}

void Block::bonk()
{
    if (m_goodie == 0 || m_released == true)
    {
        getWorld()->playSound(SOUND_PLAYER_BONK);
        return;
    }
    if (m_goodie != 0 || m_released == false)
    {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        // Introduce a goodie object of the appropriate type (Flower, Star or Mushroom) exactly 8 pixels above the block that was bonked (the same x coordinate as the block, but y+8 from the block)
        if (m_goodie == 1)
        {
            Flower *temp = new Flower(getWorld(), getX(), getY() + SPRITE_HEIGHT);
            getWorld()->addobject(temp);
        }
        else if (m_goodie == 2)
        {
            Mushroom *temp = new Mushroom(getWorld(), getX(), getY() + SPRITE_HEIGHT);
            getWorld()->addobject(temp);
        }
        else
        {
            Star *temp = new Star(getWorld(), getX(), getY() + SPRITE_HEIGHT);
            getWorld()->addobject(temp);
        }
        m_released = true;
    }
}

void Pipe::bonk()
{
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

void Flag::doSomething()
{
    if (!is_alive())
    {
        return;
    }
    // check overlap w peach
    if (getWorld()->levelender2_overlap(getX(), getY()) == true)
    {
        getWorld()->increaseScore(1000);
        set_alive(false);
    }
    // next level
}

void Mario::doSomething()
{
    if (!is_alive())
    {
        return;
    }
    // check overlap w peach
    if (getWorld()->levelender_overlap(getX(), getY()) == true)
    {
        getWorld()->increaseScore(1000);
        set_alive(false);
    }
    // game won
}

void Flower::doSomething()
{
    if (getWorld()->flower_overlap(getX(), getY(), getDirection()) == true)
    {
        getWorld()->increaseScore(50);
        set_alive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }

    // check for support
    if (getWorld()->goodie_supported(getX(), getY()) == false)
    {
        moveTo(getX(), getY() - 2);
    }
    // if not, move to y-2
    // move 2 pixels in direction facing if possible
    // otherwise change direction
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 0) == false)
        {
            moveTo(getX() + 2, getY());
        }
        else
        {
            setDirection(180);
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 180) == false)
        {
            moveTo(getX() - 2, getY());
        }
        else
        {
            setDirection(0);
        }
    }
    return;
}

void Mushroom::doSomething()
{
    if (getWorld()->mushroom_overlap(getX(), getY(), getDirection()) == true)
    {
        getWorld()->increaseScore(75);
        set_alive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }

    // check for support
    if (getWorld()->goodie_supported(getX(), getY()) == false)
    {
        moveTo(getX(), getY() - 2);
    }
    // if not, move to y-2
    // move 2 pixels in direction facing if possible
    // otherwise change direction
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 0) == false)
        {
            moveTo(getX() + 2, getY());
        }
        else
        {
            setDirection(180);
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 180) == false)
        {
            moveTo(getX() - 2, getY());
        }
        else
        {
            setDirection(0);
        }
    }
    return;
}

void Star::doSomething()
{
    if (getWorld()->star_overlap(getX(), getY(), getDirection()) == true)
    {
        getWorld()->increaseScore(100);
        set_alive(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }

    // check for support
    if (getWorld()->goodie_supported(getX(), getY()) == false)
    {
        moveTo(getX(), getY() - 2);
    }
    // if not, move to y-2
    // move 2 pixels in direction facing if possible
    // otherwise change direction
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 0) == false)
        {
            moveTo(getX() + 2, getY());
        }
        else
        {
            setDirection(180);
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 180) == false)
        {
            moveTo(getX() - 2, getY());
        }
        else
        {
            setDirection(0);
        }
    }
    return;
}

void Piranhafb::doSomething()
{
    // check overlap with peach
    if (getWorld()->piranhafb_overlap(getX(), getY(), getDirection()) == true)
    {
        set_alive(false);
        return;
    }
    // tell object that it is damaged
    // set self stat to non alive

    // check support
    // if not, move to y-2
    // similar to goodies
    if (getWorld()->goodie_supported(getX(), getY()) == false)
    {
        moveTo(getX(), getY() - 2);
    }
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 0) == false)
        {
            moveTo(getX() + 2, getY());
        }
        else
        {
            set_alive(false);
            return;
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 180) == false)
        {
            moveTo(getX() - 2, getY());
        }
        else
        {
            set_alive(false);
            return;
        }
    }
    return;
}

void Peachfb::doSomething()
{
    // check overlap with any damageable object other than peach

    if (!is_alive())
    {
        return;
    }
    if (getWorld()->peachfb_or_shell_overlap(this) == true)
    {
        set_alive(false);
        return;
    }
    // tell object that it is damaged
    // set self stat to non alive
    // check support
    // if not, move to y-2
    // similar to goodies

    if (getWorld()->goodie_supported(getX(), getY()) == false)
    {
        moveTo(getX(), getY() - 2);
    }
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 0) == false)
        {
            moveTo(getX() + 2, getY());
        }
        else
        {
            set_alive(false);
            return;
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 180) == false)
        {
            moveTo(getX() - 2, getY());
        }
        else
        {
            set_alive(false);
            return;
        }
    }
    return;
}

void Shell::doSomething()
{
    // check overlap with any damageable object other than peach
    // tell object that it is damaged
    // set self stat to non alive
    if (getWorld()->peachfb_or_shell_overlap(this) == true)
    {
        set_alive(false);
        return;
    }
    // check support
    // if not, move to y-2
    // similar to goodies
    if (getWorld()->goodie_supported(getX(), getY()) == false)
    {
        moveTo(getX(), getY() - 2);
    }
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 0) == false)
        {
            moveTo(getX() + 2, getY());
        }
        else
        {
            set_alive(false);
            return;
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 180) == true)
        {
            set_alive(false);
            return;
        }
        else
        {
            moveTo(getX() - 2, getY());
        }
    }
    return;
}

// edit for diff enemies
void Goomba::doSomething()
{
    if (is_damaged() == true)
    {
        getWorld()->increaseScore(100);
        set_alive(false);
    }
    if (is_alive() == false)
    {
        return;
    }
    // check overlap w peach
    if (getWorld()->piranhafb_overlap(getX(), getY(), getDirection()) == true)
    {
        return;
    }
    // if so, try to bonk peach

    // check to move 1 pixel in curr dir without running into blocks object
    //  if not change dir
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), getDirection()) == true)
        {
            setDirection(180);
        }
        else if (getWorld()->enemy_supported(getX() + SPRITE_WIDTH + 1, getY(), getDirection()) == false)
        {
            setDirection(180);
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), getDirection()) == true)
        {
            setDirection(0);
        }
        else if (getWorld()->enemy_supported(getX() - 1, getY(), getDirection()) == false)
        {
            setDirection(0);
        }
    }
    // check 1 pixel in its current direction without stepping partly or fully off of the edge of its current platform of blocks and/or pipes
    // if not change dir

    // move in dir facing either x-1 or x+1
    // check for block in new position
    // if so
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 0) == false)
        {
            moveTo(getX() + 1, getY());
        }
        else
        {
            return;
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 180) == false)
        {
            moveTo(getX() - 1, getY());
        }
        else
        {
            return;
        }
    }
    // otherwise update using moveto
}

void Piranha::doSomething()
{
    if (!is_alive())
    {
        return;
    }
    if (is_damaged() == true)
    {
        getWorld()->increaseScore(100);
        set_alive(false);
    }
    increaseAnimationNumber();
    if (getWorld()->piranhafb_overlap(getX(), getY(), getDirection()) == true)
    {
        return;
    }

    if (getWorld()->within_peach(this) == false)
    {
        return;
    }

    if (firing_delay() > 0)
    {
        m_firing_delay--;
        return;
    }

    if (getWorld()->within_peach_2(this) == true)
    {
        Piranhafb *temp = new Piranhafb(getWorld(), getX(), getY(), getDirection());
        getWorld()->addobject(temp);
        getWorld()->playSound(IID_PIRANHA_FIRE);
        m_firing_delay = 40;
    }
}

void Koopa::doSomething()
{
    if (is_alive() == false)
    {
        return;
    }
    // check overlap w peach
    if (is_damaged() == true)
    {
        getWorld()->increaseScore(100);
        Shell *temp = new Shell(getWorld(), getX(), getY(), getDirection());
        getWorld()->addobject(temp);
        set_alive(false);
        return;
    }
    // if so, try to bonk peach
    if (getWorld()->piranhafb_overlap(getX(), getY(), getDirection()) == true)
    {
        return;
    }

    // check to move 1 pixel in curr dir without running into blocks object
    //  if not change dir
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), getDirection()) == true)
        {
            setDirection(180);
        }
        else if (getWorld()->enemy_supported(getX() + SPRITE_WIDTH + 1, getY(), getDirection()) == false)
        {
            setDirection(180);
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), getDirection()) == true)
        {
            setDirection(0);
        }
        else if (getWorld()->enemy_supported(getX() - 1, getY(), getDirection()) == false)
        {
            setDirection(0);
        }
    }

    // check 1 pixel in its current direction without stepping partly or fully off of the edge of its current platform of blocks and/or pipes
    // if not change dir

    // move in dir facing either x-1 or x+1
    // check for block in new position
    // if so
    if (getDirection() == 0)
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 0) == false)
        {
            moveTo(getX() + 1, getY());
        }
        else
        {
            return;
        }
    }
    else
    {
        if (getWorld()->goodie_blocked(getX(), getY(), 180) == false)
        {
            moveTo(getX() - 1, getY());
        }
        else
        {
            return;
        }
    }
    // otherwise update using moveto
}

void Enemy::bonk()
{
    // if peach did not bonk, ignore
    // peach has invincibility
    getWorld()->playSound(SOUND_PLAYER_KICK);
    getWorld()->increaseScore(100);
    set_alive(false);
    // INC SCORE
    // set status to not alive
}
