#!/usr/bin/gosh
;;-*- coding: utf-8 -*-

(define (good-enough? guess x)
  (< (abs (- (* guess guess) x)) 0.001))
  

(define (improve guess x)
  (/ (+ guess (/ x guess)) 2))

(define (sqrt-iter guess x)
  (if (good-enough? guess x)
      guess
      (sqrt-iter (improve guess x) x)))

(define (sqrt x)
  (sqrt-iter 1.0 x))

(define (new-if predicate then-clause else-clause)
  (cond (predicate then-clause)
	(else else-clause)))


(define (alyssa-sqrt x)
  (alyssa-sqrt-iter 1.0 x))

(define (alyssa-sqrt-iter guess x)
  (new-if (good-enough? guess x)
	  guess
	  (alyssa-sqrt-iter (improve guess x) x)))
