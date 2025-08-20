# scheduling-algorithms

# Job Scheduling Algorithms (C++)

Short, classroom-focused implementations of two classic scheduling algorithms:

- **Moore–Hodgson** *(1‖∑Uj)* — minimizes the number of late jobs on a single machine.  
  - `myMoore(P, D)` → `vector<tuple<int,int>>` of `(jobID, endTime)` in the final order (on-time first, then late in removal order).  
  - `myMooreLate(P, D)` → `vector<int>` of late jobs in the order they were removed.

- **McNaughton** *(P‖pmtn‖Cmax)* — preemptive scheduling on `m` identical processors to minimize makespan.  
  - `myMcNaughton(P, m)` → `vector<vector<tuple<int,float,float>>>` where each inner list is a processor’s timeline of `(jobID, start, end)`.

Inputs are integer processing times (and due dates for Moore–Hodgson). Time starts at 0. Output formats match typical grading scripts for scheduling coursework.
