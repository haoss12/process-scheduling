#ifndef SCHEDULING_UTILS_HH
#define SCHEDULING_UTILS_HH

#include <iostream>
#include <string>

struct process
{
    int process_id;
    int execution_time; 
    int priority; 
};

std::istream& operator>>(std::istream& is, process& p)
{
    is >> p.process_id >> p.priority >> p.execution_time; 
    return is;
}

std::ostream& operator<<(std::ostream& os, const process& p)
{
    return os << p.process_id << " " << p.priority << " " << p.execution_time; 
}

bool comp_by_time(const process &a, const process &b)
{
    return a.execution_time < b.execution_time;
}

bool comp_by_priority(const process &a, const process &b)
{
    return a.priority < b.priority;
}


#define INACTIVE -1  //inactive cpu state

struct cpu
{
    int time;
    process *thread;
    size_t cores_count;

    cpu(size_t num): time(-1), cores_count(num)
    {
        thread = new process [num];
        for(size_t i=0; i<num; ++i)
        {
            thread[i].process_id = -1;
            thread[i].execution_time = -1;
            thread[i].priority = -1;
        }

    }

    ~cpu()
    {
        delete [] thread;
    }

    std::string print()
    {
        std::string temp = std::to_string(time);
        temp += " ";

        for(size_t i=0; i<cores_count; ++i)
        {
            temp += std::to_string(thread[i].process_id) + " ";
        }

        return temp;
    }

};

std::ostream& operator<<(std::ostream& os, const cpu& c)
{   
    std::string temp;
    for(size_t i=0; i<c.cores_count; ++i){
        temp += std::to_string(c.thread[i].process_id) + " ";
    }
    return os << c.time << " " << temp; 
}


#endif // !SCHEDULING_UTILS_HH