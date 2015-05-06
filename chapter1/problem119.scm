#!/usr/bin/gosh
;; -*- coding: utf-8 -*-

(define (fib n)
  (fib-iter 1 0 0 1 n))

(define (square a)
  (* a a))

(define (fib-iter a b p q count)
  (cond ((= count 0) b)
	((even? count)
	 (fib-iter a
		   b
		   (+ (square p) (square q)) ; p' を計算
		   (+ (* 2 p q) (square q))  ; q' を計算
		   (/ count 2)))
	(else (fib-iter (+ (* b q) (* a q) (* a p))
			(+ (* b p) (* a q))
			p
			q
			(- count 1)))))

(fib 7)
