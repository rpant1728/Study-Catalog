#include "threads/thread.h"
#include <debug.h>
#include <stddef.h>
#include <random.h>
#include <stdio.h>
#include <string.h>
#include <fixed-point.h>
#include "../devices/timer.h"
#include "threads/flags.h"
#include "threads/interrupt.h"
#include "threads/intr-stubs.h"
#include "threads/palloc.h"
#include "threads/switch.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#ifdef USERPROG
#include "userprog/process.h"
#endif


/* Random value for struct thread's `magic' member.
   Used to detect stack overflow.  See the big comment at the top
   of thread.h for details. */
#define THREAD_MAGIC 0xcd6abf4b

/* List of processes in THREAD_READY state, that is, processes
   that are ready to run but not actually running. */
static struct list ready_list;
static int64_t next_wakeup_at;
/* List of all processes.  Processes are added to this list
   when they are first scheduled and removed when they exit. */
static struct list all_list;
static struct list sleepers_list;
/* Idle thread. */
static struct thread *idle_thread;

/* Initial thread, the thread running init.c:main(). */
static struct thread *initial_thread;

/* Lock used by allocate_tid(). */
static struct lock tid_lock;
static struct lock sleepers_lock; 
/* Stack frame for kernel_thread(). */
struct kernel_thread_frame 
  {
    void *eip;                  /* Return address. */
    thread_func *function;      /* Function to call. */
    void *aux;                  /* Auxiliary data for function. */
  };

/* Statistics. */
static long long idle_ticks;    /* # of timer ticks spent idle. */
static long long kernel_ticks;  /* # of timer ticks in kernel threads. */
static long long user_ticks;    /* # of timer ticks in user programs. */

/* Scheduling. */
#define TIME_SLICE 4            /* # of timer ticks to give each thread. */
static unsigned thread_ticks;   /* # of timer ticks since last yield. */

/* If false (default), use round-robin scheduler.
   If true, use multi-level feedback queue scheduler.
   Controlled by kernel command-line option "-o mlfqs". */
bool thread_mlfqs;
static bool update_load_avg;
static bool update_priority;

static void kernel_thread (thread_func *, void *aux);

static void idle (void *aux UNUSED);
static struct thread *running_thread (void);
static struct thread *next_thread_to_run (void);
static void init_thread (struct thread *, const char *name, int priority);
static bool is_thread (struct thread *) UNUSED;
static void *alloc_frame (struct thread *, size_t size);
static void schedule (void);
void schedule_tail (struct thread *prev);
static tid_t allocate_tid (void);

static void managerial_activity (void *aux UNUSED);
static struct thread *mang_thread;
static void recent_cpu_activity (void *aux UNUSED);
static struct thread *recent_cpu_thread;

static int load_avg;
static int count;

/* Initializes the threading system by transforming the code
   that's currently running into a thread.  This can't work in
   general and it is possible in this case only because loader.S
   was careful to put the bottom of the stack at a page boundary.

   Also initializes the run queue and the tid lock.

   After calling this function, be sure to initialize the page
   allocator before trying to create any threads with
   thread_create().

   It is not safe to call thread_current() until this function
   finishes. */
static bool before(const struct list_elem *a,const struct list_elem *b,void *aux UNUSED)
{
	return list_entry(a,struct thread,elem)->wakeup_at < list_entry(b,struct thread,elem)->wakeup_at;
}

bool prio(const struct list_elem *a,const struct list_elem *b,void *aux UNUSED)
{
	return thread_get_donated_priority(list_entry(a,struct thread,elem)) > thread_get_donated_priority(list_entry(b,struct thread,elem));
}



void
thread_init (void) 
{
  ASSERT (intr_get_level () == INTR_OFF);

  lock_init (&tid_lock);
  lock_init (&sleepers_lock); 
  list_init (&ready_list);
  list_init (&all_list);
  list_init (&sleepers_list);
  
  next_wakeup_at = INT64_MAX;
  load_avg = 0;
  count = 0;
  update_load_avg = false;
  update_priority = false;
  /* Set up a thread structure for the running thread. */
  initial_thread = running_thread ();
  init_thread (initial_thread, "main", PRI_DEFAULT);
  initial_thread->status = THREAD_RUNNING;
  initial_thread->tid = allocate_tid ();
  

}

/* Starts preemptive thread scheduling by enabling interrupts.
   Also creates the idle thread. */

void
thread_wakeup(int64_t current_tick)
{
  if(current_tick>=next_wakeup_at){
    if(list_empty(&sleepers_list)){
      next_wakeup_at = INT64_MAX;
    }
    else{
      //extracting the thread with min wake up time.
      struct thread * t = list_entry(list_begin(&sleepers_list),struct thread,elem);
      /*Time is lesser or equal to next wakeup time, 
      then pop the first thread and make the next wakeup to point to the next thread 
      in ordered list */
      if(t->wakeup_at<=next_wakeup_at){
        list_pop_front (&sleepers_list);
        thread_unblock (t);
        if(list_empty(&sleepers_list)){
          next_wakeup_at = INT64_MAX;
        }
        else{
          next_wakeup_at = list_entry(list_begin(&sleepers_list),struct thread,elem)->wakeup_at;
        }
      }
      else{
        next_wakeup_at = t->wakeup_at;
      }
    }
  }
}


void
thread_start (void) 
{
  /* Create the idle thread. */
  struct semaphore idle_started;
  sema_init (&idle_started, 0);
  thread_create ("idle", PRI_MIN, idle, &idle_started);

  /* Start preemptive thread scheduling. */
  intr_enable ();

  /* Wait for the idle thread to initialize idle_thread. */
  sema_down (&idle_started);
  thread_create ("managerial", PRI_MAX, managerial_activity, NULL);

  thread_create("recent_cpu", PRI_MAX, recent_cpu_activity,NULL);

}

/* Called by the timer interrupt handler at each timer tick.
   Thus, this function runs in an external interrupt context. */
void
thread_tick (void) 
{
  struct thread *t = thread_current ();
  t->recent_cpu = _ADD_INT (t->recent_cpu, 1);
  // set_all_recent_cpu();
  /* Update statistics. */
  if (t == idle_thread)
    idle_ticks++;
#ifdef USERPROG
  else if (t->pagedir != NULL)
    user_ticks++;
#endif
  else
    kernel_ticks++;
  // thread_wakeup(timer_ticks());
	uint64_t time = timer_ticks(); 
	if(time >= next_wakeup_at && mang_thread->status==THREAD_BLOCKED){
		// struct thread *t = list_entry(&manag_list_elem,struct thread,allelem);
		thread_unblock(mang_thread);
	}
	
  /* Enforce preemption. */
  if(time%TIMER_FREQ==0)
    update_load_avg = true;
  if (++thread_ticks >= TIME_SLICE){
    update_priority = true;
    intr_yield_on_return ();
  }
  if((update_load_avg || update_priority) && recent_cpu_thread->status==THREAD_BLOCKED){
		thread_unblock(recent_cpu_thread);
	}
}

/* Prints thread statistics. */
void
thread_print_stats (void) 
{
  printf ("Thread: %lld idle ticks, %lld kernel ticks, %lld user ticks\n",
          idle_ticks, kernel_ticks, user_ticks);
}

/* Creates a new kernel thread named NAME with the given initial
   PRIORITY, which executes FUNCTION passing AUX as the argument,
   and adds it to the ready queue.  Returns the thread identifier
   for the new thread, or TID_ERROR if creation fails.

   If thread_start() has been called, then the new thread may be
   scheduled before thread_create() returns.  It could even exit
   before thread_create() returns.  Contrariwise, the original
   thread may run for any amount of time before the new thread is
   scheduled.  Use a semaphore or some other form of
   synchronization if you need to ensure ordering.

   The code provided sets the new thread's `priority' member to
   PRIORITY, but no actual priority scheduling is implemented.
   Priority scheduling is the goal of Problem 1-3. */
tid_t
thread_create (const char *name, int priority,
               thread_func *function, void *aux UNUSED) 
{
  struct thread *t = NULL;
  struct kernel_thread_frame *kf;
  struct switch_entry_frame *ef;
  struct switch_threads_frame *sf;
  tid_t tid;
  enum intr_level old_level;

  ASSERT (function != NULL);

  /* Allocate thread. */
  t = palloc_get_page (PAL_ZERO);
  if (t == NULL)
    return TID_ERROR;

  /* Initialize thread. */
  
  init_thread (t, name, priority);
  // list_init (&t->already_acquired);
  tid = t->tid = allocate_tid ();

  /* Prepare thread for first run by initializing its stack.
     Do this atomically so intermediate values for the 'stack' 
     member cannot be observed. */
  old_level = intr_disable ();

  /* Stack frame for kernel_thread(). */
  kf = alloc_frame (t, sizeof *kf);
  kf->eip = NULL;
  kf->function = function;
  kf->aux = aux;

  /* Stack frame for switch_entry(). */
  ef = alloc_frame (t, sizeof *ef);
  ef->eip = (void (*) (void)) kernel_thread;

  /* Stack frame for switch_threads(). */
  sf = alloc_frame (t, sizeof *sf);
  sf->eip = switch_entry;
  sf->ebp = 0;

  intr_set_level (old_level);

  /* Add to run queue. */
  thread_unblock (t);
  if(t!=NULL && thread_get_priority() < t->priority){
    thread_yield();
  }
  return tid;
}

/* Puts the current thread to sleep.  It will not be scheduled
   again until awoken by thread_unblock().

   This function must be called with interrupts turned off.  It
   is usually a better idea to use one of the synchronization
   primitives in synch.h. */
void
thread_block (void) 
{
  ASSERT (!intr_context ());
  ASSERT (intr_get_level () == INTR_OFF);

  thread_current ()->status = THREAD_BLOCKED;
  schedule ();
}

/* Transitions a blocked thread T to the ready-to-run state.
   This is an error if T is not blocked.  (Use thread_yield() to
   make the running thread ready.)

   This function does not preempt the running thread.  This can
   be important: if the caller had disabled interrupts itself,
   it may expect that it can atomically unblock a thread and
   update other data. */
void
thread_unblock (struct thread *t) 
{
  enum intr_level old_level;

  ASSERT (is_thread (t));
  // thread_wakeup(timer_ticks());
  old_level = intr_disable ();
  ASSERT (t->status == THREAD_BLOCKED);
  /*Adding thread in sorted order according to the priority*/
  list_push_back(&ready_list, &t->elem);
  t->status = THREAD_READY;
  intr_set_level (old_level);
}

/* Returns the name of the running thread. */
const char *
thread_name (void) 
{
  return thread_current ()->name;
}

/* Returns the running thread.
   This is running_thread() plus a couple of sanity checks.
   See the big comment at the top of thread.h for details. */
struct thread *
thread_current (void) 
{
  struct thread *t = running_thread ();
  
  /* Make sure T is really a thread.
     If either of these assertions fire, then your thread may
     have overflowed its stack.  Each thread has less than 4 kB
     of stack, so a few big automatic arrays or moderate
     recursion can cause stack overflow. */
  ASSERT (is_thread (t));
  ASSERT (t->status == THREAD_RUNNING);

  return t;
}

/* Returns the running thread's tid. */
tid_t
thread_tid (void) 
{
  return thread_current ()->tid;
}

/* Deschedules the current thread and destroys it.  Never
   returns to the caller. */
void
thread_exit (void) 
{
  ASSERT (!intr_context ());

#ifdef USERPROG
  process_exit ();
#endif

  /* Remove thread from all threads list, set our status to dying,
     and schedule another process.  That process will destroy us
     when it call schedule_tail(). */
  intr_disable ();
  list_remove (&thread_current()->allelem);
  thread_current ()->status = THREAD_DYING;
  schedule ();
  NOT_REACHED ();
}

/* Yields the CPU.  The current thread is not put to sleep and
   may be scheduled again immediately at the scheduler's whim. */
void
thread_yield (void) 
{
  struct thread *cur = thread_current ();
  enum intr_level old_level;
  
  ASSERT (!intr_context ());

  old_level = intr_disable ();
  if (cur != idle_thread) 
    list_push_back(&ready_list, &cur->elem);
  cur->status = THREAD_READY;
  schedule ();
  intr_set_level (old_level);
}

/* Invoke function 'func' on all threads, passing along 'aux'.
   This function must be called with interrupts off. */
void
thread_foreach (thread_action_func *func, void *aux)
{
  struct list_elem *e;

  ASSERT (intr_get_level () == INTR_OFF);

  for (e = list_begin (&all_list); e != list_end (&all_list);
       e = list_next (e))
    {
      struct thread *t = list_entry (e, struct thread, allelem);
      func (t, aux);
    }
}

/* Sets the current thread's priority to NEW_PRIORITY. */
void
thread_set_priority (int new_priority) 
{
  // struct list_elem * ready_head = list_begin(&ready_list);
  // int curr_highest = list_entry(ready_head, struct thread, elem)->priority;
  // struct thread * t = thread_current();
  // t->old_priority = t->priority;
  // t->priority = new_priority;
  // if(curr_highest > new_priority)
  // {
  //   thread_yield();
  // }
  struct thread *t = thread_current ();
  int cur_priority = t->priority;

  /* Thread must yield to higher priority thread if it exists, whenever its
     own priority decreases. */
  t->priority = new_priority;
  if(new_priority < cur_priority)
    thread_yield ();
}

/* Returns the current thread's priority. */
int
thread_get_priority (void) 
{
  return thread_get_donated_priority(thread_current());
}



int
thread_get_donated_priority(struct thread * t){
	// return t->priority;
  if(!list_empty(&t->already_acquired)){
    int prio_max = t->priority;
    struct list_elem * it;
    for(it = list_begin(&t->already_acquired); it!=list_end(&t->already_acquired); it = list_next(it)){
      struct lock *l = list_entry(it, struct lock, elem);
      struct list_elem * i;
      struct semaphore *sema = &l->semaphore;
      if(!list_empty(&sema->waiters)){
        for(i = list_begin(&sema->waiters); i!=list_end(&sema->waiters);i=list_next(i)){
          struct thread * th = list_entry(i, struct thread, elem);
          int prio_th = thread_get_donated_priority(th);
          if(prio_th > prio_max){
            prio_max = prio_th;
          }
        }
      }
    }
    return prio_max;
  }else{
    return t->priority;
  }
	// return PRI_MAX - (thread_get_recent_cpu()/4 )
}

/* Sets the current thread's nice value to NICE. */
void
thread_set_nice (int nice UNUSED) 
{	
	struct thread *t = thread_current();
  t->nice = nice;
  thread_set_new_priority(t);
  // struct list_elem * m = list_min(&ready_list, prio, NULL);
  // struct thread *th = list_entry (m, struct thread, elem);
  if(t->priority > thread_get_priority()){
    thread_yield();
  }


}

/* Returns the current thread's nice value. */
int
thread_get_nice (void) 
{
  /* Not yet implemented. */
	return thread_current()->nice;
}

/* Returns 100 times the system load average. */
int
thread_get_load_avg (void) 
{
  /* Not yet implemented. */
  return _TO_INT_NEAREST (_MULTIPLY_INT (load_avg, 100));
}

/* Returns 100 times the current thread's recent_cpu value. */
int
thread_get_recent_cpu (void) 
{
  /* Not yet implemented. */
	return  _TO_INT_NEAREST (_MULTIPLY_INT (thread_current()->recent_cpu, 100));
}

/* Idle thread.  Executes when no other thread is ready to run.

   The idle thread is initially put on the ready list by
   thread_start().  It will be scheduled once initially, at which
   point it initializes idle_thread, "up"s the semaphore passed
   to it to enable thread_start() to continue, and immediately
   blocks.  After that, the idle thread never appears in the
   ready list.  It is returned by next_thread_to_run() as a
   special case when the ready list is empty. */
static void
idle (void *idle_started_ UNUSED) 
{
  struct semaphore *idle_started = idle_started_;
  idle_thread = thread_current ();
  sema_up (idle_started);

  for (;;) 
    {
      /* Let someone else run. */
      intr_disable ();
      thread_block ();

      /* Re-enable interrupts and wait for the next one.

         The `sti' instruction disables interrupts until the
         completion of the next instruction, so these two
         instructions are executed atomically.  This atomicity is
         important; otherwise, an interrupt could be handled
         between re-enabling interrupts and waiting for the next
         one to occur, wasting as much as one clock tick worth of
         time.

         See [IA32-v2a] "HLT", [IA32-v2b] "STI", and [IA32-v3a]
         7.11.1 "HLT Instruction". */
      asm volatile ("sti; hlt" : : : "memory");
    }
}

/* Function used as the basis for a kernel thread. */
static void
kernel_thread (thread_func *function, void *aux) 
{
  ASSERT (function != NULL);

  intr_enable ();       /* The scheduler runs with interrupts off. */
  function (aux);       /* Execute the thread function. */
  thread_exit ();       /* If function() returns, kill the thread. */
}

/* Returns the running thread. */
struct thread *
running_thread (void) 
{
  uint32_t *esp;

  /* Copy the CPU's stack pointer into `esp', and then round that
     down to the start of a page.  Because `struct thread' is
     always at the beginning of a page and the stack pointer is
     somewhere in the middle, this locates the curent thread. */
  asm ("mov %%esp, %0" : "=g" (esp));
  return pg_round_down (esp);
}

/* Returns true if T appears to point to a valid thread. */
static bool
is_thread (struct thread *t)
{
  return t != NULL && t->magic == THREAD_MAGIC;
}

/* Does basic initialization of T as a blocked thread named
   NAME. */
static void
init_thread (struct thread *t, const char *name, int priority)
{
  ASSERT (t != NULL);
  ASSERT (PRI_MIN <= priority && priority <= PRI_MAX);
  ASSERT (name != NULL);

  memset (t, 0, sizeof *t);
  list_init(&t->already_acquired);
  t->status = THREAD_BLOCKED;
  t->seeking = NULL;
  strlcpy (t->name, name, sizeof t->name);
  t->stack = (uint8_t *) t + PGSIZE;
  t->priority = priority;
  t->old_priority = priority;
  t->magic = THREAD_MAGIC;
  t->wakeup_at = -1;

  if (t == initial_thread)
    t->nice= 0;
  else
    t->nice = thread_current ()->nice;
  t->recent_cpu = 0;
  list_push_back (&all_list, &t->allelem);
}

/* Allocates a SIZE-byte frame at the top of thread T's stack and
   returns a pointer to the frame's base. */
static void *
alloc_frame (struct thread *t, size_t size) 
{
  /* Stack data is always allocated in word-size units. */
  ASSERT (is_thread (t));
  ASSERT (size % sizeof (uint32_t) == 0);

  t->stack -= size;
  return t->stack;
}

/* Chooses and returns the next thread to be scheduled.  Should
   return a thread from the run queue, unless the run queue is
   empty.  (If the running thread can continue running, then it
   will be in the run queue.)  If the run queue is empty, return
   idle_thread. */
static struct thread *
next_thread_to_run (void) 
{
  if (list_empty (&ready_list))
    return idle_thread;
  else{
    struct list_elem * m = list_min(&ready_list, prio, NULL);
    list_remove(m);
    return list_entry (m, struct thread, elem);
  }
}

/* Completes a thread switch by activating the new thread's page
   tables, and, if the previous thread is dying, destroying it.

   At this function's invocation, we just switched from thread
   PREV, the new thread is already running, and interrupts are
   still disabled.  This function is normally invoked by
   thread_schedule() as its final action before returning, but
   the first time a thread is scheduled it is called by
   switch_entry() (see switch.S).

   It's not safe to call printf() until the thread switch is
   complete.  In practice that means that printf()s should be
   added at the end of the function.

   After this function and its caller returns, the thread switch
   is complete. */
void
schedule_tail (struct thread *prev) 
{
  struct thread *cur = running_thread ();
  
  ASSERT (intr_get_level () == INTR_OFF);

  /* Mark us as running. */
  cur->status = THREAD_RUNNING;

  /* Start new time slice. */
  thread_ticks = 0;

#ifdef USERPROG
  /* Activate the new address space. */
  process_activate ();
#endif

  /* If the thread we switched from is dying, destroy its struct
     thread.  This must happen late so that thread_exit() doesn't
     pull out the rug under itself.  (We don't free
     initial_thread because its memory was not obtained via
     palloc().) */
  if (prev != NULL && prev->status == THREAD_DYING && prev != initial_thread) 
    {
      ASSERT (prev != cur);
      palloc_free_page (prev);
    }
}

/* Schedules a new process.  At entry, interrupts must be off and
   the running process's state must have been changed from
   running to some other state.  This function finds another
   thread to run and switches to it.

   It's not safe to call printf() until schedule_tail() has
   completed. */
static void
schedule (void) 
{
  struct thread *cur = running_thread ();
  struct thread *next = next_thread_to_run ();
  struct thread *prev = NULL;

  ASSERT (intr_get_level () == INTR_OFF);
  ASSERT (cur->status != THREAD_RUNNING);
  ASSERT (is_thread (next));

  if (cur != next)
    prev = switch_threads (cur, next);
  schedule_tail (prev); 
}

/* Returns a tid to use for a new thread. */
static tid_t
allocate_tid (void) 
{
  static tid_t next_tid = 1;
  tid_t tid;

  lock_acquire (&tid_lock);
  tid = next_tid++;
  lock_release (&tid_lock);

  return tid;
}

/* Offset of `stack' member within `struct thread'.
   Used by switch.S, which can't figure it out on its own. */
uint32_t thread_stack_ofs = offsetof (struct thread, stack);

/* Temporarily increases the priority of the running thread to MAXIMUM,
after which it might want to chnage locked resource.
*/
void thread_priority_temporarily_up(void){
  struct thread * t = thread_current();
  t->old_priority = t->priority;
  // t->priority = PRI_MAX;
  thread_set_priority(PRI_MAX);
  return;
}

/* Restores the old priority of a thread which might be temp increased */
void thread_priority_restore(void){
  struct thread * t = thread_current();
  thread_set_priority(t->old_priority);
  return;
}

void thread_block_till(int64_t wakeup_at){
 	//Saving the old interrupt context
    enum intr_level old_int; 
    struct thread * t = thread_current();
    //lock which is used to gain exclusive control over sleepers list
    lock_acquire (&sleepers_lock); 
    old_int = intr_disable();
    //if the running time has already come, set the interupt as older one and return
    if(timer_ticks() >= wakeup_at){
        intr_set_level(old_int);
        return;
    }
    //Current thread should be a running thread.
    ASSERT(t->status == THREAD_RUNNING);
    t->wakeup_at = wakeup_at;
    if (wakeup_at < next_wakeup_at)
        next_wakeup_at = wakeup_at;
    /*Inserting the current thread in sleepers list as per the time of 
    wake up and releasing the lock and returning back 
    to previous interrup state.*/
	list_insert_ordered(&sleepers_list, &(t->elem), before, NULL);
	lock_release (&sleepers_lock); 
	thread_block();
	intr_set_level(old_int);
	return;

}

void thread_set_next_wakeup(void){

	//Saving the old interrupt context
	enum intr_level old_level = intr_disable(); 
	int64_t time = timer_ticks();
	//if time is greater than or equal to next wake up time
	if(time>=next_wakeup_at){
	    if(list_empty(&sleepers_list)){
	      next_wakeup_at = INT64_MAX;
	    }
	    else{
	    	//extracting the thread with min wake up time.
	      struct thread * t = list_entry(list_begin(&sleepers_list),struct thread,elem);
        /*Time is lesser or equal to next wakeup time, 
        then pop the first thread and make the next wakeup to point to the next thread 
        in ordered list */
	      if(t->wakeup_at<=next_wakeup_at){
	          list_pop_front (&sleepers_list);
	          thread_unblock (t);
		        if(list_empty(&sleepers_list)){
		            next_wakeup_at = INT64_MAX;
		        }
		        else{
              //recursively wakes up all threads having same wake up time
              if(t->wakeup_at == list_entry(list_begin(&sleepers_list),struct thread,elem)->wakeup_at){
                thread_set_next_wakeup();
              }else{
                next_wakeup_at = list_entry(list_begin(&sleepers_list),struct thread,elem)->wakeup_at;
              }
		        }
	    	}
		    else{
		      next_wakeup_at = t->wakeup_at;
		    }
	   }
	}
	//Resetting the older context of interrupt.
	intr_set_level (old_level);
	return;
}

void
timer_wakeup (void)
{
  lock_acquire (&sleepers_lock);
  while (!list_empty (&sleepers_list))
  {
    struct thread *t = list_entry (list_front (&sleepers_list),
                                  struct thread, elem);
    if (t->wakeup_at <= next_wakeup_at)
    {
      list_pop_front(&sleepers_list);
      thread_unblock(t);
    }
    else
      break;
  }

  if (list_empty (&sleepers_list))
    next_wakeup_at = INT64_MAX;
  else
    next_wakeup_at = list_entry(list_front(&sleepers_list),
                                struct thread, elem)->wakeup_at;
  lock_release (&sleepers_lock);
}

void
managerial_activity (void *aux UNUSED) 
{
  // struct semaphore *mang_started = mang_started_;
  mang_thread = thread_current ();
  // sema_up (mang_started);
  enum intr_level old_level;

  while (true) 
    {
      /* Let someone else run. */
      old_level = intr_disable ();
      thread_block ();
      intr_set_level(old_level);

      // thread_set_next_wakeup();
      timer_wakeup();
     
    }
}

void
thread_set_new_priority(struct thread * t){
  enum intr_level old_level = intr_disable ();
  int aux = _ADD_INT (_DIVIDE_INT (t->recent_cpu, 4), 2*t->nice);
  t->priority = _TO_INT_ZERO (_INT_SUB (PRI_MAX, aux));
  intr_set_level (old_level);
}

void
thread_update_recent_cpu (struct thread *t)
{
  int double_load_avg = _MULTIPLY_INT (load_avg, 2);
  int alpha = _DIVIDE (double_load_avg, _ADD_INT (double_load_avg, 1));
  int aux = _MULTIPLY (alpha, t->recent_cpu);
  t->recent_cpu = _ADD_INT (aux, t->nice);
}

void
thread_update_load_avg(void){
  struct list_elem *e;
  int cnt = 0;
  for (e = list_begin (&ready_list); e != list_end (&ready_list); e = list_next (e))
  {
  	struct thread *t = list_entry(e,struct thread,elem);
    if(t!=recent_cpu_thread && t!=mang_thread && t!=idle_thread){
      cnt++;
    }
  }
  struct thread *t = thread_current();
  if(t!=recent_cpu_thread && t!=mang_thread && t!=idle_thread){
    cnt++;
  }
  int64_t num = _ADD_INT (_MULTIPLY_INT (load_avg, 59), cnt);
  load_avg = _DIVIDE_INT (num, 60);
}

void 
set_all_priority(void)
{
	struct list_elem *e;
  for (e = list_begin (&all_list); e != list_end (&all_list); e = list_next (e))
  {
  	struct thread *t = list_entry(e,struct thread,allelem);
    if(t!=recent_cpu_thread && t!=mang_thread && t!=idle_thread){
      thread_set_new_priority(t);
    }    
  }
  thread_yield();
}

void 
set_all_recent_cpu(void)
{
	struct list_elem *e;
  for (e = list_begin (&all_list); e != list_end (&all_list); e = list_next (e))
  {
  	struct thread *t = list_entry(e,struct thread,allelem);
    if(t!=recent_cpu_thread && t!=mang_thread && t!=idle_thread){
      thread_update_recent_cpu(t);
    }
  }
  thread_yield();

}

void
recent_cpu_activity (void *aux UNUSED) 
{
  // struct semaphore *mang_started = mang_started_;
  recent_cpu_thread = thread_current ();
  // sema_up (mang_started);
  enum intr_level old_level;
  while(true) 
    {
      /* Let someone else run. */
      old_level = intr_disable ();
      thread_block ();
      intr_set_level(old_level);
      if(thread_mlfqs){
        if(update_load_avg){
         
          thread_update_load_avg();
          set_all_recent_cpu();
          update_load_avg = false;
          
        }
        if(update_priority){
          
          set_all_priority();
          update_priority = false;
          
        }
        
      }
    }
}


