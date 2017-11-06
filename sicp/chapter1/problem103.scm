(define (sum-square a b)
  (+ (* a a) (* b b)))


(define (max-square a b c)
  (cond ((and (> a c) (> b c)) (sum-square a b)) ;; c is minimum
	((and (> a b) (> c b)) (sum-square a c)) ;; b is minimum
	(else (sum-square b c))))

;#?=(max-square 2 3 1)
;#?=(max-square 2 1 3)
;#?=(max-square 1 2 3)
