#!/usr/bin/gosh
;; -*- coding: utf-8 -*-

(define (gcd a b)
  (if (= b 0)
      a
      (gcd b (remainder a b))))



(define (gcd-cond a b)
  (cond ((= b 0) a)
	(else (gcd-cond b (remainder a b)))))

(gcd 206 40)
(gcd-cond 206 40)
