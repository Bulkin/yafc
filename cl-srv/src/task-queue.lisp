(in-package :yafc-server)

(defstruct (task-queue (:conc-name tq-))
  (access-lock (make-lock "tq-access-lock"))
  (run-lock (make-lock "tq-run-lock"))
  (running (make-condition-variable))
  (exit-flag nil)
  (head nil))

(defun run-tasks (task-queue)
  "Run tasks in task-queue
Sleeps when queue is empty"
  (let ((rlock (tq-run-lock task-queue))
        (head (tq-head task-queue)))
    (acquire-lock rlock)
    (if head
        (progn (funcall (car head))
               (with-lock-held ((tq-access-lock task-queue))
                 (setf (tq-head task-queue) (cdr head))))
        (condition-wait (tq-running task-queue) rlock))
    (release-lock rlock)
    (when (not (tq-exit-flag task-queue))
      (run-tasks task-queue))))

(defun add-task (task-queue task)
  (let ((head (tq-head task-queue)))
    (with-lock-held ((tq-access-lock task-queue))
      (setf (tq-head task-queue)
            (nconc head (list task))))
    (condition-notify (tq-running task-queue))))

(defmacro add-task-m (task-queue &body body)
  "Add a task specified by some forms in body
	Returns t"
  `(progn (add-task ,task-queue
                    (lambda () (progn ,@body)))
          t))
      
(defun stop-tasks (task-queue)
  "Don't execute any more tasks in task-queue
or wait for them"
  (setf (tq-exit-flag task-queue) t)
  (condition-notify (tq-running task-queue)))

(defun run-task-queue-thread (&optional task-queue)
  "Create a thread running the task queue
Returns a task-queue instance"
  (let ((tq (or task-queue (make-task-queue))))
    (make-thread (lambda () (progn 
                              (format t "Task queue running~%")
                              (run-tasks tq)))
                 :name "run-task-queue")
    tq))

;;; ------ Simple test ------

(defun tq-test-simple-func (tq id)
  (add-task-m tq (format t "~a ~%" id))
  (sleep (/ 0.5 (1+ (random 1000))))
  (add-task-m tq (format t "~a finished~%" id)))

(defun tq-test ()
  (let ((tq (run-task-queue-thread)))
    (loop for i from 0 to 100 do
         (make-thread (lambda () (tq-test-simple-func tq i))))
    (sleep 1)
    (stop-tasks tq)
    (sleep 0.1)))
  
