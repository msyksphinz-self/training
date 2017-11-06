(use math.mt-random)
(define mt (make <mersenne-twister> :seed (sys-time)))

(define (random m)
  (mt-random-integer mt m))

(define true 1)
(define false 0)

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
  


(fast-prime? 3337 10)
(fast-prime? 561 10)


(define (smallest-divisor n) (find-divisor n 2))
(define (find-divisor n test-divisor)
  (cond ((> (square test-divisor) n) n)
	((divides? test-divisor n) test-divisor)
	(else (find-divisor n (+ test-divisor 1)))))
(define (divides? a b) (= (remainder b a) 0))
