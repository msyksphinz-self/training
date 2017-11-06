(define (fib n)
  (cond ((= n 0) 0)
	((= n 1) 1)
	(else (+ (fib (- n 1))
		 (fib (- n 2)))))
  )

(define (fib-2 n)
  (fib-iter 1 0 n))

(define (fib-iter n1 n0 count)
  (if (= count 0)
      n0
      (fib-iter (+ n1 n0) n1 (- count 1))))


(fib 1)
(fib 2)
(fib 3)
(fib 10)

(fib-2 1)
(fib-2 2)
(fib-2 3)
(fib-2 10)



