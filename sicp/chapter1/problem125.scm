(define (expmod1 base exp m)
  (cond ((= exp 0) 1)
	((even? exp)
	 (remainder
	  (square
	   (expmod1 base (/ exp 2) m))
	  m))
	(else
	 (remainder 
	  (* base
	     (expmod1 base (- exp 1) m))
	  m))))


(define (fast-expt b n)
  (if (= n 0)
      1
      (if (= (remainder n 2) 0)
	  (square (fast-expt b (/ n 2)))
	  (* b (fast-expt b (- n 1))))))

(define (expmod2 base exp m)
  (remainder (fast-expt base exp) m))


(expmod1 16 271311 5512)
(expmod2 16 271311 5512)
