(in-package :yafc-server)

(defparameter *sock-list* nil)
(defparameter *stop-flag* nil)
(defparameter *wait-timeout* 0.1)

;;; synchronized stack implementation for pushing new
;;; sockets to worker threads
(let ((sock-stack)
      (lock (make-lock))
      (cond-var (make-condition-variable)))
  
  (defun push-socket (sock)
    (with-lock-held (lock)
      (progn 
        (push sock sock-stack)
        (condition-notify cond-var)))
    sock)

  (defun pop-socket ()
    (with-lock-held (lock)
      (if (car sock-stack)
          (list (pop sock-stack))
          nil)))
  
  (defun available-sockets-cond ()
    cond-var))


(defun check-sockets (sockets func)
  "Call handler func on ready to be read sockets and return disconnected
  sockets if any exist"
  (loop for sock in sockets
     when (eq (usocket::state sock) :READ)
     when (not (funcall func (socket-stream sock)))
     collect sock))

(defun make-worker-thread-func (recv-handler)
  (lambda () 
    (loop with lock = (make-lock) and sockets = nil
       until *stop-flag*
       do (nconcf sockets (pop-socket))
       if (car sockets)
       do (progn 
            (wait-for-input sockets :ready-only nil 
                            :timeout *wait-timeout*)
            (let ((closed-sockets
                   (check-sockets sockets recv-handler)))
              (when closed-sockets
                (format t "Removing ~a~%" closed-sockets)
                (setf sockets 
                      (delete-if (lambda (x) (find x closed-sockets))
                                 sockets)))))
       else 
       do (with-lock-held (lock)
            (condition-wait (available-sockets-cond) lock)))))

(defun run-worker-threads (thread-count recv-handler)
  (when (> thread-count 0)
    (make-thread (make-worker-thread-func recv-handler)
                 :name "worker-thread")
    (run-worker-threads (1- thread-count) recv-handler)))

(defun restart-worker-threads (thread-count recv-handler)
  (mapcar (lambda (item) 
            (when (equal (sb-thread:thread-name item)
                         "worker-thread")
              (destroy-thread item)))
          (all-threads))
  (run-worker-threads thread-count recv-handler))

(defun wait-for-connections (sock)
  "Wait for connections to the passive socket sock
and push resulting sockets to the worker threads"
  (push (push-socket (socket-accept sock)) *sock-list*)
  (when (not *stop-flag*)
    (wait-for-connections sock)))
  
(defun def-recv-handler (stream)
  (handler-case (print (read stream))
    (end-of-file () nil)))

(defun serve (&key (host "127.0.0.1")
              (port #xBEEF) 
              (recv-handler #'def-recv-handler)
              (worker-thread-count 4))
  "Start a server
	Automatically manages threading and whatever
	The recv-handler must return nil on client disconnect
and t otherwise"
  (push (socket-listen host port :reuse-address t) *sock-list*)
  (make-thread  
   (lambda () (wait-for-connections (car *sock-list*)))
   :name "server-thread")
  (run-worker-threads worker-thread-count recv-handler))
