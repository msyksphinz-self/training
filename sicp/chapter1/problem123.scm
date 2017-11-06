#!/usr/bin/gosh
;; -*- coding: utf-8 -*-

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
  (if (prime? n)
      (report-prime n (- (runtime) start-time))
      #f))

(define (report-prime n elapsed-time)
  (display " *** ") (display n) (display ", ") (display elapsed-time) (newline) #t)


(define (next n)
  (if (< n 3) 3 (+ n 2)))


(search-for-prime 1000   3)
(search-for-prime 10000  3)
(search-for-prime 100000 3)
