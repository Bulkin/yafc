(in-package :yafc-server)

(defparameter *tq* (run-task-queue-thread)
  "Global task queue for inter-thread communication")

(defmacro debug-print (object)
  (if *debug*
      `(print ,object)
      object))

(defmacro handle-closed-stream (&body body)
  `(handler-case (progn ,@body)
     (end-of-file () nil)
     (socket-error () nil)))

(defmacro safe-read (stream)
  "Attempt to read from a stream and handle all 
conditions like eof"
  `(handle-closed-stream 
     (debug-print 
      (read-from-string (read-line ,stream
                                   :recursive-p t)))))

(defmacro safe-write (stream msg)
  "Add a writing task to the default task queue"
  (handle-closed-stream 
    `(add-task-m *tq* 
       (print ,msg ,stream)
       (force-output ,stream))))

(defun recv-handler (stream)
  (let ((token (safe-read stream)))
    (when token (safe-write stream token))))
      
