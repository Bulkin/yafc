(in-package :yafc-server)

(let ((global-history)
      (write-lock (make-lock "history-lock")))

  (defun history-add-item (item)
    (with-lock-held (write-lock)
      (push item global-history)))

  (defun history-get-last-items (n)
    (loop for item in global-history
       for i from 1 to n
       collect item)))
