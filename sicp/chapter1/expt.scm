#!/usr/bin/gosh
;; -*- coding: utf-8 -*-

(define (expt-1 b n)
  (if (= n 0)
      1
      (* b (expt b (- n 1)))))


(define (expt-2 b n)
  (expt2-iter b 1 n))

(define (expt2-iter b ans count)
  (if (= count 0)
      ans
      (expt2-iter b (* b ans) (- count 1))))


(define (fast-expt b n)
  (if (= n 0)
      1
      (if (= (remainder n 2) 0)
	  (square (fast-expt b (/ n 2)))
	  (* b (fast-expt b (- n 1))))))

(define (fast-expt2 b n)
  (if (= n 0)
      1
      (if (= (remainder n 2) 0)
	  (square (fast-expt b (/ n 2)))
	  (* b (square (fast-expt b (/ (- n 1) 2)))))))


(expt-1 7 3)
(expt-2 7 3)
(fast-expt 7 3)
(fast-expt2 7 3)
