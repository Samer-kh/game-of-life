#include "game.h"
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

using namespace std::this_thread;
using namespace std::chrono;
using namespace std;

//implementation du constructeur
game::game()
{
//initialisation du grid
grid = new int*[grid_size_x];
 for (int i=0;i<grid_size_x;i++)
    {
     grid[i]=new int[grid_size_y];

    }
 for(int i=0;i<grid_size_x;i++)
 {
     for (int j=0;j<grid_size_y;j++)
     {
         grid[i][j]=0;
     }
 }
//initialisation du mutex
 verrou = new std::mutex();
 }
//implementation du destructeur
game::~game()
{
    for(int i=0 ; i<grid_size_x ;i++)
    {
        delete[] grid[i];
    }
    delete[] grid;

}
void game::run()
{
    init_grid();
    while(true)
    {
        sleep_until(system_clock::now() + seconds(2));
        next_generation();
        display_grid();
    }
}

void game::init_grid()
{

    //initialise random cells with a random number
    int cell_initialized=rand() % cell_number;
    int count=0;
    int x,y;
    while (count < cell_initialized)
    {
        x= (rand() % grid_size_x);
        y= (rand() % grid_size_y);
        if (grid[x][y]==0)
        {
            grid[x][y]=1;
            count++;
        }
    }
}
int game::getLivingNieghbors(int x,int y)
{ int count =0;
    if (((x==0)&&(y==0)) || ((x==(grid_size_x-1))&&(y==(grid_size_y-1))) || ((x==0)&&((y==grid_size_y-1))) || ((x==(grid_size_x-1))&&(y==0)))
    {
        if ((x==0)&&(y==0))
        {
            count=grid[0][1] + grid[1][1] + grid[1][0];
            return count;
        }
        if ((x==(grid_size_x-1))&&(y==(grid_size_y-1)))
        {
            count=grid[x-1][y] + grid[y-1][x] + grid[x-1][y-1];
            return count;
        }
        if ((x==0)&&((y==grid_size_y-1)))
        {
            count=grid[x+1][y] + grid[x][y-1] + grid[x+1][y-1];
            return count;
        }
        if ((x==(grid_size_x-1))&&(y==0))
        {
            count=grid[x-1][y] + grid[x][y+1] + grid[x-1][y+1];
            return count;
        }
    }
    else if ((x==0) || (y==0) || ( x==(grid_size_x-1)) || (y==(grid_size_y-1)))
    {
        if ((x==0)&&(y!=0)&&(y!=(grid_size_y-1)))
        {
            count=grid[x][y-1] + grid[x][y+1] + grid[x+1][y+1] + grid[x+1][y-1] + grid[x+1][y];
            return count;
        }
        else if ((y==0)&&(x!=0)&&(x!=(grid_size_x-1)))
        {
            count=grid[x-1][y] + grid[x-1][y+1] + grid[x][y+1] + grid[x+1][y] + grid[x+1][y+1];
            return count;
        }
        else if ((x==(grid_size_x-1))&&(y!=0)&&(y!=(grid_size_y-1)))
        {
            count=grid[x][y-1] + grid[x-1][y-1] + grid[x-1][y] + grid[x-1][y+1] + grid[x-1][y+1];
            return count;
        }
        else if ((y==(grid_size_y-1))&&(x!=0)&&(x!=(grid_size_x-1)))
        {
            count=grid[x-1][y-1] + grid[x-1][y] + grid[x][y-1] + grid[x+1][y-1] + grid[x+1][y];
            return count;
        }

    }
    else
    {
        //le cas ordinaire
        for (int i = x - 1; i <= x + 1; i++)
        {
            for (int j = y - 1; j <= y + 1; j++)
            {
                if (i > 0 && i < (grid_size_x-1) && j > 0 && j < (grid_size_y-1)) {
                    if (i != x || j != y) {
                        if (grid[i][j]==1)
                        count += 1;
                    }
                }
            }
        }
        return count;
    }
    return count;

}
void game::next_generation_per_thread(game* g,int begin_x, int end_x,int begin_y, int end_y)
{   int living=0;
    for (int i=begin_x;i<end_x;i++)
    {
        for (int j=begin_y;j<end_y;j++)
        {
            //locking the mutex
            g->verrou->lock();
            //test for under population
            living=g->getLivingNieghbors(i,j);
            if ((living <2) && (g->grid[i][j]==1))
            {
                g->grid[i][j]=0;
            }
            //test for overcrowding
            living=g->getLivingNieghbors(i,j);
            if ((living > 3) && (g->grid[i][j]==1))
            {
                g->grid[i][j]=0;
            }
            //test for reproduction
            living=g->getLivingNieghbors(i,j);
            if ((living==3) && (g->grid[i][j]==0))
            {
                g->grid[i][j]=1;
            }
            //unlocking the verrou
            g->verrou->unlock();
        }
    }
}

void game::next_generation()
{
std::thread Ids[NB_THREADS];
for (int i=0;i<NB_THREADS;i++)
{
    Ids[i]=std::thread(next_generation_per_thread,this,(int) ((grid_size_x/NB_THREADS)*i),(int) ((grid_size_x/NB_THREADS)*(i+1)),(int) ((grid_size_y/NB_THREADS)*(i)),(int) ((grid_size_y/NB_THREADS)*(i+1)));

}
for(int i=0; i<NB_THREADS; i++)
{
    Ids[i].join();
}
}

void game::display_grid() {
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
     cout << endl;
}

int game::get_grid_x()
{
    return grid_size_x;
}
int game::get_grid_y()
{
    return grid_size_y;
}

