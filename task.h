#ifndef BATTMON_TASK_H_
#define BATTMON_TASK_H_

class Task {
public:
  virtual bool Setup() { return true; }
  virtual bool CanRun() { return true; }
  virtual void Run() = 0;

  void set_interval(unsigned long interval) { interval_ = interval; }

  void Tick(unsigned long now);

private:
  unsigned long last_run_ = 0;
  unsigned long interval_ = 0;
};

#endif // BATTMON_TASK_H_
