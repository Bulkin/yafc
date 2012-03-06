(defpackage :yafc-server
  (:use :cl :bordeaux-threads :usocket :alexandria))

(in-package :yafc-server)

(defparameter *debug* t)

