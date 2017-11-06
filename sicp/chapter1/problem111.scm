(define (recur-func n)
  (if (< n 3)
      n
      (+ (recur-func (- n 1))
	 (* 2 (recur-func (- n 2)))
	 (* 3 (recur-func (- n 3))))))

(define (problem-func2 n)
  (iter-func 2 1 0 n))

(define (iter-func f2 f1 f0 count)
  (if (= count 0)
      f0
      (iter-func
       (+ f2 (* 2 f1) (* 3 f0)) f2 f1
       (- count 1))))

(recur-func 2)
(recur-func 3)
(recur-func 10)

(problem-func2 2)
(problem-func2 3)
(problem-func2 10)

