(import (srfi 1))

(define (generate_random_lst length max_value) 
    ( if(= length 0)
        '()
        (cons (random max_value) (generate_random_lst (- length 1)  max_value ))
))

(define (generate_empty_lst length)
    (make-list length 0))

(define (scan lst sum)
    ( if (null? lst)
        '()
        (cons sum (scan (cdr lst) (+ sum (car lst))) )))

(define (permute elems index)
    (define n (length elems))
    (when (= n (length index))
        (define result (make-vector n))
        (for-each (lambda (pair)
                (let ((elem (car pair))
                        (idx (car (cdr pair))))
                    (vector-set! result idx elem)))
                (zip elems index))
        (vector->list result)))

;(define (generate_alist lst1 lst2)
;    (if (or (null? lst1) (null? lst2))
;    '()
;    (cons (cons (car lst1) (car lst2)) (generate_alist (cdr lst1) (cdr lst2)))))

;(define (reduce lst op init)
;    ( if (null? lst)
;        init
;        (op (car lst) (reduce (cdr lst) op init))))

;(define (generate_lst length value) 
;    ( if(= length 0)
;    '()
;    (cons value (generate_lst (- length 1) value))
;))


(define vec (make-list 10 9))

;(write vec)

(write (permute '(1 2 3 4) '(2 0 1 3)))
