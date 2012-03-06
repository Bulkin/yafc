(in-package :yafc-server)

(defparameter *tq* (run-task-queue-thread)
  "Global task queue for inter-thread communication")

(defmacro debug-print (object)
  (if *debug*
      `(print ,object)
      object))

(defmacro safe-read (stream)
  "Attempt to read from a stream and handle all 
conditions like eof"
  `(handler-case (debug-print 
                  (read-from-string (read-line ,stream
                                               :recursive-p t)))
     (end-of-file () nil)))

(defmacro safe-write (stream msg)
  "Add a writing task to the default task queue"
  `(add-task-m *tq* 
     (print ,msg ,stream)
     (force-output ,stream)))

(defun recv-handler (stream)
  (let ((token (safe-read stream)))
    (when token (safe-write stream token))
    token))
      
