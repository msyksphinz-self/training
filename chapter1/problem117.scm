#!/usr/bin/gosh
;; -*- coding: utf-8 -*-

(define (mult a b)
  (if (= b 0)
      0
      (+ (mult a (- b 1)) a)))

(define (double a)
  (+ a a))
(define (halve a)
  (/ a 2))

(define (fast-mult a b)
  (if (= b 0)
      0
      (if (= (remainder b 2) 0)
	  (double (fast-mult a (halve 2)))
	  (+ a (fast-mult a (- b 1))))))

(mult 10 5)
(mult 7 6)
