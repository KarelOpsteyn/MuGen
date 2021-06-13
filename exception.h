#ifndef EXCEPTION_H
#define EXCEPTION_H
//// EXCEPTIONS ////

//Exceptions are used to check if e.g. a random new chord adheres to the set rules in setchord().

struct Exception_NoteLimiter : public std::exception
{
    const char * what () const throw ()
    {
        return "Chord generation ran into an issue";
    }
};

struct Exception_GradeProgression : public std::exception
{
    const char * what () const throw ()
    {
        return "Grade progression could not be resolved";
    }
};

struct Exception_GradeInitialisation : public std::exception
{
    const char * what () const throw ()
    {
        return  "Bad First Grade";
    }
};

#endif // EXCEPTION_H
