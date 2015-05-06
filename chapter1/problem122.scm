(include "./prime.scm")

(define (runtime)
  (- (time->seconds (current-time)) 1136041200))

;; (define (timed-prime-test n)
;;   (newline) (display n) (start-prime-test n (runtime)))
(define (timed-prime-test n)
  (newline) (display n) (start-prime-test n (runtime)))

(define (start-prime-test n start-time)
  (if (prime? n)
      (report-prime (- (runtime) start-time))
      0))

(define (report-prime elapsed-time)
  (display " *** ") (display elapsed-time) 1)


(define (search-for-prime start n)
  (if (= n 0)
      0
      (if (timed-prime-test start)
	  (search-for-prime (+ start 1) (- n 1))
	  (search-for-prime (+ start 1) n))))
