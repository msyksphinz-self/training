#!/usr/bin/gosh
;; -*- coding: utf-8 -*-

(define (fast-expt3 b n)
  (fast-expt3-iter b 1 n))

(define (fast-expt3-iter b ans count)
  (if (= count 0)
      ans
      (if (= (remainder count 2) 0)
	  (fast-expt3-iter (* b b) ans (/ count 2))
	  (fast-expt3-iter b (* ans b) (- count 1)))))

(fast-expt3 7 3)
