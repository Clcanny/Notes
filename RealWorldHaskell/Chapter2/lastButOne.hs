lastButOne xs = if length xs > 2
                then lastButOne (tail xs)
                else head xs

anotherLastButOne xs = head (tail (reverse xs))
