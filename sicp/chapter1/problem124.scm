(use math.mt-random)
(define mt (make <mersenne-twister> :seed (sys-time)))

(define (random m)
  (mt-random-integer mt m))

(define true #t)
(define false #f)

(define (expmod base exp m)
  (cond ((= exp 0) 1)
	((even? exp)
	 (remainder
	  (square
	   (expmod base (/ exp 2) m))
	  m))
	(else
	 (remainder 
	  (* base
	     (expmod base (- exp 1) m))
	  m))))

(define (fermat-test n)
  (define (try-it a)
    (= (expmod a n n) a))
  (try-it (+ 1 (random (- n 1)))))

(define (fast-prime? n times)
  (cond ((= times 0) true)
	((fermat-test n) (fast-prime? n (- times 1)))
	(else false)))
  
(define (smallest-divisor n) (find-divisor n 2))
(define (find-divisor n test-divisor)
  (cond ((> (square test-divisor) n) n)
	((divides? test-divisor n) test-divisor)
	(else (find-divisor n (+ test-divisor 1)))))
(define (divides? a b) (= (remainder b a) 0))


(define (squrae n)
  (* n n))

(define (smallest-divisor n) (find-divisor n 2))

(define (find-divisor n test-divisor)
  (cond ((> (square test-divisor) n) n)
	((divides? test-divisor n) test-divisor)
	(else (find-divisor n (next test-divisor)))))

(define (divides? a b) (= (remainder b a) 0))

(define (prime? n)
  (= n (smallest-divisor n)))

(define (runtime)
  (- (time->seconds (current-time)) 1136041200))

(define (timed-prime-test n)
  (start-prime-test n (runtime)))

(define (start-prime-test n start-time)
  (if (fast-prime? n 10)
      (report-prime n (- (runtime) start-time))
      #f))

(define (report-prime n elapsed-time)
  (display " *** ") (display n) (display ", ") (display elapsed-time) (newline) #t)


(define (next n)
  (if (< n 3) 3 (+ n 2)))


(search-for-prime 1000   3)
(search-for-prime 10000  3)
(search-for-prime 100000 3)
