#ifndef BATTMON_TASK
#define BATTMON_TASK

class Task {
public:
  virtual bool CanRun() = 0;
  virtual void Run() = 0;

  void set_interval(unsigned long interval) { interval_ = interval; }

  void Tick(unsigned long now);

private:
  unsigned long last_run_ = 0;
  unsigned long interval_ = 0;
};

#endif // BATTMON_TASK
