#ifndef SCHEDULING_STRATEGIES_H
#define SCHEDULING_STRATEGIES_H

#include "utils.hh"
#include <memory>
#include <algorithm>
#include <vector>
#include <sstream>

class scheduler
{
public:

    scheduler(size_t n, int _strategy, int dispossession_time);
    ~scheduler() = default;

    void run();

private:

    std::vector<process> p_queue;
    std::shared_ptr<cpu> _cpu;
    int strategy;
    int dispossession_time;

    void add_to_cpu();
    void schedule();
    
    void fcfs();
    void sjf();
    void srtf();
    void rr();
    void pnd();

    bool is_cpu_free() const;   //false if there is any task going

};

scheduler::scheduler(size_t n, int _strategy, int _dispossession_time): strategy(_strategy), 
dispossession_time(_dispossession_time)
{
    _cpu = std::make_shared<cpu>(n);
}

void scheduler::schedule()
{
    _cpu->time++;   //cpu tick

    for(size_t i = 0; i < _cpu->cores_count; ++i)  //check if some cores are free now
    {
        if(_cpu->thread[i].execution_time == 0)     //if execution time equals 0, then core is free
        {
            _cpu->thread[i].process_id = INACTIVE;    
        }
    }

    for(size_t i = 0; i < _cpu->cores_count; ++i)  //check if some cores are free now
    {
        if(_cpu->thread[i].process_id == INACTIVE)     //if some core is free, we gonna put there tasks from behind
        {
            for(size_t j = i; j < _cpu->cores_count; ++j)
            {
                if(_cpu->thread[j].process_id != INACTIVE)
                {
                    process temp = _cpu->thread[i];
                    _cpu->thread[i] = _cpu->thread[j];
                    _cpu->thread[j] = temp;
                }
            }   
        }
    }

    switch (strategy)
    {
    case 0:
        fcfs();
        break;

    case 1:
        sjf();
        break;

    case 2:
        srtf();
        break;

    case 3:
        rr();
        break;

    case 4:
        pnd();
        break;
    
    default:
        break;
    }

    for(size_t i = 0; i < _cpu->cores_count; ++i)
    {
        if(_cpu->thread[i].process_id != -1) //if core is taken decrease task exec time 
        {  
            _cpu->thread[i].execution_time--;
        }
    }

}

bool scheduler::is_cpu_free() const
{
    for(size_t i = 0; i < _cpu->cores_count; ++i)
    {
        if(_cpu->thread[i].process_id != INACTIVE)    //there is any task
        {
            return false;
        }
    }

    return true;
}

void scheduler::run()
{

    int timestamp;
    std::string line;
    process temp;

    while(std::cin >> timestamp)
    {
        std::getline(std::cin, line);
        std::stringstream args(line);

        while(args >> temp)
        {
            p_queue.push_back(temp);
        }
        
        schedule();
        std::cout << _cpu->print() << std::endl;
    }

    while(!is_cpu_free())
    {
        schedule();
        std::cout << _cpu->print() << std::endl;        
    }

}

void scheduler::add_to_cpu()
{
    for(size_t i = 0; i < _cpu->cores_count; ++i)
    {
        if(_cpu->thread[i].process_id == INACTIVE)    //if there is a free core on cpu insert first task
        {
            if(!p_queue.empty())
            {
                _cpu->thread[i] = p_queue.front();  
                p_queue.erase(p_queue.begin());
            }
        }
    }
}

void scheduler::fcfs()
{
    add_to_cpu();
}

void scheduler::sjf()
{
    std::sort(p_queue.begin(), p_queue.end(), comp_by_time);

    add_to_cpu();
}


void scheduler::srtf()
{
//pop back all tasks from cpu if there is time to do it 
if (_cpu->time % dispossession_time == 0)
{
    for(size_t i = 0; i < _cpu->cores_count; ++i)  //check all cores
    {
        if(_cpu->thread[i].process_id != INACTIVE)     //if some core is active, then we pop it to the list
        {
            p_queue.push_back(_cpu->thread[i]);
            _cpu->thread[i].process_id = INACTIVE;    
        }
    }
}



//sort them 
    std::sort(p_queue.begin(), p_queue.end(), comp_by_time);

//add them to the cpu 
    add_to_cpu();
}

void scheduler::rr()
{

if (_cpu->time % dispossession_time == 0)
{
    for(size_t i = 0; i < _cpu->cores_count; ++i)  //check all cores
    {
        if(_cpu->thread[i].process_id != INACTIVE)     //if some core is active, then we pop it to the list
        {
            p_queue.push_back(_cpu->thread[i]);
            _cpu->thread[i].process_id = INACTIVE;    
        }
    }
}

    add_to_cpu();

}

void scheduler::pnd()
{

    std::sort(p_queue.begin(), p_queue.end(), comp_by_priority);

    add_to_cpu();

}

#endif // SCHEDULING_STRATEGIES_H
