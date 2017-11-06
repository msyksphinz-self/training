(define (triangle y x)
  (if (or (>= x y) (= x 0))
      1
      (+ (triangle (- y 1) (- x 1))
	 (triangle (- y 1) x))))
