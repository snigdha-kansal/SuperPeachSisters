#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld *createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    Level lev(assetPath());
    ostringstream oss;
    oss << "level";
    int lvl = getLevel();
    oss.fill('0');
    oss << setw(2) << lvl;
    oss << ".txt";
    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
    {
        cerr << "Could not find level01.txt data file" << endl;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << "level01.txt is improperly formatted" << endl;
    }
    else if (result == Level::load_success)
    {
        Level::GridEntry ge;
        for (int i = 0; i < GRID_WIDTH; i++)
        {
            for (int j = 0; j < GRID_HEIGHT; j++)
            {
                ge = lev.getContentsOf(i, j);
                if (ge == Level::peach)
                {
                    m_peach = new Peach(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                }
                else if (ge == Level::block)
                {
                    Block *temp = new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    m_actors.push_back(temp);
                }
                else if (ge == Level::pipe)
                {
                    Pipe *temp = new Pipe(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    m_actors.push_back(temp);
                }
                else if (ge == Level::flower_goodie_block)
                {
                    Block *temp = new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    temp->set_goodie(1);
                    m_actors.push_back(temp);
                }
                else if (ge == Level::mushroom_goodie_block)
                {
                    Block *temp = new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    temp->set_goodie(2);
                    m_actors.push_back(temp);
                }

                else if (ge == Level::star_goodie_block)
                {
                    Block *temp = new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    temp->set_goodie(3);
                    m_actors.push_back(temp);
                }
                else if (ge == Level::piranha)
                {
                    Piranha *temp = new Piranha(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    m_actors.push_back(temp);
                }
                else if (ge == Level::koopa)
                {
                    Koopa *temp = new Koopa(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    m_actors.push_back(temp);
                }
                else if (ge == Level::goomba)
                {
                    Goomba *temp = new Goomba(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    m_actors.push_back(temp);
                }
                else if (ge == Level::flag)
                {
                    Flag *temp = new Flag(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    m_actors.push_back(temp);
                }
                else if (ge == Level::mario)
                {
                    Mario *temp = new Mario(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    m_actors.push_back(temp);
                }
                else
                {
                    continue;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if (m_peach->is_alive())
    {
        m_peach->doSomething();
    }
    else
    {
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }

    if (m_peach->has_flag_reached())
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }

    if (m_peach->has_mario_reached())
    {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }

    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->is_alive())
        {
            m_actors[i]->doSomething();
        }
    }

    vector<Actor *>::iterator it = m_actors.begin();
    for (; it != m_actors.end();)
    { // notice: no it++
        if (!((*it)->is_alive()))
        {
            Actor *temp = *it;
            it = m_actors.erase(it);
            delete temp;
        }
        else
        {
            it++;
        }
    }

    ostringstream oss;
    oss.fill('0');
    oss << "Lives: " << getLives() << "  Level: " << setw(2) << getLevel() << "  Points: " << setw(6) << getScore();
    if (m_peach->get_jump_power())
    {
        oss << " JumpPower!";
    }
    if (m_peach->get_shoot_power())
    {
        oss << " ShootPower!";
    }
    if (m_peach->get_star_power())
    {
        oss << " StarPower!";
    }
    string s = oss.str();
    setGameStatText(s);

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    for (int i = 0; i < m_actors.size(); i++)
    {
        delete m_actors[i];
    }
    vector<Actor *>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end();)
    {
        it = m_actors.erase(it);
    }
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

bool StudentWorld::overlap(int x, int y, int dir)
{
    // right
    if (dir == 0)
    {
        for (int i = 0; i < m_actors.size(); i++)
        {
            if (m_actors[i]->blocks() == true)
            {
                if (m_actors[i]->getX() <= x && x <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
                {
                    m_actors[i]->bonk();
                    return true;
                }
                else if (m_actors[i]->getX() <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
                {
                    m_actors[i]->bonk();
                    return true;
                }
            }
            else if (m_actors[i]->is_damageable() == true)
            {
                if (m_actors[i]->getX() <= x && x <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
                {
                    if (m_peach->get_star_power() == true)
                    {
                        m_actors[i]->bonk();
                    }
                    else
                    {
                        m_peach->bonk();
                    }
                }
                else if (m_actors[i]->getX() <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
                {
                    if (m_peach->get_star_power() == true)
                    {
                        m_actors[i]->bonk();
                    }
                    else
                    {
                        m_peach->bonk();
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < m_actors.size(); i++)
        {
            if (m_actors[i]->blocks() == true)
            {
                if (m_actors[i]->getX() <= x && x <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
                {
                    m_actors[i]->bonk();
                    return true;
                }
                else if (m_actors[i]->getX() <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
                {
                    m_actors[i]->bonk();
                    return true;
                }
            }
            else if (m_actors[i]->is_damageable() == true)
            {
                if (m_actors[i]->getX() <= x && x <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
                {
                    if (m_peach->get_star_power() == true)
                    {
                        m_actors[i]->bonk();
                    }
                    else
                    {
                        m_peach->bonk();
                    }
                }
                else if (m_actors[i]->getX() <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
                {
                    if (m_peach->get_star_power() == true)
                    {
                        m_actors[i]->bonk();
                    }
                    else
                    {
                        m_peach->bonk();
                    }
                }
            }
        }
    }
    return false;
}

bool StudentWorld::support(int x, int y)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->blocks() == true)
        {
            if (m_actors[i]->getX() <= x && x <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y - 1 && y - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
            {
                return true;
            }
            else if (m_actors[i]->getX() <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && m_actors[i]->getY() <= y - 1 && y - 1 <= m_actors[i]->getY() + SPRITE_HEIGHT - 1)
            {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::fall(int x, int y)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->blocks() == true)
        {
            if (m_actors[i]->getX() <= x && x <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && y >= m_actors[i]->getY() + SPRITE_HEIGHT - 1 && m_actors[i]->getY() + SPRITE_HEIGHT - 1 >= y - 3)
            {
                return true;
            }
            else if (m_actors[i]->getX() <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= m_actors[i]->getX() + SPRITE_WIDTH - 1 && y >= m_actors[i]->getY() + SPRITE_HEIGHT - 1 && m_actors[i]->getY() + SPRITE_HEIGHT - 1 >= y - 3)
            {
                return true;
            }
        }
    }
    return false;
}

void StudentWorld::addobject(Actor *act)
{
    m_actors.push_back(act);
}

bool StudentWorld::goodie_supported(int x, int y)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->blocks() == true)
        {
            if (m_actors[i]->getX() < x && x < m_actors[i]->getX() + SPRITE_WIDTH && y == m_actors[i]->getY() + SPRITE_HEIGHT)
            {
                return true;
            }
            else if (m_actors[i]->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_actors[i]->getX() + SPRITE_WIDTH && y == m_actors[i]->getY() + SPRITE_HEIGHT)
            {
                return true;
            }
            else if (m_actors[i]->getX() == x && x + SPRITE_WIDTH == m_actors[i]->getX() + SPRITE_WIDTH && y == m_actors[i]->getY() + SPRITE_HEIGHT)
            {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::goodie_blocked(int x, int y, int dir)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->blocks() == true)
        {
            if (dir == 0)
            {
                if (m_actors[i]->getY() < y && y < m_actors[i]->getY() + SPRITE_HEIGHT && x + SPRITE_WIDTH == m_actors[i]->getX())
                {
                    return true;
                }
                else if (m_actors[i]->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_actors[i]->getY() + SPRITE_HEIGHT && x + SPRITE_WIDTH == m_actors[i]->getX())
                {
                    return true;
                }
                else if (m_actors[i]->getY() == y && y + SPRITE_HEIGHT == m_actors[i]->getY() + SPRITE_HEIGHT && x + SPRITE_WIDTH == m_actors[i]->getX())
                {
                    return true;
                }
            }
            else
            {
                if (m_actors[i]->getY() < y && y < m_actors[i]->getY() + SPRITE_WIDTH && x == m_actors[i]->getX() + SPRITE_WIDTH)
                {
                    return true;
                }
                else if (m_actors[i]->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_actors[i]->getY() + SPRITE_HEIGHT && x == m_actors[i]->getX() + SPRITE_WIDTH)
                {
                    return true;
                }
                else if (m_actors[i]->getY() == y && y + SPRITE_HEIGHT == m_actors[i]->getY() + SPRITE_HEIGHT && x == m_actors[i]->getX() + SPRITE_WIDTH)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool StudentWorld::mushroom_overlap(int x, int y, int dir)
{
    if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_jump_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_jump_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_jump_power(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_jump_power(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_jump_power(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_jump_power(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_jump_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_jump_power(true);
        return true;
    }
    return false;
}

bool StudentWorld::flower_overlap(int x, int y, int dir)
{
    if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_shoot_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_shoot_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_shoot_power(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_shoot_power(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_shoot_power(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_shoot_power(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_shoot_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_shoot_power(true);
        return true;
    }
    return false;
}

bool StudentWorld::star_overlap(int x, int y, int dir)
{
    if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_star_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_star_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_star_power(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_star_power(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_star_power(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_star_power(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_star_power(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_star_power(true);
        return true;
    }
    return false;
}

bool StudentWorld::piranhafb_overlap(int x, int y, int dir)
{
    if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->bonk();
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->bonk();
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->bonk();
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->bonk();
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->bonk();
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->bonk();
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->bonk();
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->bonk();
        return true;
    }
    return false;
}

bool StudentWorld::peachfb_or_shell_overlap(Actor *act)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->is_damageable() == true && m_actors[i]->is_alive())
        {
            if (m_actors[i]->getY() < act->getY() && act->getY() < m_actors[i]->getY() + SPRITE_HEIGHT && m_actors[i]->getX() < act->getX() && act->getX() < m_actors[i]->getX() + SPRITE_WIDTH)
            {
                m_actors[i]->set_damaged(true);
                return true;
            }
            else if (m_actors[i]->getY() < act->getY() + SPRITE_HEIGHT && act->getY() + SPRITE_HEIGHT < m_actors[i]->getY() + SPRITE_HEIGHT && m_actors[i]->getX() < act->getX() && act->getX() < m_actors[i]->getX() + SPRITE_WIDTH)
            {
                m_actors[i]->set_damaged(true);
                return true;
            }
            else if (m_actors[i]->getY() < act->getY() + SPRITE_HEIGHT && act->getY() + SPRITE_HEIGHT < m_actors[i]->getY() + SPRITE_HEIGHT && m_actors[i]->getX() < act->getX() + SPRITE_WIDTH && act->getX() + SPRITE_WIDTH < m_actors[i]->getX() + SPRITE_WIDTH)
            {
                m_actors[i]->set_damaged(true);
                return true;
            }
            else if (m_actors[i]->getY() < act->getY() && act->getY() < m_actors[i]->getY() + SPRITE_HEIGHT && m_actors[i]->getX() < act->getX() && act->getX() < m_actors[i]->getX() + SPRITE_WIDTH)
            {
                m_actors[i]->set_damaged(true);
                return true;
            }
            else if (m_actors[i]->getY() == act->getY() && act->getY() + SPRITE_HEIGHT == m_actors[i]->getY() + SPRITE_HEIGHT && m_actors[i]->getX() < act->getX() && act->getX() < m_actors[i]->getX() + SPRITE_WIDTH)
            {
                m_actors[i]->set_damaged(true);
                return true;
            }
            else if (m_actors[i]->getY() < act->getY() && act->getY() < m_actors[i]->getY() + SPRITE_HEIGHT && m_actors[i]->getX() == act->getX() && act->getX() + SPRITE_WIDTH == m_actors[i]->getX() + SPRITE_WIDTH)
            {
                m_actors[i]->set_damaged(true);
                return true;
            }
            else if (m_actors[i]->getY() == act->getY() && act->getY() + SPRITE_HEIGHT == m_actors[i]->getY() + SPRITE_HEIGHT && m_actors[i]->getX() < act->getX() + SPRITE_WIDTH && act->getX() + SPRITE_WIDTH < m_actors[i]->getX() + SPRITE_WIDTH)
            {
                m_actors[i]->set_damaged(true);
                return true;
            }
            else if (m_actors[i]->getY() < act->getY() + SPRITE_HEIGHT && act->getY() + SPRITE_HEIGHT < m_actors[i]->getY() + SPRITE_HEIGHT && m_actors[i]->getX() == act->getX() && act->getX() + SPRITE_WIDTH == m_actors[i]->getX() + SPRITE_WIDTH)
            {
                m_actors[i]->set_damaged(true);
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::enemy_supported(int x, int y, int dir)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->blocks() == true)
        {
            if (m_actors[i]->getX() < x && x < m_actors[i]->getX() + SPRITE_WIDTH && y == m_actors[i]->getY() + SPRITE_HEIGHT)
            {
                return true;
            }
            else if (m_actors[i]->getX() == x && x + SPRITE_WIDTH == m_actors[i]->getX() + SPRITE_WIDTH && y == m_actors[i]->getY() + SPRITE_HEIGHT)
            {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::within_peach(Actor *act)
{
    // Peach’s y coordinate is within 1.5*SPRITE_HEIGHT of Piranha’s y coordinate
    if (act->getY() - 1.5 * SPRITE_HEIGHT <= m_peach->getY() && m_peach->getY() <= act->getY() + 1.5 * SPRITE_HEIGHT)
    {
        if (m_peach->getX() < act->getX())
        {
            act->setDirection(180);
        }
        else if (m_peach->getX() > act->getX())
        {
            act->setDirection(0);
        }
        return true;
    }
    return false;
}

bool StudentWorld::within_peach_2(Actor *act)
{
    int temp = act->getX() - m_peach->getX();
    if (temp < 0)
    {
        temp = temp * (-1);
    }
    if (temp < 8 * SPRITE_WIDTH)
    {
        return true;
    }
    return false;
}

bool StudentWorld::levelender_overlap(int x, int y)
{
    if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_mario_reached(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_mario_reached(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_mario_reached(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_mario_reached(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_mario_reached(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_mario_reached(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_mario_reached(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_mario_reached(true);
        return true;
    }
    return false;
}

bool StudentWorld::levelender2_overlap(int x, int y)
{
    if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_flag_reached(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_flag_reached(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_flag_reached(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_flag_reached(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x && x < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_flag_reached(true);
        return true;
    }
    else if (m_peach->getY() < y && y < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_flag_reached(true);
        return true;
    }
    else if (m_peach->getY() == y && y + SPRITE_HEIGHT == m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() < x + SPRITE_WIDTH && x + SPRITE_WIDTH < m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_flag_reached(true);
        return true;
    }
    else if (m_peach->getY() < y + SPRITE_HEIGHT && y + SPRITE_HEIGHT < m_peach->getY() + SPRITE_HEIGHT && m_peach->getX() == x && x + SPRITE_WIDTH == m_peach->getX() + SPRITE_WIDTH)
    {
        m_peach->set_flag_reached(true);
        return true;
    }
    return false;
}
