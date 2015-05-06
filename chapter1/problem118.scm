#!/usr/bin/gosh
;; -*- coding utf-8 -*-

(define (fast-mult a b)
  (fast-mult-iter a b 0))

(define (double a)
  (+ a a))
(define (halve a)
  (/ a 2))
(define (fast-mult-iter a count ans)
  (if (= count 0)
      ans
      (if (= (remainder count 2) 0)
	  (fast-mult-iter (double a) (halve count) ans)
	  (fast-mult-iter a (- count 1) (+ ans a)))))


(fast-mult 7 5)

