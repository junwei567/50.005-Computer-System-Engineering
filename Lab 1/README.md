# 50005Lab1

## Explanation of main loop function

firstly, the while fscanf loop checks if there is jobs / tasks in the input file. So I used a for loop to check my buffer if any task status == 0. This means that a job is cleared, so I can schedule new jobs. After scheduling, I have to break the inner while loop to go next??? I also have to check if any child has exited prematurely, then make a new child in place of the dead child and give it a task.
</br>
after all tasks are done in the input file, I have to legally terminate all worker processes that are still alive. This is done by checking the pid of child processes are == 0 and their task status == 0. Then I can give it the termination type 'z' job. By making sure that the count of all children whose task status == 0 is equals to the number of processes, I make sure that all worker processes are terminated. 
</br>

