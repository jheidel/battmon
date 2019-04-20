#ifndef BATTMON_TASK_H_
#define BATTMON_TASK_H_

class Task {
public:
  virtual bool Setup() { return true; }
  virtual bool CanRun() { return true; }
  virtual void Run() = 0;

  void set_interval(unsigned long interval) { interval_ = interval; }

  bool SetupInit();
  void Tick(unsigned long now);

  inline bool is_first_run() { return first_run_; }

private:
  unsigned long last_run_ = 0;
  unsigned long interval_ = 0;
  bool first_run_ = true;
  bool setup_ = false;
};

#endif // BATTMON_TASK_H_
