#!/usr/bin/gosh
;; -*- coding: utf-8 -*-

(include "./prime.scm")

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


(define (search-for-prime start n)
  (if (not (= n 0))
      (if (timed-prime-test start)
	  (search-for-prime (+ start 1) (- n 1))
	  (search-for-prime (+ start 1) n))))

(search-for-prime 1000   3)
(search-for-prime 10000  3)
(search-for-prime 100000 3)
