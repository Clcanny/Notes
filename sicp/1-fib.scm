#lang racket
(require racket/trace)
(define (fib-helper-s6 stack iStack nStack)
  (define (assert condition line)
    (if (not condition)
        (begin
          (display line)
          (raise "Assert Failure"))
        condition))
  
  (define (r-head l)
    (begin
      (assert (not (empty? l)) (syntax-line #'here))
      (list-ref l (- (length l) 1))))
  (define r-head-test
    (begin
      (assert (= (r-head (list 1)) 1) (syntax-line #'here))
      (assert (= (r-head (list 1 2)) 2) (syntax-line #'here))))

  (define (r-tail l)
    (begin
      (assert (not (empty? l)) (syntax-line #'here))
      (take l (- (length l) 1))))
  (define r-tail-test
    (begin
      (assert (equal? (r-tail (list 1)) (list)) (syntax-line #'here))
      (assert (equal? (r-tail (list 1 2)) (list 1)) (syntax-line #'here))))

  (define (list-last-ele-inc l)
    (begin
      (assert (not (empty? l)) (syntax-line #'here))
      (append (r-tail l)
              (list (+ (r-head l) 1)))))
  (define list-tail-ele-inc-test
    (begin
      (assert (equal? (list-last-ele-inc (list 1)) (list 2)) (syntax-line #'here))
      (assert (equal? (list-last-ele-inc (list 1 2)) (list 1 3)) (syntax-line #'here))))

  (begin
    (assert (= (length iStack) (length nStack)) (syntax-line #'here))
    (if (and (not (empty? iStack)) (>= (r-head iStack) 3))
        (begin
          (assert(= (r-head iStack) 3) (syntax-line #'here))
          (set! iStack (r-tail iStack))
          (assert (= (length stack) (r-head nStack)) (syntax-line #'here))
          (set! stack (append stack
                              (list (+ (list-ref stack (- (r-head nStack) 2))
                                       (list-ref stack (- (r-head nStack) 1))))))
          (set! nStack (r-tail nStack))
          (fib-helper-s6 stack iStack nStack))
        (if (and (not (empty? iStack))
                 (>= (- (r-head nStack)
                        (r-head iStack))
                     (length stack)))
          (begin
            (set! iStack (append iStack (list 1)))
            (set! nStack (append nStack
                                 (list (- (r-head nStack) (r-head iStack)))))
            (fib-helper-s6 stack iStack nStack))
          (if (not (empty? iStack))
            (begin
              (set! iStack (list-last-ele-inc iStack))
              (fib-helper-s6 stack iStack nStack))
            (r-head stack))))))
(define (fib-s6 n) (fib-helper-s6 (list 0 1) (list 1) (list n)))
(fib-s6 5)
