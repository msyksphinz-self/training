#!/usr/bin/gosh
;;-*- coding: utf-8 -*-

(define (good-enough? guess x)
  (< (abs (- (* guess guess) x)) 0.001))
  

(define (improve guess x)
  (/ (+ guess (/ x guess)) 2))

(define (sqrt-iter guess x)
  (if (good-enough? guess x)
      guess
      (newton (improve guess x) x)))

(define (sqrt x)
  (sqrt-iter 1.0 x))
