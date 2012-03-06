;;;; yafc-srv.asd

(in-package :cl-user)

(asdf:defsystem :yafc-server
  :components ((:module src
                        :serial t
                        :components ((:file "package")
                                     (:file "task-queue")
                                     (:file "history")
                                     (:file "server"))))
  :depends-on (:usocket :bordeaux-threads :alexandria))
